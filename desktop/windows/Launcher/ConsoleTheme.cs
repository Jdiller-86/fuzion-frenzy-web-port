using System.Drawing;
using System.Drawing.Drawing2D;

namespace FuzionFrenzyLauncher;

internal static class ConsoleTheme
{
    internal static readonly Color Background = Color.FromArgb(35, 39, 32);
    internal static readonly Color Surface = Color.FromArgb(62, 73, 50);
    internal static readonly Color Edge = Color.FromArgb(103, 117, 85);
    internal static readonly Color Ink = Color.FromArgb(235, 236, 222);
    internal static readonly Color Muted = Color.FromArgb(177, 185, 161);
    internal static readonly Color Amber = Color.FromArgb(245, 186, 100);
    internal static readonly Color Error = Color.FromArgb(255, 169, 138);
}

// One continuous faceplate; each bay has one job and a restrained hardware label.
internal sealed class EquipmentBay : TableLayoutPanel
{
    internal EquipmentBay()
    {
        SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
        BackColor = ConsoleTheme.Surface;
    }
    protected override void OnPaintBackground(PaintEventArgs e)
    {
        if (Width < 1 || Height < 1) return;
        using var fill = new LinearGradientBrush(ClientRectangle, Color.FromArgb(77, 89, 61), Color.FromArgb(53, 68, 43), 8f);
        e.Graphics.FillRectangle(fill, ClientRectangle);
    }
    protected override void OnPaint(PaintEventArgs e)
    {
        base.OnPaint(e);
        float s = DeviceDpi / 96f;
        using var top = new Pen(Color.FromArgb(126, 140, 102));
        using var bottom = new Pen(Color.FromArgb(28, 41, 20), 2 * s);
        e.Graphics.DrawLine(top, 0, 0, Width, 0);
        e.Graphics.DrawLine(bottom, 0, Height - s, Width, Height - s);
        using var seam = new Pen(Color.FromArgb(101, 116, 81));
        e.Graphics.DrawLine(seam, 104*s, 18*s, 104*s, Height-18*s);
    }
}

internal sealed class TechButton : Button
{
    private bool hover, pressed, primary;
    internal bool Primary { get => primary; set { primary = value; Invalidate(); } }
    internal TechButton()
    {
        SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
        FlatStyle = FlatStyle.Flat;
        FlatAppearance.BorderSize = 0;
        Font = new Font("Segoe UI", 9.5f, FontStyle.Bold);
        Cursor = Cursors.Hand;
        UseVisualStyleBackColor = false;
        BackColor = ConsoleTheme.Surface;
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
        if (Width < 4 || Height < 4) return;
        bool lit = Enabled && Primary;
        Color top = !Enabled ? Color.FromArgb(63, 69, 56) : lit ? Color.FromArgb(248, 205, 139) : hover ? Color.FromArgb(110, 115, 94) : Color.FromArgb(88, 96, 75);
        Color bottom = !Enabled ? top : lit ? Color.FromArgb(211, 155, 76) : Color.FromArgb(57, 67, 46);
        if (pressed && Enabled) (top, bottom) = (bottom, top);
        var bounds = new Rectangle(1, 1, Width-3, Height-4);
        using var fill = new LinearGradientBrush(bounds, top, bottom, 90f);
        using var edge = new Pen(!Enabled ? Color.FromArgb(77, 85, 65) : lit || hover ? ConsoleTheme.Amber : ConsoleTheme.Edge);
        using var lip = new Pen(Color.FromArgb(32, 45, 23), 2);
        e.Graphics.FillRectangle(fill, bounds); e.Graphics.DrawRectangle(edge, bounds);
        e.Graphics.DrawLine(lip, 2, Height-2, Width-3, Height-2);
        Color text = !Enabled ? Color.FromArgb(132, 144, 115) : lit ? Color.FromArgb(63, 43, 19) : ConsoleTheme.Ink;
        TextRenderer.DrawText(e.Graphics, Text, Font, Rectangle.Inflate(ClientRectangle, -8, -2), text, TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine | TextFormatFlags.EndEllipsis);
        if (Focused && ShowFocusCues) ControlPaint.DrawFocusRectangle(e.Graphics, Rectangle.Inflate(ClientRectangle, -5, -5), text, bottom);
    }
}

