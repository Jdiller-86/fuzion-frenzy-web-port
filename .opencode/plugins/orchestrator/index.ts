// Persistent in-OpenCode orchestrator for the Fuzion Frenzy project.
//
// Loads automatically from .opencode/plugins/ but activates only for the
// architect worktree project (directory guard below). It keeps exactly one
// persistent manager session, wakes it only when work actually exists,
// records the same MANAGER_* lane cards the PowerShell runner writes, and
// starts the registered builder lane tasks on demand.
//
// Hard boundaries: it never edits tracked repository files, never touches
// GitHub or PRs, never invokes models directly, never merges or pushes, and
// never overrides architect goals. Decisions stay with the pinned manager
// session; actuation of builders stays with the per-lane runner and its
// locks. The hourly architect watchdog (Invoke-OpenCodeArchitect.ps1) and
// the cheap PowerShell probe (Invoke-OpenCodeManager.ps1) recover this
// heartbeat if the shared service or this plugin goes down.
import { Plugin } from "@opencode/plugin"
import { execFile } from "node:child_process"
import { existsSync, readFileSync, renameSync, writeFileSync } from "node:fs"
import { dirname, join } from "node:path"
import { promisify } from "node:util"

const exec = promisify(execFile)

const HEARTBEAT_MS = 5 * 60_000
const SPAWN_GRACE_MINUTES = 3
const HEARTBEAT_FRESH_MINUTES = 12
const RUN_MAX_MINUTES = 95
const MANAGER_TITLE = "Fuzion Frenzy manager"
const MANAGER_MODEL = "opencode-go/glm-5.3-flash"
const LANES = ["primary", "geometry", "verification"] as const
type Lane = (typeof LANES)[number]
const LANE_TASKS: Record<Lane, string> = {
  primary: "FuzionFrenzy-OpenCode-Cycle",
  geometry: "FuzionFrenzy-OpenCode-Geometry",
  verification: "FuzionFrenzy-OpenCode-Verification",
}
const LANE_BRANCHES: Record<Lane, string> = {
  primary: "codex/opencode-source",
  geometry: "codex/opencode-geometry",
  verification: "codex/opencode-verification",
}

function readJson(path: string): any {
  try {
    return JSON.parse(readFileSync(path, "utf8").replace(/^\uFEFF/, ""))
  } catch {
    return undefined
  }
}

function checkpointBrief(path: string): string {
  try {
    return readFileSync(path, "utf8").split(/\r?\n/).slice(0, 7).join(" ").replace(/\s+/g, " ").slice(0, 520)
  } catch {
    return "checkpoint unavailable"
  }
}

function minutesSince(value: unknown): number {
  if (typeof value !== "string") return Number.POSITIVE_INFINITY
  const parsed = Date.parse(value)
  if (Number.isNaN(parsed)) return Number.POSITIVE_INFINITY
  return (Date.now() - parsed) / 60_000
}

function laneStateFile(lane: Lane): string {
  return lane === "primary" ? "opencode-cycle-state.json" : `opencode-${lane}-cycle-state.json`
}

function sessionIdOf(info: any): string | undefined {
  return info?.id ?? info?.info?.id ?? info?.sessionID ?? info?.session?.id
}

function lastAssistantText(messages: unknown): string {
  // Session messages carry `type: "assistant"` with a content array of
  // { type, text } parts (reasoning followed by text); handle wrappers too.
  const source: any = messages
  const list = Array.isArray(source) ? source : Array.isArray(source?.data) ? source.data : []
  for (let index = list.length - 1; index >= 0; index -= 1) {
    const message: any = list[index]
    const kind = message?.type ?? message?.info?.type ?? message?.role
    if (kind !== "assistant") continue
    const content = message?.content ?? message?.info?.content ?? message?.parts
    if (typeof content === "string" && content.trim()) return content
    if (Array.isArray(content)) {
      const joined = content
        .map((part: any) => (typeof part?.text === "string" ? part.text : ""))
        .join("\n")
        .trim()
      if (joined) return joined
    }
    const direct = message?.text ?? message?.info?.text
    if (typeof direct === "string" && direct.trim()) return direct
  }
  return ""
}

function parseMarker(text: string, lane: Lane): { action: "RUN" | "HOLD"; task: string } | undefined {
  // Take the LAST match so instruction examples in the prompt or reasoning
  // drafts can never be mistaken for the manager's final emitted marker.
  const pattern = new RegExp(`MANAGER_${lane.toUpperCase()}:\\s*(RUN|HOLD)\\s*\\|\\s*([^\\n]{1,300})`, "g")
  let current: RegExpExecArray | null = null
  let last: RegExpExecArray | null = null
  while ((current = pattern.exec(text)) !== null) { last = current }
  if (!last) return undefined
  return { action: last[1] as "RUN" | "HOLD", task: last[2].trim() }
}

function writeAtomic(path: string, value: unknown): void {
  const temp = `${path}.tmp`
  writeFileSync(temp, `${JSON.stringify(value, null, 2)}\n`, "utf8")
  renameSync(temp, path)
}

