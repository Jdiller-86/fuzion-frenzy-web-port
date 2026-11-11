"""Compose a read-only PC settings concept from three confirmed UI components.

This creates a technical mockup only. It does not edit assets, install game
content, connect controls, or change settings.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path

import PIL
from PIL import Image, ImageDraw, ImageFont
from private_output import no_links, private_directory


ASSETS = {
    "emblem": "dded0e74ad2bff44135c6a04",
    "divider": "b4a089816ad14c0d16965def",
    "wordmark": "0393008381c77d88832749d1",
}
CANVAS = (1920, 1080)
NAVY = (10, 20, 40, 255)
PANEL = (17, 31, 54, 255)
FOCUS = (245, 249, 255, 255)
DIM = (140, 153, 173, 255)
ACCENT = (235, 52, 54, 255)
METER_ON = (242, 246, 251, 255)
METER_OFF = (52, 68, 91, 255)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for block in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def checked_assets(inventory_path: Path, joins_path: Path) -> tuple[dict, dict]:
    inventory = json.loads(inventory_path.read_text(encoding="utf-8"))
    joins = json.loads(joins_path.read_text(encoding="utf-8"))
    executable_hash = inventory["sourceIdentity"]["executable"]["sha256"]
    if joins.get("executableSha256") != executable_hash:
        raise ValueError("Inventory and join evidence refer to different executables")

    resources = {row["stableId"]: row for row in inventory["resources"]}
    proofs = {row["stableId"]: row for row in joins["resources"]}
    selected = {}
    for role, stable_id in ASSETS.items():
        row = resources.get(stable_id)
        proof = proofs.get(stable_id)
        if not row or not proof:
            raise ValueError(f"Missing inventory row or confirmed join for {stable_id}")
        if proof.get("confidence") != "confirmed" or proof.get("classification") != "ui-texture":
            raise ValueError(f"Asset {stable_id} is not a confirmed UI texture")
        if row.get("decodeStatus") != "decoded" or row.get("frameCount") != 1:
            raise ValueError(f"Asset {stable_id} is not a decoded single-frame image")
        components = proof.get("components", [])
        if not any(c.get("existence") == "confirmed-runtime" and
                   c.get("binding") == "confirmed-static-geometry" for c in components):
            raise ValueError(f"Asset {stable_id} lacks confirmed static-geometry component evidence")
        if not proof.get("atlasRegions") or not all(
                region.get("uvRect") == [0, 0, 1, 1] for region in proof["atlasRegions"]):
            raise ValueError(f"Asset {stable_id} lacks whole-texture UV evidence")
        selected[role] = (row, proof)
    return inventory, {role: pair for role, pair in selected.items()}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", required=True, type=Path)
    parser.add_argument("--joins", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args()

    inventory_path = args.inventory.resolve(strict=True)
    joins_path = args.joins.resolve(strict=True)
    inventory, selected = checked_assets(inventory_path, joins_path)
    out = private_directory(args.out, [inventory_path, joins_path,
                                       inventory["sourceIdentity"]["sourceTree"]])
    image_root = inventory_path.parent.resolve()
    loaded = {}
    records = []
    for role, (row, proof) in selected.items():
        source_candidate = no_links(image_root / row["thumbnail"])
        source = source_candidate.resolve(strict=True)
        if image_root not in source.parents or source.suffix.lower() != ".png":
            raise ValueError("Inventory preview must be a PNG within the private inventory")
        with Image.open(source) as opened:
            rgba = opened.convert("RGBA")
        if rgba.size != (row["width"], row["height"]):
            raise ValueError(f"Preview dimensions disagree for {row['stableId']}")
        loaded[role] = rgba
        records.append({
            "role": role,
            "stableId": row["stableId"],
            "resourceSha256": row["sha256"],
            "previewPath": row["thumbnail"],
            "previewSha256": sha256(source),
            "sourceDimensions": [row["width"], row["height"]],
            "sourceMode": "converted to RGBA for composition; source preview file unchanged",
            "format": row["format"],
            "headerMipLevels": row["headerMipLevels"],
            "joinConfidence": proof["confidence"],
            "confirmedComponents": proof["components"],
            "wholeTextureUvEvidence": [region["uvRect"] for region in proof["atlasRegions"]],
        })

    canvas = Image.new("RGBA", CANVAS, NAVY)
    draw = ImageDraw.Draw(canvas)
    font = ImageFont.load_default(size=34)
    small = ImageFont.load_default(size=24)
    title = ImageFont.load_default(size=48)

    # Static PC settings concept: all positions and values below are proposed.
    draw.rounded_rectangle((128, 92, 1792, 988), radius=26, fill=PANEL,
                           outline=(38, 57, 82, 255), width=2)
    # A simple block F motif is original geometry, not a reconstructed title glyph.
    draw.rectangle((180, 142, 216, 224), fill=FOCUS)
    draw.rectangle((180, 142, 248, 160), fill=FOCUS)
    draw.rectangle((180, 174, 238, 192), fill=FOCUS)
    canvas.alpha_composite(loaded["emblem"].resize((72, 72), Image.Resampling.NEAREST), (272, 132))
    draw.text((374, 142), "SETTINGS", font=title, fill=FOCUS)
    draw.text((374, 198), "PROPOSED PC OPTIONS", font=small, fill=DIM)

    # Reuse the confirmed divider texture as an unchanged source image, scaled
    # only for presentation inside this preview canvas.
    divider = loaded["divider"].resize((1500, 20), Image.Resampling.BILINEAR)
    canvas.alpha_composite(divider, (210, 246))
    # Wordmark is shown at source aspect ratio; no retouching or cropping.
    logo = loaded["wordmark"].resize((512, 128), Image.Resampling.LANCZOS)
    canvas.alpha_composite(logo, (1200, 112))

    labels = ["VSync", "Render Scale", "FPS Limit", "Aspect Ratio"]
    values = ["On", "100%", "60 FPS", "16:9"]
    top = 304
    row_height = 132
    bar_count = 10
    for index, (label, value) in enumerate(zip(labels, values)):
        y = top + index * row_height
        focused = index == 0
        color = FOCUS if focused else DIM
        if focused:
            draw.rounded_rectangle((204, y - 20, 1716, y + 92), radius=14,
                                   fill=(29, 45, 68, 255), outline=(91, 112, 143, 255), width=2)
            draw.rectangle((204, y - 20, 212, y + 92), fill=ACCENT)
        draw.text((252, y + 4), label, font=font, fill=color)
        if index == 0:
            draw.text((1130, y + 4), value, font=font, fill=color)
            draw.rounded_rectangle((1454, y - 6, 1658, y + 42), radius=12,
                                   outline=color, width=2)
            draw.text((1518, y + 9), "YES", font=font, fill=color)
        else:
            # Meter geometry is a PC mockup control, not the original game's
            # measured volume meter or a claim about the native control UI.
            active = [7, 6, 5][index - 1]
            start_x = 1100
            segment_w = 34
            gap = 8
            for segment in range(bar_count):
                x = start_x + segment * (segment_w + gap)
                draw.rounded_rectangle((x, y + 3, x + segment_w, y + 34), radius=5,
                                       fill=METER_ON if segment < active else METER_OFF)
            draw.text((1650, y + 8), value, font=font, fill=color)

    draw.text((252, 860), "ARROWS / D-PAD  CHANGE VALUE", font=small, fill=DIM)
    draw.text((252, 892), "ESC / B  CLOSE", font=small, fill=DIM)
    draw.text((252, 922), "READ-ONLY COMPONENT PREVIEW - proposed PC settings; substitute font and layout, not original screen",
              font=small, fill=(255, 207, 105, 255))
    draw.text((252, 956), "Mock values only. No apply, persistence, guest binding, or original UI behavior is implemented.",
              font=small, fill=DIM)

    image_path = out / "preview.png"
    no_links(image_path)
    canvas.convert("RGB").save(image_path, format="PNG", optimize=True)
    recipe = {
        "schemaVersion": 1,
        "kind": "private-read-only-settings-component-preview",
        "purpose": "Technical composition from three confirmed UI component bindings; not an original screen reproduction.",
        "installed": False,
        "settingsAppliedOrPersisted": False,
        "pillowVersion": PIL.__version__,
        "canvasPixels": list(CANVAS),
        "coordinates": {
            "layout": "Proposed arbitrary 1920x1080 PC composition; not guest coordinates.",
            "panel": [128, 92, 1792, 988],
            "headerDivider": [210, 246, 1710, 266],
            "rows": [{"name": name, "value": value,
                      "bounds": [204, top + i * row_height - 20, 1716, top + i * row_height + 92],
                      "focus": i == 0,
                      "valueTreatment": "text toggle" if i == 0 else "10-segment proposed meter geometry",
                      "activeSegments": None if i == 0 else [7, 6, 5][i - 1]}
                     for i, (name, value) in enumerate(zip(labels, values))],
            "emblem": [272, 132, 344, 204],
            "wordmark": [1200, 112, 1712, 240],
            "font": "Pillow ImageFont.load_default() at 24/34/48 px; not the original game's font.",
        },
        "evidence": {
            "executableSha256": inventory["sourceIdentity"]["executable"]["sha256"],
            "joinEvidenceExecutableSha256": json.loads(joins_path.read_text(encoding="utf-8"))["executableSha256"],
            "inventoryPath": str(inventory_path),
            "joinEvidencePath": str(joins_path),
            "assets": records,
            "limitations": [
                "Confirmed bindings do not prove per-draw runtime identity or global overlay coordinates.",
                "Whole-texture UV evidence is used only to place source previews in this composition.",
                "The VSync, render scale, FPS limit, and aspect ratio labels/values are proposed host settings.",
                "No actual controls, apply action, persistence, guest setting effect, or renderer hook exists here.",
                "Source previews are not modified; images are resized only for display in the composite.",
            ],
        },
        "output": {
            "file": "preview.png",
            "sha256": sha256(image_path),
            "colorMode": "RGB",
            "dimensions": list(CANVAS),
        },
    }
    recipe_path = no_links(out / "recipe.json")
    recipe_path.write_text(json.dumps(recipe, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"preview": str(image_path), "recipe": str(recipe_path),
                      "sha256": recipe["output"]["sha256"], "assets": len(records),
                      "pillowVersion": PIL.__version__, "installed": False}))


if __name__ == "__main__":
    main()
