using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Net.WebSockets;
using System.Timers;

namespace VideoBroadcaster
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            pictureBox1.Width = width;
            pictureBox1.Height = height;

            gr_result.CompositingMode = CompositingMode.SourceCopy;
            gr_result.CompositingQuality = CompositingQuality.HighQuality;
            gr_result.InterpolationMode = InterpolationMode.HighQualityBicubic;
            gr_result.SmoothingMode = SmoothingMode.HighQuality;
            gr_result.PixelOffsetMode = PixelOffsetMode.HighQuality;
        }

        static Bitmap result_btmp = new(out_display_width, out_display_height);
        static Graphics gr_result = Graphics.FromImage(result_btmp);
        static byte[] result_btmp_arr = new byte[out_display_width * out_display_height * 3];
        static ClientWebSocket WS = new();
        bool WSconnected = false;
        Task WSsend;

        const int out_display_width = 16;
        const int out_display_height = 25;
        const int width = out_display_width * 20;
        const int height = out_display_height * 20;

        private Rectangle _rec = new(0, 0, width, height);
        protected bool Runing = true;
        protected Task DrawTask = Task.Delay(0);
        int _frame = 0;
        int _totalframes = 0;
        int _missedFrames = 0;

        private async void button1_Click(object sender, EventArgs e)
        {
            Runing = !Runing;
            if (Runing)
            {
                DrawTask = Draw(_rec);
                await DrawTask;
            }
        }

        protected async Task Draw(Rectangle bounds)
        {
            //Stopwatch timer = new();
            //timer.Start();
            using Bitmap source = new(bounds.Width, bounds.Height);
            using Graphics gr_source = Graphics.FromImage(source);

            using Bitmap screen = new(width, height);
            using Graphics gr_screen = Graphics.FromImage(screen);
            gr_screen.CompositingMode = CompositingMode.SourceCopy;
            gr_screen.CompositingQuality = CompositingQuality.HighSpeed;
            gr_screen.InterpolationMode = InterpolationMode.Low;
            gr_screen.SmoothingMode = SmoothingMode.HighSpeed;
            gr_screen.PixelOffsetMode = PixelOffsetMode.HighSpeed;

            using var wrapMode = new ImageAttributes();
            wrapMode.SetWrapMode(WrapMode.TileFlipXY);

            while (Runing)
            {
                //timer.Restart();
                gr_source.CopyFromScreen(bounds.Location, Point.Empty, bounds.Size);
                gr_result.DrawImage(source, new Rectangle(0,0, out_display_width, out_display_height), 0, 0, source.Width, source.Height, GraphicsUnit.Pixel, wrapMode);
                result_btmp_arr = ImageToByte(result_btmp);
                _frame++; _totalframes++;

                if (WSconnected)
                {
                    if (WSsend == null || WSsend.IsCompleted)
                    {
                        WSsend = WS.SendAsync(result_btmp_arr, WebSocketMessageType.Binary, true, CancellationToken.None);
                    }
                    else
                    {
                        _missedFrames++;
                    }
                }

                if (checkBox2.Checked)
                {
                    if (checkBox1.Checked)
                    {
                        gr_screen.DrawImage(result_btmp, new Rectangle(0, 0, width, height), 0, 0, result_btmp.Width, result_btmp.Height, GraphicsUnit.Pixel, wrapMode);
                    }
                    else
                    {
                        gr_screen.DrawImage(source, new Rectangle(0, 0, width, height), 0, 0, source.Width, source.Height, GraphicsUnit.Pixel, wrapMode);
                    }

                    pictureBox1.Image = screen;
                }

                //timer.Stop();
                //int delay = 1000 / 30 - (int)timer.ElapsedMilliseconds;
                int delay = 0;
                await Task.Delay(delay > 0 ? delay : 1);
            }
        }

        public static byte[] ImageToByte(Bitmap img)
        {
            byte[] result = new byte[img.Height * img.Width * 3];
            int k = 0;
            for (int i = 0; i < img.Width; i++)
            {
                for (int j = 0; j < img.Height; j++)
                {
                    var c = img.GetPixel(i, j);
                    result[k++] = c.R;
                    result[k++] = c.G;
                    result[k++] = c.B;
                }
            }

            return result;
        }

        private async void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.D:
                    _rec = new Rectangle(_rec.X + 1, _rec.Y, _rec.Width, _rec.Height);
                    break;
                case Keys.A:
                    _rec = new Rectangle(_rec.X - 1, _rec.Y, _rec.Width, _rec.Height);
                    break;
                case Keys.W:
                    _rec = new Rectangle(_rec.X, _rec.Y - 1, _rec.Width, _rec.Height);
                    break;
                case Keys.S:
                    _rec = new Rectangle(_rec.X, _rec.Y + 1, _rec.Width, _rec.Height);
                    break;
                case Keys.Add:
                    _rec = new Rectangle(_rec.X, _rec.Y, _rec.Width + out_display_width, _rec.Height + out_display_height);
                    break;
                case Keys.Subtract:
                    if (_rec.Width > 19)
                    {
                        _rec = new Rectangle(_rec.X, _rec.Y, _rec.Width - out_display_width, _rec.Height - out_display_height);
                    }
                    break;

                default:
                    return;
            }

            Runing = false;
            await DrawTask;
            Runing = true;
            DrawTask = Draw(_rec);
            await DrawTask;
        }

        private async void Form1_Shown(object sender, EventArgs e)
        {
            Runing = true;
            DrawTask = Draw(_rec);
            SetTimer();
            await DrawTask;
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            checkBox1.Enabled = checkBox2.Checked;
        }

        private System.Timers.Timer aTimer;

        private void SetTimer()
        {
            // Create a timer with a two second interval.
            aTimer = new System.Timers.Timer(1000);
            // Hook up the Elapsed event for the timer. 
            aTimer.Elapsed += OnTimedEvent;
            aTimer.AutoReset = true;
            aTimer.Enabled = true;
        }

        private void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
            label2.Text = _frame.ToString();
            _frame = 0;
            label5.Text = _totalframes.ToString();
            label6.Text = _missedFrames.ToString();
        }

        private async void button2_Click(object sender, EventArgs e)
        {
            if (!WSconnected)
            {
                await WS.ConnectAsync(new Uri("ws://192.168.3.16/ws"), CancellationToken.None);
                WSconnected = true;
                button2.Text = "disconnect";
            }
            else
            {
                WS.Dispose();
                WS = new ClientWebSocket();
                WSconnected = false;
                button2.Text = "connect";
            }
        }
    }
}