export default Plugin.define({
  id: "fuzion-orchestrator",
  async setup(ctx) {
    // Directory guard: the tracked plugin file exists in every worktree, but
    // only the architect worktree project runs the orchestrator.
    const location: any = ctx.location ?? {}
    const projectDir: string = location?.project?.directory ?? location?.directory ?? process.cwd()
    const normalized = projectDir.replace(/\\/g, "/")
    if (!normalized.endsWith("/opencode-architect")) return
    const repoRoot = dirname(dirname(normalized))
    if (!existsSync(join(repoRoot, "tools", "Invoke-OpenCodeManager.ps1"))) return

    const guidancePath = join(repoRoot, ".worktrees", "opencode-manager-guidance.json")
    const heartbeatPath = join(repoRoot, ".worktrees", "opencode-manager-heartbeat.json")
    const pinPath = join(repoRoot, ".worktrees", "opencode-manager-cycle-session.txt")

    // Keep exactly one persistent manager session, pinned for the PowerShell
    // fallback too.
    let managerID: string | undefined = (await ctx.storage.get("managerSessionID")) as string | undefined
    if (!managerID && existsSync(pinPath)) {
      const pinned = readFileSync(pinPath, "utf8").trim()
      if (/^ses_[A-Za-z0-9]+$/.test(pinned)) managerID = pinned
    }
    if (!managerID) {
      const created: any = await ctx.session.create({ title: MANAGER_TITLE })
      managerID = sessionIdOf(created)
      if (!managerID) return
      try {
        await ctx.session.switchAgent({ sessionID: managerID, agent: "manager" })
      } catch {
        // The agent may already be the session default; keep going.
      }
      await ctx.storage.set("managerSessionID", managerID)
      writeFileSync(pinPath, `${managerID}\n`, "utf8")
    }

    let suppressUntil = 0

    const startLaneTask = async (lane: Lane): Promise<string> => {
      try {
        await exec(
          "powershell.exe",
          ["-NoProfile", "-NonInteractive", "-Command", `Start-ScheduledTask -TaskName '${LANE_TASKS[lane]}'`],
          { timeout: 30_000 },
        )
        return "started"
      } catch (error: any) {
        return `spawn_error: ${String(error?.message ?? error)}`
      }
    }

    const beat = async (): Promise<void> => {
      const now = Date.now()
      const states: Record<Lane, any> = { primary: {}, geometry: {}, verification: {} }
      const heads: Record<Lane, string> = { primary: "", geometry: "", verification: "" }
      for (const lane of LANES) {
        states[lane] = readJson(join(repoRoot, ".worktrees", laneStateFile(lane))) ?? {}
        try {
          const { stdout } = await exec("git", ["-C", repoRoot, "rev-parse", LANE_BRANCHES[lane]])
          heads[lane] = stdout.trim()
        } catch {
          heads[lane] = String(states[lane]?.attemptedHead ?? "")
        }
      }
      const architectGuidance = readJson(join(repoRoot, ".worktrees", "opencode-architect-guidance.json"))
      const cards = readJson(guidancePath)
      const last: any = (await ctx.storage.get("lastBeat")) ?? {}
      const laneRunning = (lane: Lane) =>
        states[lane]?.outcome === "running" && minutesSince(states[lane]?.started) <= RUN_MAX_MINUTES
      const anyRunning = LANES.some(laneRunning)
      const stateChanged = LANES.some((lane) => (states[lane]?.finished ?? null) !== (last.finished?.[lane] ?? null))
      const newGoals =
        typeof architectGuidance?.created === "string" && architectGuidance.created !== last.goalsSeen
      const firstBeat = !last.beat

      // Self-healing: a lane the cards say RUN never started (spawn failed).
      let respawned: string[] = []
      if (!firstBeat && cards?.created && minutesSince(cards.created) >= SPAWN_GRACE_MINUTES && now >= suppressUntil) {
        for (const lane of LANES) {
          if (cards?.lanes?.[lane]?.action !== "RUN") continue
          if (laneRunning(lane)) continue
          const finishedBeforeCards =
            typeof states[lane]?.finished !== "string" || states[lane].finished <= cards.created
          if (finishedBeforeCards) {
            const result = await startLaneTask(lane)
            if (result === "started") respawned.push(lane)
          }
        }
      }

      const wakeManager = firstBeat || newGoals || (!anyRunning && stateChanged)
      if (!wakeManager) {
        writeAtomic(heartbeatPath, {
          beat: new Date().toISOString(),
          outcome: "idle",
          source: "plugin",
          managerSessionID: managerID,
          running: LANES.filter(laneRunning),
          respawned,
        })
        return
      }
      if (now < suppressUntil && !newGoals) {
        writeAtomic(heartbeatPath, {
          beat: new Date().toISOString(),
          outcome: "grace",
          source: "plugin",
          managerSessionID: managerID,
        })
        return
      }

      const architectBrief =
        architectGuidance?.approved && architectGuidance?.summary
          ? String(architectGuidance.summary).slice(0, 900)
          : "No approved architect directive; preserve the current original-execution route and integration hold."
      const priorBrief = cards
        ? `Previous manager tasks: primary ${cards?.lanes?.primary?.head ?? ""} ${cards?.lanes?.primary?.action ?? ""} ${cards?.lanes?.primary?.task ?? ""}; geometry ${cards?.lanes?.geometry?.action ?? ""}; verification ${cards?.lanes?.verification?.action ?? ""} ${cards?.lanes?.verification?.task ?? ""}.`
            .slice(0, 900)
        : ""
      const snapshot = `architect=${architectGuidance?.head ?? "unknown"}; primary=${heads.primary} (${states.primary?.outcome ?? "unknown"}); geometry=${heads.geometry} (${states.geometry?.outcome ?? "unknown"}); verification=${heads.verification} (${states.verification?.outcome ?? "unknown"})`
      const sourceCheckpoint = checkpointBrief(join(repoRoot, ".worktrees", "opencode-source", "docs", "PROJECT_STATUS.md"))
      const prompt = `Manager heartbeat. ${snapshot}. Source checkpoint: ${sourceCheckpoint}. Architect directive: ${architectBrief}. ${priorBrief} Assign the next useful bounded result from the CURRENT checkpoint; if a previous task is now complete or its head advanced, do not reissue it. Dirty lanes continue their edits. HOLD geometry when the architect says HOLD or reference evidence is absent; HOLD verification without a concrete gate task. Follow lane ownership and the architect's route decisions in AGENTS.md; do not edit, merge, push, or claim playability. Reply with ONLY three one-line markers (task <=300 characters, include acceptance and stop): MANAGER_PRIMARY: RUN|HOLD | task; MANAGER_GEOMETRY: RUN|HOLD | task; MANAGER_VERIFICATION: RUN|HOLD | task.`

      await ctx.session.prompt({ sessionID: managerID, text: prompt })
      try {
        await ctx.session.wait({ sessionID: managerID })
      } catch {
        // wait is best effort; fall through and read what finished.
      }
      let reply = ""
      let messages: any = undefined
      try {
        messages = await ctx.session.context({ sessionID: managerID })
        reply = lastAssistantText(messages)
      } catch {
        reply = ""
      }
      if (!reply && messages !== undefined) {
        // Last-resort fallback: scan the serialized transcript; last-match
        // marker parsing keeps this safe against prompt examples.
        try { reply = JSON.stringify(messages) } catch { reply = "" }
      }

      const parsed: Partial<Record<Lane, { action: string; task: string }>> = {}
      for (const lane of LANES) parsed[lane] = parseMarker(reply, lane)
      // A resumed source cycle has unfinished edits. Do not let an older
      // architect directive reassign its already completed prior milestone.
      if (states.primary?.outcome === "resume" && parsed.primary?.action === "RUN") {
        parsed.primary.task = "Continue only the in-flight source edits; use the current diff and top checkpoint, finish targeted checks and one source-only commit, then report the observed next stop."
      }
      const complete = LANES.every((lane) => parsed[lane])
      let spawned: string[] = []
      if (complete) {
        const nextCards: any = {
          created: new Date().toISOString(),
          model: MANAGER_MODEL,
          architectHead: architectGuidance?.head ?? "",
          lanes: {} as Record<string, { action: string; task: string; head: string }>,
        }
        for (const lane of LANES) {
          nextCards.lanes[lane] = {
            action: parsed[lane]!.action,
            task: parsed[lane]!.task,
            head: heads[lane],
          }
        }
        if (architectGuidance?.approved && architectGuidance?.geometryCycle === "HOLD") {
          nextCards.lanes.geometry.action = "HOLD"
        }
        writeAtomic(guidancePath, nextCards)
        for (const lane of LANES) {
          if (nextCards.lanes[lane].action !== "RUN") continue
          const result = await startLaneTask(lane)
          if (result === "started") spawned.push(lane)
        }
        suppressUntil = Date.now() + SPAWN_GRACE_MINUTES * 60_000
        writeAtomic(heartbeatPath, {
          beat: new Date().toISOString(),
          outcome: "directed",
          source: "plugin",
          managerSessionID: managerID,
          spawned,
        })
      } else {
        writeAtomic(heartbeatPath, {
          beat: new Date().toISOString(),
          outcome: "marker_error",
          source: "plugin",
          managerSessionID: managerID,
          replyTail: reply.slice(-400),
        })
      }
      await ctx.storage.set("lastBeat", {
        beat: new Date().toISOString(),
        goalsSeen: architectGuidance?.created ?? null,
        finished: {
          primary: states.primary?.finished ?? null,
          geometry: states.geometry?.finished ?? null,
          verification: states.verification?.finished ?? null,
        },
      })
    }

    void beat().catch((error) => {
      console.error("[fuzion-orchestrator] first beat failed", error)
    })
    const timer = setInterval(() => {
      void beat().catch((error) => {
        console.error("[fuzion-orchestrator] heartbeat failed", error)
      })
    }, HEARTBEAT_MS)
    return () => clearInterval(timer)
  },
})