// Amber 5x7 dots: a single status line and, only while working, a progress rail.
// There are no filenames, display preferences or decorative secondary text here.
internal sealed class DotMatrixDisplay : Control
{
    private readonly System.Windows.Forms.Timer animation = new() { Interval = 100 };
    private int scan;
    private int? progress;
    private bool busy;
    internal int? Progress { get => progress; set { progress = value is int p ? Math.Clamp(p, 0, 100) : null; UpdateAnimation(); Invalidate(); } }
    internal bool Busy { get => busy; set { busy = value; UpdateAnimation(); Invalidate(); } }
    internal DotMatrixDisplay()
    {
        SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, true);
        BackColor = Color.FromArgb(19, 23, 14);
        ForeColor = ConsoleTheme.Amber;
        TabStop = false;
        AccessibleName = "Build status";
        AccessibleRole = AccessibleRole.StaticText;
        animation.Tick += (_, _) => { scan = (scan + 1) % 40; Invalidate(); };
        VisibleChanged += (_, _) => UpdateAnimation();
    }
    private void UpdateAnimation()
    {
        // A measured percentage is static until a new status arrives; animate
        // only an active operation whose progress is unknown, and never at idle.
        animation.Enabled = busy && progress is null && Visible && !IsDisposed;
    }
    protected override void OnTextChanged(EventArgs e) { base.OnTextChanged(e); AccessibleDescription = Text; Invalidate(); }
    protected override void OnPaint(PaintEventArgs e)
    {
        e.Graphics.Clear(BackColor);
        if (Width < 10 || Height < 10) return;
        e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
        float s = DeviceDpi / 96f, pad = 20*s;
        string value = Text.ToUpperInvariant();
        float available = Math.Max(1, Width-pad*2);
        float cell = Math.Max(2, Math.Min(9*s, Math.Min(available/Math.Max(60, value.Length*6), (Height-pad*2-(busy?14*s:0))/7)));
        int cols = Math.Max(1, (int)(available/cell));
        float y0 = Math.Max(pad, (Height-(busy?15*s:0)-7*cell)/2);
        using var idle = new SolidBrush(Color.FromArgb(19, ForeColor));
        using var glow = new SolidBrush(Color.FromArgb(35, ForeColor));
        using var lit = new SolidBrush(ForeColor);
        for (int y=0; y<7; y++) for (int x=0; x<cols; x++)
            e.Graphics.FillEllipse(idle, pad+x*cell, y0+y*cell, cell*.48f, cell*.48f);
        int cursor = 0;
        foreach (char ch in value)
        {
            string bits = Glyphs.GetValueOrDefault(ch, Glyphs[' ']);
            for (int y=0; y<7; y++) for (int x=0; x<5; x++) if (bits[y*5+x]=='1' && cursor+x<cols)
            {
                float px=pad+(cursor+x)*cell, py=y0+y*cell;
                e.Graphics.FillEllipse(glow, px-cell*.23f, py-cell*.23f, cell*.94f, cell*.94f);
                e.Graphics.FillEllipse(lit, px, py, cell*.48f, cell*.48f);
            }
            cursor+=6;
        }
        if (busy)
        {
            const int count=40;
            float step=available/count, py=Height-17*s;
            for(int i=0;i<count;i++)
            {
                bool active=progress is int p ? i<count*p/100f : (i-scan+count)%count<5;
                e.Graphics.FillEllipse(active?lit:idle, pad+i*step, py, Math.Max(2,step*.48f), 3*s);
            }
        }
        using var bezel = new Pen(Color.FromArgb(25, 32, 19), 3*s);
        e.Graphics.DrawRectangle(bezel, 1, 1, Width-3, Height-3);
    }
    protected override void Dispose(bool disposing) { if(disposing)animation.Dispose(); base.Dispose(disposing); }
    private static readonly Dictionary<char,string> Glyphs = new()
    {
        ['A'] = "01110100011000111111100011000110001",
        ['B'] = "11110100011000111110100011000111110",
        ['C'] = "01111100001000010000100001000001111",
        ['D'] = "11110100011000110001100011000111110",
        ['E'] = "11111100001000011110100001000011111",
        ['F'] = "11111100001000011110100001000010000",
        ['G'] = "01111100001000010111100011000101111",
        ['H'] = "10001100011000111111100011000110001",
        ['I'] = "11111001000010000100001000010011111",
        ['J'] = "00111000100001000010100101001001100",
        ['K'] = "10001100101010011000101001001010001",
        ['L'] = "10000100001000010000100001000011111",
        ['M'] = "10001110111010110101100011000110001",
        ['N'] = "10001110011010110011100011000110001",
        ['O'] = "01110100011000110001100011000101110",
        ['P'] = "11110100011000111110100001000010000",
        ['Q'] = "01110100011000110001101011001001101",
        ['R'] = "11110100011000111110101001001010001",
        ['S'] = "01111100001000001110000010000111110",
        ['T'] = "11111001000010000100001000010000100",
        ['U'] = "10001100011000110001100011000101110",
        ['V'] = "10001100011000110001100010101000100",
        ['W'] = "10001100011000110101101011010101010",
        ['X'] = "10001100010101000100010101000110001",
        ['Y'] = "10001100010101000100001000010000100",
        ['Z'] = "11111000010001000100010001000011111",
        ['0'] = "01110100011001110101110011000101110",
        ['1'] = "00100011000010000100001000010001110",
        ['2'] = "01110100010000100010001000100011111",
        ['3'] = "11110000010000101110000010000111110",
        ['4'] = "00010001100101010010111110001000010",
        ['5'] = "11111100001000011110000010000111110",
        ['6'] = "01110100001000011110100011000101110",
        ['7'] = "11111000010001000100010000100001000",
        ['8'] = "01110100011000101110100011000101110",
        ['9'] = "01110100011000101111000010000101110",
        ['.'] = "00000000000000000000000000011000110",
        ['%'] = "11001110100010001000101100011000000",
        ['-'] = "00000000000000011111000000000000000",
        ['/'] = "00001000100001000100010000100010000",
        [':'] = "00000001100011000000001100011000000",
        [' '] = "00000000000000000000000000000000000",
    };
}
