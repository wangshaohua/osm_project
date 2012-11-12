using System;
using System.Collections;//.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;


namespace gis
{
    public partial class Form1 : Form
    {
        private double[] startlong;
        private double[] startlat;
        private double[] endlong;
        private double[] endlat;
        private double[] nodelat;
        private double[] nodelong;


        public Form1()
        {
            InitializeComponent();
            LoadData();
            Form2Paint();
           // myDrawLine();
            //this.Paint += new System.Windows.Forms.PaintEventHandler(this.Form2Paint);
        }

        private void Form2Paint()
        {
            //Bitmap bmp = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            Bitmap bmp = new Bitmap(3000, 3500);
            SolidBrush brush = new SolidBrush(Color.Gold);
            Graphics g = Graphics.FromImage(bmp);//创建一个Graphics对象

           // g = pictureBox1.f
            
            Pen mypen = new Pen(Color.Red); //创建一个红色画笔
            Pen mypen2 = new Pen(Color.Blue);
            int length = startlat.Length;
            int scale = 500;
            for (int i = 0; i < length; ++i)
            {
                g.DrawLine(mypen, (int)((startlat[i] - 40) * scale), (int)((startlong[i] + 74) * scale), (int)((endlat[i] - 40) * scale), (int)((endlong[i] + 74) * scale));
               
            }
            for (int i = 0; i < nodelat.Length; ++i)
            {
                g.DrawLine(mypen, (int)((nodelat[i] - 40) * scale), (int)((nodelong[i] + 74) * scale), (int)((nodelat[i] - 40) * scale), (int)((nodelong[i] + 74) * scale));
            }

            g.DrawLine(mypen, new Point(10, 10), new Point(10, 10));
            bmp.Save("aa.jpg");
                pictureBox1.BackgroundImage = bmp;
        }

        private void LoadData()
        {

            #region 读取数组
            //string line;
            //string[] s;
            //string temp;
            FileStream fs = new FileStream("C:\\Users\\mfc\\Desktop\\GIS2\\GIS\\WA_Vertices.txt", FileMode.Open, FileAccess.Read);
            StreamReader reader = new StreamReader(fs, Encoding.Default);
            reader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);

