// Type: MegaLoad_.NET.MegaLoadForm
// Assembly: MegaLoad .NET, Version=1.0.2628.34316, Culture=neutral, PublicKeyToken=null
// MVID: C0A102E7-E817-4560-BBFC-BB5054A5A3E4
// Assembly location: C:\Program Files\MicroSyl\MegaLoad\MegaLoad .NET.exe

using Microsoft.Win32;
using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;

namespace MegaLoad_.NET
{
  public class MegaLoadForm : Form
  {
    private bool Reg = false;
    private string FlashFileNameHex = "";
    private string EEpromFileNameHex = "";
    private GroupBox groupBox1;
    private TextBox FlashFileName;
    private Button FlashOpen;
    private GroupBox groupBox2;
    private TextBox EEpromFileName;
    private Button EEpromOpen;
    private GroupBox groupBox3;
    private CheckBox BLB12;
    private CheckBox BLB11;
    private CheckBox BLB02;
    private CheckBox BLB01;
    private Label label1;
    private GroupBox groupBox4;
    private GroupBox groupBox5;
    private GroupBox groupBox6;
    private ComboBox CommSpeed;
    private Label label2;
    private Label label3;
    private CheckBox RTS;
    private Label label4;
    private Label label5;
    private Label label6;
    private Label label7;
    private Label label8;
    private Label Device;
    private Label PageSize;
    private Label BootSize;
    private Label FlashSize;
    private Label EEpromSize;
    private GroupBox groupBox7;
    private GroupBox groupBox8;
    private GroupBox groupBox9;
    private ListBox MessageList;
    private ProgressBar ProgressBar;
    private OpenFileDialog OpenFileDialog;
    private ComboBox PortSelect;
    private byte[] Flash;
    private int FlashMin;
    private int FlashMax;
    private byte[] EEprom;
    private bool[] EEpromUse;
    private int EEpromMin;
    private int EEpromMax;
    private int BytePtr;
    private int PageSizeInt;
    private int FlashSizeInt;
    private int BootSizeInt;
    private int EEpromSizeInt;
    private int PagePtr;
    private int Retry;
    private char MemType;
    private TextBox Status;
    private CheckBox DTR;
    private Button SendReset;
    private Button MonitorButton;
    private MonitorForm Monitor;
    private AboutForm About;
    private Button PortOpenClose;
    private GroupBox groupBox10;
    private Button AboutButton;
    private ToolTip ToolTip;
    private ListBox listBox1;
    private SerialPort Comm1;
    private IContainer components;

    public MegaLoadForm()
    {
      this.InitializeComponent();
    }

