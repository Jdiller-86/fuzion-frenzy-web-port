using System.Drawing;
using System.Drawing.Drawing2D;

namespace FuzionFrenzyLauncher;

// Original vector artwork: console-era geometry with restrained modern contrast.
internal static class ConsoleTheme
{
    internal static readonly Color Cyan = Color.FromArgb(74, 225, 235);
    internal static readonly Color Edge = Color.FromArgb(43, 70, 89);
    internal static readonly Color Ink = Color.FromArgb(233, 244, 249);
    internal static GraphicsPath CutRectangle(RectangleF bounds, float cut)
    {
        var path = new GraphicsPath();
        path.AddPolygon(new[] {
            new PointF(bounds.Left, bounds.Top), new PointF(bounds.Right - cut, bounds.Top),
            new PointF(bounds.Right, bounds.Top + cut), new PointF(bounds.Right, bounds.Bottom),
            new PointF(bounds.Left + cut, bounds.Bottom), new PointF(bounds.Left, bounds.Bottom - cut)
        });
        return path;
    }
}

internal sealed class ConsoleHeader : Control
{
    internal ConsoleHeader()
    {
        SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
        BackColor = Color.FromArgb(13, 25, 38);
    }
    protected override void OnPaint(PaintEventArgs e)
    {
        base.OnPaint(e);
        var g = e.Graphics;
        g.SmoothingMode = SmoothingMode.AntiAlias;
        float s = DeviceDpi / 96f;
        using var background = new LinearGradientBrush(ClientRectangle, Color.FromArgb(22, 44, 62), Color.FromArgb(11, 22, 34), 15f);
        g.FillRectangle(background, ClientRectangle);
        using var grid = new Pen(Color.FromArgb(14, ConsoleTheme.Cyan));
        for (float x = Width * .57f; x < Width; x += 24 * s) g.DrawLine(grid, x, 0, x - 80 * s, Height);
        for (float y = 12 * s; y < Height; y += 24 * s) g.DrawLine(grid, Width * .58f, y, Width, y);
        using var border = new Pen(ConsoleTheme.Edge);
        g.DrawRectangle(border, 0, 0, Width - 1, Height - 1);
        using var accent = new SolidBrush(ConsoleTheme.Cyan);
        g.FillRectangle(accent, 0, 0, 4 * s, Height);
        using var eyebrow = new Font("Bahnschrift", 9, FontStyle.Regular);
        using var title = new Font("Bahnschrift", 29, FontStyle.Bold);
        using var body = new Font("Segoe UI", 10);
        TextRenderer.DrawText(g, "FUZION FRENZY  /  WINDOWS", eyebrow, new Point((int)(24*s), (int)(17*s)), ConsoleTheme.Cyan);
        TextRenderer.DrawText(g, "BUILD. LAUNCH. PLAY.", title, new Rectangle((int)(20*s), (int)(36*s), (int)(Width*.73), (int)(49*s)), ConsoleTheme.Ink, TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis | TextFormatFlags.SingleLine);
        TextRenderer.DrawText(g, "Your original game. A new way to play.", body, new Point((int)(24*s), (int)(95*s)), Color.FromArgb(163, 190, 206));
        // A small disc/orbit motif recalls console startup graphics without borrowed assets.
        float diameter = 106 * s, cx = Width - 90*s, cy = Height*.5f;
        using var orbit = new Pen(Color.FromArgb(68, 146, 168), 1.4f*s);
        using var bright = new Pen(ConsoleTheme.Cyan, 3*s);
        g.DrawEllipse(orbit, cx-diameter/2, cy-diameter/2, diameter, diameter);
        g.DrawArc(bright, cx-diameter/2, cy-diameter/2, diameter, diameter, -60, 115);
        g.DrawArc(bright, cx-diameter*.34f, cy-diameter*.34f, diameter*.68f, diameter*.68f, 115, 185);
        g.DrawEllipse(orbit, cx-9*s, cy-9*s, 18*s, 18*s);
        g.DrawLine(orbit, cx-66*s, cy, cx-48*s, cy);
        g.DrawLine(orbit, cx+48*s, cy, cx+66*s, cy);
        for (int n=0; n<5; n++) g.FillRectangle(accent, Width-(170+n*12)*s, Height-8*s, 7*s, 2*s);
    }
}

internal sealed class ConsoleCard : TableLayoutPanel
{
    internal ConsoleCard()
    {
        SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
    }
    protected override void OnPaint(PaintEventArgs e)
    {
        base.OnPaint(e);
        using var edge = new Pen(ConsoleTheme.Edge);
        e.Graphics.DrawRectangle(edge, 0, 0, Width-1, Height-1);
        using var light = new SolidBrush(ConsoleTheme.Cyan);
        e.Graphics.FillRectangle(light, 20 * DeviceDpi / 96f, 0, 38 * DeviceDpi / 96f, 2 * DeviceDpi / 96f);
    }
}

