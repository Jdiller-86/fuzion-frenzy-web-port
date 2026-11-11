using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;

namespace FuzionFrenzyLauncher;

internal static class Program
{
    [STAThread]
    private static void Main()
    {
        ApplicationConfiguration.Initialize();
        Application.Run(new LauncherForm());
    }
}

internal sealed class LauncherForm : Form
{
    private static readonly Color Bg = Color.FromArgb(9, 15, 24);
    private static readonly Color Panel = Color.FromArgb(17, 28, 41);
    private static readonly Color Panel2 = Color.FromArgb(24, 40, 55);
    private static readonly Color TextColor = Color.FromArgb(238, 242, 249);
    private static readonly Color Muted = Color.FromArgb(164, 177, 198);
    private static readonly Color Accent = Color.FromArgb(74, 225, 235);
    private static readonly string UserRoot = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "FuzionFrenzy");
    private readonly string preferencesFile = Path.Combine(UserRoot, "launcher-settings.json");
    private readonly TextBox sourcePath = new();
    private readonly TextBox workspacePath = new();
    private readonly TextBox gamePath = new();
    private readonly TextBox buildLog = new();
    private readonly Label status = new();
    private readonly Label gameState = new();
    private readonly SegmentProgress buildProgress = new();
    private readonly System.Windows.Forms.Timer statusTimer = new() { Interval = 500 };
    private readonly Button buildButton = new TechButton();
    private readonly Button cancelButton = new TechButton();
    private readonly NumericUpDown jobs = new();
    private readonly ComboBox displayMode = new();
    private readonly NumericUpDown width = new();
    private readonly NumericUpDown height = new();
    private readonly ComboBox aspect = new();
    private readonly CheckBox ultrawide = new();
    private readonly CheckBox vsync = new();
    private readonly NumericUpDown renderScale = new();
    private readonly NumericUpDown frameCap = new();
    private readonly NumericUpDown anisotropic = new();
    private readonly ComboBox hdr = new();
    private readonly ComboBox uiArtwork = new();
    private Process? activeProcess;
    private bool loading;
    private bool cancelRequested;

    public LauncherForm()
    {
        Text = "Fuzion Frenzy · Windows Launcher v0.1.4";
        StartPosition = FormStartPosition.CenterScreen;
        MinimumSize = new Size(960, 740);
        Size = new Size(1120, 940);
        AutoScaleMode = AutoScaleMode.Dpi;
        BackColor = Bg;
        ForeColor = TextColor;
        Font = new Font("Segoe UI", 9.5f);
        BuildLayout();
        LoadPreferences();
        uiArtwork.SelectedIndexChanged += (_, _) => SavePreferences();
        sourcePath.TextChanged += (_, _) => SavePreferences();
        workspacePath.TextChanged += (_, _) => SavePreferences();
        gamePath.TextChanged += (_, _) => { SavePreferences(); RefreshGameState(); };
        RefreshGameState();
        statusTimer.Tick += (_, _) => ReadBuildStatus();
        FormClosing += (_, _) => { if (activeProcess is { HasExited: false }) activeProcess.Kill(entireProcessTree: true); };
    }

    private void BuildLayout()
    {
        var shell = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 1, RowCount = 3, BackColor = Bg, Padding = new Padding(24, 18, 24, 12) };
        shell.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
        shell.RowStyles.Add(new RowStyle(SizeType.Absolute, 56));
        shell.RowStyles.Add(new RowStyle(SizeType.Percent, 100));
        shell.RowStyles.Add(new RowStyle(SizeType.Absolute, 30));
        Controls.Add(shell);
        var navigation = new FlowLayoutPanel { Dock = DockStyle.Fill, WrapContents = false, Margin = new Padding(0), BackColor = Bg };
        shell.Controls.Add(navigation, 0, 0);
        var host = new System.Windows.Forms.Panel { Dock = DockStyle.Fill, Margin = new Padding(0), BackColor = Bg };
        shell.Controls.Add(host, 0, 1);
        TableLayoutPanel MakePage() {
            var result = new TableLayoutPanel { Dock = DockStyle.Fill, AutoScroll = true, ColumnCount = 1, BackColor = Bg, Padding = new Padding(0, 0, 12, 8), Margin = new Padding(0) };
            result.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
            host.Controls.Add(result);
            return result;
        }
        var page = MakePage();
        var settingsPage = MakePage();
        var controlsPage = MakePage();
        var pages = new[] { page, settingsPage, controlsPage };
        var tabs = new List<TechButton>();
        void ShowPage(int index) {
            for (int n = 0; n < pages.Length; n++) { pages[n].Visible = n == index; tabs[n].Selected = n == index; }
            pages[index].BringToFront();
        }
        string[] names = { "01   BUILD + PLAY", "02   GAME SETTINGS", "03   CONTROLS" };
        for (int n = 0; n < names.Length; n++) {
            int index = n;
            var tab = new TechButton { Text = names[n], Width = 202, Height = 40, Margin = new Padding(0, 0, 10, 0), AccessibleName = names[n][5..] };
            tab.Click += (_, _) => ShowPage(index);
            tabs.Add(tab); navigation.Controls.Add(tab);
        }
        ShowPage(0);
        var footer = new Label { Text = "LOCAL FILES ONLY  /  NO GAME DATA UPLOADED                                      WINDOWS x64  ·  BUILDER 0.1.4", ForeColor = Muted, Font = new Font("Bahnschrift", 8.5f), Dock = DockStyle.Fill, TextAlign = ContentAlignment.MiddleLeft, AutoEllipsis = true, Margin = new Padding(0, 6, 0, 0) };
        shell.Controls.Add(footer, 0, 2);

        var statusPanel = Card();
        var statusLine = new FlowLayoutPanel { Dock = DockStyle.Fill, AutoSize = true, WrapContents = true, BackColor = Panel, Margin = new Padding(0) };
        status.Text = "Ready to build"; status.Font = new Font(Font, FontStyle.Bold); status.ForeColor = Accent; status.AutoSize = true; status.Margin = new Padding(0, 4, 16, 2);
        gameState.Text = "No game build found"; gameState.ForeColor = Muted; gameState.AutoSize = true; gameState.Margin = new Padding(0, 5, 0, 2);
        statusLine.Controls.Add(status); statusLine.Controls.Add(gameState); statusPanel.Controls.Add(statusLine);
        page.Controls.Add(statusPanel);

        var sourceCard = SectionCard("01 / YOUR GAME DISC", "Select your Fuzion Frenzy ISO or extracted game folder. Everything stays on your PC.");
        var sourceRow = PathRow(sourcePath, "Choose ISO", ChooseSource, "Select a disc image or extracted game folder");
        sourceCard.Controls.Add(sourceRow);
        page.Controls.Add(sourceCard);

        var locationCard = SectionCard("BUILD LOCATIONS", "Defaults keep your game and working files in your Windows user profile.");
        locationCard.Controls.Add(PathRow(workspacePath, "Browse…", () => ChooseFolder(workspacePath, "Choose build workspace"), "Build workspace"));
        locationCard.Controls.Add(PathRow(gamePath, "Browse…", () => ChooseFolder(gamePath, "Choose game folder"), "Game folder"));
        locationCard.Visible = false;
        var locationToggle = MakeButton("Edit build locations", false, () => { locationCard.Visible = !locationCard.Visible; });
        locationToggle.Width = 200;
        var sourceActions = new FlowLayoutPanel { AutoSize = true, Dock = DockStyle.Top, WrapContents = true, Margin = new Padding(0, 8, 0, 0) };
        sourceActions.Controls.Add(MakeButton("Use extracted folder", false, () => ChooseFolder(sourcePath, "Choose extracted game files")));
        sourceActions.Controls.Add(locationToggle);
        sourceCard.Controls.Add(sourceActions);
        page.Controls.Add(locationCard);

        var buildCard = SectionCard("02 / BUILD + LAUNCH", "Compile the executable locally. Existing saves and settings are retained.");
        var actions = new FlowLayoutPanel { AutoSize = true, Dock = DockStyle.Top, WrapContents = true, BackColor = Panel, Margin = new Padding(0, 6, 0, 3) };
        var prerequisites = MakeButton("Install Build Tools", false); prerequisites.Width = 176; prerequisites.Click += async (_, _) => await InstallTools();
        buildButton.Text = "Build Game"; buildButton.Width = 145; buildButton.Height = 38; StyleButton(buildButton, true); buildButton.Click += async (_, _) => await RunBuild();
        cancelButton.Text = "Cancel Build"; cancelButton.Width = 120; cancelButton.Height = 38; StyleButton(cancelButton, false); cancelButton.Enabled = false; cancelButton.Click += (_, _) => CancelBuild();
        var play = MakeButton("Play Game", false); play.Width = 125; play.Click += (_, _) => PlayGame();
        var jobsLabel = new Label { Text = "Parallel jobs", ForeColor = Muted, AutoSize = true, Margin = new Padding(12, 11, 2, 0) };
        jobs.Minimum = 1; jobs.Maximum = Math.Max(1, Environment.ProcessorCount); jobs.Value = Math.Max(1, Math.Min(Environment.ProcessorCount, 8)); jobs.Width = 60; jobs.Margin = new Padding(0, 4, 8, 0); jobs.BackColor = Panel2; jobs.ForeColor = TextColor;
        actions.Controls.Add(buildButton); actions.Controls.Add(play); actions.Controls.Add(cancelButton); actions.Controls.Add(prerequisites); actions.Controls.Add(jobsLabel); actions.Controls.Add(jobs);
        buildCard.Controls.Add(actions);
        buildProgress.Dock = DockStyle.Top; buildProgress.Height = 12; buildProgress.Minimum = 0; buildProgress.Maximum = 100; buildProgress.Margin = new Padding(0, 4, 0, 0); buildCard.Controls.Add(buildProgress);
        buildLog.Multiline = true; buildLog.ReadOnly = true; buildLog.ScrollBars = ScrollBars.Vertical; buildLog.WordWrap = false; buildLog.Dock = DockStyle.Top; buildLog.Height = 154; buildLog.BackColor = Color.FromArgb(8, 17, 25); buildLog.ForeColor = Color.FromArgb(157, 204, 216); buildLog.BorderStyle = BorderStyle.None; buildLog.Font = new Font("Consolas", 9); buildLog.Margin = new Padding(0, 8, 0, 2);
        buildCard.Controls.Add(new Label { Text = "BUILD ACTIVITY", Font = new Font("Bahnschrift", 8.5f), ForeColor = Accent, AutoSize = true, Margin = new Padding(0, 12, 0, 2) });
        buildCard.Controls.Add(buildLog); page.Controls.Add(buildCard);

        var tools = new FlowLayoutPanel { Dock = DockStyle.Top, AutoSize = true, WrapContents = true, BackColor = Panel, Margin = new Padding(0, 8, 0, 0) };
        tools.Controls.Add(MakeButton("Open Game Folder", false, () => OpenPath(gamePath.Text)));
        tools.Controls.Add(MakeButton("Open Saves", false, () => OpenPath(Path.Combine(gamePath.Text, "hdd"))));
        tools.Controls.Add(MakeButton("Open Logs", false, () => OpenPath(Path.Combine(gamePath.Text, "logs"))));
        buildCard.Controls.Add(tools);

        var settingsCard = SectionCard("DISPLAY + QUALITY", "Tune your next session. Save to apply these preferences to the game.");
        var grid = new TableLayoutPanel { Dock = DockStyle.Top, AutoSize = true, ColumnCount = 4, RowCount = 6, BackColor = Panel, Margin = new Padding(0, 6, 0, 0) };
        grid.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 115)); grid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50)); grid.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 115)); grid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50));
        AddField(grid, 0, 0, "Display mode", displayMode, new[] { "borderless", "fullscreen", "windowed" });
        ConfigureNumber(width, 0, 16384, 0, 100); ConfigureNumber(height, 0, 16384, 0, 100);
        AddField(grid, 1, 0, "Width", width); AddField(grid, 1, 2, "Height", height);
        AddField(grid, 2, 0, "Aspect ratio", aspect, new[] { "auto", "4:3", "16:9", "16:10", "21:9", "32:9" });
        vsync.Text = "Enabled"; vsync.AutoSize = true; vsync.BackColor = Panel; vsync.ForeColor = TextColor; vsync.Margin = new Padding(3, 7, 3, 3);
        AddField(grid, 2, 2, "VSync", vsync);
        ConfigureNumber(renderScale, 25, 400, 100, 1); ConfigureNumber(frameCap, 0, 1000, 60, 5); ConfigureNumber(anisotropic, 0, 16, 0, 1);
        AddField(grid, 3, 0, "Render scale (%)", renderScale); AddField(grid, 3, 2, "Frame cap (0 = off)", frameCap);
        AddField(grid, 4, 0, "Anisotropic filter", anisotropic);
        AddField(grid, 4, 2, "HDR (Windows)", hdr, new[] { "off", "auto", "on" });
        ultrawide.Text = "Widen on wide displays"; ultrawide.Checked = true; ultrawide.AutoSize = true; ultrawide.BackColor = Panel; ultrawide.ForeColor = TextColor; ultrawide.Margin = new Padding(3, 7, 3, 3);
        AddField(grid, 5, 0, "Ultrawide", ultrawide);
        AddField(grid, 5, 2, "UI artwork", uiArtwork, new[] { "4x preview", "Original pixels", "Replacements off" });
        settingsCard.Controls.Add(grid);
        var saveSettings = MakeButton("Save Game Settings", true); saveSettings.Click += (_, _) => SaveGameSettings();
        var settingsNote = new FlowLayoutPanel { Dock = DockStyle.Top, AutoSize = true, BackColor = Panel, Margin = new Padding(0, 8, 0, 0) }; settingsNote.Controls.Add(saveSettings);
        settingsNote.Controls.Add(new Label { Text = "F1: display settings. Pause gameplay first. UI artwork takes effect on Play.", ForeColor = Muted, AutoSize = true, Margin = new Padding(7, 11, 0, 0) });
        settingsCard.Controls.Add(settingsNote); settingsPage.Controls.Add(settingsCard);
        var settingsHelp = SectionCard("IN-GAME ADJUSTMENTS", "F1 opens the styled display panel. Pause the game first; the panel itself does not pause gameplay.");
        settingsHelp.Controls.Add(new Label { Text = "Apply changes live. Save keeps them for next time. Cancel discards unapplied edits.\nUI artwork mode is saved automatically here and takes effect on your next launch.", AutoSize = true, ForeColor = Muted, Margin = new Padding(0, 8, 0, 0) });
        settingsPage.Controls.Add(settingsHelp);

        var controlsCard = SectionCard("KEYBOARD + CONTROLLER", "Use the keyboard and mouse as player one, or connect a controller. Input follows the focused game window.");
        controlsCard.Controls.Add(new Label { Text = "W A S D: left stick   ·   Arrow keys: D-pad   ·   Right mouse + mouse: right stick\nSpace: A   ·   Escape / Backspace: B   ·   X / Y: X / Y   ·   Z / C: triggers\n1 / 2: black / white   ·   Enter: Start   ·   Tab: Back   ·   Shift / Ctrl: stick clicks\nF1: in-game settings   ·   F2: frame-time graph", AutoSize = true, ForeColor = Muted, BackColor = Panel, Margin = new Padding(0, 5, 0, 0) });
        controlsPage.Controls.Add(controlsCard);
        var setupHelp = SectionCard("FIRST BUILD", "Install Build Tools if prompted, select your disc, then choose Build Game.");
        setupHelp.Controls.Add(new Label { Text = "The first build downloads the compiler and source code and may take a while.\nLater builds reuse the local cache. Your game files are never uploaded.\nRequires Windows 10/11 x64 and a D3D12-capable graphics card.", AutoSize = true, ForeColor = Muted, Margin = new Padding(0, 8, 0, 0) });
        controlsPage.Controls.Add(setupHelp);
    }

    private static void ConfigureNumber(NumericUpDown number, decimal min, decimal max, decimal value, decimal increment)
    { number.Minimum = min; number.Maximum = max; number.Value = value; number.Increment = increment; number.Width = 135; number.BackColor = Panel2; number.ForeColor = TextColor; number.Margin = new Padding(3, 3, 8, 4); }

    private static void AddField(TableLayoutPanel grid, int row, int col, string name, Control control, string[]? options = null)
    {
        if (options is not null && control is ComboBox combo) { combo.DropDownStyle = ComboBoxStyle.DropDownList; combo.Width = 145; combo.BackColor = Panel2; combo.ForeColor = TextColor; combo.Items.AddRange(options); combo.SelectedIndex = 0; combo.Margin = new Padding(3, 3, 8, 4); }
        var label = new Label { Text = name, ForeColor = Muted, AutoSize = true, Anchor = AnchorStyles.Left, Margin = new Padding(0, 8, 4, 4) };
        control.Dock = DockStyle.Fill;
        grid.Controls.Add(label, col, row); grid.Controls.Add(control, col + 1, row);
    }

    private static System.Windows.Forms.Panel Card() => new() { Dock = DockStyle.Top, AutoSize = true, BackColor = Panel, Padding = new Padding(18, 12, 18, 12), Margin = new Padding(0, 0, 0, 14) };
    private static TableLayoutPanel SectionCard(string heading, string description)
    {
        var p = new ConsoleCard { Dock = DockStyle.Top, AutoSize = true, AutoSizeMode = AutoSizeMode.GrowAndShrink, ColumnCount = 1, BackColor = Panel, Padding = new Padding(20, 18, 20, 18), Margin = new Padding(0, 0, 0, 14) };
        p.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
        p.Controls.Add(new Label { Text = heading, Font = new Font("Bahnschrift", 11, FontStyle.Bold), ForeColor = TextColor, AutoSize = true, Margin = new Padding(0, 0, 0, 5) });
        p.Controls.Add(new Label { Text = description, ForeColor = Muted, AutoSize = true, Dock = DockStyle.Top, Margin = new Padding(0, 2, 0, 12) });
        return p;
    }

    private Control PathRow(TextBox box, string buttonText, Action browse, string hint)
    {
        box.Dock = DockStyle.Fill; box.BackColor = Panel2; box.ForeColor = TextColor; box.BorderStyle = BorderStyle.FixedSingle; box.Margin = new Padding(0, 4, 8, 4); box.PlaceholderText = hint;
        var row = new TableLayoutPanel { Dock = DockStyle.Top, AutoSize = true, ColumnCount = 2, BackColor = Panel, Margin = new Padding(0, 0, 0, 2) };
        row.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100)); row.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 112));
        var btn = MakeButton(buttonText, false); btn.Width = 105; btn.Click += (_, _) => browse();
        row.Controls.Add(box, 0, 0); row.Controls.Add(btn, 1, 0); return row;
    }

    private static Button MakeButton(string label, bool primary, Action? action = null)
    {
        var b = new TechButton { Text = label, AutoSize = false, Height = 36, Width = 155, FlatStyle = FlatStyle.Flat, BackColor = primary ? Accent : Panel2, ForeColor = primary ? Color.FromArgb(9, 20, 34) : TextColor, Font = new Font("Segoe UI", 9.5f, FontStyle.Bold), Margin = new Padding(0, 2, 8, 2), Cursor = Cursors.Hand };
        b.FlatAppearance.BorderColor = primary ? Accent : Color.FromArgb(66, 82, 107); b.FlatAppearance.BorderSize = 1;
        b.Primary = primary;
        if (action is not null) b.Click += (_, _) => action();
        return b;
    }
    private static void StyleButton(Button b, bool primary)
    {
        b.FlatStyle = FlatStyle.Flat; b.Font = new Font("Segoe UI", 9.5f, FontStyle.Bold); b.Cursor = Cursors.Hand; b.Margin = new Padding(0, 2, 8, 2);
        if (b is TechButton tech) tech.Primary = primary;
    }

    private void ChooseSource()
    {
        using var dlg = new OpenFileDialog { Title = "Choose your local Fuzion Frenzy disc image", Filter = "Xbox disc images (*.iso;*.xiso)|*.iso;*.xiso|All files (*.*)|*.*", CheckFileExists = true };
        if (dlg.ShowDialog(this) == DialogResult.OK) sourcePath.Text = dlg.FileName;
    }

    private void ChooseFolder(TextBox target, string description)
    {
        using var dlg = new FolderBrowserDialog { Description = description, UseDescriptionForTitle = true, ShowNewFolderButton = true };
        if (Directory.Exists(target.Text)) dlg.SelectedPath = target.Text;
        if (dlg.ShowDialog(this) == DialogResult.OK) target.Text = dlg.SelectedPath;
    }

    private async Task RunBuild()
    {
        var script = Path.Combine(AppContext.BaseDirectory, "scripts", "Build-Game.ps1");
        if (!File.Exists(script)) { MessageBox.Show(this, "The bundled Build-Game.ps1 script is missing. Reinstall the launcher to restore it.", "Build script unavailable", MessageBoxButtons.OK, MessageBoxIcon.Error); return; }
        if (!Directory.Exists(sourcePath.Text) && !File.Exists(sourcePath.Text)) { MessageBox.Show(this, "Choose an existing ISO/XISO image or extracted game folder first.", "Game files needed", MessageBoxButtons.OK, MessageBoxIcon.Information); return; }
        if (string.IsNullOrWhiteSpace(workspacePath.Text) || string.IsNullOrWhiteSpace(gamePath.Text)) return;
        try { Directory.CreateDirectory(workspacePath.Text); Directory.CreateDirectory(gamePath.Text); SavePreferences(); }
        catch (Exception ex) { MessageBox.Show(this, ex.Message, "Build location unavailable", MessageBoxButtons.OK, MessageBoxIcon.Error); return; }
        var psi = PowerShellStart(script); psi.RedirectStandardOutput = true; psi.RedirectStandardError = true; psi.UseShellExecute = false; psi.CreateNoWindow = true;
        psi.ArgumentList.Add("-InputPath"); psi.ArgumentList.Add(sourcePath.Text); psi.ArgumentList.Add("-Workspace"); psi.ArgumentList.Add(workspacePath.Text); psi.ArgumentList.Add("-GameDirectory"); psi.ArgumentList.Add(gamePath.Text); psi.ArgumentList.Add("-Jobs"); psi.ArgumentList.Add(((int)jobs.Value).ToString());
        psi.ArgumentList.Add("-StatusPath"); psi.ArgumentList.Add(Path.Combine(workspacePath.Text, "build-status.json"));
        try
        {
            activeProcess = new Process { StartInfo = psi, EnableRaisingEvents = true };
            if (!activeProcess.Start()) throw new InvalidOperationException("PowerShell did not start.");
            cancelRequested = false;
            buildButton.Enabled = false; cancelButton.Enabled = true; status.Text = "Building…"; status.ForeColor = Accent; Log("Starting local Windows build.");
            buildProgress.Value = 0; statusTimer.Start();
            var outputTask = Pump(activeProcess.StandardOutput); var errorTask = Pump(activeProcess.StandardError);
            await activeProcess.WaitForExitAsync(); await Task.WhenAll(outputTask, errorTask);
            if (cancelRequested) { status.Text = "Build cancelled"; status.ForeColor = Muted; Log("Build cancelled."); }
            else if (activeProcess.ExitCode == 0) { status.Text = "Build complete"; status.ForeColor = Color.FromArgb(119, 224, 165); buildProgress.Value = 100; Log("Build completed successfully."); }
            else { status.Text = $"Build failed ({activeProcess.ExitCode})"; status.ForeColor = Color.FromArgb(255, 149, 137); Log($"Build script exited with code {activeProcess.ExitCode}."); }
        }
        catch (Exception ex) { status.Text = "Build could not start"; status.ForeColor = Color.FromArgb(255, 149, 137); Log(ex.Message); MessageBox.Show(this, ex.Message, "Build failed to start", MessageBoxButtons.OK, MessageBoxIcon.Error); }
        finally { statusTimer.Stop(); activeProcess?.Dispose(); activeProcess = null; buildButton.Enabled = true; cancelButton.Enabled = false; RefreshGameState(); }
    }

    private void ReadBuildStatus()
    {
        var file = Path.Combine(workspacePath.Text, "build-status.json");
        if (!File.Exists(file)) return;
        try
        {
            using var doc = JsonDocument.Parse(File.ReadAllText(file)); var root = doc.RootElement;
            int progress = root.TryGetProperty("progress", out var p) && p.TryGetInt32(out var n) ? Math.Clamp(n, 0, 100) : 0;
            buildProgress.Value = progress;
            string phase = root.TryGetProperty("phase", out var ph) ? ph.GetString() ?? "Building" : "Building";
            string message = root.TryGetProperty("message", out var msg) ? msg.GetString() ?? "" : "";
            string state = root.TryGetProperty("status", out var st) ? st.GetString() ?? "building" : "building";
            bool terminal = state.Equals("failed", StringComparison.OrdinalIgnoreCase) || state.Equals("succeeded", StringComparison.OrdinalIgnoreCase) || state.Equals("completed", StringComparison.OrdinalIgnoreCase) || state.Equals("cancelled", StringComparison.OrdinalIgnoreCase);
            status.Text = terminal && message.Length > 0 ? message : $"{phase} · {progress}%";
        }
        catch (IOException) { } catch (JsonException) { }
    }

    private async Task Pump(StreamReader reader)
    {
        while (await reader.ReadLineAsync() is { } line)
        {
            if (IsDisposed) return;
            if (InvokeRequired) BeginInvoke(new Action(() => Log(line))); else Log(line);
        }
    }
    private void CancelBuild()
    {
        if (activeProcess is not { HasExited: false } p) return;
        status.Text = "Stopping build…"; Log("Cancellation requested; stopping the build process tree.");
        cancelRequested = true;
        try { p.Kill(entireProcessTree: true); } catch (InvalidOperationException) { }
    }

    private async Task InstallTools()
    {
        if (activeProcess is { HasExited: false }) { Log("Wait for the current operation to finish, or cancel it first."); return; }
        var script = Path.Combine(AppContext.BaseDirectory, "scripts", "Install-Prerequisites.ps1");
        if (!File.Exists(script)) { MessageBox.Show(this, "The bundled Install-Prerequisites.ps1 script is missing. Reinstall the launcher to restore it.", "Installer unavailable", MessageBoxButtons.OK, MessageBoxIcon.Error); return; }
        var answer = MessageBox.Show(this, "This step downloads and installs the Windows build toolchain. Downloads are large and Windows may ask for administrator approval. Continue?", "Install Windows Build Tools", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning);
        if (answer != DialogResult.OK) return;
        try
        {
            var logDirectory = Path.Combine(UserRoot, "logs");
            Directory.CreateDirectory(logDirectory);
            var logPath = Path.Combine(logDirectory, $"prerequisites-{DateTime.Now:yyyyMMdd-HHmmss}.log");
            var psi = PowerShellStart(script); psi.ArgumentList.Add("-Install");
            psi.ArgumentList.Add("-LogPath"); psi.ArgumentList.Add(logPath);
            // Keep WinGet in the user's session. Individual installers request
            // elevation; their parent must retain its output and user aliases.
            psi.UseShellExecute = false; psi.CreateNoWindow = true;
            psi.RedirectStandardOutput = true; psi.RedirectStandardError = true;
            activeProcess = new Process { StartInfo = psi };
            if (!activeProcess.Start()) throw new InvalidOperationException("PowerShell did not start.");
            buildButton.Enabled = false; cancelButton.Enabled = true;
            status.Text = "Installing build tools…"; status.ForeColor = Accent;
            Log($"Installing prerequisites. Log: {logPath}");
            var outputTask = Pump(activeProcess.StandardOutput); var errorTask = Pump(activeProcess.StandardError);
            await activeProcess.WaitForExitAsync(); await Task.WhenAll(outputTask, errorTask);
            status.Text = activeProcess.ExitCode != 0 ? "Build tools installation failed" : "Build tools installer finished";
            if (activeProcess.ExitCode != 0) Log($"Prerequisite installer exited with code {activeProcess.ExitCode}. Full details: {logPath}");
            RefreshGameState();
        }
        catch (System.ComponentModel.Win32Exception ex) when (ex.NativeErrorCode == 1223)
        { status.Text = "Administrator approval was cancelled"; Log("Windows administrator approval was cancelled."); }
        catch (Exception ex) { status.Text = "Could not start installer"; Log(ex.Message); MessageBox.Show(this, ex.Message, "Installer unavailable", MessageBoxButtons.OK, MessageBoxIcon.Error); }
        finally { activeProcess?.Dispose(); activeProcess = null; buildButton.Enabled = true; cancelButton.Enabled = false; }
    }

    private static ProcessStartInfo PowerShellStart(string script)
    {
        var psi = new ProcessStartInfo("powershell.exe") { WorkingDirectory = AppContext.BaseDirectory };
        psi.ArgumentList.Add("-NoProfile"); psi.ArgumentList.Add("-ExecutionPolicy"); psi.ArgumentList.Add("Bypass"); psi.ArgumentList.Add("-File"); psi.ArgumentList.Add(script);
        return psi;
    }

    private void PlayGame()
    {
        var exe = Path.Combine(gamePath.Text, "FuzionFrenzy.exe");
        if (!File.Exists(exe)) { MessageBox.Show(this, "Build the game first. The launcher will look for FuzionFrenzy.exe in the selected game folder.", "Game not built", MessageBoxButtons.OK, MessageBoxIcon.Information); return; }
        try {
            var psi = new ProcessStartInfo(exe) { WorkingDirectory = gamePath.Text, UseShellExecute = false };
            var pack = Path.Combine(Path.GetFullPath(gamePath.Text), "ui.bgra");
            // Scope settings to this child; never change the user's environment.
            psi.Environment.Remove("FUZION_UI_PACK");
            psi.Environment["FUZION_UI_REPLACE"] = uiArtwork.SelectedIndex switch { 1 => "identity", 2 => "off", _ => "pilot" };
            if (File.Exists(pack)) psi.Environment["FUZION_UI_PACK"] = pack;
            else Log("No local UI artwork pack found. Rebuild with this launcher to add it.");
            Process.Start(psi); Log("Started FuzionFrenzy.exe. F1 opens settings; pause gameplay first.");
        }
        catch (Exception ex) { MessageBox.Show(this, ex.Message, "Could not start game", MessageBoxButtons.OK, MessageBoxIcon.Error); }
    }
    private void RefreshGameState()
    {
        var exe = Path.Combine(gamePath.Text, "FuzionFrenzy.exe");
        gameState.Text = File.Exists(exe) ? $"Game ready · {File.GetLastWriteTime(exe):g}" : "No game build found";
        gameState.ForeColor = File.Exists(exe) ? Color.FromArgb(119, 224, 165) : Muted;
    }
    private void OpenPath(string path)
    {
        if (string.IsNullOrWhiteSpace(path)) return;
        try { Directory.CreateDirectory(path); Process.Start(new ProcessStartInfo("explorer.exe") { UseShellExecute = true, ArgumentList = { path } }); }
        catch (Exception ex) { MessageBox.Show(this, ex.Message, "Could not open folder", MessageBoxButtons.OK, MessageBoxIcon.Error); }
    }

    private void SaveGameSettings()
    {
        var exe = Path.Combine(gamePath.Text, "FuzionFrenzy.exe");
        if (!File.Exists(exe)) { MessageBox.Show(this, "Build the game before editing its settings.", "Game not built", MessageBoxButtons.OK, MessageBoxIcon.Information); return; }
        var file = Path.Combine(gamePath.Text, "settings.toml");
        var values = new Dictionary<string, string> {
            ["display_mode"] = Quote(displayMode.Text), ["width"] = ((int)width.Value).ToString(), ["height"] = ((int)height.Value).ToString(),
            ["render_percent"] = ((int)renderScale.Value).ToString(), ["fps_cap"] = ((int)frameCap.Value).ToString(), ["vsync"] = vsync.Checked ? "true" : "false",
            ["anisotropic_filtering"] = ((int)anisotropic.Value).ToString(), ["aspect_ratio"] = Quote(aspect.Text), ["ultrawide"] = ultrawide.Checked ? "true" : "false", ["hdr"] = Quote(hdr.Text)
        };
        try
        {
            string content = File.Exists(file) ? File.ReadAllText(file) : string.Empty;
            File.WriteAllText(file, MergeToml(content, values), new UTF8Encoding(false));
            SavePreferences(); Log("Game settings saved. Unknown existing settings and comments were preserved."); status.Text = "Settings saved"; status.ForeColor = Color.FromArgb(119, 224, 165);
        }
        catch (Exception ex) { MessageBox.Show(this, ex.Message, "Could not save settings", MessageBoxButtons.OK, MessageBoxIcon.Error); }
    }
    private static string MergeToml(string existing, Dictionary<string, string> updates)
    {
        var lines = existing.Replace("\r\n", "\n").Split('\n').ToList();
        var found = new HashSet<string>(StringComparer.Ordinal);
        var keyPattern = new Regex(@"^(\s*)([A-Za-z0-9_-]+)(\s*=\s*)(.*)$");
        bool topLevel = true;
        for (int i = 0; i < lines.Count; i++)
        {
            if (lines[i].TrimStart().StartsWith('[')) { topLevel = false; continue; }
            if (!topLevel) continue;
            var match = keyPattern.Match(lines[i]);
            if (!match.Success || !updates.TryGetValue(match.Groups[2].Value, out var value)) continue;
            lines[i] = match.Groups[1].Value + match.Groups[2].Value + match.Groups[3].Value + value + CommentSuffix(match.Groups[4].Value);
            found.Add(match.Groups[2].Value);
        }
        int insertAt = lines.FindIndex(line => line.TrimStart().StartsWith('['));
        if (insertAt < 0) insertAt = lines.Count;
        var additions = updates.Where(item => !found.Contains(item.Key)).Select(item => item.Key + " = " + item.Value).ToArray();
        lines.InsertRange(insertAt, additions);
        return string.Join(Environment.NewLine, lines).TrimEnd() + Environment.NewLine;
    }
    private static string CommentSuffix(string rhs)
    {
        bool quoted = false; bool escaped = false;
        for (int i = 0; i < rhs.Length; i++)
        {
            char c = rhs[i];
            if (escaped) { escaped = false; continue; }
            if (c == '\\' && quoted) { escaped = true; continue; }
            if (c == '"') quoted = !quoted;
            if (c == '#' && !quoted) return " " + rhs[i..];
        }
        return string.Empty;
    }
    private static string Quote(string value) => "\"" + value.Replace("\\", "\\\\").Replace("\"", "\\\"") + "\"";

    private void LoadPreferences()
    {
        loading = true;
        try
        {
            var prefs = File.Exists(preferencesFile) ? JsonSerializer.Deserialize<LauncherPreferences>(File.ReadAllText(preferencesFile)) : null;
            sourcePath.Text = prefs?.InputPath ?? ""; workspacePath.Text = prefs?.Workspace ?? Path.Combine(UserRoot, "workspace"); gamePath.Text = prefs?.GameDirectory ?? Path.Combine(UserRoot, "game");
            Select(uiArtwork, prefs?.UiArtwork ?? "4x preview");
            var settings = Path.Combine(gamePath.Text, "settings.toml");
            if (File.Exists(settings))
            {
                var values = ReadTopLevelToml(settings);
                Select(displayMode, values.GetValueOrDefault("display_mode", "borderless")); Select(aspect, values.GetValueOrDefault("aspect_ratio", "auto")); Select(hdr, values.GetValueOrDefault("hdr", "off"));
                width.Value = Number(values, "width", 0, width.Maximum); height.Value = Number(values, "height", 0, height.Maximum);
                renderScale.Value = Number(values, "render_percent", 100, renderScale.Maximum); frameCap.Value = Number(values, "fps_cap", 60, frameCap.Maximum); anisotropic.Value = Number(values, "anisotropic_filtering", 0, anisotropic.Maximum);
                vsync.Checked = bool.TryParse(values.GetValueOrDefault("vsync", "true"), out var v) && v;
                ultrawide.Checked = bool.TryParse(values.GetValueOrDefault("ultrawide", "true"), out var u) && u;
            }
        }
        catch { /* Keep safe UI defaults if a local preference file is malformed. */ }
        finally { loading = false; }
    }
    private static Dictionary<string, string> ReadTopLevelToml(string path)
    {
        var map = new Dictionary<string, string>(StringComparer.Ordinal);
        foreach (var raw in File.ReadLines(path))
        {
            var line = raw.Trim(); if (line.StartsWith('#') || line.StartsWith('[')) continue;
            var i = line.IndexOf('='); if (i <= 0) continue;
            var key = line[..i].Trim(); var value = line[(i + 1)..].Split('#', 2)[0].Trim();
            if (value.Length >= 2 && value[0] == '"' && value[^1] == '"') value = value[1..^1];
            if (key.IndexOfAny(new[] { '.', ' ', '\t' }) < 0) map[key] = value;
        }
        return map;
    }
    private static decimal Number(Dictionary<string, string> values, string key, decimal fallback, decimal max)
        => decimal.TryParse(values.GetValueOrDefault(key), out var v) ? Math.Clamp(v, 0, max) : fallback;
    private static void Select(ComboBox box, string value)
    { int index = box.Items.IndexOf(value); if (index >= 0) box.SelectedIndex = index; }

    private void SavePreferences()
    {
        if (loading) return;
        try { Directory.CreateDirectory(UserRoot); File.WriteAllText(preferencesFile, JsonSerializer.Serialize(new LauncherPreferences(sourcePath.Text, workspacePath.Text, gamePath.Text, uiArtwork.Text), new JsonSerializerOptions { WriteIndented = true })); } catch { }
    }
    private void Log(string line)
    {
        if (buildLog.IsDisposed) return;
        buildLog.AppendText($"[{DateTime.Now:HH:mm:ss}] {line}{Environment.NewLine}");
    }
    private sealed record LauncherPreferences(string InputPath, string Workspace, string GameDirectory, string UiArtwork = "4x preview");
}