    protected override void Dispose(bool disposing)
    {
      this.Comm1.Close();
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
      this.components = (IContainer) new Container();
      ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof (MegaLoadForm));
      this.groupBox1 = new GroupBox();
      this.FlashOpen = new Button();
      this.FlashFileName = new TextBox();
      this.groupBox2 = new GroupBox();
      this.EEpromOpen = new Button();
      this.EEpromFileName = new TextBox();
      this.groupBox3 = new GroupBox();
      this.label1 = new Label();
      this.BLB01 = new CheckBox();
      this.BLB12 = new CheckBox();
      this.BLB11 = new CheckBox();
      this.BLB02 = new CheckBox();
      this.groupBox4 = new GroupBox();
      this.PortOpenClose = new Button();
      this.RTS = new CheckBox();
      this.DTR = new CheckBox();
      this.label3 = new Label();
      this.label2 = new Label();
      this.CommSpeed = new ComboBox();
      this.PortSelect = new ComboBox();
      this.MonitorButton = new Button();
      this.SendReset = new Button();
      this.groupBox5 = new GroupBox();
      this.Device = new Label();
      this.label8 = new Label();
      this.label7 = new Label();
      this.label6 = new Label();
      this.label5 = new Label();
      this.label4 = new Label();
      this.PageSize = new Label();
      this.BootSize = new Label();
      this.FlashSize = new Label();
      this.EEpromSize = new Label();
      this.groupBox6 = new GroupBox();
      this.MessageList = new ListBox();
      this.groupBox7 = new GroupBox();
      this.Status = new TextBox();
      this.groupBox8 = new GroupBox();
      this.ProgressBar = new ProgressBar();
      this.groupBox9 = new GroupBox();
      this.listBox1 = new ListBox();
      this.OpenFileDialog = new OpenFileDialog();
      this.groupBox10 = new GroupBox();
      this.AboutButton = new Button();
      this.ToolTip = new ToolTip(this.components);
      this.Comm1 = new SerialPort(this.components);
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.groupBox3.SuspendLayout();
      this.groupBox4.SuspendLayout();
      this.groupBox5.SuspendLayout();
      this.groupBox6.SuspendLayout();
      this.groupBox7.SuspendLayout();
      this.groupBox8.SuspendLayout();
      this.groupBox9.SuspendLayout();
      this.groupBox10.SuspendLayout();
      this.SuspendLayout();
      this.groupBox1.Controls.Add((Control) this.FlashOpen);
      this.groupBox1.Controls.Add((Control) this.FlashFileName);
      this.groupBox1.Location = new Point(8, 8);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new Size(536, 48);
      this.groupBox1.TabIndex = 0;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "File to be programed in the Flash";
      this.FlashOpen.Location = new Point(448, 16);
      this.FlashOpen.Name = "FlashOpen";
      this.FlashOpen.Size = new Size(72, 24);
      this.FlashOpen.TabIndex = 1;
      this.FlashOpen.Text = "Open";
      this.FlashOpen.Click += new EventHandler(this.FlashOpen_Click);
      this.FlashFileName.Location = new Point(8, 16);
      this.FlashFileName.Name = "FlashFileName";
      this.FlashFileName.ReadOnly = true;
      this.FlashFileName.Size = new Size(424, 20);
      this.FlashFileName.TabIndex = 0;
      this.groupBox2.Controls.Add((Control) this.EEpromOpen);
      this.groupBox2.Controls.Add((Control) this.EEpromFileName);
      this.groupBox2.Location = new Point(8, 64);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new Size(536, 48);
      this.groupBox2.TabIndex = 1;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "File to be programed in the EEprom";
      this.EEpromOpen.Location = new Point(448, 16);
      this.EEpromOpen.Name = "EEpromOpen";
      this.EEpromOpen.Size = new Size(72, 24);
      this.EEpromOpen.TabIndex = 1;
      this.EEpromOpen.Text = "Open";
      this.EEpromOpen.Click += new EventHandler(this.EEpromOpen_Click);
      this.EEpromFileName.Location = new Point(8, 16);
      this.EEpromFileName.Name = "EEpromFileName";
      this.EEpromFileName.ReadOnly = true;
      this.EEpromFileName.Size = new Size(424, 20);
      this.EEpromFileName.TabIndex = 0;
      this.groupBox3.Controls.Add((Control) this.label1);
      this.groupBox3.Controls.Add((Control) this.BLB01);
      this.groupBox3.Controls.Add((Control) this.BLB12);
      this.groupBox3.Controls.Add((Control) this.BLB11);
      this.groupBox3.Controls.Add((Control) this.BLB02);
      this.groupBox3.Location = new Point(8, 120);
      this.groupBox3.Name = "groupBox3";
      this.groupBox3.Size = new Size(536, 40);
      this.groupBox3.TabIndex = 2;
      this.groupBox3.TabStop = false;
      this.groupBox3.Text = "BootLoader Lock Bits to be program";
      this.label1.ForeColor = SystemColors.GrayText;
      this.label1.Location = new Point(296, 16);
      this.label1.Name = "label1";
      this.label1.Size = new Size(192, 16);
      this.label1.TabIndex = 7;
      this.label1.Text = "Check means programmed (bit = 0)";
      this.BLB01.Location = new Point(208, 16);
      this.BLB01.Name = "BLB01";
      this.BLB01.Size = new Size(64, 16);
      this.BLB01.TabIndex = 6;
      this.BLB01.Text = "BLB01";
      this.BLB01.CheckedChanged += new EventHandler(this.BLB01_CheckedChanged);
      this.BLB12.Location = new Point(16, 16);
      this.BLB12.Name = "BLB12";
      this.BLB12.Size = new Size(64, 16);
      this.BLB12.TabIndex = 3;
      this.BLB12.Text = "BLB12";
      this.BLB12.CheckedChanged += new EventHandler(this.BLB12_CheckedChanged);
      this.BLB11.Location = new Point(80, 16);
      this.BLB11.Name = "BLB11";
      this.BLB11.Size = new Size(64, 16);
      this.BLB11.TabIndex = 4;
      this.BLB11.Text = "BLB11";
      this.BLB11.CheckedChanged += new EventHandler(this.BLB11_CheckedChanged);
      this.BLB02.Location = new Point(144, 16);
      this.BLB02.Name = "BLB02";
      this.BLB02.Size = new Size(64, 16);
      this.BLB02.TabIndex = 5;
      this.BLB02.Text = "BLB02";
      this.BLB02.CheckedChanged += new EventHandler(this.BLB02_CheckedChanged);
      this.groupBox4.Controls.Add((Control) this.PortOpenClose);
      this.groupBox4.Controls.Add((Control) this.RTS);
      this.groupBox4.Controls.Add((Control) this.DTR);
      this.groupBox4.Controls.Add((Control) this.label3);
      this.groupBox4.Controls.Add((Control) this.label2);
      this.groupBox4.Controls.Add((Control) this.CommSpeed);
      this.groupBox4.Controls.Add((Control) this.PortSelect);
      this.groupBox4.Location = new Point(8, 168);
      this.groupBox4.Name = "groupBox4";
      this.groupBox4.Size = new Size(168, 144);
      this.groupBox4.TabIndex = 3;
      this.groupBox4.TabStop = false;
      this.groupBox4.Text = "Comm Setup";
      this.PortOpenClose.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Regular, GraphicsUnit.Point, (byte) 0);
      this.PortOpenClose.Location = new Point(32, 112);
      this.PortOpenClose.Name = "PortOpenClose";
      this.PortOpenClose.Size = new Size(104, 24);
      this.PortOpenClose.TabIndex = 12;
      this.PortOpenClose.Text = "Close Port";
      this.PortOpenClose.Click += new EventHandler(this.PortOpenClose_Click);
      this.RTS.Location = new Point(88, 80);
      this.RTS.Name = "RTS";
      this.RTS.Size = new Size(72, 16);
      this.RTS.TabIndex = 5;
      this.RTS.Text = "RTS";
      this.RTS.CheckedChanged += new EventHandler(this.RTS_CheckedChanged);
      this.DTR.Location = new Point(8, 80);
      this.DTR.Name = "DTR";
      this.DTR.Size = new Size(72, 16);
      this.DTR.TabIndex = 4;
      this.DTR.Text = "DTR";
      this.DTR.CheckedChanged += new EventHandler(this.DTR_CheckedChanged);
      this.label3.Location = new Point(88, 24);
      this.label3.Name = "label3";
      this.label3.Size = new Size(64, 16);
      this.label3.TabIndex = 3;
      this.label3.Text = "Speed";
      this.label2.Location = new Point(8, 24);
      this.label2.Name = "label2";
      this.label2.Size = new Size(64, 16);
      this.label2.TabIndex = 2;
      this.label2.Text = "CommPort";
      this.CommSpeed.Items.AddRange(new object[5]
      {
        (object) "9600bps",
        (object) "19200bps",
        (object) "38400bps",
        (object) "57600bps",
        (object) "115kbps"
      });
      this.CommSpeed.Location = new Point(88, 40);
      this.CommSpeed.Name = "CommSpeed";
      this.CommSpeed.Size = new Size(72, 21);
      this.CommSpeed.TabIndex = 1;
      this.CommSpeed.Text = "9600";
      this.CommSpeed.SelectionChangeCommitted += new EventHandler(this.CommSpeed_SelectionChangeCommitted);
      this.PortSelect.ImeMode = ImeMode.Off;
      this.PortSelect.Items.AddRange(new object[19]
      {
        (object) "ComPort",
        (object) "Com1",
        (object) "Com2",
        (object) "Com3",
        (object) "Com4",
        (object) "Com5",
        (object) "Com6",
        (object) "Com7",
        (object) "Com8",
        (object) "Com9",
        (object) "Com10",
        (object) "Com12",
        (object) "Com13",
        (object) "Com14",
        (object) "Com15",
        (object) "Com16",
        (object) "Com17",
        (object) "Com18",
        (object) "Com19"
      });
      this.PortSelect.Location = new Point(8, 40);
      this.PortSelect.Name = "PortSelect";
      this.PortSelect.Size = new Size(72, 21);
      this.PortSelect.TabIndex = 0;
      this.PortSelect.Text = "ComPort";
      this.PortSelect.SelectionChangeCommitted += new EventHandler(this.PortSelect_SelectionChangeCommitted);
      this.MonitorButton.Location = new Point(88, 24);
      this.MonitorButton.Name = "MonitorButton";
      this.MonitorButton.Size = new Size(72, 24);
      this.MonitorButton.TabIndex = 11;
      this.MonitorButton.Text = "Monitor";
      this.MonitorButton.Click += new EventHandler(this.Monitor_Click);
      this.SendReset.Location = new Point(8, 24);
      this.SendReset.Name = "SendReset";
      this.SendReset.Size = new Size(72, 24);
      this.SendReset.TabIndex = 10;
      this.SendReset.Text = "Send Reset";
      this.SendReset.Click += new EventHandler(this.SendReset_Click);
      this.groupBox5.Controls.Add((Control) this.Device);
      this.groupBox5.Controls.Add((Control) this.label8);
      this.groupBox5.Controls.Add((Control) this.label7);
      this.groupBox5.Controls.Add((Control) this.label6);
      this.groupBox5.Controls.Add((Control) this.label5);
      this.groupBox5.Controls.Add((Control) this.label4);
      this.groupBox5.Controls.Add((Control) this.PageSize);
      this.groupBox5.Controls.Add((Control) this.BootSize);
      this.groupBox5.Controls.Add((Control) this.FlashSize);
      this.groupBox5.Controls.Add((Control) this.EEpromSize);
      this.groupBox5.Location = new Point(184, 168);
      this.groupBox5.Name = "groupBox5";
      this.groupBox5.Size = new Size(160, 144);
      this.groupBox5.TabIndex = 4;
      this.groupBox5.TabStop = false;
      this.groupBox5.Text = "Target";
      this.Device.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.Device.ForeColor = SystemColors.ActiveCaption;
      this.Device.Location = new Point(80, 24);
      this.Device.Name = "Device";
      this.Device.Size = new Size(64, 16);
      this.Device.TabIndex = 5;
      this.Device.Text = "xxxx";
      this.label8.Location = new Point(8, 120);
      this.label8.Name = "label8";
      this.label8.Size = new Size(72, 16);
      this.label8.TabIndex = 4;
      this.label8.Text = "EEpromSize:";
      this.label7.Location = new Point(8, 96);
      this.label7.Name = "label7";
      this.label7.Size = new Size(72, 16);
      this.label7.TabIndex = 3;
      this.label7.Text = "Flash Size:";
      this.label6.Location = new Point(8, 72);
      this.label6.Name = "label6";
      this.label6.Size = new Size(72, 16);
      this.label6.TabIndex = 2;
      this.label6.Text = "BootSize:";
      this.label5.Location = new Point(8, 48);
      this.label5.Name = "label5";
      this.label5.Size = new Size(72, 16);
      this.label5.TabIndex = 1;
      this.label5.Text = "PageSize:";
      this.label4.Location = new Point(8, 24);
      this.label4.Name = "label4";
      this.label4.Size = new Size(72, 16);
      this.label4.TabIndex = 0;
      this.label4.Text = "Device:";
      this.PageSize.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.PageSize.ForeColor = SystemColors.ActiveCaption;
      this.PageSize.Location = new Point(80, 48);
      this.PageSize.Name = "PageSize";
      this.PageSize.Size = new Size(64, 16);
      this.PageSize.TabIndex = 6;
      this.PageSize.Text = "xxxx";
      this.BootSize.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.BootSize.ForeColor = SystemColors.ActiveCaption;
      this.BootSize.Location = new Point(80, 72);
      this.BootSize.Name = "BootSize";
      this.BootSize.Size = new Size(64, 16);
      this.BootSize.TabIndex = 7;
      this.BootSize.Text = "xxxx";
      this.FlashSize.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.FlashSize.ForeColor = SystemColors.ActiveCaption;
      this.FlashSize.Location = new Point(80, 96);
      this.FlashSize.Name = "FlashSize";
      this.FlashSize.Size = new Size(64, 16);
      this.FlashSize.TabIndex = 8;
      this.FlashSize.Text = "xxxx";
      this.EEpromSize.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Bold, GraphicsUnit.Point, (byte) 0);
      this.EEpromSize.ForeColor = SystemColors.ActiveCaption;
      this.EEpromSize.Location = new Point(80, 120);
      this.EEpromSize.Name = "EEpromSize";
      this.EEpromSize.Size = new Size(64, 16);
      this.EEpromSize.TabIndex = 9;
      this.EEpromSize.Text = "xxxx";
      this.groupBox6.Controls.Add((Control) this.MessageList);
      this.groupBox6.Location = new Point(352, 168);
      this.groupBox6.Name = "groupBox6";
      this.groupBox6.Size = new Size(192, 384);
      this.groupBox6.TabIndex = 5;
      this.groupBox6.TabStop = false;
      this.groupBox6.Text = "Messages";
      this.MessageList.Location = new Point(8, 16);
      this.MessageList.Name = "MessageList";
      this.MessageList.Size = new Size(176, 355);
      this.MessageList.TabIndex = 0;
      this.groupBox7.Controls.Add((Control) this.Status);
      this.groupBox7.Location = new Point(8, 448);
      this.groupBox7.Name = "groupBox7";
      this.groupBox7.Size = new Size(336, 48);
      this.groupBox7.TabIndex = 6;
      this.groupBox7.TabStop = false;
      this.groupBox7.Text = "Status";
      this.Status.Location = new Point(8, 16);
      this.Status.Name = "Status";
      this.Status.Size = new Size(320, 20);
      this.Status.TabIndex = 0;
      this.groupBox8.Controls.Add((Control) this.ProgressBar);
      this.groupBox8.Location = new Point(8, 504);
      this.groupBox8.Name = "groupBox8";
      this.groupBox8.Size = new Size(336, 48);
      this.groupBox8.TabIndex = 7;
      this.groupBox8.TabStop = false;
      this.groupBox8.Text = "Progress";
      this.ProgressBar.Location = new Point(8, 16);
      this.ProgressBar.Name = "ProgressBar";
      this.ProgressBar.Size = new Size(320, 24);
      this.ProgressBar.TabIndex = 0;
      this.groupBox9.Controls.Add((Control) this.listBox1);
      this.groupBox9.Location = new Point(184, 320);
      this.groupBox9.Name = "groupBox9";
      this.groupBox9.Size = new Size(160, 120);
      this.groupBox9.TabIndex = 8;
      this.groupBox9.TabStop = false;
      this.groupBox9.Text = "Thanks to read";
      this.listBox1.Items.AddRange(new object[55]
      {
        (object) "Hi AVR Fans!",
        (object) "",
        (object) "  If you find this software ",
        (object) "useful, I would be ",
        (object) "pleased to receive some ",
        (object) "Atmel Megas' of your",
        (object) "choice. ",
        (object) "",
        (object) "I have spent a lot of ",
        (object) "time and effort to ",
        (object) "write this software ",
        (object) "and would really",
        (object) "appreciate it is ",
        (object) "you took a a few ",
        (object) "minutes to put a",
        (object) " 'Mega' in the mail ",
        (object) "to me.",
        (object) "",
        (object) "My favourite Atmel ",
        (object) "Mega Micros are",
        (object) "ATMega128, ",
        (object) "AtMega64, AtMega8 ",
        (object) "(TQFP) - Thank you",
        (object) "",
        (object) "If you'd rather not",
        (object) "send parts via mail",
        (object) "would be ",
        (object) "pleased to receive",
        (object) "some $ of your ",
        (object) "choice to my paypal ",
        (object) "account:",
        (object) "sbissonnette@microsyl.com",
        (object) "",
        (object) "The source code for ",
        (object) "the windows ",
        (object) "application is ",
        (object) "available for US$100 ",
        (object) "and is subject to a ",
        (object) "Non Disclosure ",
        (object) "Agreement.",
        (object) "",
        (object) "The code is written",
        (object) " in Microsoft Visual C# ",
        (object) ".NET Studio 2.0. ",
        (object) "",
        (object) "Thanks you for using",
        (object) " Megaload.",
        (object) "",
        (object) "Yours faithfully,",
        (object) "",
        (object) "Sylvain Bissonnette",
        (object) "660 Marco-Polo",
        (object) "Boucherville, Qc",
        (object) "J4B 5R4",
        (object) "CANADA"
      });
      this.listBox1.Location = new Point(8, 16);
      this.listBox1.Name = "listBox1";
      this.listBox1.Size = new Size(144, 95);
      this.listBox1.TabIndex = 0;
      this.groupBox10.Controls.Add((Control) this.AboutButton);
      this.groupBox10.Controls.Add((Control) this.SendReset);
      this.groupBox10.Controls.Add((Control) this.MonitorButton);
      this.groupBox10.Location = new Point(8, 320);
      this.groupBox10.Name = "groupBox10";
      this.groupBox10.Size = new Size(168, 120);
      this.groupBox10.TabIndex = 12;
      this.groupBox10.TabStop = false;
      this.groupBox10.Text = "Command";
      this.AboutButton.Location = new Point(8, 80);
      this.AboutButton.Name = "AboutButton";
      this.AboutButton.Size = new Size(152, 24);
      this.AboutButton.TabIndex = 12;
      this.AboutButton.Text = "About";
      this.AboutButton.Click += new EventHandler(this.AboutBoutton_Click);
      this.Comm1.DataReceived += new SerialDataReceivedEventHandler(this.Comm1_DataReceived);
      this.AutoScaleBaseSize = new Size(5, 13);
      this.ClientSize = new Size(554, 560);
      this.Controls.Add((Control) this.groupBox10);
      this.Controls.Add((Control) this.groupBox9);
      this.Controls.Add((Control) this.groupBox8);
      this.Controls.Add((Control) this.groupBox7);
      this.Controls.Add((Control) this.groupBox6);
      this.Controls.Add((Control) this.groupBox5);
      this.Controls.Add((Control) this.groupBox4);
      this.Controls.Add((Control) this.groupBox3);
      this.Controls.Add((Control) this.groupBox2);
      this.Controls.Add((Control) this.groupBox1);
      this.FormBorderStyle = FormBorderStyle.FixedSingle;
      this.Icon = (Icon) componentResourceManager.GetObject("$this.Icon");
      this.MaximizeBox = false;
      this.Name = "MegaLoadForm";
      this.StartPosition = FormStartPosition.CenterScreen;
      this.Text = "MegaLoad .NET V:6.3";
      this.Load += new EventHandler(this.MegaLoadForm_Load);
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      this.groupBox2.ResumeLayout(false);
      this.groupBox2.PerformLayout();
      this.groupBox3.ResumeLayout(false);
      this.groupBox4.ResumeLayout(false);
      this.groupBox5.ResumeLayout(false);
      this.groupBox6.ResumeLayout(false);
      this.groupBox7.ResumeLayout(false);
      this.groupBox7.PerformLayout();
      this.groupBox8.ResumeLayout(false);
      this.groupBox9.ResumeLayout(false);
      this.groupBox10.ResumeLayout(false);
      this.ResumeLayout(false);
    }

    [STAThread]
    private static void Main()
    {
      Application.Run((Form) new MegaLoadForm());
    }

    private bool CommSetup(int Port, int Speed)
    {
      this.Comm1.Close();
      if (Port == 1)
        this.Comm1.PortName = "COM1";
      if (Port == 2)
        this.Comm1.PortName = "COM2";
      if (Port == 3)
        this.Comm1.PortName = "COM3";
      if (Port == 4)
        this.Comm1.PortName = "COM4";
      if (Port == 5)
        this.Comm1.PortName = "COM5";
      if (Port == 6)
        this.Comm1.PortName = "COM6";
      if (Port == 7)
        this.Comm1.PortName = "COM7";
      if (Port == 8)
        this.Comm1.PortName = "COM8";
      if (Port == 9)
        this.Comm1.PortName = "COM9";
      if (Port == 10)
        this.Comm1.PortName = "COM10";
      if (Port == 11)
        this.Comm1.PortName = "COM11";
      if (Port == 12)
        this.Comm1.PortName = "COM12";
      if (Port == 13)
        this.Comm1.PortName = "COM13";
      if (Port == 14)
        this.Comm1.PortName = "COM14";
      if (Port == 15)
        this.Comm1.PortName = "COM15";
      if (Port == 16)
        this.Comm1.PortName = "COM16";
      if (Port == 17)
        this.Comm1.PortName = "COM17";
      if (Port == 18)
        this.Comm1.PortName = "COM18";
      if (Port == 19)
        this.Comm1.PortName = "COM19";
      if (Port == 20)
        this.Comm1.PortName = "COM20";
      if (Speed == 0)
        this.Comm1.BaudRate = 9600;
      if (Speed == 1)
        this.Comm1.BaudRate = 19200;
      if (Speed == 2)
        this.Comm1.BaudRate = 38400;
      if (Speed == 3)
        this.Comm1.BaudRate = 57600;
      if (Speed == 4)
        this.Comm1.BaudRate = 115200;
      try
      {
        this.Comm1.Open();
        this.Comm1.DtrEnable = this.DTR.Checked;
        this.Comm1.RtsEnable = this.RTS.Checked;
        return true;
      }
      catch
      {
        return false;
      }
    }

    private byte ASCIItoHEX(char ch)
    {
      if ((int) ch >= 48 && (int) ch <= 57)
        return (byte) ((uint) (byte) ch - 48U);
      if ((int) ch >= 65 && (int) ch <= 70)
        return (byte) ((int) (byte) ch - 65 + 10);
      if ((int) ch >= 97 && (int) ch <= 102)
        return (byte) ((int) (byte) ch - 97 + 10);
      else
        return (byte) 0;
    }

    private void SendMessage(string Message)
    {
      int count = this.MessageList.Items.Count;
      if (this.MessageList.InvokeRequired)
        this.Invoke((Delegate) new MegaLoadForm.SetMessageListCallback(this.SendMessage), new object[1]
        {
          (object) Message
        });
      else if (Message.Length == 0)
      {
        this.MessageList.Items.Clear();
      }
      else
      {
        this.MessageList.Items.Insert(count, (object) Message);
        this.MessageList.SelectedIndex = count;
      }
    }

    private void SetStatusText(string Message)
    {
      if (this.Status.InvokeRequired)
        this.Invoke((Delegate) new MegaLoadForm.SetStatusTextCallback(this.SetStatusText), new object[1]
        {
          (object) Message
        });
      else
        this.Status.Text = Message;
    }

    private void SetLabelText(string Message, Label label)
    {
      if (label.InvokeRequired)
        this.Invoke((Delegate) new MegaLoadForm.SetLabelTextCallback(this.SetLabelText), (object) Message, (object) label);
      else
        label.Text = Message;
    }

    private void SetProgressBar(int max, int val)
    {
      if (this.ProgressBar.InvokeRequired)
      {
        this.Invoke((Delegate) new MegaLoadForm.SetProgressBarCallback(this.SetProgressBar), (object) max, (object) val);
      }
      else
      {
        this.ProgressBar.Maximum = max;
        this.ProgressBar.Value = val;
      }
    }

    private bool FillFlash()
    {
      int num1 = 0;
      char[] chArray1 = new char[512];
      StreamReader streamReader;
      try
      {
        streamReader = new StreamReader((Stream) new FileStream(this.FlashFileNameHex, FileMode.Open, FileAccess.Read));
      }
      catch
      {
        return false;
      }
      if (streamReader == StreamReader.Null)
        return false;
      this.SendMessage("Open Flash Hex File");
      int num2 = 0;
      this.FlashMin = (int) ushort.MaxValue;
      this.FlashMax = 0;
      int num3 = 0;
      for (int index = 0; index < 131072; ++index)
        this.Flash[index] = byte.MaxValue;
      while (true)
      {
        byte num4 = (byte) 0;
        int num5 = 0;
        string str = streamReader.ReadLine();
        if (str != null)
        {
          char[] chArray2 = str.ToCharArray();
          char[] chArray3 = chArray2;
          int index1 = num5;
          int num6 = 1;
          int num7 = index1 + num6;
          if ((int) chArray3[index1] == 58)
          {
            MegaLoadForm megaLoadForm1 = this;
            char[] chArray4 = chArray2;
            int index2 = num7;
            int num8 = 1;
            int num9 = index2 + num8;
            int num10 = (int) chArray4[index2];
            int num11 = (int) megaLoadForm1.ASCIItoHEX((char) num10) << 4;
            MegaLoadForm megaLoadForm2 = this;
            char[] chArray5 = chArray2;
            int index3 = num9;
            int num12 = 1;
            int num13 = index3 + num12;
            int num14 = (int) chArray5[index3];
            int num15 = (int) megaLoadForm2.ASCIItoHEX((char) num14);
            byte num16 = (byte) (num11 + num15);
            byte num17 = (byte) ((uint) num4 + (uint) num16);
            MegaLoadForm megaLoadForm3 = this;
            char[] chArray6 = chArray2;
            int index4 = num13;
            int num18 = 1;
            int num19 = index4 + num18;
            int num20 = (int) chArray6[index4];
            int num21 = (int) megaLoadForm3.ASCIItoHEX((char) num20) << 4;
            MegaLoadForm megaLoadForm4 = this;
            char[] chArray7 = chArray2;
            int index5 = num19;
            int num22 = 1;
            int num23 = index5 + num22;
            int num24 = (int) chArray7[index5];
            int num25 = (int) megaLoadForm4.ASCIItoHEX((char) num24);
            byte num26 = (byte) (num21 + num25);
            byte num27 = (byte) ((uint) num17 + (uint) num26);
            MegaLoadForm megaLoadForm5 = this;
            char[] chArray8 = chArray2;
            int index6 = num23;
            int num28 = 1;
            int num29 = index6 + num28;
            int num30 = (int) chArray8[index6];
            int num31 = (int) megaLoadForm5.ASCIItoHEX((char) num30) << 4;
            MegaLoadForm megaLoadForm6 = this;
            char[] chArray9 = chArray2;
            int index7 = num29;
            int num32 = 1;
            int num33 = index7 + num32;
            int num34 = (int) chArray9[index7];
            int num35 = (int) megaLoadForm6.ASCIItoHEX((char) num34);
            byte num36 = (byte) (num31 + num35);
            byte num37 = (byte) ((uint) num27 + (uint) num36);
            MegaLoadForm megaLoadForm7 = this;
            char[] chArray10 = chArray2;
            int index8 = num33;
            int num38 = 1;
            int num39 = index8 + num38;
            int num40 = (int) chArray10[index8];
            int num41 = (int) megaLoadForm7.ASCIItoHEX((char) num40) << 4;
            MegaLoadForm megaLoadForm8 = this;
            char[] chArray11 = chArray2;
            int index9 = num39;
            int num42 = 1;
            int num43 = index9 + num42;
            int num44 = (int) chArray11[index9];
            int num45 = (int) megaLoadForm8.ASCIItoHEX((char) num44);
            byte num46 = (byte) (num41 + num45);
            byte num47 = (byte) ((uint) num37 + (uint) num46);
            int num48 = ((int) num26 << 8) + (int) num36;
            if ((int) num46 == 0)
            {
              if (this.FlashMin > num3 + num48)
                this.FlashMin = num3 + num48;
              for (int index10 = 0; index10 < (int) num16; ++index10)
              {
                byte[] numArray = this.Flash;
                int index11 = num3 + num48 + index10;
                MegaLoadForm megaLoadForm9 = this;
                char[] chArray12 = chArray2;
                int index12 = num43;
                int num49 = 1;
                int num50 = index12 + num49;
                int num51 = (int) chArray12[index12];
                int num52 = (int) megaLoadForm9.ASCIItoHEX((char) num51) << 4;
                MegaLoadForm megaLoadForm10 = this;
                char[] chArray13 = chArray2;
                int index13 = num50;
                int num53 = 1;
                num43 = index13 + num53;
                int num54 = (int) chArray13[index13];
                int num55 = (int) megaLoadForm10.ASCIItoHEX((char) num54);
                int num56 = (int) (byte) (num52 + num55);
                numArray[index11] = (byte) num56;
                num47 += this.Flash[num3 + num48 + index10];
                if (this.FlashMax < num3 + num48 + index10)
                  this.FlashMax = num3 + num48 + index10;
              }
            }
            if ((int) num46 != 1)
            {
              if ((int) num46 == 2)
              {
                MegaLoadForm megaLoadForm9 = this;
                char[] chArray12 = chArray2;
                int index10 = num43;
                int num49 = 1;
                int num50 = index10 + num49;
                int num51 = (int) chArray12[index10];
                int num52 = (int) megaLoadForm9.ASCIItoHEX((char) num51) << 4;
                MegaLoadForm megaLoadForm10 = this;
                char[] chArray13 = chArray2;
                int index11 = num50;
                int num53 = 1;
                int num54 = index11 + num53;
                int num55 = (int) chArray13[index11];
                int num56 = (int) megaLoadForm10.ASCIItoHEX((char) num55);
                byte num57 = (byte) (num52 + num56);
                byte num58 = (byte) ((uint) num47 + (uint) num57);
                MegaLoadForm megaLoadForm11 = this;
                char[] chArray14 = chArray2;
                int index12 = num54;
                int num59 = 1;
                int num60 = index12 + num59;
                int num61 = (int) chArray14[index12];
                int num62 = (int) megaLoadForm11.ASCIItoHEX((char) num61) << 4;
                MegaLoadForm megaLoadForm12 = this;
                char[] chArray15 = chArray2;
                int index13 = num60;
                int num63 = 1;
                num43 = index13 + num63;
                int num64 = (int) chArray15[index13];
                int num65 = (int) megaLoadForm12.ASCIItoHEX((char) num64);
                byte num66 = (byte) (num62 + num65);
                num47 = (byte) ((uint) num58 + (uint) num66);
                num3 = ((int) num57 << 8) + (int) num66 << 4;
              }
              if ((int) num46 == 4)
              {
                MegaLoadForm megaLoadForm9 = this;
                char[] chArray12 = chArray2;
                int index10 = num43;
                int num49 = 1;
                int num50 = index10 + num49;
                int num51 = (int) chArray12[index10];
                int num52 = (int) megaLoadForm9.ASCIItoHEX((char) num51) << 4;
                MegaLoadForm megaLoadForm10 = this;
                char[] chArray13 = chArray2;
                int index11 = num50;
                int num53 = 1;
                int num54 = index11 + num53;
                int num55 = (int) chArray13[index11];
                int num56 = (int) megaLoadForm10.ASCIItoHEX((char) num55);
                byte num57 = (byte) (num52 + num56);
                byte num58 = (byte) ((uint) num47 + (uint) num57);
                MegaLoadForm megaLoadForm11 = this;
                char[] chArray14 = chArray2;
                int index12 = num54;
                int num59 = 1;
                int num60 = index12 + num59;
                int num61 = (int) chArray14[index12];
                int num62 = (int) megaLoadForm11.ASCIItoHEX((char) num61) << 4;
                MegaLoadForm megaLoadForm12 = this;
                char[] chArray15 = chArray2;
                int index13 = num60;
                int num63 = 1;
                num43 = index13 + num63;
                int num64 = (int) chArray15[index13];
                int num65 = (int) megaLoadForm12.ASCIItoHEX((char) num64);
                byte num66 = (byte) (num62 + num65);
                num47 = (byte) ((uint) num58 + (uint) num66);
                num3 = ((int) num57 << 8) + (int) num66 << 16;
              }
              MegaLoadForm megaLoadForm13 = this;
              char[] chArray16 = chArray2;
              int index14 = num43;
              int num67 = 1;
              int num68 = index14 + num67;
              int num69 = (int) chArray16[index14];
              int num70 = (int) megaLoadForm13.ASCIItoHEX((char) num69) << 4;
              MegaLoadForm megaLoadForm14 = this;
              char[] chArray17 = chArray2;
              int index15 = num68;
              int num71 = 1;
              num1 = index15 + num71;
              int num72 = (int) chArray17[index15];
              int num73 = (int) megaLoadForm14.ASCIItoHEX((char) num72);
              if ((int) (byte) (num70 + num73) == (int) (byte) (256U - (uint) num47))
                ++num2;
              else
                goto label_24;
            }
            else
              goto label_18;
          }
          else
            break;
        }
        else
          goto label_27;
      }
      this.SendMessage("Error in Flash Hex File Line " + num2.ToString());
      return false;
label_18:
      this.SendMessage("Flash Hex File OK " + (this.FlashMax - this.FlashMin + 1).ToString() + " Bytes");
      streamReader.Close();
      return true;
label_24:
      this.SendMessage("Error in Flash Hex File " + num2.ToString());
      streamReader.Close();
      return false;
label_27:
      this.SendMessage("Flash Hex File OK " + (this.FlashMax - this.FlashMin + 1).ToString() + " Bytes");
      streamReader.Close();
      return true;
    }

    private void SendFlashPage()
    {
      int num = 0;
      if (this.PagePtr * this.PageSizeInt > this.FlashMax)
      {
        this.CommWriteByte(byte.MaxValue);
        this.CommWriteByte(byte.MaxValue);
        this.SendMessage("Flash Prog Done!");
        if (this.Retry == 0)
          this.SetStatusText("Succesful finished, Waiting for next target");
        else
          this.SetStatusText("Succesful finished with retrys, Waiting for next target");
        this.SetProgressBar(1, 0);
        this.PagePtr = 0;
      }
      else
      {
        this.CommWriteByte((byte) (this.PagePtr >> 8 & (int) byte.MaxValue));
        this.CommWriteByte((byte) (this.PagePtr & (int) byte.MaxValue));
        byte val = (byte) 0;
        this.Comm1.Write(this.Flash, this.PagePtr * this.PageSizeInt, this.PageSizeInt);
        for (; num < this.PageSizeInt; ++num)
          val += this.Flash[this.PagePtr * this.PageSizeInt + num];
        this.CommWriteByte(val);
        this.SendMessage("Sending Page #" + this.PagePtr.ToString());
        if (this.PageSizeInt > 0)
          this.SetProgressBar(this.FlashMax / this.PageSizeInt, this.PagePtr);
      }
    }

    private bool FillEEprom()
    {
      int num1 = 0;
      char[] chArray1 = new char[512];
      StreamReader streamReader;
      try
      {
        streamReader = new StreamReader((Stream) new FileStream(this.EEpromFileNameHex, FileMode.Open, FileAccess.Read));
      }
      catch
      {
        return false;
      }
      if (streamReader == StreamReader.Null)
        return false;
      this.SendMessage("Open EEprom Hex File");
      int num2 = 0;
      this.EEpromMin = (int) ushort.MaxValue;
      this.EEpromMax = 0;
      int num3 = 0;
      for (int index = 0; index < 131072; ++index)
      {
        this.EEprom[index] = byte.MaxValue;
        this.EEpromUse[index] = false;
      }
      while (true)
      {
        byte num4 = (byte) 0;
        int num5 = 0;
        string str = streamReader.ReadLine();
        if (str != null)
        {
          char[] chArray2 = str.ToCharArray();
          char[] chArray3 = chArray2;
          int index1 = num5;
          int num6 = 1;
          int num7 = index1 + num6;
          if ((int) chArray3[index1] == 58)
          {
            MegaLoadForm megaLoadForm1 = this;
            char[] chArray4 = chArray2;
            int index2 = num7;
            int num8 = 1;
            int num9 = index2 + num8;
            int num10 = (int) chArray4[index2];
            int num11 = (int) megaLoadForm1.ASCIItoHEX((char) num10) << 4;
            MegaLoadForm megaLoadForm2 = this;
            char[] chArray5 = chArray2;
            int index3 = num9;
            int num12 = 1;
            int num13 = index3 + num12;
            int num14 = (int) chArray5[index3];
            int num15 = (int) megaLoadForm2.ASCIItoHEX((char) num14);
            byte num16 = (byte) (num11 + num15);
            byte num17 = (byte) ((uint) num4 + (uint) num16);
            MegaLoadForm megaLoadForm3 = this;
            char[] chArray6 = chArray2;
            int index4 = num13;
            int num18 = 1;
            int num19 = index4 + num18;
            int num20 = (int) chArray6[index4];
            int num21 = (int) megaLoadForm3.ASCIItoHEX((char) num20) << 4;
            MegaLoadForm megaLoadForm4 = this;
            char[] chArray7 = chArray2;
            int index5 = num19;
            int num22 = 1;
            int num23 = index5 + num22;
            int num24 = (int) chArray7[index5];
            int num25 = (int) megaLoadForm4.ASCIItoHEX((char) num24);
            byte num26 = (byte) (num21 + num25);
            byte num27 = (byte) ((uint) num17 + (uint) num26);
            MegaLoadForm megaLoadForm5 = this;
            char[] chArray8 = chArray2;
            int index6 = num23;
            int num28 = 1;
            int num29 = index6 + num28;
            int num30 = (int) chArray8[index6];
            int num31 = (int) megaLoadForm5.ASCIItoHEX((char) num30) << 4;
            MegaLoadForm megaLoadForm6 = this;
            char[] chArray9 = chArray2;
            int index7 = num29;
            int num32 = 1;
            int num33 = index7 + num32;
            int num34 = (int) chArray9[index7];
            int num35 = (int) megaLoadForm6.ASCIItoHEX((char) num34);
            byte num36 = (byte) (num31 + num35);
            byte num37 = (byte) ((uint) num27 + (uint) num36);
            MegaLoadForm megaLoadForm7 = this;
            char[] chArray10 = chArray2;
            int index8 = num33;
            int num38 = 1;
            int num39 = index8 + num38;
            int num40 = (int) chArray10[index8];
            int num41 = (int) megaLoadForm7.ASCIItoHEX((char) num40) << 4;
            MegaLoadForm megaLoadForm8 = this;
            char[] chArray11 = chArray2;
            int index9 = num39;
            int num42 = 1;
            int num43 = index9 + num42;
            int num44 = (int) chArray11[index9];
            int num45 = (int) megaLoadForm8.ASCIItoHEX((char) num44);
            byte num46 = (byte) (num41 + num45);
            byte num47 = (byte) ((uint) num37 + (uint) num46);
            int num48 = ((int) num26 << 8) + (int) num36;
            if (num48 <= 4096)
            {
              if ((int) num46 == 0)
              {
                if (this.EEpromMin > num3 + num48)
                  this.EEpromMin = num3 + num48;
                for (int index10 = 0; index10 < (int) num16; ++index10)
                {
                  byte[] numArray = this.EEprom;
                  int index11 = num3 + num48 + index10;
                  MegaLoadForm megaLoadForm9 = this;
                  char[] chArray12 = chArray2;
                  int index12 = num43;
                  int num49 = 1;
                  int num50 = index12 + num49;
                  int num51 = (int) chArray12[index12];
                  int num52 = (int) megaLoadForm9.ASCIItoHEX((char) num51) << 4;
                  MegaLoadForm megaLoadForm10 = this;
                  char[] chArray13 = chArray2;
                  int index13 = num50;
                  int num53 = 1;
                  num43 = index13 + num53;
                  int num54 = (int) chArray13[index13];
                  int num55 = (int) megaLoadForm10.ASCIItoHEX((char) num54);
                  int num56 = (int) (byte) (num52 + num55);
                  numArray[index11] = (byte) num56;
                  this.EEpromUse[num3 + num48 + index10] = true;
                  num47 += this.EEprom[num3 + num48 + index10];
                  if (this.EEpromMax < num3 + num48 + index10)
                    this.EEpromMax = num3 + num48 + index10;
                }
              }
              if ((int) num46 != 1)
              {
                if ((int) num46 == 2)
                {
                  MegaLoadForm megaLoadForm9 = this;
                  char[] chArray12 = chArray2;
                  int index10 = num43;
                  int num49 = 1;
                  int num50 = index10 + num49;
                  int num51 = (int) chArray12[index10];
                  int num52 = (int) megaLoadForm9.ASCIItoHEX((char) num51) << 4;
                  MegaLoadForm megaLoadForm10 = this;
                  char[] chArray13 = chArray2;
                  int index11 = num50;
                  int num53 = 1;
                  int num54 = index11 + num53;
                  int num55 = (int) chArray13[index11];
                  int num56 = (int) megaLoadForm10.ASCIItoHEX((char) num55);
                  byte num57 = (byte) (num52 + num56);
                  byte num58 = (byte) ((uint) num47 + (uint) num57);
                  MegaLoadForm megaLoadForm11 = this;
                  char[] chArray14 = chArray2;
                  int index12 = num54;
                  int num59 = 1;
                  int num60 = index12 + num59;
                  int num61 = (int) chArray14[index12];
                  int num62 = (int) megaLoadForm11.ASCIItoHEX((char) num61) << 4;
                  MegaLoadForm megaLoadForm12 = this;
                  char[] chArray15 = chArray2;
                  int index13 = num60;
                  int num63 = 1;
                  num43 = index13 + num63;
                  int num64 = (int) chArray15[index13];
                  int num65 = (int) megaLoadForm12.ASCIItoHEX((char) num64);
                  byte num66 = (byte) (num62 + num65);
                  num47 = (byte) ((uint) num58 + (uint) num66);
                  num3 = ((int) num57 << 8) + (int) num66 << 4;
                }
                if ((int) num46 == 4)
                {
                  MegaLoadForm megaLoadForm9 = this;
                  char[] chArray12 = chArray2;
                  int index10 = num43;
                  int num49 = 1;
                  int num50 = index10 + num49;
                  int num51 = (int) chArray12[index10];
                  int num52 = (int) megaLoadForm9.ASCIItoHEX((char) num51) << 4;
                  MegaLoadForm megaLoadForm10 = this;
                  char[] chArray13 = chArray2;
                  int index11 = num50;
                  int num53 = 1;
                  int num54 = index11 + num53;
                  int num55 = (int) chArray13[index11];
                  int num56 = (int) megaLoadForm10.ASCIItoHEX((char) num55);
                  byte num57 = (byte) (num52 + num56);
                  byte num58 = (byte) ((uint) num47 + (uint) num57);
                  MegaLoadForm megaLoadForm11 = this;
                  char[] chArray14 = chArray2;
                  int index12 = num54;
                  int num59 = 1;
                  int num60 = index12 + num59;
                  int num61 = (int) chArray14[index12];
                  int num62 = (int) megaLoadForm11.ASCIItoHEX((char) num61) << 4;
                  MegaLoadForm megaLoadForm12 = this;
                  char[] chArray15 = chArray2;
                  int index13 = num60;
                  int num63 = 1;
                  num43 = index13 + num63;
                  int num64 = (int) chArray15[index13];
                  int num65 = (int) megaLoadForm12.ASCIItoHEX((char) num64);
                  byte num66 = (byte) (num62 + num65);
                  num47 = (byte) ((uint) num58 + (uint) num66);
                  num3 = ((int) num57 << 8) + (int) num66 << 16;
                }
                MegaLoadForm megaLoadForm13 = this;
                char[] chArray16 = chArray2;
                int index14 = num43;
                int num67 = 1;
                int num68 = index14 + num67;
                int num69 = (int) chArray16[index14];
                int num70 = (int) megaLoadForm13.ASCIItoHEX((char) num69) << 4;
                MegaLoadForm megaLoadForm14 = this;
                char[] chArray17 = chArray2;
                int index15 = num68;
                int num71 = 1;
                num1 = index15 + num71;
                int num72 = (int) chArray17[index15];
                int num73 = (int) megaLoadForm14.ASCIItoHEX((char) num72);
                if ((int) (byte) (num70 + num73) == (int) (byte) (256U - (uint) num47))
                  ++num2;
                else
                  goto label_26;
              }
              else
                goto label_20;
            }
            else
              goto label_10;
          }
          else
            break;
        }
        else
          goto label_29;
      }
      this.SendMessage("Error in EEprom Hex File Line " + num2.ToString());
      return false;
label_10:
      this.SendMessage("EEprom Hex File Out Of Range");
      streamReader.Close();
      return false;
label_20:
      this.SendMessage("EEprom Hex File OK " + (this.EEpromMax - this.EEpromMin + 1).ToString() + " Bytes");
      streamReader.Close();
      return true;
label_26:
      this.SendMessage("Error in EEprom Hex File " + num2.ToString());
      streamReader.Close();
      return false;
label_29:
      this.SendMessage("EEprom Hex File OK " + (this.EEpromMax - this.EEpromMin + 1).ToString() + " Bytes");
      streamReader.Close();
      return true;
    }

    private void SendEEpromByte()
    {
      if (this.BytePtr > this.EEpromMax)
      {
        this.CommWriteByte(byte.MaxValue);
        this.CommWriteByte(byte.MaxValue);
        this.SendMessage("EEprom Prog Done!");
        if (this.Retry == 0)
          this.SetStatusText("Succesful finished, Waiting for next target");
        else
          this.SetStatusText("Succesful finished with retrys, Waiting for next target");
        this.ProgressBar.Value = 0;
        this.BytePtr = 0;
      }
      else
      {
        while (!this.EEpromUse[this.BytePtr] && this.BytePtr < 4094)
          ++this.BytePtr;
        this.CommWriteByte((byte) (this.BytePtr >> 8 & (int) byte.MaxValue));
        this.CommWriteByte((byte) (this.BytePtr & (int) byte.MaxValue));
        this.CommWriteByte(this.EEprom[this.BytePtr]);
        this.CommWriteByte((byte) ((uint) (byte) ((uint) (byte) (this.BytePtr >> 8 & (int) byte.MaxValue) + (uint) (byte) (this.BytePtr & (int) byte.MaxValue)) + (uint) this.EEprom[this.BytePtr]));
        this.SendMessage("Sending Byte #" + this.BytePtr.ToString());
        if (this.EEpromSizeInt > 0)
          this.SetProgressBar(this.EEpromMax, this.BytePtr);
      }
    }

    private void SendLockBit()
    {
      byte val = (byte) 0;
      if (this.BLB01.Checked)
        val += (byte) 4;
      if (this.BLB02.Checked)
        val += (byte) 8;
      if (this.BLB11.Checked)
        val += (byte) 16;
      if (this.BLB12.Checked)
        val += (byte) 32;
      this.CommWriteByte(val);
      this.CommWriteByte(~val);
      this.SendMessage("Sending LockBits");
    }

    private void FlashOpen_Click(object sender, EventArgs e)
    {
      this.OpenFileDialog.Filter = "HEX File (*.hex)|*.hex|All files (*.*)|*.*";
      this.OpenFileDialog.ReadOnlyChecked = true;
      int num = (int) this.OpenFileDialog.ShowDialog((IWin32Window) this);
      string str = ((object) this.OpenFileDialog.FileName).ToString();
      if (str == null)
        return;
      this.FlashFileNameHex = str;
      this.FlashFileName.Text = str;
      this.SendMessage("Flash File Selected");
      this.SetStatusText("Ready, Waiting for target");
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("FlashFileName", (object) str);
    }

    private void EEpromOpen_Click(object sender, EventArgs e)
    {
      this.OpenFileDialog.Filter = "HEX File (*.hex)|*.hex|All files (*.*)|*.*";
      this.OpenFileDialog.ReadOnlyChecked = true;
      int num = (int) this.OpenFileDialog.ShowDialog((IWin32Window) this);
      string str = ((object) this.OpenFileDialog.FileName).ToString();
      if (str == null)
        return;
      this.EEpromFileNameHex = str;
      this.EEpromFileName.Text = str;
      this.SendMessage("EEprom File Selected");
      this.SetStatusText("Ready, Waiting for target");
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("EEpromFileName", (object) str);
    }

    private void PortSelect_SelectionChangeCommitted(object sender, EventArgs e)
    {
      if (this.PortSelect.SelectedIndex == 0)
        return;
      if (this.CommSetup(this.PortSelect.SelectedIndex, this.CommSpeed.SelectedIndex))
      {
        (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("CommPort", (object) this.PortSelect.SelectedIndex);
      }
      else
      {
        this.PortSelect.SelectedIndex = 0;
        int num = (int) MessageBox.Show("Invalid Port Selection", "MegaLoad .NET", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
      }
    }

    private void CommSpeed_SelectionChangeCommitted(object sender, EventArgs e)
    {
      if (this.CommSetup(this.PortSelect.SelectedIndex, this.CommSpeed.SelectedIndex))
        (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("CommSpeed", (object) this.CommSpeed.SelectedIndex);
      else
        this.PortSelect.SelectedIndex = 0;
    }

    private void MegaLoadForm_Load(object sender, EventArgs e)
    {
      this.CommSpeed.SelectedIndex = 0;
      this.Flash = new byte[524288];
      this.EEprom = new byte[131072];
      this.EEpromUse = new bool[131072];
      RegistryKey registryKey = Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET");
      if (registryKey == null)
      {
        this.FlashFileName.Text = "";
        this.EEpromFileName.Text = "";
        this.PortSelect.SelectedIndex = 0;
        this.CommSpeed.SelectedIndex = 0;
      }
      try
      {
        if (registryKey.GetValue("FlashFileName") != null)
        {
          this.FlashFileNameHex = registryKey.GetValue("FlashFileName").ToString();
          this.FlashFileName.Text = this.FlashFileNameHex;
        }
        else
          this.FlashFileName.Text = "";
      }
      catch
      {
        this.FlashFileName.Text = "";
      }
      try
      {
        if (registryKey.GetValue("EEpromFileName") != null)
        {
          this.EEpromFileNameHex = registryKey.GetValue("EEpromFileName").ToString();
          this.EEpromFileName.Text = this.EEpromFileNameHex;
        }
        else
          this.EEpromFileName.Text = "";
      }
      catch
      {
        this.EEpromFileName.Text = "";
      }
      try
      {
        if (registryKey.GetValue("CommPort") != null)
          this.PortSelect.SelectedIndex = (int) Convert.ToInt16(registryKey.GetValue("CommPort").ToString());
        else
          this.PortSelect.SelectedIndex = 0;
      }
      catch
      {
        this.PortSelect.SelectedIndex = 0;
      }
      try
      {
        if (registryKey.GetValue("CommSpeed") != null)
          this.CommSpeed.SelectedIndex = (int) Convert.ToInt16(registryKey.GetValue("CommSpeed").ToString());
        else
          this.CommSpeed.SelectedIndex = 0;
      }
      catch
      {
        this.CommSpeed.SelectedIndex = 0;
      }
      try
      {
        if (registryKey.GetValue("CommDTR") != null)
        {
          this.DTR.Checked = Convert.ToBoolean(registryKey.GetValue("CommDTR").ToString());
          if (this.Comm1.IsOpen)
            this.Comm1.DtrEnable = this.DTR.Checked;
        }
      }
      catch
      {
      }
      try
      {
        if (registryKey.GetValue("CommRTS") != null)
        {
          this.RTS.Checked = Convert.ToBoolean(registryKey.GetValue("CommRTS").ToString());
          if (this.Comm1.IsOpen)
            this.Comm1.RtsEnable = this.RTS.Checked;
        }
      }
      catch
      {
      }
      try
      {
        if (registryKey.GetValue("BLB12") != null)
          this.BLB12.Checked = Convert.ToBoolean(registryKey.GetValue("BLB12").ToString());
      }
      catch
      {
      }
      try
      {
        if (registryKey.GetValue("BLB11") != null)
          this.BLB11.Checked = Convert.ToBoolean(registryKey.GetValue("BLB11").ToString());
      }
      catch
      {
      }
      try
      {
        if (registryKey.GetValue("BLB02") != null)
          this.BLB02.Checked = Convert.ToBoolean(registryKey.GetValue("BLB02").ToString());
      }
      catch
      {
      }
      try
      {
        if (registryKey.GetValue("BLB01") != null)
          this.BLB01.Checked = Convert.ToBoolean(registryKey.GetValue("BLB01").ToString());
      }
      catch
      {
      }
      try
      {
        if (!this.CommSetup(this.PortSelect.SelectedIndex, this.CommSpeed.SelectedIndex))
        {
          this.PortSelect.SelectedIndex = 0;
          this.CommSpeed.SelectedIndex = 0;
        }
      }
      catch
      {
      }
      this.SetStatusText("Ready, Waiting for target");
      if (!this.FillFlash())
      {
        this.SetStatusText("No Flash File...  Open file first!");
        this.FlashFileName.Text = "";
      }
      this.ToolTip.SetToolTip((Control) this.AboutButton, "Some nice thing to read!");
      this.ToolTip.SetToolTip((Control) this.PortOpenClose, "Close & Open PC Comm Port. (this allows other software to use the Comm Port");
      this.ToolTip.SetToolTip((Control) this.MonitorButton, "Open terminal screen (useful for debugging)");
      this.ToolTip.SetToolTip((Control) this.SendReset, "Send a reset string on the comport, could be use to reset your MCU, string is RESET");
      this.ToolTip.SetToolTip((Control) this.DTR, "Toggle the state of DTR (could be used to reset target or provide power for optical RS485 Converter)");
      this.ToolTip.SetToolTip((Control) this.RTS, "Toggle the state of RTS (could be used to reset target or provide power for optical RS485 Converter)");
      this.ToolTip.SetToolTip((Control) this.Status, "Status of the application");
      this.ToolTip.SetToolTip((Control) this.PortSelect, "Select Comm Port to connect to target");
      this.ToolTip.SetToolTip((Control) this.ProgressBar, "Display progress indicator of Flash/EEPROM programming");
      this.ToolTip.SetToolTip((Control) this.MessageList, "Application Status Information");
      this.ToolTip.SetToolTip((Control) this.EEpromSize, "Displays Target's EEPROM size (in bytes)");
      this.ToolTip.SetToolTip((Control) this.FlashSize, "Displays Target's FLASH size (in bytes)");
      this.ToolTip.SetToolTip((Control) this.BootSize, "Displays Bootloader size (in words)");
      this.ToolTip.SetToolTip((Control) this.PageSize, "Display Target's Page Size (in bytes)");
      this.ToolTip.SetToolTip((Control) this.Device, "Display Target 's Device Type");
      this.ToolTip.SetToolTip((Control) this.CommSpeed, "Select Communication speed");
      this.ToolTip.SetToolTip((Control) this.BLB01, "BLB01->Lock bit setting");
      this.ToolTip.SetToolTip((Control) this.BLB02, "BLB02->Lock bit setting");
      this.ToolTip.SetToolTip((Control) this.BLB11, "BLB11->Lock bit setting");
      this.ToolTip.SetToolTip((Control) this.BLB12, "BLB12->Lock bit setting");
      this.ToolTip.SetToolTip((Control) this.EEpromOpen, "Open EEPROM data file to be programmed (in HEX format)");
      this.ToolTip.SetToolTip((Control) this.FlashOpen, "Open Flash data file to be programmed (in HEX format)");
      this.ToolTip.SetToolTip((Control) this.EEpromFileName, "Filename of  EEPROM data file to be programmed on bootload (in HEX format)");
      this.ToolTip.SetToolTip((Control) this.FlashFileName, "Filename of Flash data file to be programmed on bootload (in HEX format)");
    }

    private void DTR_CheckedChanged(object sender, EventArgs e)
    {
      if (this.Comm1.IsOpen)
        this.Comm1.DtrEnable = this.DTR.Checked;
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("CommDTR", (object) (bool) (this.DTR.Checked ? 1 : 0));
    }

    private void RTS_CheckedChanged(object sender, EventArgs e)
    {
      if (this.Comm1.IsOpen)
        this.Comm1.RtsEnable = this.RTS.Checked;
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("CommRTS", (object) (bool) (this.RTS.Checked ? 1 : 0));
    }

    private void BLB12_CheckedChanged(object sender, EventArgs e)
    {
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("BLB12", (object) (bool) (this.BLB12.Checked ? 1 : 0));
    }

    private void BLB11_CheckedChanged(object sender, EventArgs e)
    {
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("BLB11", (object) (bool) (this.BLB11.Checked ? 1 : 0));
    }

    private void BLB02_CheckedChanged(object sender, EventArgs e)
    {
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("BLB02", (object) (bool) (this.BLB02.Checked ? 1 : 0));
    }

    private void BLB01_CheckedChanged(object sender, EventArgs e)
    {
      (Registry.CurrentUser.OpenSubKey("Software\\MicroSyl\\MegaLoad .NET", true) ?? Registry.CurrentUser.CreateSubKey("Software\\MicroSyl\\MegaLoad .NET")).SetValue("BLB01", (object) (bool) (this.BLB01.Checked ? 1 : 0));
    }

    private void SendReset_Click(object sender, EventArgs e)
    {
      if (this.Comm1.IsOpen)
      {
        this.Comm1.Write("RESET");
        this.SendMessage("Reset has been send");
      }
      else
        this.SendMessage("Your comm is close!");
    }

    private byte[] ByteToByteArray(byte val)
    {
      return new byte[1]
      {
        val
      };
    }

    private void CommWriteByte(byte val)
    {
      this.Comm1.Write(this.ByteToByteArray(val), 0, 1);
    }

    private void CommWriteChar(char val)
    {
      this.Comm1.Write(this.ByteToByteArray((byte) val), 0, 1);
    }

    private void Comm1_DataReceived(object sender, SerialDataReceivedEventArgs e)
    {
      while (this.Comm1.BytesToRead > 0)
      {
        char ch = (char) this.Comm1.ReadByte();
        if (this.Monitor != null)
          this.Monitor.AddChar(ch);
        if ((int) ch == 85)
          this.CommWriteByte((byte) 85);
        if ((int) ch == 62)
        {
          this.CommWriteChar('<');
          this.MemType = 'F';
          this.SendMessage("");
          this.Retry = 0;
          if (this.FillFlash())
          {
            this.PagePtr = 0;
            this.Retry = 0;
            this.SetStatusText("Programming flash.... please wait");
          }
          else
          {
            this.SetStatusText("No Flash File... Open file first!");
            this.MemType = 'E';
            this.CommWriteByte(byte.MaxValue);
            this.CommWriteByte(byte.MaxValue);
            this.Comm1.ReadChar();
          }
        }
        if ((int) ch == 33 && (int) this.MemType == 70)
        {
          this.SendFlashPage();
          ++this.PagePtr;
        }
        if ((int) ch == 64 && (int) this.MemType == 70)
        {
          --this.PagePtr;
          ++this.Retry;
          this.SendFlashPage();
          ++this.PagePtr;
        }
        if ((int) ch == 41)
        {
          this.MemType = 'E';
          this.Retry = 0;
          if (this.FillEEprom())
          {
            this.BytePtr = 0;
            this.Retry = 0;
            this.SetStatusText("Programming eeprom.... please wait");
          }
          else
          {
            this.SetStatusText("No eeprom File... Open file first!");
            this.MemType = 'F';
            this.CommWriteByte(byte.MaxValue);
            this.CommWriteByte(byte.MaxValue);
            this.Comm1.ReadChar();
          }
        }
        if ((int) ch == 33 && (int) this.MemType == 69)
        {
          this.SendEEpromByte();
          ++this.BytePtr;
        }
        if ((int) ch == 64 && (int) this.MemType == 69)
        {
          --this.BytePtr;
          ++this.Retry;
          this.SendEEpromByte();
          ++this.BytePtr;
        }
        if ((int) ch == 37)
          this.SendLockBit();
        if (this.Retry > 3)
        {
          this.SendMessage("Programming Fail");
          this.SetStatusText("Error occured aborted...");
          while (this.Comm1.BytesToRead > 0)
          {
            this.CommWriteByte(byte.MaxValue);
            this.CommWriteByte(byte.MaxValue);
            this.Comm1.ReadChar();
          }
        }
        if ((int) ch == 65)
          this.SetLabelText("Mega 8", this.Device);
        if ((int) ch == 66)
          this.SetLabelText("Mega 16", this.Device);
        if ((int) ch == 67)
          this.SetLabelText("Mega 64", this.Device);
        if ((int) ch == 68)
          this.SetLabelText("Mega 128", this.Device);
        if ((int) ch == 69)
          this.SetLabelText("Mega 32", this.Device);
        if ((int) ch == 70)
          this.SetLabelText("Mega 162", this.Device);
        if ((int) ch == 71)
          this.SetLabelText("Mega 169", this.Device);
        if ((int) ch == 72)
          this.SetLabelText("Mega8515", this.Device);
        if ((int) ch == 73)
          this.SetLabelText("Mega8535", this.Device);
        if ((int) ch == 74)
          this.SetLabelText("Mega163", this.Device);
        if ((int) ch == 75)
          this.SetLabelText("Mega323", this.Device);
        if ((int) ch == 76)
          this.SetLabelText("Mega48", this.Device);
        if ((int) ch == 77)
          this.SetLabelText("Mega88", this.Device);
        if ((int) ch == 78)
          this.SetLabelText("Mega168", this.Device);
        if ((int) ch == 79)
          this.SetLabelText("tiny2313", this.Device);
        if ((int) ch == 80)
          this.SetLabelText("tiny13", this.Device);
        if ((int) ch == 128)
          this.SetLabelText("Mega165", this.Device);
        if ((int) ch == 129)
          this.SetLabelText("Mega325x", this.Device);
        if ((int) ch == 130)
          this.SetLabelText("Mega645x", this.Device);
        if ((int) ch == 131)
          this.SetLabelText("Mega329x", this.Device);
        if ((int) ch == 132)
          this.SetLabelText("Mega649x", this.Device);
        if ((int) ch == 133)
          this.SetLabelText("Mega406", this.Device);
        if ((int) ch == 134)
          this.SetLabelText("Mega640", this.Device);
        if ((int) ch == 135)
          this.SetLabelText("Mega128x", this.Device);
        if ((int) ch == 136)
          this.SetLabelText("Mega256x", this.Device);
        if ((int) ch == 137)
          this.SetLabelText("MCAN128", this.Device);
        if ((int) ch == 81)
        {
          this.SetLabelText("32 Bytes", this.PageSize);
          this.PageSizeInt = 32;
        }
        if ((int) ch == 82)
        {
          this.SetLabelText("64 Bytes", this.PageSize);
          this.PageSizeInt = 64;
        }
        if ((int) ch == 83)
        {
          this.SetLabelText("128 Bytes", this.PageSize);
          this.PageSizeInt = 128;
        }
        if ((int) ch == 84)
        {
          this.SetLabelText("256 Bytes", this.PageSize);
          this.PageSizeInt = 256;
        }
        if ((int) ch == 86)
        {
          this.SetLabelText("512 Bytes", this.PageSize);
          this.PageSizeInt = 512;
        }
        if ((int) ch == 97)
        {
          this.SetLabelText("128 Words", this.BootSize);
          this.BootSizeInt = 128;
        }
        if ((int) ch == 98)
        {
          this.SetLabelText("256 Words", this.BootSize);
          this.BootSizeInt = 256;
        }
        if ((int) ch == 99)
        {
          this.SetLabelText("512 Words", this.BootSize);
          this.BootSizeInt = 512;
        }
        if ((int) ch == 100)
        {
          this.SetLabelText("1k Words", this.BootSize);
          this.BootSizeInt = 1024;
        }
        if ((int) ch == 101)
        {
          this.SetLabelText("2k Words", this.BootSize);
          this.BootSizeInt = 2048;
        }
        if ((int) ch == 102)
        {
          this.SetLabelText("4k Words", this.BootSize);
          this.BootSizeInt = 4096;
        }
        if ((int) ch == 103)
        {
          this.SetLabelText("1k Bytes", this.FlashSize);
          this.FlashSizeInt = 1024;
        }
        if ((int) ch == 104)
        {
          this.SetLabelText("2k Bytes", this.FlashSize);
          this.FlashSizeInt = 2048;
        }
        if ((int) ch == 105)
        {
          this.SetLabelText("4k Bytes", this.FlashSize);
          this.FlashSizeInt = 4096;
        }
        if ((int) ch == 108)
        {
          this.SetLabelText("8k Bytes", this.FlashSize);
          this.FlashSizeInt = 8192;
        }
        if ((int) ch == 109)
        {
          this.SetLabelText("16k Bytes", this.FlashSize);
          this.FlashSizeInt = 16384;
        }
        if ((int) ch == 110)
        {
          this.SetLabelText("32k Bytes", this.FlashSize);
          this.FlashSizeInt = 32768;
        }
        if ((int) ch == 111)
        {
          this.SetLabelText("64k Bytes", this.FlashSize);
          this.FlashSizeInt = 65536;
        }
        if ((int) ch == 112)
        {
          this.SetLabelText("128k Bytes", this.FlashSize);
          this.FlashSizeInt = 131072;
        }
        if ((int) ch == 113)
        {
          this.SetLabelText("256k Bytes", this.FlashSize);
          this.FlashSizeInt = 262144;
        }
        if ((int) ch == 114)
        {
          this.SetLabelText("40k Bytes", this.FlashSize);
          this.FlashSizeInt = 40960;
        }
        if ((int) ch == 46)
        {
          this.SetLabelText("64 Bytes", this.EEpromSize);
          this.EEpromSizeInt = 512;
        }
        if ((int) ch == 47)
        {
          this.SetLabelText("128 Bytes", this.EEpromSize);
          this.EEpromSizeInt = 512;
        }
        if ((int) ch == 48)
        {
          this.SetLabelText("256 Bytes", this.EEpromSize);
          this.EEpromSizeInt = 512;
        }
        if ((int) ch == 49)
        {
          this.SetLabelText("512 Bytes", this.EEpromSize);
          this.EEpromSizeInt = 512;
        }
        if ((int) ch == 50)
        {
          this.SetLabelText("1k Bytes", this.EEpromSize);
          this.EEpromSizeInt = 1024;
        }
        if ((int) ch == 51)
        {
          this.SetLabelText("2k Bytes", this.EEpromSize);
          this.EEpromSizeInt = 2048;
        }
        if ((int) ch == 52)
        {
          this.SetLabelText("4k Bytes", this.EEpromSize);
          this.EEpromSizeInt = 4096;
        }
      }
    }

    private void Monitor_Click(object sender, EventArgs e)
    {
      this.Monitor = new MonitorForm(this.Comm1);
      this.Monitor.Visible = true;
    }

    private void PortOpenClose_Click(object sender, EventArgs e)
    {
      if (this.PortOpenClose.Text == "Close Port")
      {
        this.Comm1.Close();
        this.PortOpenClose.Text = "Open Port";
      }
      else
      {
        try
        {
          this.Comm1.Open();
          this.PortOpenClose.Text = "Close Port";
        }
        catch
        {
        }
      }
    }

    private void AboutBoutton_Click(object sender, EventArgs e)
    {
      this.About = new AboutForm(this, this.Comm1, this.Reg);
      this.About.Visible = true;
    }

    private delegate void SetMessageListCallback(string Message);

    private delegate void SetStatusTextCallback(string Message);

    private delegate void SetLabelTextCallback(string Message, Label label);

    private delegate void SetProgressBarCallback(int max, int val);
  }
}
