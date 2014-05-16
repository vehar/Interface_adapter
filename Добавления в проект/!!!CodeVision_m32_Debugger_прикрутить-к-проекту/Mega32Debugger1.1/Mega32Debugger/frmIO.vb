Option Explicit On 
Option Strict On

Imports System.IO
Imports System.Xml

Public Class frmIO
    Inherits System.Windows.Forms.Form

    Private changes As Boolean
    Private MyConfig As ConfigInfo

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
    Friend WithEvents dtaGridIO As System.Windows.Forms.DataGrid
    Friend WithEvents btnSave As System.Windows.Forms.Button
    Friend WithEvents btnClose As System.Windows.Forms.Button
    Friend WithEvents Registers As Mega32Debugger.debugger
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.dtaGridIO = New System.Windows.Forms.DataGrid
        Me.Registers = New Mega32Debugger.debugger
        Me.btnSave = New System.Windows.Forms.Button
        Me.btnClose = New System.Windows.Forms.Button
        CType(Me.dtaGridIO, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.Registers, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'dtaGridIO
        '
        Me.dtaGridIO.CaptionFont = New System.Drawing.Font("Tahoma", 9.75!, System.Drawing.FontStyle.Bold)
        Me.dtaGridIO.DataMember = ""
        Me.dtaGridIO.DataSource = Me.Registers.register
        Me.dtaGridIO.Font = New System.Drawing.Font("Tahoma", 9.0!)
        Me.dtaGridIO.HeaderForeColor = System.Drawing.SystemColors.ControlText
        Me.dtaGridIO.Location = New System.Drawing.Point(24, 16)
        Me.dtaGridIO.Name = "dtaGridIO"
        Me.dtaGridIO.Size = New System.Drawing.Size(264, 376)
        Me.dtaGridIO.TabIndex = 0
        '
        'Registers
        '
        Me.Registers.DataSetName = "debugger"
        Me.Registers.Locale = New System.Globalization.CultureInfo("en-US")
        '
        'btnSave
        '
        Me.btnSave.Location = New System.Drawing.Point(32, 408)
        Me.btnSave.Name = "btnSave"
        Me.btnSave.Size = New System.Drawing.Size(88, 32)
        Me.btnSave.TabIndex = 5
        Me.btnSave.Text = "Save"
        '
        'btnClose
        '
        Me.btnClose.Location = New System.Drawing.Point(192, 408)
        Me.btnClose.Name = "btnClose"
        Me.btnClose.Size = New System.Drawing.Size(88, 32)
        Me.btnClose.TabIndex = 6
        Me.btnClose.Text = "Close"
        '
        'frmIO
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 16)
        Me.ClientSize = New System.Drawing.Size(304, 454)
        Me.ControlBox = False
        Me.Controls.Add(Me.btnSave)
        Me.Controls.Add(Me.btnClose)
        Me.Controls.Add(Me.dtaGridIO)
        Me.Font = New System.Drawing.Font("Tahoma", 9.75!)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.MaximizeBox = False
        Me.Name = "frmIO"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "I/O Registers"
        CType(Me.dtaGridIO, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.Registers, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)

    End Sub

#End Region
    Private Sub btnQuit_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnClose.Click
        If (changes AndAlso MsgBox("Are you sure you would like to close?", MsgBoxStyle.YesNo) = MsgBoxResult.Yes) Then
            Me.Close()
        Else
            Me.Close()
        End If
    End Sub

    Private Sub btnSave_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSave.Click
        Dim textWriter As XmlTextWriter = New XmlTextWriter(MyConfig.CONFIG_FILE, Nothing)

        Registers.WriteXml(textWriter)
        textWriter.Close()
    End Sub

    Private Sub frmIO_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Try
            changes = False
            Dim FS As Stream
            'Read the config file
            FS = New FileStream(MyConfig.CONFIG_FILE, FileMode.Open)
            Registers.Clear()
            Registers.ReadXml(FS)
            FS.Close()
        Catch x As Exception
        End Try
    End Sub

    Private Sub dtaGridIO_Validated(ByVal sender As Object, ByVal e As System.EventArgs) Handles dtaGridIO.Validated
        changes = True
    End Sub
End Class
