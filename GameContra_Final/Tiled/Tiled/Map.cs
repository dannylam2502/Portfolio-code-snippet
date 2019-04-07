using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using System.Collections;
using System.IO;


namespace Tiled
{
    class Map
    {
        String FilePath =@"C:\Users\darke_000\Desktop\Map_Test\stage1.txt";
        public Bitmap mainBitmap;
        public Rectangle section;
        public int x = 0;
        public int y = 0;
        public int count = 0;
        public int saved = 0;
        public ArrayList bmList;
        public void CropImage()
        {
            bmList = new ArrayList();
            for (x = 0; x < mainBitmap.Width ;x+= section.Width)
            {
                for(y=0;y<mainBitmap.Height - 480;y+= section.Height)
                {
                    // An empty bitmap which will hold the cropped image
                    Bitmap bmp = new Bitmap(section.Width, section.Height);

                    Graphics g = Graphics.FromImage(bmp);

                    // Draw the given area (section) of the source image
                    // at location 0,0 on the empty bitmap (bmp)
                    section.X = x;
                    section.Y = y;
                    g.DrawImage(mainBitmap, 0, 0, section, GraphicsUnit.Pixel);
                    count++;
                    //bmp.Save(@"D:\TiledMap\Notcut\abc" + count.ToString() + ".bmp");
                    Bitmap temp = bmp;
                    bmList.Add(temp);
                }
            }
            Filter();
            //bmp.Dispose();           
        }
        public void Filter()
        {
            for(int i=0;i<bmList.Count-1;i++)
            {
                for(int j=i+1;j<bmList.Count;j++)
                {
                    if (Compare((Bitmap)bmList[i], (Bitmap)bmList[j]))
                    {
                        bmList.RemoveAt(j);
                        j--;
                    }
                }
            }
            for (int i = 0; i < bmList.Count;i++ )
            {
                Bitmap copy = (Bitmap)bmList[i];
                copy.Save(@"C:\Users\darke_000\Desktop\Map_Test\" + i.ToString() + ".png");
            }
                saved = bmList.Count;
        }
        public static bool Compare(Bitmap firstBitmap, Bitmap secondBitmap)
        {
            //bool flat = true;
            Color firstPixel;
            Color secondPixel;

            if (firstBitmap.Width == secondBitmap.Width && firstBitmap.Height == secondBitmap.Height)
            {
                for (int i = 0; i < firstBitmap.Width; i++)
                {
                    for (int j = 0; j < firstBitmap.Height; j++)
                    {
                        firstPixel = firstBitmap.GetPixel(i, j);
                        secondPixel = secondBitmap.GetPixel(i, j);
                        //firstPixel = firstBitmap.GetPixel(i, j);
                        //secondPixel = secondBitmap.GetPixel(i, j);
                        if (firstPixel != secondPixel)
                        {
                            return false;
                        }
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }
        public void WriteMatrix()
        {
            Graphics g;
            Bitmap bmp;
            StreamWriter sw = new StreamWriter(FilePath);
            for (y = 0; y < mainBitmap.Height-480; y += section.Width)
            {
                for (x = 0; x < mainBitmap.Width; x += section.Height)
                {
                    // An empty bitmap which will hold the cropped image

                    Point trans = new Point(x, (int)(Math.Sin(3.14) *x + Math.Cos(3.14) * (y - 1088) + 1088));
                    trans.Y -= 32;

                    bmp = new Bitmap(section.Width, section.Height);

                    g = Graphics.FromImage(bmp);
                    g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                    g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

                    // Draw the given area (section) of the source image
                    // at location 0,0 on the empty bitmap (bmp)
                    section.X = trans.X;
                    section.Y = trans.Y;
                    g.DrawImage(mainBitmap, 0, 0, section, GraphicsUnit.Pixel);
                    for (int i = 0; i < bmList.Count;i++)
                    {
                        Bitmap copy = (Bitmap)bmList[i];
                        if (Compare(copy, bmp))
                        {
                            sw.Write(i.ToString() + " ");
                            break;
                        }
                    }
                }
                sw.WriteLine();
            }
            sw.Close();
        }
    }
}