internal sealed class TechButton : Button
{
    private bool hover;
    private bool pressed;
    private bool primary;
    private bool selected;
    internal bool Primary { get => primary; set { primary = value; Invalidate(); } }
    internal bool Selected { get => selected; set { selected = value; Invalidate(); } }
    internal TechButton()
    {
        SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
        FlatStyle = FlatStyle.Flat;
        FlatAppearance.BorderSize = 0;
        Font = new Font("Segoe UI", 9.5f, FontStyle.Bold);
        Cursor = Cursors.Hand;
        UseVisualStyleBackColor = false;
        BackColor = Color.FromArgb(17, 28, 41);
    }
    protected override void OnMouseEnter(EventArgs e) { hover = true; Invalidate(); base.OnMouseEnter(e); }
    protected override void OnMouseLeave(EventArgs e) { hover = pressed = false; Invalidate(); base.OnMouseLeave(e); }
    protected override void OnMouseDown(MouseEventArgs e) { pressed = true; Invalidate(); base.OnMouseDown(e); }
    protected override void OnMouseUp(MouseEventArgs e) { pressed = false; Invalidate(); base.OnMouseUp(e); }
    protected override void OnKeyDown(KeyEventArgs e) { if (e.KeyCode == Keys.Space) { pressed = true; Invalidate(); } base.OnKeyDown(e); }
    protected override void OnKeyUp(KeyEventArgs e) { pressed = false; Invalidate(); base.OnKeyUp(e); }
    protected override void OnEnabledChanged(EventArgs e) { Invalidate(); base.OnEnabledChanged(e); }
    protected override void OnGotFocus(EventArgs e) { Invalidate(); base.OnGotFocus(e); }
    protected override void OnLostFocus(EventArgs e) { pressed = false; Invalidate(); base.OnLostFocus(e); }
    protected override void OnPaint(PaintEventArgs e)
    {
        e.Graphics.Clear(Parent?.BackColor ?? BackColor);
        e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
        float s = DeviceDpi / 96f;
        bool lit = Enabled && (Primary || Selected);
        Color top = !Enabled ? Color.FromArgb(22, 33, 44) : lit ? Color.FromArgb(97, 236, 240) : hover ? Color.FromArgb(38, 68, 86) : Color.FromArgb(30, 50, 67);
        Color bottom = !Enabled ? top : lit ? Color.FromArgb(40, 178, 196) : Color.FromArgb(19, 34, 48);
        if (pressed && Enabled) (top, bottom) = (bottom, top);
        var bounds = new RectangleF(1, 1, Math.Max(1, Width-3), Math.Max(1, Height-3));
        using var path = ConsoleTheme.CutRectangle(bounds, 7*s);
        using var fill = new LinearGradientBrush(bounds, top, bottom, 90);
        using var edge = new Pen(!Enabled ? Color.FromArgb(43, 58, 69) : lit || hover ? ConsoleTheme.Cyan : ConsoleTheme.Edge);
        e.Graphics.FillPath(fill, path); e.Graphics.DrawPath(edge, path);
        Color text = !Enabled ? Color.FromArgb(105, 126, 139) : lit ? Color.FromArgb(7, 32, 42) : ConsoleTheme.Ink;
        TextRenderer.DrawText(e.Graphics, Text, Font, Rectangle.Inflate(ClientRectangle, -9, -2), text, TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine | TextFormatFlags.EndEllipsis);
        if (Focused && ShowFocusCues) ControlPaint.DrawFocusRectangle(e.Graphics, Rectangle.Inflate(ClientRectangle, -5, -5), text, bottom);
    }
}

internal sealed class SegmentProgress : Control
{
    private int value;
    internal int Minimum { get; set; }
    internal int Maximum { get; set; } = 100;
    internal int Value { get => value; set { this.value = Math.Clamp(value, Minimum, Maximum); Invalidate(); } }
    internal SegmentProgress()
    {
        SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
        AccessibleName = "Build progress"; AccessibleRole = AccessibleRole.ProgressBar;
    }
    protected override AccessibleObject CreateAccessibilityInstance() => new ProgressAccessibility(this);
    private sealed class ProgressAccessibility(SegmentProgress owner) : ControlAccessibleObject(owner)
    {
        public override string? Value { get => $"{owner.Value}%"; set { } }
    }
    protected override void OnPaint(PaintEventArgs e)
    {
        e.Graphics.Clear(Color.FromArgb(9, 20, 29));
        const int count = 48;
        float step = Width / (float)count;
        int filled = (int)Math.Ceiling(count * Value / (float)Math.Max(1, Maximum));
        using var active = new SolidBrush(ConsoleTheme.Cyan);
        using var idle = new SolidBrush(Color.FromArgb(30, 56, 70));
        for (int n=0; n<count; n++) e.Graphics.FillRectangle(n < filled ? active : idle, n*step, 1, Math.Max(1, step-3*DeviceDpi/96f), Height-2);
    }
}
