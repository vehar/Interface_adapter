Public Class frmUser
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
    Friend WithEvents txtBkg As System.Windows.Forms.RichTextBox
    Friend WithEvents btnOK As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.txtBkg = New System.Windows.Forms.RichTextBox
        Me.btnOK = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'txtBkg
        '
        Me.txtBkg.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle
        Me.txtBkg.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.txtBkg.Location = New System.Drawing.Point(8, 8)
        Me.txtBkg.Name = "txtBkg"
        Me.txtBkg.ReadOnly = True
        Me.txtBkg.Size = New System.Drawing.Size(464, 344)
        Me.txtBkg.TabIndex = 7
        Me.txtBkg.Text = "CodeVision 1.24.6 Debugger for Atmel Mega32 Microcontrollers PC Interface" & Microsoft.VisualBasic.ChrW(10) & "Version" & _
        " 1.1" & Microsoft.VisualBasic.ChrW(10) & "William Goodrich - Cornell University" & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "1. Introduction  " & Microsoft.VisualBasic.ChrW(10) & "Refer to the progr" & _
        "am or ECE 476 web site for background information on the purpose of this program" & _
        ".  The GForge site containing the source code and all documentation is located a" & _
        "t: https://gforge.cis.cornell.edu/projects/mega32debug/." & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "2. Installation Instru" & _
        "ctions" & Microsoft.VisualBasic.ChrW(10) & "This program does not require any files beyond the executable.  The confi" & _
        "g file is automatically generated if it does not exist." & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "3. Getting Started" & Microsoft.VisualBasic.ChrW(10) & "The " & _
        "code for the CodeVision 1.24.6 Debugger for Atmel Mega32 microcontrollers must b" & _
        "e downloaded and loaded on the mcu.  More in-depth instructions for this program" & _
        " are available at http://instruct1.cit.cornell.edu/courses/ee476/RTOS/Debugger/i" & _
        "ndex.html The MCU must be in debug mode to enable the program to establish a con" & _
        "nection and communicate with the mcu.  A serial connection must be established b" & _
        "etween the mcu and the computer running this program.  If the connection does no" & _
        "t use the default port 1 then port must be changed using the Setup-Communication" & _
        " command at the top of the screen.  The test connection button can be used to ch" & _
        "eck if a connection can be established." & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "4. Read From MCU" & Microsoft.VisualBasic.ChrW(10) & "The program is now set" & _
        "up to start reading from the MCU.  The commands in the group Read Data From MCU " & _
        "read from the MCU.  The command In Map File reads all data storage used in the l" & _
        "oaded map file.  If no map file is loaded then a dialog will be displayed to sel" & _
        "ect one.  The command All Registers reads the information on both the data and I" & _
        "/O registers from the mcu.  All Memory reads the information on all memory on th" & _
        "e mcu. Each command always retrieves the data displayed under MCU Info." & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "5. Writ" & _
        "e To MCU" & Microsoft.VisualBasic.ChrW(10) & "The list view on the right contains all the data storage information.  " & _
        "The first column contains the register name or memory address.  If the value lis" & _
        "ted in this column is double-clicked then a message box appears where a new valu" & _
        "e for the register/memory address can be entered.  The values should be entered " & _
        "in hex.  The program ensures that the values are valid." & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "6. Loading MAP File" & Microsoft.VisualBasic.ChrW(10) & "The" & _
        " map file can also be selected using the command File-Open Map File." & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "7. MCU Com" & _
        "mands" & Microsoft.VisualBasic.ChrW(10) & "Functions of the buttons listed under the group Other Commands:" & Microsoft.VisualBasic.ChrW(10) & "Run Target" & _
        " - Exits the debugger and returns to running the target" & Microsoft.VisualBasic.ChrW(10) & "Stop Target - Attempts (" & _
        "if set enabled in debugger.c) to stop the target and enter debug mode" & Microsoft.VisualBasic.ChrW(10) & "Reset MCU " & _
        "- Reset" & Microsoft.VisualBasic.ChrW(10) & Microsoft.VisualBasic.ChrW(10) & "8. Additional Features" & Microsoft.VisualBasic.ChrW(10) & "The information for the data registers, I/O regi" & _
        "sters, MCU info, and memory used in the map file can be printed out using File-P" & _
        "rint. The names of all I/O registers can be changed using Setup-I/O Register."
        '
        'btnOK
        '
        Me.btnOK.Location = New System.Drawing.Point(200, 360)
        Me.btnOK.Name = "btnOK"
        Me.btnOK.Size = New System.Drawing.Size(72, 32)
        Me.btnOK.TabIndex = 8
        Me.btnOK.Text = "OK"
        '
        'frmUser
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 15)
        Me.ClientSize = New System.Drawing.Size(482, 400)
        Me.ControlBox = False
        Me.Controls.Add(Me.btnOK)
        Me.Controls.Add(Me.txtBkg)
        Me.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Name = "frmUser"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "User Manual"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub btnOK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOK.Click
        Me.Close()
    End Sub
End Class
