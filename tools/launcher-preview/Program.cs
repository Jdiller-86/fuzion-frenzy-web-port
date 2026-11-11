using System.Drawing.Imaging;

// Render the real Windows controls, with no ISO and without starting a build.
internal static class Preview
{
    [STAThread]
    private static void Main(string[] args)
    {
        Application.SetHighDpiMode(HighDpiMode.DpiUnaware);
        Application.EnableVisualStyles();
        Application.SetCompatibleTextRenderingDefault(false);
        var output = Path.GetFullPath(args[0]);
        Directory.CreateDirectory(output);
        using var form = new FuzionFrenzyLauncher.LauncherForm();
        form.StartPosition = FormStartPosition.Manual;
        form.Location = new Point(30, 30);
        form.Shown += (_, _) => form.BeginInvoke(new Action(() => {
            try {
                foreach (var (caption, filename) in new[] {
                    ("BUILD + PLAY", "build-play.png"),
                    ("GAME SETTINGS", "game-settings.png"),
                    ("CONTROLS", "controls.png") }) {
                    FindButton(form, caption).PerformClick();
                    form.PerformLayout();
                    form.Refresh();
                    Application.DoEvents();
                    using var bitmap = new Bitmap(form.Width, form.Height);
                    form.DrawToBitmap(bitmap, new Rectangle(Point.Empty, form.Size));
                    bitmap.Save(Path.Combine(output, filename), ImageFormat.Png);
                }
            } finally { form.Close(); }
        }));
        Application.Run(form);
    }
    private static Button FindButton(Control parent, string caption)
    {
        foreach (Control child in parent.Controls) {
            if (child is Button button && button.Text == caption) return button;
            try { return FindButton(child, caption); } catch (KeyNotFoundException) { }
        }
        throw new KeyNotFoundException(caption);
    }
}
