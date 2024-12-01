namespace VideoBroadcaster
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            button1 = new Button();
            pictureBox1 = new PictureBox();
            checkBox1 = new CheckBox();
            checkBox2 = new CheckBox();
            label1 = new Label();
            label2 = new Label();
            button2 = new Button();
            label3 = new Label();
            label4 = new Label();
            label5 = new Label();
            label6 = new Label();
            label7 = new Label();
            label8 = new Label();
            checkBox3 = new CheckBox();
            trackBar1 = new TrackBar();
            label10 = new Label();
            label9 = new Label();
            textBox1 = new TextBox();
            groupBox2 = new GroupBox();
            label11 = new Label();
            ((System.ComponentModel.ISupportInitialize)pictureBox1).BeginInit();
            ((System.ComponentModel.ISupportInitialize)trackBar1).BeginInit();
            groupBox2.SuspendLayout();
            SuspendLayout();
            // 
            // button1
            // 
            button1.Location = new Point(338, 164);
            button1.Name = "button1";
            button1.Size = new Size(167, 23);
            button1.TabIndex = 0;
            button1.Text = "button1";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // pictureBox1
            // 
            pictureBox1.Location = new Point(12, 12);
            pictureBox1.Name = "pictureBox1";
            pictureBox1.Size = new Size(320, 280);
            pictureBox1.TabIndex = 1;
            pictureBox1.TabStop = false;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(338, 39);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(152, 19);
            checkBox1.TabIndex = 2;
            checkBox1.Text = "Показать вид отправки";
            checkBox1.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            checkBox2.AutoSize = true;
            checkBox2.Checked = true;
            checkBox2.CheckState = CheckState.Checked;
            checkBox2.Location = new Point(338, 14);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new Size(113, 19);
            checkBox2.TabIndex = 3;
            checkBox2.Text = "Показать захват";
            checkBox2.UseVisualStyleBackColor = true;
            checkBox2.CheckedChanged += checkBox2_CheckedChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(6, 19);
            label1.Name = "label1";
            label1.Size = new Size(26, 15);
            label1.TabIndex = 4;
            label1.Text = "fps:";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(98, 19);
            label2.Name = "label2";
            label2.Size = new Size(13, 15);
            label2.TabIndex = 5;
            label2.Text = "0";
            // 
            // button2
            // 
            button2.Location = new Point(338, 193);
            button2.Name = "button2";
            button2.Size = new Size(167, 23);
            button2.TabIndex = 7;
            button2.Text = "connect";
            button2.UseVisualStyleBackColor = true;
            button2.Click += button2_Click;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(6, 34);
            label3.Name = "label3";
            label3.Size = new Size(74, 15);
            label3.TabIndex = 8;
            label3.Text = "Total frames:";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(6, 49);
            label4.Name = "label4";
            label4.Size = new Size(86, 15);
            label4.TabIndex = 9;
            label4.Text = "Missed frames:";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(98, 34);
            label5.Name = "label5";
            label5.Size = new Size(13, 15);
            label5.TabIndex = 10;
            label5.Text = "0";
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Location = new Point(98, 49);
            label6.Name = "label6";
            label6.Size = new Size(13, 15);
            label6.TabIndex = 11;
            label6.Text = "0";
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new Point(6, 64);
            label7.Name = "label7";
            label7.Size = new Size(55, 15);
            label7.TabIndex = 12;
            label7.Text = "WS state:";
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Location = new Point(67, 64);
            label8.Name = "label8";
            label8.Size = new Size(12, 15);
            label8.TabIndex = 13;
            label8.Text = "-";
            // 
            // checkBox3
            // 
            checkBox3.AutoSize = true;
            checkBox3.Location = new Point(338, 64);
            checkBox3.Name = "checkBox3";
            checkBox3.Size = new Size(166, 19);
            checkBox3.TabIndex = 14;
            checkBox3.Text = "Отразить по горизонтали";
            checkBox3.UseVisualStyleBackColor = true;
            // 
            // trackBar1
            // 
            trackBar1.AccessibleName = "";
            trackBar1.Location = new Point(338, 104);
            trackBar1.Maximum = 255;
            trackBar1.Name = "trackBar1";
            trackBar1.RightToLeftLayout = true;
            trackBar1.Size = new Size(166, 45);
            trackBar1.TabIndex = 15;
            trackBar1.TickStyle = TickStyle.None;
            trackBar1.Value = 255;
            trackBar1.Scroll += trackBar1_Scroll;
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Location = new Point(403, 86);
            label10.Name = "label10";
            label10.Size = new Size(25, 15);
            label10.TabIndex = 17;
            label10.Text = "255";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new Point(337, 138);
            label9.Name = "label9";
            label9.Size = new Size(24, 15);
            label9.TabIndex = 19;
            label9.Text = "url:";
            // 
            // textBox1
            // 
            textBox1.Location = new Point(363, 135);
            textBox1.Name = "textBox1";
            textBox1.Size = new Size(141, 23);
            textBox1.TabIndex = 20;
            textBox1.Text = "ws://192.168.3.16/ws";
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(label1);
            groupBox2.Controls.Add(label2);
            groupBox2.Controls.Add(label3);
            groupBox2.Controls.Add(label4);
            groupBox2.Controls.Add(label5);
            groupBox2.Controls.Add(label8);
            groupBox2.Controls.Add(label6);
            groupBox2.Controls.Add(label7);
            groupBox2.Location = new Point(338, 222);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new Size(167, 88);
            groupBox2.TabIndex = 21;
            groupBox2.TabStop = false;
            groupBox2.Text = "status";
            // 
            // label11
            // 
            label11.AutoSize = true;
            label11.Location = new Point(343, 86);
            label11.Name = "label11";
            label11.Size = new Size(54, 15);
            label11.TabIndex = 22;
            label11.Text = "Яркость:";
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(510, 315);
            Controls.Add(label11);
            Controls.Add(groupBox2);
            Controls.Add(label10);
            Controls.Add(textBox1);
            Controls.Add(label9);
            Controls.Add(checkBox3);
            Controls.Add(button2);
            Controls.Add(checkBox2);
            Controls.Add(checkBox1);
            Controls.Add(pictureBox1);
            Controls.Add(button1);
            Controls.Add(trackBar1);
            KeyPreview = true;
            Name = "Form1";
            Text = "Form1";
            Shown += Form1_Shown;
            KeyDown += Form1_KeyDown;
            ((System.ComponentModel.ISupportInitialize)pictureBox1).EndInit();
            ((System.ComponentModel.ISupportInitialize)trackBar1).EndInit();
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button button1;
        private PictureBox pictureBox1;
        private CheckBox checkBox1;
        private CheckBox checkBox2;
        private Label label1;
        private Label label2;
        private Button button2;
        private Label label3;
        private Label label4;
        private Label label5;
        private Label label6;
        private Label label7;
        private Label label8;
        private CheckBox checkBox3;
        private TrackBar trackBar1;
        private Label label10;
        private Label label9;
        private TextBox textBox1;
        private GroupBox groupBox2;
        private Label label11;
    }
}