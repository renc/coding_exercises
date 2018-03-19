using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Button m_button1;
        public Form1()
        {
            InitializeComponent();
            m_button1 = new Button();
            m_button1.Size = new Size(40, 40);
            m_button1.Location = new Point(30, 30);
            m_button1.Text = "Click me";
            this.Controls.Add(m_button1);
            m_button1.Click += new EventHandler(button1_Click);
        }
        private void button1_Click(Object sender, EventArgs eg)
        {
            MessageBox.Show("Hello World");
        }
    }
}
