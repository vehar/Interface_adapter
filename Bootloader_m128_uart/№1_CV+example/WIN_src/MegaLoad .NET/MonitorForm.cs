// Type: MegaLoad_.NET.MonitorForm
// Assembly: MegaLoad .NET, Version=1.0.2628.34316, Culture=neutral, PublicKeyToken=null
// MVID: C0A102E7-E817-4560-BBFC-BB5054A5A3E4
// Assembly location: C:\Program Files\MicroSyl\MegaLoad\MegaLoad .NET.exe

using System;
using System.ComponentModel;
using System.Drawing;
using System.IO.Ports;
using System.Resources;
using System.Windows.Forms;

namespace MegaLoad_.NET
{
  public class MonitorForm : Form
  {
    private SerialPort Comm = (SerialPort) null;
    private Container components = (Container) null;
    private TextBox RxChar;
    private GroupBox groupBox1;
    private GroupBox groupBox2;
    private Button ClearInput;
    private TextBox TxChar;
    private Button ClearOutput;

    public MonitorForm(SerialPort CommArg)
    {
      this.InitializeComponent();
      this.Comm = CommArg;
    }

    private void SetText(char ch)
    {
      if (this.RxChar.InvokeRequired)
      {
        this.Invoke((Delegate) new MonitorForm.SetTextCallback(this.SetText), new object[1]
        {
          (object) ch
        });
      }
      else
      {
        TextBox textBox = this.RxChar;
        string str = textBox.Text + ch.ToString();
        textBox.Text = str;
      }
    }

    public void AddChar(char ch)
    {
      if ((int) ch < 32 || (int) ch > 126)
        return;
      this.SetText(ch);
    }

    protected override void Dispose(bool disposing)
    {
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    private void InitializeComponent()
    {
      ResourceManager resourceManager = new ResourceManager(typeof (MonitorForm));
      this.RxChar = new TextBox();
      this.groupBox1 = new GroupBox();
      this.groupBox2 = new GroupBox();
      this.ClearInput = new Button();
      this.TxChar = new TextBox();
      this.ClearOutput = new Button();
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.SuspendLayout();
      this.RxChar.Location = new Point(8, 16);
      this.RxChar.Multiline = true;
      this.RxChar.Name = "RxChar";
      this.RxChar.ReadOnly = true;
      this.RxChar.Size = new Size(144, 288);
      this.RxChar.TabIndex = 0;
      this.RxChar.Text = "";
      this.groupBox1.Controls.Add((Control) this.ClearInput);
      this.groupBox1.Controls.Add((Control) this.RxChar);
      this.groupBox1.Location = new Point(8, 16);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new Size(160, 344);
      this.groupBox1.TabIndex = 1;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Input";
      this.groupBox2.Controls.Add((Control) this.ClearOutput);
      this.groupBox2.Controls.Add((Control) this.TxChar);
      this.groupBox2.Location = new Point(184, 16);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new Size(160, 344);
      this.groupBox2.TabIndex = 2;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Output";
      this.ClearInput.Location = new Point(16, 312);
      this.ClearInput.Name = "ClearInput";
      this.ClearInput.Size = new Size(128, 24);
      this.ClearInput.TabIndex = 1;
      this.ClearInput.Text = "Clear Input";
      this.ClearInput.Click += new EventHandler(this.ClearInput_Click);
      this.TxChar.Location = new Point(8, 16);
      this.TxChar.Multiline = true;
      this.TxChar.Name = "TxChar";
      this.TxChar.Size = new Size(144, 288);
      this.TxChar.TabIndex = 0;
      this.TxChar.Text = "";
      this.TxChar.KeyPress += new KeyPressEventHandler(this.TxChar_KeyPress);
      this.ClearOutput.Location = new Point(16, 312);
      this.ClearOutput.Name = "ClearOutput";
      this.ClearOutput.Size = new Size(128, 24);
      this.ClearOutput.TabIndex = 1;
      this.ClearOutput.Text = "Clear Output";
      this.ClearOutput.Click += new EventHandler(this.ClearOutput_Click);
      this.AutoScaleBaseSize = new Size(5, 13);
      this.ClientSize = new Size(354, 376);
      this.Controls.Add((Control) this.groupBox2);
      this.Controls.Add((Control) this.groupBox1);
      this.FormBorderStyle = FormBorderStyle.FixedSingle;
      this.Icon = (Icon) resourceManager.GetObject("$this.Icon");
      this.MaximizeBox = false;
      this.Name = "MonitorForm";
      this.StartPosition = FormStartPosition.CenterScreen;
      this.Text = "Monitor";
      this.groupBox1.ResumeLayout(false);
      this.groupBox2.ResumeLayout(false);
      this.ResumeLayout(false);
    }

    private void TxChar_KeyPress(object sender, KeyPressEventArgs e)
    {
      if (!this.Comm.IsOpen)
        return;
      this.Comm.Write(e.KeyChar.ToString());
    }

    private void ClearInput_Click(object sender, EventArgs e)
    {
      this.RxChar.Text = "";
    }

    private void ClearOutput_Click(object sender, EventArgs e)
    {
      this.TxChar.Text = "";
    }

    private delegate void SetTextCallback(char ch);
  }
}
