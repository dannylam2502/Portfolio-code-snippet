namespace Tiled
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.lbBackGround = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.txtWidth = new System.Windows.Forms.TextBox();
            this.txtHeight = new System.Windows.Forms.TextBox();
            this.lbWidth = new System.Windows.Forms.Label();
            this.lbHeight = new System.Windows.Forms.Label();
            this.txtBackGround = new System.Windows.Forms.TextBox();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.ptBox = new System.Windows.Forms.PictureBox();
            this.lbTotal = new System.Windows.Forms.Label();
            this.lbSaved = new System.Windows.Forms.Label();
            this.txtTotal = new System.Windows.Forms.TextBox();
            this.txtSaved = new System.Windows.Forms.TextBox();
            this.btnWrite = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.ptBox)).BeginInit();
            this.SuspendLayout();
            // 
            // lbBackGround
            // 
            this.lbBackGround.AutoSize = true;
            this.lbBackGround.Location = new System.Drawing.Point(12, 39);
            this.lbBackGround.Name = "lbBackGround";
            this.lbBackGround.Size = new System.Drawing.Size(67, 13);
            this.lbBackGround.TabIndex = 0;
            this.lbBackGround.Text = "BackGround";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(112, 126);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Split";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // txtWidth
            // 
            this.txtWidth.Location = new System.Drawing.Point(87, 91);
            this.txtWidth.Name = "txtWidth";
            this.txtWidth.Size = new System.Drawing.Size(100, 20);
            this.txtWidth.TabIndex = 2;
            this.txtWidth.Text = "32";
            // 
            // txtHeight
            // 
            this.txtHeight.Location = new System.Drawing.Point(267, 91);
            this.txtHeight.Name = "txtHeight";
            this.txtHeight.Size = new System.Drawing.Size(100, 20);
            this.txtHeight.TabIndex = 3;
            this.txtHeight.Text = "32";
            // 
            // lbWidth
            // 
            this.lbWidth.AutoSize = true;
            this.lbWidth.Location = new System.Drawing.Point(15, 91);
            this.lbWidth.Name = "lbWidth";
            this.lbWidth.Size = new System.Drawing.Size(35, 13);
            this.lbWidth.TabIndex = 4;
            this.lbWidth.Text = "width:";
            // 
            // lbHeight
            // 
            this.lbHeight.AutoSize = true;
            this.lbHeight.Location = new System.Drawing.Point(216, 91);
            this.lbHeight.Name = "lbHeight";
            this.lbHeight.Size = new System.Drawing.Size(41, 13);
            this.lbHeight.TabIndex = 5;
            this.lbHeight.Text = "Height:";
            // 
            // txtBackGround
            // 
            this.txtBackGround.Location = new System.Drawing.Point(87, 39);
            this.txtBackGround.Name = "txtBackGround";
            this.txtBackGround.Size = new System.Drawing.Size(204, 20);
            this.txtBackGround.TabIndex = 6;
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(297, 36);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(31, 23);
            this.btnBrowse.TabIndex = 7;
            this.btnBrowse.Text = "....";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.button2_Click);
            // 
            // ptBox
            // 
            this.ptBox.Location = new System.Drawing.Point(228, 235);
            this.ptBox.Name = "ptBox";
            this.ptBox.Size = new System.Drawing.Size(162, 72);
            this.ptBox.TabIndex = 8;
            this.ptBox.TabStop = false;
            // 
            // lbTotal
            // 
            this.lbTotal.AutoSize = true;
            this.lbTotal.Location = new System.Drawing.Point(15, 174);
            this.lbTotal.Name = "lbTotal";
            this.lbTotal.Size = new System.Drawing.Size(92, 13);
            this.lbTotal.TabIndex = 9;
            this.lbTotal.Text = "Number of splited:";
            // 
            // lbSaved
            // 
            this.lbSaved.AutoSize = true;
            this.lbSaved.Location = new System.Drawing.Point(18, 204);
            this.lbSaved.Name = "lbSaved";
            this.lbSaved.Size = new System.Drawing.Size(84, 13);
            this.lbSaved.TabIndex = 10;
            this.lbSaved.Text = "Numbers saved:";
            // 
            // txtTotal
            // 
            this.txtTotal.Location = new System.Drawing.Point(112, 174);
            this.txtTotal.Name = "txtTotal";
            this.txtTotal.Size = new System.Drawing.Size(100, 20);
            this.txtTotal.TabIndex = 11;
            // 
            // txtSaved
            // 
            this.txtSaved.Location = new System.Drawing.Point(112, 201);
            this.txtSaved.Name = "txtSaved";
            this.txtSaved.Size = new System.Drawing.Size(100, 20);
            this.txtSaved.TabIndex = 12;
            // 
            // btnWrite
            // 
            this.btnWrite.Location = new System.Drawing.Point(228, 126);
            this.btnWrite.Name = "btnWrite";
            this.btnWrite.Size = new System.Drawing.Size(75, 23);
            this.btnWrite.TabIndex = 13;
            this.btnWrite.Text = "Write";
            this.btnWrite.UseVisualStyleBackColor = true;
            this.btnWrite.Click += new System.EventHandler(this.btnWrite_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(389, 304);
            this.Controls.Add(this.btnWrite);
            this.Controls.Add(this.txtSaved);
            this.Controls.Add(this.txtTotal);
            this.Controls.Add(this.lbSaved);
            this.Controls.Add(this.lbTotal);
            this.Controls.Add(this.ptBox);
            this.Controls.Add(this.btnBrowse);
            this.Controls.Add(this.txtBackGround);
            this.Controls.Add(this.lbHeight);
            this.Controls.Add(this.lbWidth);
            this.Controls.Add(this.txtHeight);
            this.Controls.Add(this.txtWidth);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.lbBackGround);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.ptBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbBackGround;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txtWidth;
        private System.Windows.Forms.TextBox txtHeight;
        private System.Windows.Forms.Label lbWidth;
        private System.Windows.Forms.Label lbHeight;
        private System.Windows.Forms.TextBox txtBackGround;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.PictureBox ptBox;
        private System.Windows.Forms.Label lbTotal;
        private System.Windows.Forms.Label lbSaved;
        private System.Windows.Forms.TextBox txtTotal;
        private System.Windows.Forms.TextBox txtSaved;
        private System.Windows.Forms.Button btnWrite;
    }
}

