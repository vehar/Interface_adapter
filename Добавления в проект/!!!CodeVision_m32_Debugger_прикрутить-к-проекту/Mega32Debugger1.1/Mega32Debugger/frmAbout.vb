Public Class frmAbout
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents lblLine As System.Windows.Forms.Label
    Friend WithEvents btnOK As System.Windows.Forms.Button
    Friend WithEvents txtBkg As System.Windows.Forms.RichTextBox
    Friend WithEvents txtAbout As System.Windows.Forms.RichTextBox
    Friend WithEvents RichTextBox1 As System.Windows.Forms.RichTextBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.lblLine = New System.Windows.Forms.Label
        Me.btnOK = New System.Windows.Forms.Button
        Me.txtBkg = New System.Windows.Forms.RichTextBox
        Me.txtAbout = New System.Windows.Forms.RichTextBox
        Me.RichTextBox1 = New System.Windows.Forms.RichTextBox
        Me.SuspendLayout()
        '
        'lblLine
        '
        Me.lblLine.BackColor = System.Drawing.SystemColors.InfoText
        Me.lblLine.Location = New System.Drawing.Point(0, 224)
        Me.lblLine.Name = "lblLine"
        Me.lblLine.Size = New System.Drawing.Size(368, 1)
        Me.lblLine.TabIndex = 1
        '
        'btnOK
        '
        Me.btnOK.Location = New System.Drawing.Point(264, 248)
        Me.btnOK.Name = "btnOK"
        Me.btnOK.Size = New System.Drawing.Size(72, 32)
        Me.btnOK.TabIndex = 1
        Me.btnOK.Text = "OK"
        '
        'txtBkg
        '
        Me.txtBkg.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.txtBkg.Font = New System.Drawing.Font("Tahoma", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txtBkg.Location = New System.Drawing.Point(8, 8)
        Me.txtBkg.Name = "txtBkg"
        Me.txtBkg.ReadOnly = True
        Me.txtBkg.Size = New System.Drawing.Size(184, 208)
        Me.txtBkg.TabIndex = 6
        Me.txtBkg.Text = "This program requires the microcontroller to be running the CodeVision 1.24.6 Deb" & _
        "ugger for Atmel Mega32 microcontrollers developed by Bruce Land.  The debugger c" & _
        "an be found at: " & Microsoft.VisualBasic.ChrW(10) & "http://instruct1.cit.cornell.edu/courses/ee476/RTOS/Debugger/" & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & _
        "The debugger was developed to support ECE 476 Designing with Microcontrollers at" & _
        " Cornell University.  The home page for the course can be found at:" & Microsoft.VisualBasic.ChrW(10) & "http://instr" & _
        "uct1.cit.cornell.edu/courses/ee476/"
        '
        'txtAbout
        '
        Me.txtAbout.BackColor = System.Drawing.SystemColors.Control
        Me.txtAbout.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.txtAbout.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txtAbout.Location = New System.Drawing.Point(200, 8)
        Me.txtAbout.Name = "txtAbout"
        Me.txtAbout.ReadOnly = True
        Me.txtAbout.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None
        Me.txtAbout.Size = New System.Drawing.Size(160, 208)
        Me.txtAbout.TabIndex = 7
        Me.txtAbout.Text = "Mega32 Debugger PC Interface" & Microsoft.VisualBasic.ChrW(10) & "Version 1.1" & Microsoft.VisualBasic.ChrW(10) & "10/24/2005" & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "Written By" & Microsoft.VisualBasic.ChrW(10) & "William Goodrich" & Microsoft.VisualBasic.ChrW(10) & _
        "Cornell University" & Microsoft.VisualBasic.ChrW(10) & "MEng Candidate Dec 2005" & Microsoft.VisualBasic.ChrW(10) & "wjg3@cornell.edu" & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "Advised By" & Microsoft.VisualBasic.ChrW(10) & "Bruce La" & _
        "nd" & Microsoft.VisualBasic.ChrW(10) & "brl4@cornell.edu"
        '
        'RichTextBox1
        '
        Me.RichTextBox1.BackColor = System.Drawing.SystemColors.Control
        Me.RichTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None
        Me.RichTextBox1.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.RichTextBox1.Location = New System.Drawing.Point(8, 240)
        Me.RichTextBox1.Name = "RichTextBox1"
        Me.RichTextBox1.ReadOnly = True
        Me.RichTextBox1.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None
        Me.RichTextBox1.Size = New System.Drawing.Size(224, 64)
        Me.RichTextBox1.TabIndex = 8
        Me.RichTextBox1.Text = "All rights retained by Cornell University" & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "Program distributed under GNU General " & _
        "Public License"
        '
        'frmAbout
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 15)
        Me.ClientSize = New System.Drawing.Size(370, 312)
        Me.ControlBox = False
        Me.Controls.Add(Me.RichTextBox1)
        Me.Controls.Add(Me.txtAbout)
        Me.Controls.Add(Me.txtBkg)
        Me.Controls.Add(Me.btnOK)
        Me.Controls.Add(Me.lblLine)
        Me.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Name = "frmAbout"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "About Mega32 Debugger"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub btnOK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOK.Click
        Me.Close()
    End Sub

    Private Sub txtBkg_LinkClicked(ByVal sender As Object, ByVal e As System.Windows.Forms.LinkClickedEventArgs) Handles txtBkg.LinkClicked
        System.Diagnostics.Process.Start(e.LinkText)
    End Sub
End Class
