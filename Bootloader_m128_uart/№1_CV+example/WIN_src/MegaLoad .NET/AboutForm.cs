// Type: MegaLoad_.NET.AboutForm
// Assembly: MegaLoad .NET, Version=1.0.2628.34316, Culture=neutral, PublicKeyToken=null
// MVID: C0A102E7-E817-4560-BBFC-BB5054A5A3E4
// Assembly location: C:\Program Files\MicroSyl\MegaLoad\MegaLoad .NET.exe

using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO.Ports;
using System.Windows.Forms;

namespace MegaLoad_.NET
{
  public class AboutForm : Form
  {
    private SerialPort Comm = (SerialPort) null;
    private Container components = (Container) null;
    private PictureBox pictureBox1;
    private Label label1;
    private Label label2;
    private LinkLabel linkLabel1;
    private LinkLabel linkLabel2;
    private ListBox listBox1;
    private MegaLoadForm MegaLoad;
    private GroupBox groupBox1;
    private LinkLabel linkLabel3;
    private Label label4;

    public AboutForm(MegaLoadForm FormArg, SerialPort CommArg, bool Reg)
    {
      this.InitializeComponent();
      this.MegaLoad = FormArg;
      this.Comm = CommArg;
      this.Comm.Close();
    }

    protected override void Dispose(bool disposing)
    {
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
      ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof (AboutForm));
      this.pictureBox1 = new PictureBox();
      this.label1 = new Label();
      this.label2 = new Label();
      this.linkLabel1 = new LinkLabel();
      this.linkLabel2 = new LinkLabel();
      this.listBox1 = new ListBox();
      this.groupBox1 = new GroupBox();
      this.linkLabel3 = new LinkLabel();
      this.label4 = new Label();
      this.pictureBox1.BeginInit();
      this.groupBox1.SuspendLayout();
      this.SuspendLayout();
      this.pictureBox1.Image = (Image) componentResourceManager.GetObject("pictureBox1.Image");
      this.pictureBox1.Location = new Point(24, 24);
      this.pictureBox1.Name = "pictureBox1";
      this.pictureBox1.Size = new Size(40, 40);
      this.pictureBox1.TabIndex = 0;
      this.pictureBox1.TabStop = false;
      this.label1.Font = new Font("Microsoft Sans Serif", 15.75f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.label1.Location = new Point(72, 24);
      this.label1.Name = "label1";
      this.label1.Size = new Size(248, 32);
      this.label1.TabIndex = 1;
      this.label1.Text = "MegaLoad .NET V:6.3";
      this.label2.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label2.Location = new Point(144, 56);
      this.label2.Name = "label2";
      this.label2.Size = new Size(40, 24);
      this.label2.TabIndex = 2;
      this.label2.Text = "By:";
      this.linkLabel1.Font = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.linkLabel1.Location = new Point(72, 80);
      this.linkLabel1.Name = "linkLabel1";
      this.linkLabel1.Size = new Size(184, 24);
      this.linkLabel1.TabIndex = 3;
      this.linkLabel1.TabStop = true;
      this.linkLabel1.Text = "sbissonnette@MicroSyl.Com";
      this.linkLabel1.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
      this.linkLabel2.Font = new Font("Microsoft Sans Serif", 14.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.linkLabel2.Location = new Point(80, 104);
      this.linkLabel2.Name = "linkLabel2";
      this.linkLabel2.Size = new Size(176, 32);
      this.linkLabel2.TabIndex = 4;
      this.linkLabel2.TabStop = true;
      this.linkLabel2.Text = "www.microsyl.com";
      this.linkLabel2.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabel2_LinkClicked);
      this.listBox1.Items.AddRange(new object[37]
      {
        (object) "Hi AVR Fans!",
        (object) "",
        (object) "  If you find this software useful, I would be ",
        (object) "pleased to receive some Atmel 'Megas' ",
        (object) "of your choice. ",
        (object) "",
        (object) "I have spent a lot of time and effort to write this ",
        (object) "software and would really appreciate it is you took ",
        (object) "a a few minutes to put a 'Mega' in the mail to me.",
        (object) "",
        (object) "My favourite Atmel Mega Micros are ATMega128, ",
        (object) "AtMega64, AtMega8 (TQFP) - Thank you",
        (object) "",
        (object) "If you'd rather not send parts via mail would be ",
        (object) "pleased to receive some $ to my paypal account:",
        (object) "sbissonnette@microsyl.com",
        (object) "",
        (object) "The source code for the windows application is ",
        (object) "available for US$100 and is subject to a ",
        (object) "Non Disclosure Agreement.",
        (object) "",
        (object) "The code is written in Microsoft Visual C# .NET Studio. ",
        (object) "",
        (object) "You will also need a external component for the serial ",
        (object) "communication ",
        (object) "(I use 'CommPort 3.0' from 'http://www.winsoft.sk/')",
        (object) "",
        (object) "",
        (object) "Thanks you for using Megaload.",
        (object) "",
        (object) "Yours faithfully,",
        (object) "",
        (object) "Sylvain Bissonnette",
        (object) "660 Marco-Polo",
        (object) "Boucherville, Qc",
        (object) "J4B 5R4",
        (object) "CANADA"
      });
      this.listBox1.Location = new Point(16, 232);
      this.listBox1.Name = "listBox1";
      this.listBox1.Size = new Size(304, 108);
      this.listBox1.TabIndex = 5;
      this.groupBox1.Controls.Add((Control) this.linkLabel3);
      this.groupBox1.Controls.Add((Control) this.label4);
      this.groupBox1.Location = new Point(8, 8);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new Size(320, 384);
      this.groupBox1.TabIndex = 8;
      this.groupBox1.TabStop = false;
      this.linkLabel3.Font = new Font("Microsoft Sans Serif", 14.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.linkLabel3.Location = new Point(64, 176);
      this.linkLabel3.Name = "linkLabel3";
      this.linkLabel3.Size = new Size(192, 32);
      this.linkLabel3.TabIndex = 9;
      this.linkLabel3.TabStop = true;
      this.linkLabel3.Text = "www.imagecraft.com";
      this.linkLabel3.LinkClicked += new LinkLabelLinkClickedEventHandler(this.linkLabel3_LinkClicked);
      this.label4.Font = new Font("Microsoft Sans Serif", 11.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.label4.Location = new Point(64, 152);
      this.label4.Name = "label4";
      this.label4.Size = new Size(192, 24);
      this.label4.TabIndex = 9;
      this.label4.Text = "BootLoader C Compiler";
      this.AutoScaleBaseSize = new Size(5, 13);
      this.BackColor = SystemColors.Control;
      this.ClientSize = new Size(338, 400);
      this.Controls.Add((Control) this.listBox1);
      this.Controls.Add((Control) this.linkLabel2);
      this.Controls.Add((Control) this.linkLabel1);
      this.Controls.Add((Control) this.label2);
      this.Controls.Add((Control) this.label1);
      this.Controls.Add((Control) this.pictureBox1);
      this.Controls.Add((Control) this.groupBox1);
      this.FormBorderStyle = FormBorderStyle.FixedSingle;
      this.Icon = (Icon) componentResourceManager.GetObject("$this.Icon");
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "AboutForm";
      this.StartPosition = FormStartPosition.CenterScreen;
      this.Text = "About";
      this.Deactivate += new EventHandler(this.AboutForm_Deactivate);
      this.pictureBox1.EndInit();
      this.groupBox1.ResumeLayout(false);
      this.ResumeLayout(false);
    }

    private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
    {
      Process.Start("www.microsyl.com");
    }

    private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
    {
      Process.Start("mailto:sbissonnette@microsyl.com");
    }

    private void AboutForm_Deactivate(object sender, EventArgs e)
    {
      this.Close();
    }

    private void linkLabel3_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
    {
      Process.Start("www.imagecraft.com/software/");
    }
  }
}
