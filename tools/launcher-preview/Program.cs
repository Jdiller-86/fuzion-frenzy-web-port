using System.Drawing.Imaging;
using System.Reflection;
using FuzionFrenzyLauncher;

// Render actual Windows controls. Sample ready/progress states are visual
// demonstrations only: no ISO, game execution, tool installation or build runs.
internal static class Preview
{
    [STAThread]
    private static void Main(string[] args)
    {
        Application.SetHighDpiMode(HighDpiMode.DpiUnaware);
        Application.EnableVisualStyles();Application.SetCompatibleTextRenderingDefault(false);
        var output=Path.GetFullPath(args[0]);Directory.CreateDirectory(output);
        using var form=new LauncherForm();form.StartPosition=FormStartPosition.Manual;form.Location=new Point(25,25);
        form.Shown+=(_,_)=>form.BeginInvoke(new Action(()=>{
            try{
                Capture(form,"control-room-setup.png",output);
                Field<TextBox>(form,"sourcePath").Text=@"C:\Games\Fuzion Frenzy (USA).iso";
                var matrix=Field<DotMatrixDisplay>(form,"matrix");matrix.Text="READY";
                var primary=Field<Button>(form,"primaryButton");primary.Text="Play game";primary.Enabled=true;
                var rebuild=Field<Button>(form,"buildButton");rebuild.Visible=true;rebuild.Enabled=true;
                Capture(form,"control-room-ready.png",output);
                CaptureDialog(form,"settingsDialog","Settings…","game-settings.png",output);
                CaptureDialog(form,"optionsDialog","Build options…","build-options.png",output);
                CaptureDialog(form,"controlsDialog","Controls…","controls.png",output);
                matrix.Text="COMPILE 61%";matrix.Progress=61;matrix.Busy=true;
                primary.Text="Cancel";rebuild.Enabled=false;
                Capture(form,"control-room-progress.png",output);
                File.WriteAllText(Path.Combine(output,"capture-notes.txt"),"Actual Windows launcher controls. Ready/progress states are presentation samples; no game files or compilation were used.\n");
            }finally{form.Close();}
        }));
        Application.Run(form);
    }
    private static T Field<T>(Form form,string name) where T:class => (T)form.GetType().GetField(name,BindingFlags.Instance|BindingFlags.NonPublic)!.GetValue(form)!;
    private static void Capture(Form form,string name,string output)
    {
        form.PerformLayout();form.Refresh();Application.DoEvents();
        using var bitmap=new Bitmap(form.Width,form.Height);form.DrawToBitmap(bitmap,new Rectangle(Point.Empty,form.Size));bitmap.Save(Path.Combine(output,name),ImageFormat.Png);
    }
    private static void CaptureDialog(Form owner,string field,string caption,string name,string output)
    {
        var dialog=Field<Form>(owner,field);
        dialog.Shown+=(_,_)=>dialog.BeginInvoke(new Action(()=>{Capture(dialog,name,output);dialog.DialogResult=DialogResult.Cancel;}));
        FindButton(owner,caption).PerformClick();
    }
    private static Button FindButton(Control parent,string caption)
    {
        foreach(Control child in parent.Controls){if(child is Button b&&b.Text==caption)return b;try{return FindButton(child,caption);}catch(KeyNotFoundException){}}
        throw new KeyNotFoundException(caption);
    }
}
