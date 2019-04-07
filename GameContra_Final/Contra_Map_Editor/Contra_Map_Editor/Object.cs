using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Contra_Map_Editor
{
    
    class Object
    {
        int range = 600;

        public int  _id,_type, _pos_x, _pos_y, _hp, _damage, _range,_width,_height,_item_type;
        public Object(int type, int Pos_X, int Pos_Y,int width,int height,int item_type)
        {
            _type = type;
            _pos_x = Pos_X;
            _pos_y = Pos_Y;
            _width = width;
            _height = height;
            _item_type = item_type;
            //switch (_type)
            //{
            //    case 0:
            //        _hp = 10;
            //        _damage = 1;
            //        _range = range;
            //        break;
            //    case 1:
            //        _hp = 10;
            //        _damage = 1;
            //        _range = range;
            //        break;
            //    case 2:
            //        _hp = 1;
            //        _damage = 1;
            //        _range = range;
            //        break;
            //    case 3:
            //        _hp = 3;
            //        _damage = 0;
            //        _range = 0;
            //        break;
            //    case 4:
            //        _hp = 1;
            //        _damage = 1;
            //        _range = range;
            //        break;
            //    case 5:
            //        _hp = 0;
            //        _damage = 0;
            //        _range = 0;
            //        break;
            //    case 6:
            //        _hp = 1;
            //        _damage = 0;
            //        _range = 0;
            //        break;
            //    case 7:
            //        _hp = 0;
            //        _damage = 0;
            //        _range = 0;
            //        break;
            //    case 8:
            //        _hp = 0;
            //        _damage = 0;
            //        _range = 0;
            //        break;
            //    case 9:
            //        _hp = 10;
            //        _damage = 1;
            //        _range = 600;
            //        break;
            //    case 10:
            //        _hp = 0;
            //        _damage = 1;
            //        _range = 600;
            //        break;
            //    case 11:
            //        _hp = 0;
            //        _damage = 1;
            //        _range = 600;
            //        break;
            //    case 12:
            //        _hp = 0;
            //        _damage = 0;
            //        _range = 600;
            //        break;
            //    case 13:
            //        _hp = 2;
            //        _damage = 1;
            //        _range = 600;
            //        break;
            //    case 14:
            //        _hp = 20;
            //        _damage = 1;
            //        _range = 600;
            //        break;
            //}
        }

        public Image getImageByType()
        {
            Image image = new Bitmap(32,32);
            switch (_type)
            {
                case 13:
                    image = Properties.Resources.OBJ_REDCANON;
                    break;
                case 12:
                    image = Properties.Resources.OBJ_CANON;
                    break;
                case 11:
                    image = Properties.Resources.OBJ_RIFLE;
                    break;
                case 24:
                    image = Properties.Resources.OBJ_FALCON;
                    break;
                case 10:
                    image = Properties.Resources.OBJ_SOLDIER;
                    break;
                case 5:
                    image = Properties.Resources.OBJ_PLATFORM;
                    break;
                case 21:
                    image = Properties.Resources.OBJ_FLYINGCAPSULE;
                    break;
                case 7:
                    image = Properties.Resources.OBJ_BRIDGE;
                    break;
                case 6:
                    image = Properties.Resources.OBJ_WATER;
                    break;
                case 15:
                    image = Properties.Resources.OBJ_ARTILLERY;
                    break;
                case 18:
                    image = Properties.Resources.OBJ_BOM;
                    break;
                case 19:
                    image = Properties.Resources.OBJ_FIRE;
                    break;
                case 8:
                    image = Properties.Resources.OBJ_PLATFORM_ROCK;
                    break;
                case 16:
                    image = Properties.Resources.OBJ_RIFLEMAN_DIVE;
                    break;
                case 14:
                    image = Properties.Resources.OBJ_TANK;
                    break;
                case 17:
                    image = Properties.Resources.OBJ_ROCK;
                    break;
                case 23:
                    image = Properties.Resources.OBJ_HIDE_RIFLEMAN;
                    break;

                //item type
                case 30:
                    image = Properties.Resources.Type_B;
                    break;
                case 31:
                    image = Properties.Resources.Type_F;
                    break;
                case 32:
                    image = Properties.Resources.Type_L;
                    break;
                case 33:
                    image = Properties.Resources.Type_M;
                    break;
                case 34:
                    image = Properties.Resources.Type_R;
                    break;
                case 35:
                    image = Properties.Resources.Type_S;
                    break;
                case 36:
                    image = Properties.Resources.Type_SSS;
                    break;
            }
            return image;
        }

        public static Image getImageByType(int type)
        {
            Image image = new Bitmap(16,8);
            switch (type)
            {
                //case 0:
                //    image = Properties.Resources.OBJ_REDCANON;
                //    break;
                //case 1:
                //    image = Properties.Resources.OBJ_CANON;
                //    break;
                //case 2:
                //    image = Properties.Resources.OBJ_RIFLE;
                //    break;
                //case 3:
                //    image = Properties.Resources.OBJ_FALCON;
                //    break;
                //case 4:
                //    image = Properties.Resources.OBJ_SOLDIER;
                //    break;
                //case 5:
                //    image = Properties.Resources.OBJ_PLATFORM;
                //    break;
                //case 6:
                //    image = Properties.Resources.OBJ_FLYINGCAPSULE;
                //    break;
                //case 7:
                //    image = Properties.Resources.OBJ_BRIDGE;
                //    break;
                //case 8:
                //    image = Properties.Resources.OBJ_WATER;
                //    break;
                //case 9:
                //    image = Properties.Resources.OBJ_ARTILLERY;
                //    break;
                //case 10:
                //    image = Properties.Resources.OBJ_BOM;
                //    break;
                //case 11:
                //    image = Properties.Resources.OBJ_FIRE;
                //    break;
                //case 12:
                //    image = Properties.Resources.OBJ_PLATFORM_ROCK;
                //    break;
                //case 13:
                //    image = Properties.Resources.OBJ_RIFLEMAN_DIVE;
                //    break;
                //case 14:
                //    image = Properties.Resources.OBJ_TANK;
                //    break;
                //case 15:
                //    image = Properties.Resources.OBJ_ROCK;
                //    break;

                case 13:
                    image = Properties.Resources.OBJ_REDCANON;
                    break;
                case 12:
                    image = Properties.Resources.OBJ_CANON;
                    break;
                case 11:
                    image = Properties.Resources.OBJ_RIFLE;
                    break;
                case 24:
                    image = Properties.Resources.OBJ_FALCON;
                    break;
                case 10:
                    image = Properties.Resources.OBJ_SOLDIER;
                    break;
                case 5:
                    image = Properties.Resources.OBJ_PLATFORM;
                    break;
                case 21:
                    image = Properties.Resources.OBJ_FLYINGCAPSULE;
                    break;
                case 7:
                    image = Properties.Resources.OBJ_BRIDGE;
                    break;
                case 6:
                    image = Properties.Resources.OBJ_WATER;
                    break;
                case 15:
                    image = Properties.Resources.OBJ_ARTILLERY;
                    break;
                case 18:
                    image = Properties.Resources.OBJ_BOM;
                    break;
                case 19:
                    image = Properties.Resources.OBJ_FIRE;
                    break;
                case 8:
                    image = Properties.Resources.OBJ_PLATFORM_ROCK;
                    break;
                case 16:
                    image = Properties.Resources.OBJ_RIFLEMAN_DIVE;
                    break;
                case 14:
                    image = Properties.Resources.OBJ_TANK;
                    break;
                case 17:
                    image = Properties.Resources.OBJ_ROCK;
                    break;
                case 23:
                    image = Properties.Resources.OBJ_HIDE_RIFLEMAN;
                    break;

                    //item type
                case 30:
                    image = Properties.Resources.Type_B;
                    break;
                case 31:
                    image = Properties.Resources.Type_F;
                    break;
                case 32:
                    image = Properties.Resources.Type_L;
                    break;
                case 33:
                    image = Properties.Resources.Type_M;
                    break;
                case 34:
                    image = Properties.Resources.Type_R;
                    break;
                case 35:
                    image = Properties.Resources.Type_S;
                    break;
                case 36:
                    image = Properties.Resources.Type_SSS;
                    break;

            }
            return image;
        }

        public int getType()
        {
            return _type;
        }

        public static Point transform(Point origin,Point point)
        {
            Point trans = new Point(point.X, (int)(Math.Sin(3.14) * (point.X - origin.X) + Math.Cos(3.14) * (point.Y - origin.Y) + origin.Y));
            return trans;
        }
    }
}