            long line_number = 0;
            while (reader.ReadLine() != null)
            {
                line_number++;
            }
            nodelat = new double[line_number];
            nodelong = new double[line_number];
            //double[,] a = new double[line_number, 3];
            Hashtable[] a = new Hashtable[2];
            a[0] = new Hashtable();
            a[1] = new Hashtable();
            reader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);
            for (int i = 0; i < line_number; i++)
            {
                string line = reader.ReadLine();
                string[] s = line.Split(' ');
                for (long j = 0; j < 2; j++)
                {
                    a[j][long.Parse(s[0])] = double.Parse(s[j + 1]);
                }
                //textBox1.Text = a[1, 1].ToString();
            }
            FileStream fss = new FileStream("C:\\Users\\mfc\\Desktop\\GIS2\\GIS\\WA_Edges.txt", FileMode.Open, FileAccess.Read);
            StreamReader readers = new StreamReader(fss, Encoding.Default);
            readers.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);

            long line_numbers = 0;
            while (readers.ReadLine() != null)
            {
                line_numbers++;
            }
            Hashtable[] b = new Hashtable[2];
            b[0] = new Hashtable();
            b[1] = new Hashtable();
            
            startlong = new double[line_numbers];
            startlat = new double[line_numbers];
            endlong = new double[line_numbers];
            endlat = new double[line_numbers];
            readers.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);
            Console.WriteLine(line_numbers);
            for (long i = 0; i < line_numbers; i++)
            {
                string line = readers.ReadLine();
                string[] s = line.Split(' ');
                //for (long j = 1; j < 3; j++)
                //{
                long n1 = long.Parse(s[1]), n2 = long.Parse(s[2]);
                //double lat = (System.Double)a[0][n];
                startlat[i] = (System.Double)a[0][n1];
                startlong[i] = (System.Double)a[1][n1];
                endlat[i] = (System.Double)a[0][n2];
                endlong[i] = (System.Double)a[1][n2];


                
            }
            textBox1.Text = line_numbers.ToString();
            //myDrawLine();

            #endregion

            #region 作图
            


            #endregion

            #region 关闭文件
            reader.Close(); reader.Dispose();
            fs.Close(); fs.Dispose();
            #endregion

        }
       
        private void button1_Click(object sender, EventArgs e)
        {
            //Form2Paint(sender,e);
            #region 手动选择txt位置
            OpenFileDialog open = new OpenFileDialog();
            if (open.ShowDialog(this) == System.Windows.Forms.DialogResult.OK)
            {
                string filepath = open.FileName;//路径
                textBox1.Text = System.IO.File.ReadAllText(filepath, Encoding.Default);
            }
            #endregion`

            #region 读取数组
            //string line;
            //string[] s;
            //string temp;
            FileStream fs = new FileStream("..\\..\\..\\..\\gis\\WA_Nodes.txt", FileMode.Open, FileAccess.Read);    
            StreamReader reader = new StreamReader(fs, Encoding.Default);
            reader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);

            int line_number = 0;
            while (reader.ReadLine() != null)
            {  
                line_number++;
            }
            double[,] a=new double[line_number,3];
            reader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);
            for (int i = 0; i < line_number; i++)
            {
                string line = reader.ReadLine();
                string[] s = line.Split(' ');            
                for (int j = 0; j < 3; j++)
                {
                    string temp = s[j];               
                    a[i, j] = double.Parse(temp);                       
                }
                double []nodelat= new double [line_number];
                double []nodelong = new double[line_number];
                nodelat[i] = a[i, 1];
                nodelong[i] = a[i, 2]; 
                //textBox1.Text=a[1,1].ToString();
            }
            FileStream fss = new FileStream("..\\..\\..\\..\\gis\\WA_Edges.txt", FileMode.Open, FileAccess.Read);
            StreamReader readers = new StreamReader(fss, Encoding.Default);
            readers.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);

            int line_numbers = 0;
            while (readers.ReadLine() != null)
            {
                line_numbers++;
            }
            int[,] b = new int[line_numbers, 4];
            double[] startlong = new double[line_numbers];
            double[] startlat = new double[line_numbers];
            double[] endlong = new double[line_numbers];
            double[] endlat = new double[line_numbers];
            readers.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);
            for (int i = 0; i < line_numbers; i++)
            {
                string line = readers.ReadLine();
                string[] s = line.Split(' ');
                for (int j = 0; j < 4; j++)
                {
                    string temp = s[j];
                    b[i, j] = int.Parse(temp);
                }


                startlat[i] = a[b[i,1],1];
                startlong[i]= a[b[i,1],2];
                endlat[i]= a[b[i,2],1];
                endlong[i]= a[b[i,2],2];


                textBox1.Text = a[1, 1].ToString();
            }
            //myDrawLine();

        #endregion

            #region 作图
        //    double [] xstr = new double[1000];
        //    double[] ystr = new double[1000];
        //for (int i = 0; i < 1000 ;i++)
        //{
        //     ystr[i]= a[i,2];
        //     xstr[i]= a[i,1];
        //}
        ////Chart1.Series["Series1"].Points.DataBindXY(xstr, ystr);
    //}


            //Graphics g = this.CreateGraphics();//创建一个Graphics对象
            //g.Clip = new Region(new Rectangle(10, 100, 100000, 100000));

            //Pen mypen = new Pen(Color.Red); //创建一个红色画笔
            //for (int i = 0; i < line_numbers; ++i)
            //{
            //    g.DrawLine(mypen, (int)startlat[i], (int)startlong[i], (int)endlat[i], (int)endlong[i]);
            
            //}
            //g.DrawLine(mypen,10, 100,20,200);


            #endregion



        }
        
    }
}
