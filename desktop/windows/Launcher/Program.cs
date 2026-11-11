using System.Diagnostics;
using System.Drawing;
using System.Globalization;
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
    private const string Version = "0.2.0";
    private static readonly Color Bg = ConsoleTheme.Background;
    private static readonly Color Panel2 = Color.FromArgb(51, 61, 42);
    private static readonly string UserRoot = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "FuzionFrenzy");
    private readonly string preferencesFile = Path.Combine(UserRoot, "launcher-settings.json");
    private readonly TextBox sourcePath = new(), workspacePath = new(), gamePath = new(), buildLog = new();
    private readonly Label status = new(), displaySummary = new();
    private readonly DotMatrixDisplay matrix = new();
    private readonly System.Windows.Forms.Timer statusTimer = new() { Interval = 500 };
    private readonly Button primaryButton = new TechButton(), buildButton = new TechButton();
    private readonly Button sourceButton = new TechButton(), optionsButton = new TechButton(), toolsButton = new TechButton(), settingsButton = new TechButton(), logButton = new TechButton();
    private readonly NumericUpDown jobs = new(), width = new(), height = new(), renderScale = new(), frameCap = new(), anisotropic = new();
    private readonly ComboBox displayMode = new(), aspect = new(), hdr = new(), uiArtwork = new();
    private readonly CheckBox ultrawide = new(), vsync = new();
    private readonly ToolTip hints = new() { AutoPopDelay = 18000, InitialDelay = 450, ReshowDelay = 200 };
    private readonly ContextMenuStrip sourceMenu = new();
    private readonly System.Windows.Forms.Panel logPanel = new();
    private Form settingsDialog = null!, optionsDialog = null!, controlsDialog = null!;
    private Process? activeProcess;
    private StreamWriter? operationLog;
    private string? currentLogPath, idleMessage;
    private string operationWorkspace = "";
    private DateTime operationStartedUtc;
    private bool loading, cancelRequested, operationRunning;

    public LauncherForm()
    {
        Text = $"Fuzion Frenzy · Windows Builder v{Version}";
        StartPosition = FormStartPosition.CenterScreen;
        AutoScaleDimensions = new SizeF(96, 96);
        AutoScaleMode = AutoScaleMode.Dpi;
        MinimumSize = new Size(1000, 700);
        Size = new Size(1160, 820);
        BackColor = Bg; ForeColor = ConsoleTheme.Ink;
        Font = new Font("Segoe UI", 10f);
        BuildLayout();
        settingsDialog = CreateSettingsDialog();
        optionsDialog = CreateBuildOptionsDialog();
        controlsDialog = CreateControlsDialog();
        LoadPreferences();
        LoadGameSettings();
        sourcePath.TextChanged += (_, _) => { if (!loading) { idleMessage = null; ClearNotice(); SavePreferences(); RefreshGameState(); } };
        RefreshGameState(); UpdateDisplaySummary();
        statusTimer.Tick += (_, _) => ReadBuildStatus();
        FormClosing += (_, _) => { if (activeProcess is { HasExited: false }) activeProcess.Kill(entireProcessTree: true); };
    }

    private void BuildLayout()
    {
        var shell = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 1, RowCount = 2, BackColor = Bg, Padding = new Padding(24, 24, 24, 10) };
        shell.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
        shell.RowStyles.Add(new RowStyle(SizeType.Percent, 100)); shell.RowStyles.Add(new RowStyle(SizeType.Absolute, 28));
        Controls.Add(shell);
        var page = new TableLayoutPanel { Dock = DockStyle.Fill, AutoScroll = true, ColumnCount = 1, BackColor = Bg, Padding = new Padding(0, 0, 10, 0), Margin = new Padding(0) };
        page.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100)); shell.Controls.Add(page, 0, 0);
        var bank = new TableLayoutPanel { Dock = DockStyle.Top, AutoSize = true, ColumnCount = 1, Margin = new Padding(0), Padding = new Padding(1), BackColor = ConsoleTheme.Edge };
        bank.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
        page.Controls.Add(bank);

        var game = Bay("GAME");
        var gameBody = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 2, AutoSize = true, Margin = new Padding(0), BackColor = ConsoleTheme.Surface };
        gameBody.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100)); gameBody.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 165));
        matrix.Dock = DockStyle.Fill; matrix.Height = 145; matrix.Margin = new Padding(0, 0, 24, 0); matrix.Text = "SELECT SOURCE";
        gameBody.Controls.Add(matrix, 0, 0);
        var launch = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 1, RowCount = 2, Margin = new Padding(0), BackColor = ConsoleTheme.Surface };
        launch.RowStyles.Add(new RowStyle(SizeType.Percent, 62)); launch.RowStyles.Add(new RowStyle(SizeType.Percent, 38));
        primaryButton.Text = "Build game"; primaryButton.Dock = DockStyle.Fill; primaryButton.Margin = new Padding(0, 15, 0, 9); StyleButton(primaryButton, true);
        primaryButton.Click += async (_, _) => { if (operationRunning) CancelBuild(); else if (GameExists()) PlayGame(); else await RunBuild(); };
        buildButton.Text = "Rebuild"; buildButton.Dock = DockStyle.Fill; buildButton.Margin = new Padding(0, 0, 0, 15); StyleButton(buildButton, false);
        buildButton.Click += async (_, _) => await RunBuild();
        launch.Controls.Add(primaryButton, 0, 0); launch.Controls.Add(buildButton, 0, 1); gameBody.Controls.Add(launch, 1, 0);
        game.Controls.Add(gameBody, 1, 0); bank.Controls.Add(game);

        var source = Bay("SOURCE");
        var sourceBody = new TableLayoutPanel { Dock = DockStyle.Fill, AutoSize = true, ColumnCount = 2, Margin = new Padding(0), BackColor = ConsoleTheme.Surface };
        sourceBody.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100)); sourceBody.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 165));
        StylePath(sourcePath); sourcePath.PlaceholderText = "ISO/XISO image or extracted game folder"; sourcePath.Margin = new Padding(0, 4, 24, 6);
        sourceButton.Text = "Choose source…"; sourceButton.Dock = DockStyle.Fill; sourceButton.Margin = new Padding(0, 0, 0, 3); sourceButton.Height = 38; StyleButton(sourceButton, false);
        sourceMenu.Items.Add("ISO / XISO image…", null, (_, _) => ChooseSource());
        sourceMenu.Items.Add("Extracted game folder…", null, (_, _) => ChooseFolder(sourcePath, "Choose extracted game files"));
        sourceButton.Click += (_, _) => sourceMenu.Show(sourceButton, new Point(0, sourceButton.Height));
        sourceBody.Controls.Add(sourcePath, 0, 0); sourceBody.Controls.Add(sourceButton, 1, 0);
        var sourceActions = Flow(); sourceActions.Margin = new Padding(0, 6, 0, 0);
        optionsButton.Text = "Build options…"; ConfigureAction(optionsButton, 146); optionsButton.Click += (_, _) => OpenBuildOptions();
        toolsButton.Text = "Install build tools…"; ConfigureAction(toolsButton, 170); toolsButton.Click += async (_, _) => await InstallTools();
        sourceActions.Controls.Add(optionsButton); sourceActions.Controls.Add(toolsButton); sourceBody.Controls.Add(sourceActions, 0, 1); sourceBody.SetColumnSpan(sourceActions, 2);
        source.Controls.Add(sourceBody, 1, 0); bank.Controls.Add(source);

        var display = Bay("DISPLAY");
        var displayBody = new TableLayoutPanel { Dock = DockStyle.Fill, AutoSize = true, ColumnCount = 2, Margin = new Padding(0), BackColor = ConsoleTheme.Surface };
        displayBody.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100)); displayBody.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 165));
        displaySummary.Dock = DockStyle.Fill; displaySummary.AutoSize = true; displaySummary.ForeColor = ConsoleTheme.Ink; displaySummary.Font = new Font("Segoe UI", 10f); displaySummary.Margin = new Padding(0, 5, 20, 5);
        settingsButton.Text = "Settings…"; settingsButton.Dock = DockStyle.Fill; settingsButton.Height = 44; settingsButton.Margin = new Padding(0, 3, 0, 3); StyleButton(settingsButton, false); settingsButton.Click += (_, _) => OpenSettings();
        displayBody.Controls.Add(displaySummary, 0, 0); displayBody.Controls.Add(settingsButton, 1, 0); display.Controls.Add(displayBody, 1, 0); bank.Controls.Add(display);

        status.AutoSize = false; status.Height = 39; status.Dock = DockStyle.Top; status.TextAlign = ContentAlignment.MiddleLeft; status.AutoEllipsis = true; status.ForeColor = ConsoleTheme.Muted; status.Margin = new Padding(4, 10, 4, 0); status.Visible = false; page.Controls.Add(status);
        var utilities = Flow(); utilities.BackColor = Bg; utilities.Margin = new Padding(0, 19, 0, 13);
        utilities.Controls.Add(MakeButton("Game folder", false, () => OpenPath(gamePath.Text), 135));
        utilities.Controls.Add(MakeButton("Saves", false, () => OpenPath(Path.Combine(gamePath.Text, "hdd")), 100));
        logButton.Text = "Build log"; ConfigureAction(logButton, 120); logButton.Click += (_, _) => ShowLog(!logPanel.Visible); utilities.Controls.Add(logButton);
        utilities.Controls.Add(MakeButton("Controls…", false, () => ShowDialog(controlsDialog), 120));
        page.Controls.Add(utilities);
        logPanel.Dock = DockStyle.Top; logPanel.Height = 210; logPanel.Visible = false; logPanel.Margin = new Padding(0); logPanel.BackColor = Panel2;
        var logContents = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 1, RowCount = 2, Padding = new Padding(12), BackColor = Panel2 };
        logContents.RowStyles.Add(new RowStyle(SizeType.Percent, 100)); logContents.RowStyles.Add(new RowStyle(SizeType.Absolute, 36));
        buildLog.Multiline = true; buildLog.ReadOnly = true; buildLog.ScrollBars = ScrollBars.Vertical; buildLog.WordWrap = false; buildLog.Dock = DockStyle.Fill; buildLog.BackColor = Color.FromArgb(22, 28, 18); buildLog.ForeColor = Color.FromArgb(205, 208, 183); buildLog.BorderStyle = BorderStyle.None; buildLog.Font = new Font("Consolas", 9f);
        logContents.Controls.Add(buildLog, 0, 0);
        var logActions = Flow(); logActions.BackColor = Panel2;
        logActions.Controls.Add(MakeButton("Open log folders…", false, ShowLogFolders, 175));
        logContents.Controls.Add(logActions, 0, 1); logPanel.Controls.Add(logContents); page.Controls.Add(logPanel);
        shell.Controls.Add(new Label { Text = $"WINDOWS x64  ·  BUILDER {Version}", Dock = DockStyle.Fill, TextAlign = ContentAlignment.MiddleRight, ForeColor = ConsoleTheme.Muted, Font = new Font("Bahnschrift", 8.5f), AutoEllipsis = true, Margin = new Padding(0, 4, 0, 0) }, 0, 1);
        hints.SetToolTip(sourcePath, "The original files are needed to build or rebuild. An installed game can be played without reselecting its source.");
        hints.SetToolTip(optionsButton, "Build workspace, installed game folder and parallel jobs.");
        hints.SetToolTip(toolsButton, "Install Git, Python, CMake, Ninja, Clang and the Microsoft C++ tools.");
    }

    private static EquipmentBay Bay(string name)
    {
        var bay = new EquipmentBay { Dock = DockStyle.Top, AutoSize = true, ColumnCount = 2, Margin = new Padding(0), Padding = new Padding(20, 23, 24, 23) };
        bay.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 104)); bay.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100));
        bay.Controls.Add(new Label { Text = name, AutoSize = true, Font = new Font("Bahnschrift", 9f, FontStyle.Bold), ForeColor = Color.FromArgb(198, 207, 178), BackColor = ConsoleTheme.Surface, Margin = new Padding(0, 12, 17, 0), Anchor = AnchorStyles.Left }, 0, 0);
        return bay;
    }
    private static FlowLayoutPanel Flow() => new() { Dock = DockStyle.Top, AutoSize = true, WrapContents = true, Margin = new Padding(0), BackColor = ConsoleTheme.Surface };
    private static void StyleButton(Button button, bool primary)
    { button.Font = new Font("Segoe UI", 9.5f, FontStyle.Bold); button.Cursor = Cursors.Hand; if (button is TechButton tech) tech.Primary = primary; }
    private static void ConfigureAction(Button button, int width)
    { button.Width = width; button.Height = 35; button.Margin = new Padding(0, 0, 10, 0); StyleButton(button, false); }
    private static Button MakeButton(string text, bool primary, Action action, int width = 145)
    { var button = new TechButton { Text = text }; ConfigureAction(button, width); StyleButton(button, primary); button.Click += (_, _) => action(); return button; }
    private static void StylePath(TextBox box)
    { box.Dock = DockStyle.Fill; box.BackColor = Panel2; box.ForeColor = ConsoleTheme.Ink; box.BorderStyle = BorderStyle.FixedSingle; }
    private Form Dialog(string title, Size size)
    { return new Form { Text = title, StartPosition = FormStartPosition.CenterParent, ClientSize = size, MinimumSize = size, FormBorderStyle = FormBorderStyle.FixedDialog, MaximizeBox = false, MinimizeBox = false, ShowInTaskbar = false, BackColor = Bg, ForeColor = ConsoleTheme.Ink, Font = Font, AutoScaleDimensions = new SizeF(96,96), AutoScaleMode = AutoScaleMode.Dpi }; }
    private static TableLayoutPanel DialogPage(Form dialog)
    {
        var page = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 1, AutoScroll = true, Padding = new Padding(24), BackColor = Bg };
        page.ColumnStyles.Add(new ColumnStyle(SizeType.Percent,100)); dialog.Controls.Add(page); return page;
    }
    private static void AddField(TableLayoutPanel grid, int row, string label, Control control, string[]? options = null)
    {
        if (options is not null && control is ComboBox combo) { combo.DropDownStyle = ComboBoxStyle.DropDownList; combo.Items.AddRange(options); combo.SelectedIndex = 0; combo.FlatStyle = FlatStyle.Flat; }
        control.Dock = DockStyle.Fill; control.BackColor = Panel2; control.ForeColor = ConsoleTheme.Ink; control.Margin = new Padding(3, 5, 0, 7);
        grid.Controls.Add(new Label { Text = label, ForeColor = ConsoleTheme.Muted, AutoSize = true, Anchor = AnchorStyles.Left, Margin = new Padding(0, 6, 14, 6) },0,row); grid.Controls.Add(control,1,row);
    }
    private static TableLayoutPanel FormGrid()
    { var grid = new TableLayoutPanel { Dock = DockStyle.Top, AutoSize = true, ColumnCount = 2, Margin = new Padding(0) }; grid.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute,220)); grid.ColumnStyles.Add(new ColumnStyle(SizeType.Percent,100)); return grid; }
    private static void NumberControl(NumericUpDown number, decimal min, decimal max, decimal value, decimal step = 1)
    { number.Minimum=min; number.Maximum=max; number.Value=value; number.Increment=step; }

    private Form CreateSettingsDialog()
    {
        var dialog=Dialog("Game settings",new Size(650,650)); var page=DialogPage(dialog); var grid=FormGrid();
        NumberControl(width,0,16384,0,100); NumberControl(height,0,16384,0,100); NumberControl(renderScale,10,400,100,10); NumberControl(frameCap,0,1000,60,5); NumberControl(anisotropic,0,16,0);
        AddField(grid,0,"Window mode",displayMode,new[]{"borderless","fullscreen","windowed"});
        AddField(grid,1,"Width (0 = automatic)",width); AddField(grid,2,"Height (0 = automatic)",height);
        AddField(grid,3,"Aspect ratio",aspect,new[]{"auto","4:3","16:9","16:10","21:9","32:9"});
        AddField(grid,4,"Frame cap (0 = unlimited)",frameCap); AddField(grid,5,"Render scale (%)",renderScale);
        vsync.Text="Enabled"; vsync.Checked=true; vsync.AutoSize=true; AddField(grid,6,"Vertical sync",vsync);
        ultrawide.Text="Widen on wide displays"; ultrawide.Checked=true; ultrawide.AutoSize=true; AddField(grid,7,"Ultrawide",ultrawide);
        AddField(grid,8,"Anisotropic filtering",anisotropic); AddField(grid,9,"HDR",hdr,new[]{"off","auto","on"});
        AddField(grid,10,"UI artwork",uiArtwork,new[]{"4x preview","Original pixels","Replacements off"});
        page.Controls.Add(grid);
        page.Controls.Add(new Label { Text="Saved settings apply on the next launch. F1 opens in-game display settings; pause gameplay first.", ForeColor=ConsoleTheme.Muted, AutoSize=true, Dock=DockStyle.Top, Margin=new Padding(0,15,0,20) });
        var actions=Flow(); actions.BackColor=Bg;
        var save=MakeButton("Save",true,()=>{if(SaveGameSettings())dialog.DialogResult=DialogResult.OK;},120);
        var cancel=MakeButton("Cancel",false,()=>dialog.DialogResult=DialogResult.Cancel,120);
        actions.Controls.Add(save); actions.Controls.Add(cancel); page.Controls.Add(actions); dialog.AcceptButton=save; dialog.CancelButton=cancel;
        return dialog;
    }
    private Form CreateBuildOptionsDialog()
    {
        var dialog=Dialog("Build options",new Size(760,330)); var page=DialogPage(dialog);
        page.Controls.Add(PathField("Build workspace",workspacePath,"Choose build workspace"));
        page.Controls.Add(PathField("Game folder",gamePath,"Choose installed game folder"));
        var grid=FormGrid(); NumberControl(jobs,0,64,0); AddField(grid,0,"Parallel jobs (0 = automatic)",jobs); page.Controls.Add(grid);
        var actions=Flow(); actions.BackColor=Bg; actions.Margin=new Padding(0,18,0,0);
        var save=MakeButton("Save",true,()=>{
            try {
                workspacePath.Text=Path.GetFullPath(workspacePath.Text); gamePath.Text=Path.GetFullPath(gamePath.Text);
                SavePreferences(); LoadGameSettings(); idleMessage=null; ClearNotice(); RefreshGameState(); UpdateDisplaySummary(); dialog.DialogResult=DialogResult.OK;
            } catch(Exception ex){MessageBox.Show(dialog,ex.Message,"Invalid build location",MessageBoxButtons.OK,MessageBoxIcon.Error);}
        },120);
        var cancel=MakeButton("Cancel",false,()=>dialog.DialogResult=DialogResult.Cancel,120);
        actions.Controls.Add(save);actions.Controls.Add(cancel);page.Controls.Add(actions);dialog.AcceptButton=save;dialog.CancelButton=cancel;return dialog;
    }
    private TableLayoutPanel PathField(string label,TextBox box,string description)
    {
        var row=new TableLayoutPanel{Dock=DockStyle.Top,AutoSize=true,ColumnCount=2,Margin=new Padding(0,0,0,17)};
        row.ColumnStyles.Add(new ColumnStyle(SizeType.Percent,100));row.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute,100));
        row.Controls.Add(new Label{Text=label,AutoSize=true,ForeColor=ConsoleTheme.Muted,Margin=new Padding(0,0,0,8)},0,0);row.SetColumnSpan(row.Controls[0],2);
        StylePath(box);box.Margin=new Padding(0,5,12,0);row.Controls.Add(box,0,1);
        var browse=MakeButton("Browse…",false,()=>ChooseFolder(box,description),95);row.Controls.Add(browse,1,1);return row;
    }
    private Form CreateControlsDialog()
    {
        var dialog=Dialog("Controls",new Size(610,520));var page=DialogPage(dialog);var grid=FormGrid();
        string[][] rows={new[]{"Move","W A S D"},new[]{"D-pad","Arrow keys"},new[]{"Right stick","Right mouse + mouse"},new[]{"A / B","Space / Escape or Backspace"},new[]{"X / Y","X / Y"},new[]{"Triggers","Z / C"},new[]{"Black / White","1 / 2"},new[]{"Start / Back","Enter / Tab"},new[]{"Stick clicks","Shift / Ctrl"},new[]{"Display settings","F1"},new[]{"Performance","F2"}};
        for(int i=0;i<rows.Length;i++)AddField(grid,i,rows[i][0],new Label{Text=rows[i][1],AutoSize=true});page.Controls.Add(grid);
        page.Controls.Add(new Label{Text="Controllers are detected in connection order. Input follows the focused game window.",ForeColor=ConsoleTheme.Muted,AutoSize=true,Dock=DockStyle.Top,Margin=new Padding(0,15,0,18)});
        var close=MakeButton("Close",false,()=>dialog.DialogResult=DialogResult.Cancel,120);page.Controls.Add(close);dialog.CancelButton=close;return dialog;
    }
    private void ShowDialog(Form dialog) { dialog.DialogResult=DialogResult.None; dialog.ShowDialog(this); }
    private IEnumerable<Control> SettingControls() => new Control[]{displayMode,width,height,aspect,frameCap,renderScale,vsync,ultrawide,anisotropic,hdr,uiArtwork};
    private void OpenSettings()
    {
        if(operationRunning)return;
        LoadGameSettings();
        var snapshot=SettingControls().ToDictionary(c=>c,c=>c switch{NumericUpDown n=>(object)n.Value,CheckBox b=>b.Checked,_=>c.Text});
        ShowDialog(settingsDialog);
        if(settingsDialog.DialogResult!=DialogResult.OK)foreach(var pair in snapshot){if(pair.Key is NumericUpDown n)n.Value=(decimal)pair.Value;else if(pair.Key is CheckBox b)b.Checked=(bool)pair.Value;else pair.Key.Text=(string)pair.Value;}
        UpdateDisplaySummary();
    }
    private void OpenBuildOptions()
    {
        if(operationRunning)return;
        string oldWorkspace=workspacePath.Text,oldGame=gamePath.Text;decimal oldJobs=jobs.Value;
        ShowDialog(optionsDialog);
        if(optionsDialog.DialogResult!=DialogResult.OK){workspacePath.Text=oldWorkspace;gamePath.Text=oldGame;jobs.Value=oldJobs;}
    }
    private void ChooseSource()
    {
        using var dlg=new OpenFileDialog{Title="Choose your local Fuzion Frenzy disc image",Filter="Xbox disc images (*.iso;*.xiso)|*.iso;*.xiso|All files (*.*)|*.*",CheckFileExists=true};
        if(dlg.ShowDialog(this)==DialogResult.OK)sourcePath.Text=dlg.FileName;
    }
    private void ChooseFolder(TextBox target,string description)
    { using var dlg=new FolderBrowserDialog{Description=description,UseDescriptionForTitle=true,ShowNewFolderButton=true};if(Directory.Exists(target.Text))dlg.SelectedPath=target.Text;if(dlg.ShowDialog(this)==DialogResult.OK)target.Text=dlg.SelectedPath; }

    private bool GameExists() => File.Exists(Path.Combine(gamePath.Text,"FuzionFrenzy.exe"));
    private bool SourceExists() => File.Exists(sourcePath.Text) || Directory.Exists(sourcePath.Text);
    private void RefreshGameState()
    {
        bool exists=GameExists(), source=SourceExists();
        primaryButton.Text=operationRunning ? (cancelRequested?"Stopping…":"Cancel") : exists?"Play game":"Build game";
        primaryButton.Enabled=operationRunning?!cancelRequested:exists||source;
        buildButton.Visible=exists; buildButton.Enabled=!operationRunning&&source;
        sourcePath.Enabled=sourceButton.Enabled=optionsButton.Enabled=toolsButton.Enabled=settingsButton.Enabled=!operationRunning;
        hints.SetToolTip(buildButton,source?"Recompile using the selected local game files.":"Choose a local ISO or extracted game folder to rebuild.");
        if(!operationRunning){matrix.Busy=false;matrix.Progress=null;matrix.Text=idleMessage??(exists?"READY":source?"READY TO BUILD":"SELECT SOURCE");}
    }
    private void UpdateDisplaySummary()
    {
        string cap=frameCap.Value==0?"Unlimited FPS":$"{frameCap.Value:0} FPS";
        displaySummary.Text=$"{CultureInfo.InvariantCulture.TextInfo.ToTitleCase(displayMode.Text)}  ·  {(aspect.Text=="auto"?"Auto aspect":aspect.Text)}  ·  {cap}\nRender scale {renderScale.Value:0}%  ·  UI artwork: {uiArtwork.Text}";
    }
    private void Notice(string text,bool error=false)
    {
        status.Text=text.Replace('\r',' ').Replace('\n',' ');status.ForeColor=error?ConsoleTheme.Error:ConsoleTheme.Muted;
        status.Visible=!string.IsNullOrWhiteSpace(text);hints.SetToolTip(status,text);
    }
    private void ClearNotice(){status.Text="";status.Visible=false;}
    private void ShowLog(bool show){logPanel.Visible=show;logButton.Text=show?"Hide build log":"Build log";}
    private void ShowLogFolders()
    {
        var menu=new ContextMenuStrip();
        menu.Items.Add("Builder logs",null,(_,_)=>OpenPath(Path.Combine(UserRoot,"logs")));
        menu.Items.Add("Game logs",null,(_,_)=>OpenPath(Path.Combine(gamePath.Text,"logs")));
        menu.Closed += (_, _) => menu.Dispose();
        menu.Show(Cursor.Position);
    }
    private void BeginOperation(string kind)
    {
        operationRunning=true;cancelRequested=false;idleMessage=null;ClearNotice();
        operationStartedUtc=DateTime.UtcNow;operationWorkspace=workspacePath.Text;
        matrix.Progress=null;matrix.Text=kind=="build"?"CHECK TOOLS":"INSTALL TOOLS";matrix.Busy=true;
        buildLog.Clear();
        try{
            var directory=Path.Combine(UserRoot,"logs");Directory.CreateDirectory(directory);
            currentLogPath=Path.Combine(directory,$"launcher-{kind}-{DateTime.Now:yyyyMMdd-HHmmss}-{Guid.NewGuid().ToString("N")[..6]}.log");
            operationLog=new StreamWriter(new FileStream(currentLogPath,FileMode.CreateNew,FileAccess.Write,FileShare.Read),new UTF8Encoding(false)){AutoFlush=true};
        }catch(IOException){currentLogPath=null;}catch(UnauthorizedAccessException){currentLogPath=null;}
        RefreshGameState();
    }
    private void FinishOperation()
    {
        statusTimer.Stop();activeProcess?.Dispose();activeProcess=null;
        operationLog?.Dispose();operationLog=null;operationRunning=false;cancelRequested=false;RefreshGameState();
    }
    private async Task RunBuild()
    {
        if(operationRunning)return;
        var script=Path.Combine(AppContext.BaseDirectory,"scripts","Build-Game.ps1");
        if(!File.Exists(script)){Notice("The bundled build script is missing. Reinstall the builder.",true);return;}
        if(!SourceExists()){Notice("Choose a local ISO/XISO or extracted game folder to build.",true);return;}
        if(string.IsNullOrWhiteSpace(workspacePath.Text)||string.IsNullOrWhiteSpace(gamePath.Text)){OpenBuildOptions();return;}
        try{Directory.CreateDirectory(workspacePath.Text);Directory.CreateDirectory(gamePath.Text);SavePreferences();}
        catch(Exception ex){Notice(ex.Message,true);return;}
        var psi=PowerShellStart(script);psi.RedirectStandardOutput=true;psi.RedirectStandardError=true;psi.UseShellExecute=false;psi.CreateNoWindow=true;
        foreach(var arg in new[]{"-InputPath",sourcePath.Text,"-Workspace",workspacePath.Text,"-GameDirectory",gamePath.Text,"-Jobs",((int)jobs.Value).ToString(CultureInfo.InvariantCulture),"-StatusPath",Path.Combine(workspacePath.Text,"build-status.json")})psi.ArgumentList.Add(arg);
        BeginOperation("build");
        try{
            activeProcess=new Process{StartInfo=psi};if(!activeProcess.Start())throw new InvalidOperationException("PowerShell did not start.");
            Log("Starting local Windows build.");statusTimer.Start();
            var output=Pump(activeProcess.StandardOutput);var errors=Pump(activeProcess.StandardError);
            await activeProcess.WaitForExitAsync();await Task.WhenAll(output,errors);ReadBuildStatus();
            if(cancelRequested){idleMessage="CANCELLED";ClearNotice();Log("Build cancelled.");}
            else if(activeProcess.ExitCode==0){idleMessage=null;ClearNotice();Log("Build completed successfully.");}
            else{idleMessage="BUILD FAILED";if(!status.Visible)Notice($"Build exited with code {activeProcess.ExitCode}. Open the build log for details.",true);ShowLog(true);Log($"Build script exited with code {activeProcess.ExitCode}.");}
        }catch(Exception ex){idleMessage="BUILD FAILED";Notice(ex.Message,true);Log(ex.Message);ShowLog(true);}
        finally{FinishOperation();}
    }
    private void ReadBuildStatus()
    {
        var file=Path.Combine(operationWorkspace,"build-status.json");if(!File.Exists(file))return;
        try{
            using var doc=JsonDocument.Parse(File.ReadAllText(file));var root=doc.RootElement;
            if(root.ValueKind!=JsonValueKind.Object)return;
            // Ignore a previous run until this build writes its first status.
            if(!root.TryGetProperty("updatedAtUtc",out var updated)||updated.ValueKind!=JsonValueKind.String||!DateTime.TryParse(updated.GetString(),CultureInfo.InvariantCulture,DateTimeStyles.RoundtripKind,out var stamp)||stamp.ToUniversalTime()<operationStartedUtc)return;
            int? progress=root.TryGetProperty("progress",out var p)&&p.ValueKind==JsonValueKind.Number&&p.TryGetInt32(out int n)?Math.Clamp(n,0,100):null;
            string phase=root.TryGetProperty("phase",out var ph)&&ph.ValueKind==JsonValueKind.String?ph.GetString()??"compile":"compile";
            string state=root.TryGetProperty("status",out var st)&&st.ValueKind==JsonValueKind.String?st.GetString()??"running":"running";
            string heading=phase switch{"validation"=>"VALIDATE","prerequisites"=>"CHECK TOOLS","checkout"=>"GET SOURCE","extract"=>"READ GAME","configure"=>"CONFIGURE","compile"=>"COMPILE","ui-assets"=>"UI ASSETS","install"=>"INSTALL","complete"=>"FINISHING","error"=>"BUILD FAILED",_=>"BUILDING"};
            matrix.Progress=progress;matrix.Text=progress is int percent&&state!="failed"?$"{heading} {percent}%":heading;
            if(state=="failed"){
                string message=root.TryGetProperty("error",out var error)&&error.ValueKind==JsonValueKind.String?error.GetString()??"Build failed.":"Build failed. Open the build log for details.";
                Notice(message,true);
            }
        }catch(IOException){}catch(JsonException){}
    }
    private async Task Pump(StreamReader reader)
    {
        while(await reader.ReadLineAsync() is {} line){if(IsDisposed)return;if(InvokeRequired)BeginInvoke(new Action(()=>Log(line)));else Log(line);}
    }
    private void CancelBuild()
    {
        if(activeProcess is not {HasExited:false} process)return;
        cancelRequested=true;matrix.Text="STOPPING";matrix.Progress=null;Log("Cancellation requested; stopping the process tree.");RefreshGameState();
        try{process.Kill(entireProcessTree:true);}catch(InvalidOperationException){}
    }
    private async Task InstallTools()
    {
        if(operationRunning)return;
        var script=Path.Combine(AppContext.BaseDirectory,"scripts","Install-Prerequisites.ps1");
        if(!File.Exists(script)){Notice("The bundled prerequisite installer is missing. Reinstall the builder.",true);return;}
        if(MessageBox.Show(this,"This downloads and installs the Windows build toolchain. Downloads are large and individual installers may request administrator approval.","Install build tools",MessageBoxButtons.OKCancel,MessageBoxIcon.Information)!=DialogResult.OK)return;
        BeginOperation("tools");ShowLog(true);
        try{
            string logDirectory=Path.Combine(UserRoot,"logs");Directory.CreateDirectory(logDirectory);
            string transcript=Path.Combine(logDirectory,$"prerequisites-{DateTime.Now:yyyyMMdd-HHmmss}.log");
            var psi=PowerShellStart(script);psi.ArgumentList.Add("-Install");psi.ArgumentList.Add("-LogPath");psi.ArgumentList.Add(transcript);psi.UseShellExecute=false;psi.CreateNoWindow=true;psi.RedirectStandardOutput=true;psi.RedirectStandardError=true;
            activeProcess=new Process{StartInfo=psi};if(!activeProcess.Start())throw new InvalidOperationException("PowerShell did not start.");
            Log($"Installing prerequisites. Log: {transcript}");
            var output=Pump(activeProcess.StandardOutput);var errors=Pump(activeProcess.StandardError);
            await activeProcess.WaitForExitAsync();await Task.WhenAll(output,errors);
            if(cancelRequested){idleMessage="CANCELLED";ClearNotice();}
            else if(activeProcess.ExitCode!=0){idleMessage="TOOLS FAILED";Notice($"Build-tool installation exited with code {activeProcess.ExitCode}. See the build log.",true);}
            else{idleMessage=null;Notice("Build-tool installation finished.");}
        }catch(Exception ex){idleMessage="TOOLS FAILED";Notice(ex.Message,true);Log(ex.Message);}
        finally{FinishOperation();}
    }
    private static ProcessStartInfo PowerShellStart(string script)
    {
        var psi=new ProcessStartInfo("powershell.exe"){WorkingDirectory=AppContext.BaseDirectory};
        foreach(string arg in new[]{"-NoProfile","-ExecutionPolicy","Bypass","-File",script})psi.ArgumentList.Add(arg);return psi;
    }
    private void PlayGame()
    {
        if(operationRunning||!GameExists())return;
        try{
            var exe=Path.Combine(Path.GetFullPath(gamePath.Text),"FuzionFrenzy.exe");
            var psi=new ProcessStartInfo(exe){WorkingDirectory=Path.GetFullPath(gamePath.Text),UseShellExecute=false};
            var pack=Path.Combine(psi.WorkingDirectory,"ui.bgra");psi.Environment.Remove("FUZION_UI_PACK");
            psi.Environment["FUZION_UI_REPLACE"]=uiArtwork.SelectedIndex switch{1=>"identity",2=>"off",_=>"pilot"};
            if(File.Exists(pack))psi.Environment["FUZION_UI_PACK"]=pack;
            else Log("No local UI artwork pack found. Rebuild to add it.");
            Process.Start(psi);ClearNotice();Log("Started FuzionFrenzy.exe.");
        }catch(Exception ex){Notice(ex.Message,true);Log(ex.Message);ShowLog(true);}
    }
    private void OpenPath(string path)
    {
        if(string.IsNullOrWhiteSpace(path))return;
        try{Directory.CreateDirectory(path);Process.Start(new ProcessStartInfo("explorer.exe"){UseShellExecute=true,ArgumentList={path}});}
        catch(Exception ex){MessageBox.Show(this,ex.Message,"Could not open folder",MessageBoxButtons.OK,MessageBoxIcon.Error);}
    }
    private bool SaveGameSettings()
    {
        var values=new Dictionary<string,string>{["display_mode"]=Quote(displayMode.Text),["width"]=((int)width.Value).ToString(CultureInfo.InvariantCulture),["height"]=((int)height.Value).ToString(CultureInfo.InvariantCulture),["render_percent"]=((int)renderScale.Value).ToString(CultureInfo.InvariantCulture),["fps_cap"]=((int)frameCap.Value).ToString(CultureInfo.InvariantCulture),["vsync"]=vsync.Checked?"true":"false",["anisotropic_filtering"]=((int)anisotropic.Value).ToString(CultureInfo.InvariantCulture),["aspect_ratio"]=Quote(aspect.Text),["ultrawide"]=ultrawide.Checked?"true":"false",["hdr"]=Quote(hdr.Text)};
        try{
            Directory.CreateDirectory(gamePath.Text);var file=Path.Combine(gamePath.Text,"settings.toml");
            AtomicText(file,MergeToml(File.Exists(file)?File.ReadAllText(file):"",values));SavePreferences();
            Notice("Settings saved. Applies on the next launch.");UpdateDisplaySummary();return true;
        }catch(Exception ex){MessageBox.Show(settingsDialog,ex.Message,"Could not save settings",MessageBoxButtons.OK,MessageBoxIcon.Error);return false;}
    }
    private static void AtomicText(string file,string content)
    {
        var temporary=file+"."+Guid.NewGuid().ToString("N")+".tmp";
        try{File.WriteAllText(temporary,content,new UTF8Encoding(false));File.Move(temporary,file,true);}
        finally{if(File.Exists(temporary))File.Delete(temporary);}
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
        loading=true;
        try{
            var prefs=File.Exists(preferencesFile)?JsonSerializer.Deserialize<LauncherPreferences>(File.ReadAllText(preferencesFile)):null;
            sourcePath.Text=prefs?.InputPath??"";workspacePath.Text=prefs?.Workspace??Path.Combine(UserRoot,"workspace");gamePath.Text=prefs?.GameDirectory??Path.Combine(UserRoot,"game");
            Select(uiArtwork,prefs?.UiArtwork??"4x preview");jobs.Value=Math.Clamp(prefs?.ParallelJobs??0,0,64);
        }catch{sourcePath.Text="";workspacePath.Text=Path.Combine(UserRoot,"workspace");gamePath.Text=Path.Combine(UserRoot,"game");}
        finally{loading=false;}
    }
    private void LoadGameSettings()
    {
        try{
            var file=Path.Combine(gamePath.Text,"settings.toml");var values=File.Exists(file)?ReadTopLevelToml(file):new Dictionary<string,string>();
            Select(displayMode,values.GetValueOrDefault("display_mode","borderless"));Select(aspect,values.GetValueOrDefault("aspect_ratio","auto"));Select(hdr,values.GetValueOrDefault("hdr","off"));
            foreach(var (control,key,fallback) in new[]{(width,"width",0m),(height,"height",0m),(renderScale,"render_percent",100m),(frameCap,"fps_cap",60m),(anisotropic,"anisotropic_filtering",0m)})control.Value=Math.Clamp(Number(values,key,fallback,control.Maximum),control.Minimum,control.Maximum);
            vsync.Checked=!bool.TryParse(values.GetValueOrDefault("vsync","true"),out var v)||v;ultrawide.Checked=!bool.TryParse(values.GetValueOrDefault("ultrawide","true"),out var u)||u;
        }catch(IOException){}catch(UnauthorizedAccessException){}
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
        if(loading)return;
        try{Directory.CreateDirectory(UserRoot);AtomicText(preferencesFile,JsonSerializer.Serialize(new LauncherPreferences(sourcePath.Text,workspacePath.Text,gamePath.Text,uiArtwork.Text,(int)jobs.Value),new JsonSerializerOptions{WriteIndented=true}));}
        catch(IOException){}catch(UnauthorizedAccessException){}
    }
    private void Log(string line)
    {
        if(buildLog.IsDisposed)return;
        string text=$"[{DateTime.Now:HH:mm:ss}] {line}{Environment.NewLine}";
        // Retain the full log on disk, but bound the on-screen tail during long builds.
        if(buildLog.TextLength>180000){buildLog.Select(0,90000);buildLog.SelectedText="";}
        buildLog.AppendText(text);
        try{operationLog?.Write(text);}catch(IOException){operationLog?.Dispose();operationLog=null;}
    }
    protected override void Dispose(bool disposing)
    {
        if(disposing){statusTimer.Dispose();hints.Dispose();sourceMenu.Dispose();operationLog?.Dispose();settingsDialog?.Dispose();optionsDialog?.Dispose();controlsDialog?.Dispose();}
        base.Dispose(disposing);
    }
    private sealed record LauncherPreferences(string InputPath,string Workspace,string GameDirectory,string UiArtwork="4x preview",int ParallelJobs=0);
}
