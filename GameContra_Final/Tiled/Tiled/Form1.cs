using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tiled
{
    public partial class Form1 : Form
    {
        Map map = new Map();
        public Form1()
        {
            InitializeComponent();
            map.section = new Rectangle();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlgOpen = new OpenFileDialog();
            if(dlgOpen.ShowDialog() == DialogResult.OK)
            {
                txtBackGround.Text = dlgOpen.FileName;
                map.mainBitmap = (Bitmap)Bitmap.FromFile(dlgOpen.FileName);
                map.section.Width = Int32.Parse(txtWidth.Text);
                map.section.Height = Int16.Parse(txtHeight.Text);
            }
        }
        

        private void button1_Click(object sender, EventArgs e)
        {
            map.CropImage();
            txtTotal.Text = map.count.ToString();
            txtSaved.Text = map.saved.ToString();
        }

        private void btnWrite_Click(object sender, EventArgs e)
        {
            map.WriteMatrix();
        }

    }
}
