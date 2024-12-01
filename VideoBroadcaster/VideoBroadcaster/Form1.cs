using System.Diagnostics;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Net;
using System.Net.Sockets;
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

        static Bitmap result_btmp = new(out_display_width, out_display_height + 3);
        static Graphics gr_result = Graphics.FromImage(result_btmp);
        static byte[] result_btmp_arr = new byte[out_display_width * out_display_height * 3];
        bool WSconnected;
        Task WSsend;

        Socket s = new(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        IPAddress broadcast;
        IPEndPoint ep;

        const int out_display_width = 16;
        const int out_display_height = 25;
        const int width = 32 * 10;
        const int height = 28 * 10;

        private Rectangle bounds = new(600, 0, width, height);
        private bool Runing = true;
        private Task DrawTask = Task.Delay(0);
        int _frame;
        int _totalframes;

        protected async Task Draw()
        {
            Stopwatch timer = new();
            timer.Start();

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
                using Bitmap source = new(bounds.Width, bounds.Height);
                using Graphics gr_source = Graphics.FromImage(source);
                gr_source.CopyFromScreen(bounds.Location, Point.Empty, bounds.Size);
                gr_result.DrawImage(source, new Rectangle(0, 0, out_display_width, out_display_height + 3), 0, 0, source.Width, source.Height, GraphicsUnit.Pixel, wrapMode);
                result_btmp_arr = ImageToByte(result_btmp, checkBox3.Checked, trackBar1.Value, trackBar1.Maximum);
                _frame++; _totalframes++;

                if (WSconnected)
                {
                    if (WSsend != null)
                    {
                        await WSsend;
                    }
                    WSsend = s.SendToAsync(result_btmp_arr, ep);
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
                //int delay = 1000 / 100 - (int)timer.ElapsedMilliseconds;
                //await Task.Delay(delay > 0 ? delay : 1);
                await Task.Delay(1);
            }
        }

        public static byte[] ImageToByte(Bitmap img, bool flipHorizontally, int brightness, int maxBrightness)
        {
            byte[] result = new byte[out_display_width * out_display_height * 3];
            int k = 0;
            for (int i = 0; i < img.Width; i++)
            {
                if (i % 2 == 0)
                {
                    for (int j = 5; j < img.Height - 1; j++)
                    {
                        var c = img.GetPixel(flipHorizontally ? img.Width - 1 - i : i, j);
                        result[k++] = (byte)(brightness * c.R / maxBrightness);
                        result[k++] = (byte)(brightness * c.G / maxBrightness);
                        result[k++] = (byte)(brightness * c.B / maxBrightness);
                    }
                }
                else
                {
                    for (int j = img.Height - 1; j >= 0; j--)
                    {
                        var c = img.GetPixel(flipHorizontally ? img.Width - 1 - i : i, j);
                        result[k++] = (byte)(brightness * c.R / maxBrightness);
                        result[k++] = (byte)(brightness * c.G / maxBrightness);
                        result[k++] = (byte)(brightness * c.B / maxBrightness);
                    }
                }
            }

            return result;
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.D:
                    bounds = new Rectangle(bounds.X + 1, bounds.Y, bounds.Width, bounds.Height);
                    break;
                case Keys.A:
                    bounds = new Rectangle(bounds.X - 1, bounds.Y, bounds.Width, bounds.Height);
                    break;
                case Keys.W:
                    bounds = new Rectangle(bounds.X, bounds.Y - 1, bounds.Width, bounds.Height);
                    break;
                case Keys.S:
                    bounds = new Rectangle(bounds.X, bounds.Y + 1, bounds.Width, bounds.Height);
                    break;
                case Keys.Add:
                    bounds = new Rectangle(bounds.X, bounds.Y, (int)(bounds.Width * 1.1), (int)(bounds.Height * 1.1));
                    break;
                case Keys.Subtract:
                    if (bounds.Width > 19)
                    {
                        bounds = new Rectangle(bounds.X, bounds.Y, (int)(bounds.Width * 0.9), (int)(bounds.Height * 0.9));
                    }
                    break;

                default:
                    return;
            }
        }

        private async void Form1_Shown(object sender, EventArgs e)
        {
            Runing = true;
            DrawTask = Draw();
            SetTimer();
            await DrawTask;
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

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            checkBox1.Enabled = checkBox2.Checked;
        }

        private void OnTimedEvent(Object source, ElapsedEventArgs e)
        {
            label2.Text = _frame.ToString();
            _frame = 0;
            label5.Text = _totalframes.ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (!WSconnected)
            {
                var addr = textBox1.Text.Trim().Split(':');
                broadcast = IPAddress.Parse(addr[0]);
                ep = new(broadcast, addr.Length > 1 ? int.Parse(addr[1]) : 80);
                WSconnected = true;
                button2.Text = "disconnect";
            }
            else
            {
                WSconnected = false;
                button2.Text = "connect";
            }
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            label10.Text = trackBar1.Value.ToString();
        }
    }
}