Public Class frmComm
    Inherits System.Windows.Forms.Form

    Private commPort As Integer
    Private MyCommPort As SerialCommuication
    Private MyConfig As ConfigInfo
#Region " Windows Form Designer generated code "

    Public Sub New(ByVal portNum As Integer)
        MyBase.New()
        commPort = portNum
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
    Friend WithEvents btnSave As System.Windows.Forms.Button
    Friend WithEvents btnClose As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents lstComm As System.Windows.Forms.ListBox
    Friend WithEvents btnTest As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.btnSave = New System.Windows.Forms.Button
        Me.btnClose = New System.Windows.Forms.Button
        Me.btnTest = New System.Windows.Forms.Button
        Me.lstComm = New System.Windows.Forms.ListBox
        Me.Label1 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'btnSave
        '
        Me.btnSave.Location = New System.Drawing.Point(16, 152)
        Me.btnSave.Name = "btnSave"
        Me.btnSave.Size = New System.Drawing.Size(56, 32)
        Me.btnSave.TabIndex = 4
        Me.btnSave.Text = "Save"
        '
        'btnClose
        '
        Me.btnClose.Location = New System.Drawing.Point(104, 152)
        Me.btnClose.Name = "btnClose"
        Me.btnClose.Size = New System.Drawing.Size(56, 32)
        Me.btnClose.TabIndex = 5
        Me.btnClose.Text = "Close"
        '
        'btnTest
        '
        Me.btnTest.Location = New System.Drawing.Point(24, 104)
        Me.btnTest.Name = "btnTest"
        Me.btnTest.Size = New System.Drawing.Size(128, 32)
        Me.btnTest.TabIndex = 3
        Me.btnTest.Text = "Test Connection"
        '
        'lstComm
        '
        Me.lstComm.ItemHeight = 16
        Me.lstComm.Items.AddRange(New Object() {"Com1", "Com2", "Com3", "Com4", "Com5", "Com6"})
        Me.lstComm.Location = New System.Drawing.Point(80, 16)
        Me.lstComm.Name = "lstComm"
        Me.lstComm.Size = New System.Drawing.Size(80, 68)
        Me.lstComm.TabIndex = 2
        '
        'Label1
        '
        Me.Label1.Font = New System.Drawing.Font("Tahoma", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(16, 16)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(48, 40)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Comm Port"
        '
        'frmComm
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 16)
        Me.ClientSize = New System.Drawing.Size(178, 200)
        Me.ControlBox = False
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.lstComm)
        Me.Controls.Add(Me.btnTest)
        Me.Controls.Add(Me.btnSave)
        Me.Controls.Add(Me.btnClose)
        Me.Font = New System.Drawing.Font("Tahoma", 9.75!)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "frmComm"
        Me.ShowInTaskbar = False
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent
        Me.Text = "Communication Setup"
        Me.ResumeLayout(False)

    End Sub

#End Region
    Public Function GetCommValue() As Integer
        Return commPort
    End Function

    Private Sub btnClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) _
        Handles btnClose.Click

        'If selected different comm port then make sure want to close
        If (commPort = (lstComm.SelectedIndex + 1)) Then
            Me.Close()
        ElseIf (MsgBox("Are you sure you would like to close without saving?", _
            MsgBoxStyle.YesNo) = MsgBoxResult.Yes) Then
            Me.Close()
        End If
    End Sub

    Private Sub frmComm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        'selects the currently used commport from the list
        lstComm.SelectedIndex = commPort - 1
        MyCommPort = New SerialCommuication
        MyConfig = New ConfigInfo
    End Sub

    Private Sub btnTest_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnTest.Click
        Dim testCommPort As Integer = lstComm.SelectedIndex + 1
        If (MyCommPort.InitializeComm(testCommPort, True)) Then
            MsgBox("Connection successfully established.")
        End If
        MyCommPort.CloseComm()
    End Sub

    Private Sub btnSave_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSave.Click
        commPort = lstComm.SelectedIndex + 1
        MyConfig.saveConfigFile(commPort)
    End Sub
End Class
