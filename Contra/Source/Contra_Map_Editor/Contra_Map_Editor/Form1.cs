using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Contra_Map_Editor
{
    public partial class Form1 : Form
    {
        Timer time;                                             //update lại form sau 10ms
        StreamWriter g_FileStream,savequadtree_filestream;                              //nhập xuất file
        Pen g_Pen = new Pen(Brushes.Black);                     //bút vẽ
        Bitmap map,backBuffer,objDrawingSurface;                //kỹ thuật double buffer
        bool is_ClickLoadBtn = false;                           //trạng thái của nút load
        int hScrollBar_Value = 0,vScrollBar_Value=0;                               //giá trị của thanh cuộn
        int numOfCells_y = 0;                                   //số hàng của map
        int numOfCells_x = 0;                                   //số cột của map
        int cellSize = 64;                                      //kích thước mỗi ô trong map_matrix
        int g_itemchoice=0;

        bool isClickItem = false;
        Point mouse;
        Size sizechange = new Size(0, 0);
        List<Object> objects = new List<Object>();

        Node root_node; // quad tree

        bool is_DrawGrid = false;

        Graphics g; //graphics for panel

        Pen pen;

        //hàm khởi tạo
        public Form1()
        {
            InitializeComponent();
            time = new Timer();
            time.Interval = 10;
            time.Start();
            time.Tick += time_Tick;
            backBuffer = new Bitmap(panel1.Width, panel1.Height);
            addResources();
            pen = new Pen(Brushes.Red, 5);
        }

        //add resources of enemies to listview
        void addResources()
        {
            imageList1.Images.Add(Properties.Resources.OBJ_REDCANON);
            imageList1.Images.Add(Properties.Resources.OBJ_CANON);
            imageList1.Images.Add(Properties.Resources.OBJ_RIFLE);
            imageList1.Images.Add(Properties.Resources.OBJ_FALCON);
            imageList1.Images.Add(Properties.Resources.OBJ_SOLDIER);
            imageList1.Images.Add(Properties.Resources.OBJ_PLATFORM);
            imageList1.Images.Add(Properties.Resources.OBJ_FLYINGCAPSULE);
            imageList1.Images.Add(Properties.Resources.OBJ_BRIDGE);
            imageList1.Images.Add(Properties.Resources.OBJ_WATER);
            imageList1.Images.Add(Properties.Resources.OBJ_ARTILLERY);
            imageList1.Images.Add(Properties.Resources.OBJ_BOM);
            imageList1.Images.Add(Properties.Resources.OBJ_FIRE);
            imageList1.Images.Add(Properties.Resources.OBJ_PLATFORM_ROCK);
            imageList1.Images.Add(Properties.Resources.OBJ_RIFLEMAN_DIVE);
            imageList1.Images.Add(Properties.Resources.OBJ_TANK);
            imageList1.Images.Add(Properties.Resources.OBJ_ROCK);

            //item type
            imageList1.Images.Add(Properties.Resources.Type_B);
            imageList1.Images.Add(Properties.Resources.Type_F);
            imageList1.Images.Add(Properties.Resources.Type_L);
            imageList1.Images.Add(Properties.Resources.Type_M);
            imageList1.Images.Add(Properties.Resources.Type_R);
            imageList1.Images.Add(Properties.Resources.Type_S);
            imageList1.Images.Add(Properties.Resources.Type_SSS);

            imageList1.Images.Add(Properties.Resources.OBJ_HIDE_RIFLEMAN);

            for (int i = 0; i < imageList1.Images.Count; i++)
            {
                ListViewItem item = new ListViewItem();
                item.ImageIndex = i;
                item.Text = "Type" + (i + 1).ToString();
                listView1.Items.Add(item);
            }
        }

        //hàm dùng để update lại form sau 10ms
        void time_Tick(object sender, EventArgs e)
        {
            panel1_Paint(sender,new PaintEventArgs(panel1.CreateGraphics(),new Rectangle(panel1.Location.X,panel1.Location.Y,panel1.Width,panel1.Height)));
        }

        //button save
        private void button1_Click(object sender, EventArgs e)
        {
            SaveFileDialog saved = new SaveFileDialog();
            if (saved.ShowDialog() == DialogResult.OK)
            {
                g_FileStream = new StreamWriter(saved.FileName + "Objects.txt");
                g_FileStream.WriteLine("ID" + "\t\t" + "Type" + "\t\t" + "Pos_X" + "\t\t" + "Pos_Y" + "\t\t" + "Width" + "\t\t" + "Height" + "\t\t" + "Item_Type" + "\t\t" + "Damage" + "\t\t" + "Range");
                int temp=0;
                foreach (Object o in this.objects)
                {
                    o._id = temp;
                    
                    g_FileStream.WriteLine(temp + "\t\t" + o._type + "\t\t" +
                                   o._pos_x + "\t\t" + (Object.transform(new Point(0,map.Height/2),new Point(o._pos_x,o._pos_y)).Y - o._height) + "\t\t" + o._width + "\t\t" + o._height + "\t\t" + o._item_type + "\t\t" + o._damage + "\t\t" + o._range);

                    //g_FileStream.WriteLine(temp + "\t\t" + o._type + "\t\t" +
                    //                o._pos_x + "\t\t" + ((map.Height-o._height-o._pos_y)) + "\t\t" + o._width + "\t\t" + o._height + "\t\t" + o._hp + "\t\t" + o._damage + "\t\t" + o._range);
                    temp++;
                }
                g_FileStream.Close();

                savequadtree_filestream = new StreamWriter(saved.FileName + "QuadTree.txt");
                savequadtree_filestream.WriteLine("Pos_X" + "\t\t" + "Pos_Y" + "\t\t" + "Width" + "\t\t" + "Height" + "\t\t" + "Node_Num" + "\t" + "Objs_Num" + "\t" + "List_Obj");

                root_node = new Node("0", new Rectangle(0, 0, map.Width, map.Height));
                foreach (Object o in objects)
                {
                    root_node.Insert(o);
                }
                root_node.Save(savequadtree_filestream);

                savequadtree_filestream.Close();


                MessageBox.Show("Done!!!");
            }

        }

        //button clear
        private void button4_Click(object sender, EventArgs e)
        {
            if (is_ClickLoadBtn)
            {
                g.Clear(Color.White);
                is_ClickLoadBtn = false;
            }
            objects.RemoveRange(0, objects.Count);
        }


        //sự kiện thay đổi giá trị của thanh cuộn ngang
        private void hScrollBar1_ValueChanged(object sender, EventArgs e)
        {
            hScrollBar_Value = hScrollBar1.Value*cellSize;
            Invalidate();
        }

        //vẽ lên panel
        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            
            if (is_ClickLoadBtn)
            {
                g = Graphics.FromImage(objDrawingSurface);
                g.Clear(Color.White);
            }

            if (is_ClickLoadBtn)
            {
                float temp = (float)(cellSize *numOfCells_y) / map.Height;
                g.DrawImage(map, -hScrollBar_Value, -vScrollBar_Value, map.Width*temp, cellSize * numOfCells_y);
            }

            if (is_DrawGrid)
            {
                for (int y = 0; y < numOfCells_y; ++y)
                {
                    g.DrawLine(g_Pen, -hScrollBar_Value, y * cellSize, numOfCells_x * cellSize, y * cellSize);
                }

                for (int x = 0; x < numOfCells_x; ++x)
                {
                    g.DrawLine(g_Pen, x * cellSize, 0, x * cellSize, (numOfCells_y - 1) * cellSize);
                }
            }

            if(isClickItem)
            {
                Image image = Object.getImageByType(getTypeFromListImage(g_itemchoice));
                Rectangle rec = new Rectangle(0, 0, Object.getImageByType(getTypeFromListImage(g_itemchoice)).Width, Object.getImageByType(g_itemchoice).Height);
                rec.X = mouse.X;
                rec.Y = mouse.Y;
                g.DrawImage(image, mouse.X,mouse.Y);
            }

            if (objects.Count != 0)
            {
                for (int i = 0; i < objects.Count; i++)
                {
                    //if (objects.ElementAt(i)._type != 5)
                    //    g.DrawImage(objects.ElementAt(i).getImageByType(), objects.ElementAt(i)._pos_x - objects.ElementAt(i).getImageByType().Width / 2 - hScrollBar_Value, objects.ElementAt(i)._pos_y - objects.ElementAt(i).getImageByType().Height / 2);
                    //else
                    //    g.DrawRectangle(pen, objects.ElementAt(i)._pos_x - objects.ElementAt(i)._width/2 - hScrollBar_Value, objects.ElementAt(i)._pos_y - objects.ElementAt(i)._height/2, objects.ElementAt(i)._width, objects.ElementAt(i)._height);

                    g.DrawImage(objects.ElementAt(i).getImageByType(), objects.ElementAt(i)._pos_x*2 - hScrollBar_Value, objects.ElementAt(i)._pos_y*2 - vScrollBar_Value);

                    int item_type = 0;
                    if (objects.ElementAt(i)._item_type != 0)
                    {
                        g.DrawImage(Object.getImageByType(objects.ElementAt(i)._item_type + 29 ), objects.ElementAt(i)._pos_x * 2 - hScrollBar_Value, objects.ElementAt(i)._pos_y * 2 - vScrollBar_Value);
                    }
                }
            }

            Graphics graphics = panel1.CreateGraphics();
            objDrawingSurface = backBuffer;
            graphics.DrawImageUnscaled(backBuffer, 0, 0);
        }

        //button load
        private void button2_Click(object sender, EventArgs e)
        {            
            OpenFileDialog open = new OpenFileDialog();
            if (open.ShowDialog() == DialogResult.OK)
            {
                map = (Bitmap)Bitmap.FromFile(open.FileName);
                if (sizechange != map.Size)
                {
                    is_ClickLoadBtn = true;
                    numOfCells_x = map.Width / (cellSize / 2) + 1;
                    numOfCells_y = map.Height / (cellSize / 2);
                    objDrawingSurface = new Bitmap(numOfCells_x * cellSize, numOfCells_y * cellSize, PixelFormat.Format24bppRgb);
                    hScrollBar1.Maximum = numOfCells_x;
                    vScrollBar1.Maximum = numOfCells_y+5;
                }
            }
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count == 0)
                return;
            g_itemchoice = listView1.SelectedItems[0].ImageIndex;
            isClickItem = true;
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            Graphics gra = panel1.CreateGraphics();
            //mouse.X = e.X-imageList1.Images[g_itemchoice].Width/2;
            //mouse.Y = e.Y-imageList1.Images[g_itemchoice].Height/2;
            mouse.X = e.X;
            mouse.Y = e.Y;
        }

        private void panel1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (isClickItem)
                {
                    int width=0, height=0,pos_x=0,pos_y=0;
                    //width = imageList1.Images[g_itemchoice].Width;
                    //height = imageList1.Images[g_itemchoice].Height;
                    
                    if(g_itemchoice == 5)
                    {
                        foreach (Object obj in this.objects)
                        {
                            if (obj.getType() == 5 && e.Y + vScrollBar_Value > obj._pos_y*2 && e.Y + vScrollBar_Value < obj._pos_y*2 + obj._height*2 && e.X + hScrollBar_Value> obj._pos_x*2 + obj._width*2)
                            {
                                pos_x = obj._pos_x*2 + obj._width*2 - hScrollBar_Value;
                                pos_y = obj._pos_y*2 - vScrollBar_Value;
                            }
                            else
                            {
                                pos_x = e.X;
                                pos_y = e.Y;
                            }
                        }
                    }

                    if (g_itemchoice == 8)
                    {
                        foreach (Object obj in this.objects)
                        {
                            if (obj.getType() == 8 && e.Y > obj._pos_y*2 && e.Y < obj._pos_y*2 + obj._height*2 && e.X + hScrollBar_Value > obj._pos_x*2 + obj._width*2)
                            {
                                pos_x = obj._pos_x*2 + obj._width*2 - hScrollBar_Value;
                                pos_y = obj._pos_y*2 - vScrollBar_Value;
                            }
                            else
                            {
                                pos_x = e.X;
                                pos_y = e.Y;
                            }
                        }
                    }

                    if (g_itemchoice != 5 && g_itemchoice != 8 || objects.Count == 0)
                    {
                        pos_x = e.X;
                        pos_y = e.Y;
                    }

                    width = Object.getImageByType(getTypeFromListImage(g_itemchoice)).Width;
                    height = Object.getImageByType(getTypeFromListImage(g_itemchoice)).Height;

                    int item_type = 0;
                    if (getTypeFromListImage(g_itemchoice) >= 30)
                    {
                        foreach (Object obj in objects)
                        {
                            if (e.X < obj._pos_x * 2 + obj._width * 2 - hScrollBar_Value &&
                                e.X > obj._pos_x * 2 - hScrollBar_Value &&
                                e.Y < obj._pos_y * 2 + obj._height * 2 - vScrollBar_Value &&
                                e.Y > obj._pos_y * 2 - vScrollBar_Value)
                            {
                                item_type = getTypeFromListImage(g_itemchoice) - 29;
                                obj._item_type = item_type;
                            }
                        }
                    }

                    if (getTypeFromListImage(g_itemchoice) < 30)
                    {
                        Object obj1 = new Object(getTypeFromListImage(g_itemchoice), pos_x / 2 + hScrollBar_Value / 2, pos_y / 2 + vScrollBar_Value / 2, width / 2, height / 2, item_type);
                        objects.Add(obj1);
                    }
                }

            }

            if (e.Button == MouseButtons.Right)
            {
                for (int i = 0; i < objects.Count; i++)
                {
                    if (e.X < objects.ElementAt(i)._pos_x*2 + objects.ElementAt(i)._width*2 - hScrollBar_Value  &&
                        e.X > objects.ElementAt(i)._pos_x*2 - hScrollBar_Value &&
                        e.Y < objects.ElementAt(i)._pos_y*2 + objects.ElementAt(i)._height*2 - vScrollBar_Value &&
                        e.Y > objects.ElementAt(i)._pos_y * 2 - vScrollBar_Value)
                        objects.RemoveAt(i);
                }
            }

            if (e.Button != MouseButtons.Right && e.Button != MouseButtons.Left)
                isClickItem = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            is_DrawGrid = !is_DrawGrid;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            String temp = "";
            OpenFileDialog open = new OpenFileDialog();
            if (open.ShowDialog() == DialogResult.OK)
            {
                Point transform;
                StreamReader g_reader = new StreamReader(open.FileName);
                g_reader.ReadLine();
                objects.RemoveRange(0, objects.Count);
                while(!g_reader.EndOfStream)
                {
                    temp = g_reader.ReadLine();
                    if (temp != null)
                    {
                        transform = Object.transform(new Point(0,map.Height/2),new Point(SplitString(temp, 3),SplitString(temp, 4)));
                        //Object obj = new Object(getTypeFromListImage(SplitString(temp, 2)), transform.X, transform.Y - SplitString(temp, 6), SplitString(temp, 5), SplitString(temp, 6), SplitString(temp, 7));
                        Object obj = new Object(SplitString(temp, 2), transform.X, transform.Y - SplitString(temp, 6), SplitString(temp, 5), SplitString(temp, 6), SplitString(temp, 7));
                        objects.Add(obj);
                    }
                }
                g_reader.Close();
            }
        }

        private int SplitString(String source, int numOfElement)
        {
            int kq = 0;
            String temp = "";
            int temp_count = 0;

            for (int i = 0; i < source.Length; i++)
            {
                if (source.ElementAt(i) == '\t')
                    temp_count++;
                if (temp_count == numOfElement * 2)
                    break;
                if (temp_count == (numOfElement - 1) * 2)
                    temp += source.ElementAt(i);
            }

            kq = Int32.Parse(temp);
            return kq;
        }

        private void vScrollBar1_ValueChanged(object sender, EventArgs e)
        {
            vScrollBar_Value = vScrollBar1.Value * cellSize;
            Invalidate();
        }


        private int getTypeFromListImage(int type)
        {
            int Type = 0;
            switch (type)
            {
                case 0:
                    Type = 13;
                    break;
                case 1:
                    Type = 12;
                    break;
                case 2:
                    Type = 11;
                    break;
                case 3:
                    Type = 24;
                    break;
                case 4:
                    Type = 10;
                    break;
                case 5:
                    Type = 5;
                    break;
                case 6:
                    Type = 21;
                    break;
                case 7:
                    Type = 7;
                    break;
                case 8:
                    Type = 6;
                    break;
                case 9:
                    Type = 15;
                    break;
                case 10:
                    Type = 18;
                    break;
                case 11:
                    Type = 19;
                    break;
                case 12:
                    Type = 8;
                    break;
                case 13:
                    Type = 16;
                    break;
                case 14:
                    Type = 14;
                    break;
                case 15:
                    Type = 17;
                    break;
                
                    //item type
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                    Type = type + 14;
                    break;
                case 23:
                    Type = 23;
                    break;
            }
            return Type;
        }
    }
}
