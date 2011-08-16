using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Configuration;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        int i = 0;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            i++;
            textBox1.Text = "Hello, World:" + i+","+System.DateTime.Now.ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MessageBox.Show(Resource1.String1); // 访问资源
        }

        private void button3_Click(object sender, EventArgs e)
        {
            // 微软不太建议动态写入app.config
            // 添加引用 System.configuration
            string s = "AppSettings:";
            foreach (string key in ConfigurationManager.AppSettings)
            {
                string value = ConfigurationManager.AppSettings[key];
                s += "\n"+key + ":" + value + ";";
            }
            MessageBox.Show(s+"\n"+ConfigurationManager.AppSettings["aaa"]);
            //MessageBox.Show(System.Configuration.ConfigurationManager.GetSection("userSettings")["aaa"]);
        }
    }
}
