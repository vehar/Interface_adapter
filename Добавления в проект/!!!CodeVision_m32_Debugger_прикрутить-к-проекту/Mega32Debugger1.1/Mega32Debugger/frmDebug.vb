Option Explicit On 
Option Strict On

Imports System.Windows.Forms
Imports System.IO
Imports System.Text
Imports System.Xml

'************
Imports System.Threading

Public Class frmDebug
    Inherits System.Windows.Forms.Form
    Public Structure memoryStorageType
        Dim location As String      'memory location
        Dim value As String         'data stored at memory location
        Dim varName As String       'variable name of memory
        Dim byteLocation As String  'byte location of data in variable 
        Dim fullValue As String     'full value of variable
        Dim baseLocation As Integer 'location of first byte of variable
        Dim extra As String         'contains bit names or other information
    End Structure
    Private Enum Printing
        dataRegisters
        ioRegisters
        memory
    End Enum
    Private Const MEMORY_SIZE As Integer = 2048
    Private Const INFO_SIZE As Integer = 3
    Private Const DATA_REGS As Integer = 32
    Private Const IO_REGS As Integer = 64
    'Memory begins after registers
    Private Const MEM_OFFSET As Integer = DATA_REGS + IO_REGS

    Private dataRegisters(DATA_REGS) As memoryStorageType
    Private ioRegisters(IO_REGS) As memoryStorageType
    Private memory(MEMORY_SIZE) As memoryStorageType
    Private mcuInfo(INFO_SIZE) As memoryStorageType

    Private MyCommPort As SerialCommuication
    Private MyConfig As ConfigInfo
    Private lastMemTypeSelected As String   'Type of memory selected
    Private commPort As Integer             'Communications port used
    Private openMapFile As Boolean          'Has a map file been opened yet
    Private printType As Printing           'Used to determine which type of page printing
    Private printIndex As Integer           'Allows memory printing to span multiple pages
    'Date - Used for printing - Required so seconds don't change while printing
    Private dateStr As String
    'Used to get rid of prefix of data storage locations
    Private ioPrefixSize As Integer
    Private dataPrefixSize As Integer
    Private memPrefixSize As Integer


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
    Protected Friend WithEvents lvwMemory As System.Windows.Forms.ListView
    Protected WithEvents lvwMemType As System.Windows.Forms.ListView
    Friend WithEvents ColumnHeader1 As System.Windows.Forms.ColumnHeader
    Friend WithEvents mnuMain As System.Windows.Forms.MainMenu
    Friend WithEvents mnuFile As System.Windows.Forms.MenuItem
    Friend WithEvents mnuHelp As System.Windows.Forms.MenuItem
    Friend WithEvents mnuSetup As System.Windows.Forms.MenuItem
    Friend WithEvents mnuExit As System.Windows.Forms.MenuItem
    Friend WithEvents mnuOpen As System.Windows.Forms.MenuItem
    Friend WithEvents fileDialog As System.Windows.Forms.OpenFileDialog
    Friend WithEvents mnuIO As System.Windows.Forms.MenuItem
    Friend WithEvents mnuComm As System.Windows.Forms.MenuItem
    Friend WithEvents mnuAbout As System.Windows.Forms.MenuItem
    Friend WithEvents lblDoubleClick As System.Windows.Forms.Label
    Friend WithEvents btnRunTarget As System.Windows.Forms.Button
    Friend WithEvents btnReset As System.Windows.Forms.Button
    Friend WithEvents mnuPrint As System.Windows.Forms.MenuItem
    Friend WithEvents barProgress As System.Windows.Forms.ProgressBar
    Friend WithEvents btnStop As System.Windows.Forms.Button
    Friend WithEvents grpAdditional As System.Windows.Forms.GroupBox
    Friend WithEvents btnReadUsed As System.Windows.Forms.Button
    Friend WithEvents grpRead As System.Windows.Forms.GroupBox
    Friend WithEvents PrintDocument1 As System.Drawing.Printing.PrintDocument
    Friend WithEvents PrintPreviewDialog1 As System.Windows.Forms.PrintPreviewDialog
    Friend WithEvents PrintDialog1 As System.Windows.Forms.PrintDialog
    Friend WithEvents mnuPreview As System.Windows.Forms.MenuItem
    Friend WithEvents btnAllMem As System.Windows.Forms.Button
    Friend WithEvents btnAllReg As System.Windows.Forms.Button
    Friend WithEvents mnuUserMan As System.Windows.Forms.MenuItem
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Dim ListViewItem1 As System.Windows.Forms.ListViewItem = New System.Windows.Forms.ListViewItem("Data Registers", 0)
        Dim ListViewItem2 As System.Windows.Forms.ListViewItem = New System.Windows.Forms.ListViewItem("I/O Registers", 1)
        Dim ListViewItem3 As System.Windows.Forms.ListViewItem = New System.Windows.Forms.ListViewItem("MCU Info")
        Dim ListViewItem4 As System.Windows.Forms.ListViewItem = New System.Windows.Forms.ListViewItem("All Memory")
        Dim resources As System.Resources.ResourceManager = New System.Resources.ResourceManager(GetType(frmDebug))
        Me.lvwMemory = New System.Windows.Forms.ListView
        Me.lvwMemType = New System.Windows.Forms.ListView
        Me.ColumnHeader1 = New System.Windows.Forms.ColumnHeader
        Me.mnuMain = New System.Windows.Forms.MainMenu
        Me.mnuFile = New System.Windows.Forms.MenuItem
        Me.mnuOpen = New System.Windows.Forms.MenuItem
        Me.mnuPrint = New System.Windows.Forms.MenuItem
        Me.mnuPreview = New System.Windows.Forms.MenuItem
        Me.mnuExit = New System.Windows.Forms.MenuItem
        Me.mnuSetup = New System.Windows.Forms.MenuItem
        Me.mnuComm = New System.Windows.Forms.MenuItem
        Me.mnuIO = New System.Windows.Forms.MenuItem
        Me.mnuHelp = New System.Windows.Forms.MenuItem
        Me.mnuUserMan = New System.Windows.Forms.MenuItem
        Me.mnuAbout = New System.Windows.Forms.MenuItem
        Me.fileDialog = New System.Windows.Forms.OpenFileDialog
        Me.lblDoubleClick = New System.Windows.Forms.Label
        Me.grpAdditional = New System.Windows.Forms.GroupBox
        Me.btnStop = New System.Windows.Forms.Button
        Me.btnReset = New System.Windows.Forms.Button
        Me.btnRunTarget = New System.Windows.Forms.Button
        Me.barProgress = New System.Windows.Forms.ProgressBar
        Me.grpRead = New System.Windows.Forms.GroupBox
        Me.btnAllMem = New System.Windows.Forms.Button
        Me.btnAllReg = New System.Windows.Forms.Button
        Me.btnReadUsed = New System.Windows.Forms.Button
        Me.PrintDocument1 = New System.Drawing.Printing.PrintDocument
        Me.PrintPreviewDialog1 = New System.Windows.Forms.PrintPreviewDialog
        Me.PrintDialog1 = New System.Windows.Forms.PrintDialog
        Me.grpAdditional.SuspendLayout()
        Me.grpRead.SuspendLayout()
        Me.SuspendLayout()
        '
        'lvwMemory
        '
        Me.lvwMemory.AutoArrange = False
        Me.lvwMemory.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable
        Me.lvwMemory.Location = New System.Drawing.Point(176, 8)
        Me.lvwMemory.MultiSelect = False
        Me.lvwMemory.Name = "lvwMemory"
        Me.lvwMemory.Size = New System.Drawing.Size(584, 368)
        Me.lvwMemory.TabIndex = 2
        Me.lvwMemory.View = System.Windows.Forms.View.Details
        '
        'lvwMemType
        '
        Me.lvwMemType.AutoArrange = False
        Me.lvwMemType.Columns.AddRange(New System.Windows.Forms.ColumnHeader() {Me.ColumnHeader1})
        Me.lvwMemType.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable
        Me.lvwMemType.HideSelection = False
        ListViewItem1.StateImageIndex = 0
        ListViewItem1.Tag = "Data Register"
        ListViewItem2.StateImageIndex = 0
        ListViewItem2.Tag = "I/O Register"
        ListViewItem3.StateImageIndex = 0
        ListViewItem3.Tag = "MCU"
        ListViewItem4.StateImageIndex = 0
        ListViewItem4.Tag = "All Memory Address"
        Me.lvwMemType.Items.AddRange(New System.Windows.Forms.ListViewItem() {ListViewItem1, ListViewItem2, ListViewItem3, ListViewItem4})
        Me.lvwMemType.Location = New System.Drawing.Point(8, 8)
        Me.lvwMemType.MultiSelect = False
        Me.lvwMemType.Name = "lvwMemType"
        Me.lvwMemType.Size = New System.Drawing.Size(160, 120)
        Me.lvwMemType.TabIndex = 1
        Me.lvwMemType.View = System.Windows.Forms.View.Details
        '
        'ColumnHeader1
        '
        Me.ColumnHeader1.Text = "Type of Information"
        Me.ColumnHeader1.Width = 154
        '
        'mnuMain
        '
        Me.mnuMain.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuFile, Me.mnuSetup, Me.mnuHelp})
        '
        'mnuFile
        '
        Me.mnuFile.Index = 0
        Me.mnuFile.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuOpen, Me.mnuPrint, Me.mnuPreview, Me.mnuExit})
        Me.mnuFile.Text = "File"
        '
        'mnuOpen
        '
        Me.mnuOpen.Index = 0
        Me.mnuOpen.Text = "Open Map File"
        '
        'mnuPrint
        '
        Me.mnuPrint.Index = 1
        Me.mnuPrint.Text = "Print"
        '
        'mnuPreview
        '
        Me.mnuPreview.Index = 2
        Me.mnuPreview.Text = "Print Preview"
        '
        'mnuExit
        '
        Me.mnuExit.Index = 3
        Me.mnuExit.Text = "Exit"
        '
        'mnuSetup
        '
        Me.mnuSetup.Index = 1
        Me.mnuSetup.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuComm, Me.mnuIO})
        Me.mnuSetup.Text = "Setup"
        '
        'mnuComm
        '
        Me.mnuComm.Index = 0
        Me.mnuComm.Text = "Communication"
        '
        'mnuIO
        '
        Me.mnuIO.Index = 1
        Me.mnuIO.Text = "I/O Register"
        '
        'mnuHelp
        '
        Me.mnuHelp.Index = 2
        Me.mnuHelp.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.mnuUserMan, Me.mnuAbout})
        Me.mnuHelp.Text = "Help"
        '
        'mnuUserMan
        '
        Me.mnuUserMan.Index = 0
        Me.mnuUserMan.Text = "User Manual"
        '
        'mnuAbout
        '
        Me.mnuAbout.Index = 1
        Me.mnuAbout.Text = "About"
        '
        'fileDialog
        '
        Me.fileDialog.DefaultExt = "map"
        Me.fileDialog.Filter = "Map Files (*.map) | *.map"
        '
        'lblDoubleClick
        '
        Me.lblDoubleClick.ImageAlign = System.Drawing.ContentAlignment.TopCenter
        Me.lblDoubleClick.Location = New System.Drawing.Point(184, 392)
        Me.lblDoubleClick.Name = "lblDoubleClick"
        Me.lblDoubleClick.Size = New System.Drawing.Size(576, 40)
        Me.lblDoubleClick.TabIndex = 6
        Me.lblDoubleClick.Text = "Dynamic Load"
        Me.lblDoubleClick.TextAlign = System.Drawing.ContentAlignment.TopCenter
        '
        'grpAdditional
        '
        Me.grpAdditional.Controls.Add(Me.btnStop)
        Me.grpAdditional.Controls.Add(Me.btnReset)
        Me.grpAdditional.Controls.Add(Me.btnRunTarget)
        Me.grpAdditional.Location = New System.Drawing.Point(8, 288)
        Me.grpAdditional.Name = "grpAdditional"
        Me.grpAdditional.Size = New System.Drawing.Size(160, 144)
        Me.grpAdditional.TabIndex = 8
        Me.grpAdditional.TabStop = False
        Me.grpAdditional.Text = "Other Commands"
        '
        'btnStop
        '
        Me.btnStop.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnStop.Location = New System.Drawing.Point(32, 64)
        Me.btnStop.Name = "btnStop"
        Me.btnStop.Size = New System.Drawing.Size(96, 32)
        Me.btnStop.TabIndex = 9
        Me.btnStop.Text = "Stop Target"
        '
        'btnReset
        '
        Me.btnReset.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnReset.Location = New System.Drawing.Point(32, 104)
        Me.btnReset.Name = "btnReset"
        Me.btnReset.Size = New System.Drawing.Size(96, 32)
        Me.btnReset.TabIndex = 10
        Me.btnReset.Text = "Reset MCU"
        '
        'btnRunTarget
        '
        Me.btnRunTarget.Font = New System.Drawing.Font("Tahoma", 9.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnRunTarget.Location = New System.Drawing.Point(32, 24)
        Me.btnRunTarget.Name = "btnRunTarget"
        Me.btnRunTarget.Size = New System.Drawing.Size(96, 32)
        Me.btnRunTarget.TabIndex = 8
        Me.btnRunTarget.Text = "Run Target"
        '
        'barProgress
        '
        Me.barProgress.Location = New System.Drawing.Point(176, 392)
        Me.barProgress.Maximum = 2144
        Me.barProgress.Name = "barProgress"
        Me.barProgress.Size = New System.Drawing.Size(584, 40)
        Me.barProgress.Step = 1
        Me.barProgress.TabIndex = 9
        Me.barProgress.Value = 1
        Me.barProgress.Visible = False
        '
        'grpRead
        '
        Me.grpRead.Controls.Add(Me.btnAllMem)
        Me.grpRead.Controls.Add(Me.btnAllReg)
        Me.grpRead.Controls.Add(Me.btnReadUsed)
        Me.grpRead.Location = New System.Drawing.Point(8, 136)
        Me.grpRead.Name = "grpRead"
        Me.grpRead.Size = New System.Drawing.Size(160, 144)
        Me.grpRead.TabIndex = 10
        Me.grpRead.TabStop = False
        Me.grpRead.Text = "Read Data From MCU"
        '
        'btnAllMem
        '
        Me.btnAllMem.Font = New System.Drawing.Font("Tahoma", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnAllMem.Location = New System.Drawing.Point(32, 104)
        Me.btnAllMem.Name = "btnAllMem"
        Me.btnAllMem.Size = New System.Drawing.Size(96, 32)
        Me.btnAllMem.TabIndex = 6
        Me.btnAllMem.Text = "All Memory"
        '
        'btnAllReg
        '
        Me.btnAllReg.Location = New System.Drawing.Point(32, 64)
        Me.btnAllReg.Name = "btnAllReg"
        Me.btnAllReg.Size = New System.Drawing.Size(96, 32)
        Me.btnAllReg.TabIndex = 5
        Me.btnAllReg.Text = "All Registers"
        '
        'btnReadUsed
        '
        Me.btnReadUsed.Font = New System.Drawing.Font("Tahoma", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.btnReadUsed.Location = New System.Drawing.Point(24, 24)
        Me.btnReadUsed.Name = "btnReadUsed"
        Me.btnReadUsed.Size = New System.Drawing.Size(112, 32)
        Me.btnReadUsed.TabIndex = 4
        Me.btnReadUsed.Text = "In Map File"
        '
        'PrintDocument1
        '
        '
        'PrintPreviewDialog1
        '
        Me.PrintPreviewDialog1.AutoScrollMargin = New System.Drawing.Size(0, 0)
        Me.PrintPreviewDialog1.AutoScrollMinSize = New System.Drawing.Size(0, 0)
        Me.PrintPreviewDialog1.ClientSize = New System.Drawing.Size(400, 300)
        Me.PrintPreviewDialog1.Document = Me.PrintDocument1
        Me.PrintPreviewDialog1.Enabled = True
        Me.PrintPreviewDialog1.Icon = CType(resources.GetObject("PrintPreviewDialog1.Icon"), System.Drawing.Icon)
        Me.PrintPreviewDialog1.Location = New System.Drawing.Point(66, 87)
        Me.PrintPreviewDialog1.MinimumSize = New System.Drawing.Size(375, 250)
        Me.PrintPreviewDialog1.Name = "PrintPreviewDialog1"
        Me.PrintPreviewDialog1.TransparencyKey = System.Drawing.Color.Empty
        Me.PrintPreviewDialog1.Visible = False
        '
        'frmDebug
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 16)
        Me.ClientSize = New System.Drawing.Size(772, 441)
        Me.Controls.Add(Me.grpRead)
        Me.Controls.Add(Me.barProgress)
        Me.Controls.Add(Me.grpAdditional)
        Me.Controls.Add(Me.lblDoubleClick)
        Me.Controls.Add(Me.lvwMemType)
        Me.Controls.Add(Me.lvwMemory)
        Me.Font = New System.Drawing.Font("Tahoma", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MaximizeBox = False
        Me.Menu = Me.mnuMain
        Me.Name = "frmDebug"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "Mega32 Debugger"
        Me.grpAdditional.ResumeLayout(False)
        Me.grpRead.ResumeLayout(False)
        Me.ResumeLayout(False)

    End Sub

#End Region

    Private Sub EnableAction(ByVal value As Boolean)
        'Enables or Disables all functionality of the program - Used to prevent user 
        'from causing any action while reading values form MCU

        btnReadUsed.Enabled = value
        btnAllReg.Enabled = value
        btnAllMem.Enabled = value
        btnRunTarget.Enabled = value
        btnStop.Enabled = value
        btnReset.Enabled = value
        mnuFile.Enabled = value
        mnuSetup.Enabled = value
        mnuHelp.Enabled = value
        lvwMemType.Enabled = value
        lvwMemory.Enabled = value
        mnuOpen.Enabled = value
        mnuPrint.Enabled = value
        mnuExit.Enabled = value
        mnuComm.Enabled = value
        mnuIO.Enabled = value
        mnuAbout.Enabled = value
        mnuUserMan.Enabled = value
    End Sub

    Private Sub ChangeUsedMemoryValues(ByVal numItems As Integer, _
        ByVal valueArray() As memoryStorageType)
        Dim counter As Integer
        Dim usedCounter As Integer = 0 'Counts entries printed to screen

        'Split up to speed up loading of variables on screen
        'Display all values for selected memory type
        For counter = 0 To (numItems - 1)
            If (valueArray(counter).byteLocation.Length > 0) Then
                lvwMemory.Items.Add(valueArray(counter).location)
                lvwMemory.Items(usedCounter).SubItems.Add(valueArray(counter).value)
                lvwMemory.Items(usedCounter).SubItems.Add(valueArray(counter).varName)
                lvwMemory.Items(usedCounter).SubItems.Add(valueArray(counter).byteLocation)
                lvwMemory.Items(usedCounter).SubItems.Add(valueArray(counter).fullValue)
                usedCounter += 1
            End If
        Next counter
    End Sub

    Private Sub ChangeDisplayValues(ByVal numItems As Integer, _
        ByVal valueArray() As memoryStorageType)

        Dim counter As Integer
        'Display all values for selected memory type
        For counter = 0 To (numItems - 1)
            lvwMemory.Items.Add(valueArray(counter).location)
            lvwMemory.Items(counter).SubItems.Add(valueArray(counter).value)
            lvwMemory.Items(counter).SubItems.Add(valueArray(counter).varName)
            lvwMemory.Items(counter).SubItems.Add(valueArray(counter).byteLocation)
            lvwMemory.Items(counter).SubItems.Add(valueArray(counter).fullValue)
            lvwMemory.Items(counter).SubItems.Add(valueArray(counter).extra)
        Next counter
    End Sub

    Private Sub ChangeDisplay()
        'Clear the contents of the memory window and re-display the columns
        'Each type of data storage has different columns to display

        lvwMemory.Clear()
        Select Case (lastMemTypeSelected)
            Case "Data Register"
                lvwMemory.Columns.Add("Register", 60, 0)
                ChangeDisplayValues(DATA_REGS, dataRegisters)
            Case "All Memory Address"
                lvwMemory.Columns.Add("Address", 60, 0)
                ChangeDisplayValues(MEMORY_SIZE, memory)
            Case "Memory Address"
                lvwMemory.Columns.Add("Address", 60, 0)
                ChangeUsedMemoryValues(MEMORY_SIZE, memory)
            Case "I/O Register"
                lvwMemory.Columns.Add("Register", 60, 0)
                ChangeDisplayValues(IO_REGS, ioRegisters)
            Case Else
                lvwMemory.Columns.Add("Type Of Information", 150, 0)
                ChangeDisplayValues(INFO_SIZE, mcuInfo)
        End Select

        If (lastMemTypeSelected <> "MCU") Then
            lvwMemory.Columns.Add("Value", 50, 0)
            lvwMemory.Columns.Add("Variable", 120, 0)

            If (lastMemTypeSelected <> "I/O Register") Then
                lvwMemory.Columns.Add("Byte Location", 90, 0)
                lvwMemory.Columns.Add("Full Value of Variable", 140, 0)
                If (lastMemTypeSelected = "Data Register") Then
                    lvwMemory.Columns.Add("Type", 90, 0)
                End If
            End If
        Else
            lvwMemory.Columns.Add("Value", 75, 0)
            lvwMemory.Columns.Add("Additional Info", 250, 0)
        End If

    End Sub

    Private Sub GetFullValues(ByRef valueArray() As memoryStorageType, _
        ByVal size As Integer)
        Dim i, j As Integer
        Dim fullValue As String

        'Clear all full values previously computed
        For i = 0 To (size - 1)
            valueArray(i).fullValue = ""
        Next

        'Check to see if variable stored is > 1 byte
        For i = 0 To (size - 1)
            If (valueArray(i).byteLocation.Length > 0) Then
                fullValue = ""
                j = 0
                'check all registers to form the full value
                While (j < size)
                    If (valueArray(i).baseLocation = valueArray(j).baseLocation) Then
                        'Check if full value already determined
                        If (valueArray(j).fullValue.Length <> 0) Then
                            fullValue = valueArray(j).fullValue
                            Exit While
                        Else
                            'Assumes the values are in order  
                            fullValue = CStr(IIf(valueArray(j).value.Length() = 1, _
                                "0", "")) + valueArray(j).value + fullValue
                        End If
                    End If
                    j += 1
                End While
                valueArray(i).fullValue = fullValue
            End If
        Next
    End Sub

    Private Sub frmDebug_Load(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles MyBase.Load
        Dim count As Integer

        'Initialize arrays used to store memory information
        For count = 0 To (DATA_REGS - 1)
            dataRegisters(count).location = "R" & CStr(count)
            dataRegisters(count).byteLocation = ""  'necessary for fullValue computation
            dataRegisters(count).value = ""
            dataRegisters(count).fullValue = ""
        Next

        'Hard code functions of data registers
        For count = 0 To 15
            dataRegisters(count).extra = "Global"
        Next

        For count = 16 To 21
            dataRegisters(count).extra = "Local"
        Next

        For count = 22 To 31
            dataRegisters(count).extra = "State Info"
        Next

        'I/O register and memory addresses are in hex
        For count = 0 To (IO_REGS - 1)
            ioRegisters(count).location = "0x" & CStr(Hex$(count))
        Next

        For count = 0 To (MEMORY_SIZE - 1)
            memory(count).location = "0x" & CStr(Hex$(count + MEM_OFFSET))
            memory(count).byteLocation = ""
            memory(count).value = ""
            memory(count).fullValue = ""
        Next

        mcuInfo(0).location = "MCU Status Register"
        mcuInfo(0).varName = "Bits in SREG: I T H S V N Z C"
        mcuInfo(1).location = "Top of Data Stack"
        mcuInfo(2).location = "Top of Hardware Stack"

        ioPrefixSize = 2 'Size of "0x" in characters
        memPrefixSize = 2 '0x
        dataPrefixSize = 1 'R

        openMapFile = False 'Have not yet loaded a map file
        lblDoubleClick.Text = "Double-click on register or memory address to change the " + _
            "value on the MCU" + Chr(10) + "Use file menu to load compiler generated .map file"
        MyCommPort = New SerialCommuication 'Used to serially communicate with MCU
        MyConfig = New ConfigInfo 'Used to load config file
        lvwMemType.Items(0).Selected = True 'Force selection of first type of memory
        MyConfig.readIORegisterFile(ioRegisters, True, commPort)
        printType = Printing.dataRegisters 'Prints dataRegisters first
    End Sub

    Private Function ParseMapPart(ByVal txtLine As String, _
        ByRef charLocation As Integer) As StringBuilder
        'Aids in parsing of map file
        Dim sb As New StringBuilder
        Dim j As Integer = 0

        'Builds string until finds white space
        While (Not Char.IsWhiteSpace(txtLine.Chars(charLocation)))
            sb.Insert(j, txtLine.Chars(charLocation))
            charLocation += 1
            j += 1
        End While

        'Finds end of white space to enable reading of next string
        While (Char.IsWhiteSpace(txtLine.Chars(charLocation)))
            charLocation += 1
        End While

        Return sb
    End Function

    Private Function LoadMapFile() As Boolean
        'Reads a compiler generated map file
        Dim fs As FileStream
        Dim sr As StreamReader
        Dim txtLine As String 'Read a line from the map file
        Dim strLocation As String
        Dim varName As New StringBuilder
        Dim address As New StringBuilder
        Dim size As New StringBuilder
        Dim delimiters() As Char = {CChar(",")}
        Dim addressParts() As String
        Dim firstChr As Char
        Const HEX_STR As String = "&H"
        Const INCORRECT_VERSION_MSG As String = "Incorrect File Type"
        Dim iPart As IEnumerator
        Dim byteCnt, baseMemLocation, location, i, locationCnt As Integer

        If fileDialog.ShowDialog() = DialogResult.OK Then
            'if selected a file to open then attempt to read out information
            Try
                fs = New FileStream(fileDialog.FileName, FileMode.Open, _
                    System.IO.FileAccess.Read)
                sr = New StreamReader(fs)

                'Check if correct requires line that contains "Variable" before all
                'variables are listed
                txtLine = sr.ReadLine()
                While (txtLine.IndexOf("Variable") = -1) AndAlso (sr.Peek > -1)
                    txtLine = sr.ReadLine()
                End While

                If (txtLine.IndexOf("Variable") = -1) Then
                    MsgBox(INCORRECT_VERSION_MSG)
                    Exit Function
                End If

                'Re-initialize variable names and byte locations
                'necessary if map file aleady read
                For i = 0 To (DATA_REGS - 1)
                    dataRegisters(i).varName = ""
                    dataRegisters(i).byteLocation = ""
                    dataRegisters(i).fullValue = ""
                    dataRegisters(i).baseLocation = i
                Next

                For i = 0 To (MEMORY_SIZE - 1)
                    memory(i).varName = ""
                    memory(i).byteLocation = ""
                    memory(i).fullValue = ""
                    memory(i).baseLocation = i
                Next

                'In case problems occur while loading map file
                openMapFile = False

                While (sr.Peek > -1)
                    byteCnt = 0
                    txtLine = sr.ReadLine()

                    'Read Variable Name
                    varName = ParseMapPart(txtLine, byteCnt)
                    'Read address(es)
                    address = ParseMapPart(txtLine, byteCnt)

                    'Read size of variable
                    size.Remove(0, size.Length) 'Clear the StringBuilder
                    i = 0
                    While (byteCnt < txtLine.Length)
                        size.Insert(i, txtLine.Chars(byteCnt))
                        i += 1
                        byteCnt += 1
                    End While

                    addressParts = address.ToString.Split(delimiters)
                    iPart = addressParts.GetEnumerator
                    locationCnt = 1

                    'loop thru all locations seperated by commas
                    While iPart.MoveNext
                        If (iPart.Current.ToString.Length <> 0) Then
                            firstChr = iPart.Current.ToString.Chars(0)
                            strLocation = iPart.Current.ToString.Replace("R", "0")
                            strLocation = strLocation.Replace("h", "")

                            Select Case firstChr
                                Case CChar("R")
                                    location = CInt(strLocation)
                                    If (locationCnt = 1) Then
                                        baseMemLocation = location
                                    End If
                                    dataRegisters(location).varName = varName.ToString
                                    dataRegisters(location).byteLocation = _
                                        CStr(locationCnt)
                                    dataRegisters(location).baseLocation = baseMemLocation
                                Case Else
                                    location = CInt(HEX_STR + strLocation) - MEM_OFFSET
                                    For i = location To (location + CInt(size.ToString))
                                        memory(i).varName = varName.ToString
                                        memory(i).byteLocation = CStr(i - location + 1)
                                        memory(i).baseLocation = location
                                    Next i
                            End Select
                        End If
                        locationCnt += 1
                    End While
                End While

                'Add a new type of information (used memory) if not currently shown
                If (Not lvwMemType.Items(2).Text.Equals("Used Memory")) Then
                    lvwMemType.Items.Insert(2, New ListViewItem("Used Memory"))
                    lvwMemType.Items(2).Font = lvwMemType.Items(1).Font
                    lvwMemType.Items(2).Tag = "Memory Address"
                End If

                openMapFile = True
                Return True
            Catch ex As Exception
                MsgBox("Unable to load map file.", MsgBoxStyle.OKOnly)
                Return False
            End Try
        End If

        Return False
    End Function

    Private Sub mnuOpen_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuOpen.Click
        If LoadMapFile() Then
            'if successfully loaded map file then get full values for all variables
            '(check if variables are multiple bytes in length)
            GetFullValues(dataRegisters, DATA_REGS)
            GetFullValues(memory, MEMORY_SIZE)
            ChangeDisplay()
        End If
    End Sub

    Private Function ParseBuffer(ByRef MyCommPort As SerialCommuication) As String
        Dim readBuffer() As Byte
        Dim byteCnt, insertCnt As Integer
        Dim sb As New StringBuilder

        readBuffer = MyCommPort.ReadBytes() 'Read Serial Port

        'Parse Out The Information In The Array - msg of form dataReg %d= %x\n\r
        byteCnt = 0
        While ((byteCnt < readBuffer.Length) AndAlso (readBuffer(byteCnt) <> Asc("=")))
            byteCnt += 1
        End While

        If (byteCnt = readBuffer.Length) Then
            'The messages were sent to fast - message does not contain "="
            Return ""
        End If

        insertCnt = 0
        byteCnt += 1 'Ignore the equal sign

        'Ignore any leading spaces
        If ((byteCnt < readBuffer.Length) AndAlso (readBuffer(byteCnt) = Asc(" "))) Then
            byteCnt += 1
        End If

        sb.Remove(0, sb.Length) 'Clear the StringBuilder
        While ((byteCnt < readBuffer.Length) AndAlso (readBuffer(byteCnt) <> 10) _
            AndAlso (readBuffer(byteCnt) <> 13))
            'Read until end of line
            sb.Insert(insertCnt, Chr(readBuffer(byteCnt)))
            byteCnt += 1
            insertCnt += 1
        End While

        'Dim sb2 As New StringBuilder
        'i = 0
        'While ((i < readBuffer.Length))
        'Read until end of line
        'sb2.Insert(i, Chr(readBuffer(i)))
        'i += 1
        'End While
        'Debug.WriteLine(sb2.ToString)

        Return sb.ToString()
    End Function

    Private Sub lvwMemType_SelectedIndexChanged(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles lvwMemType.SelectedIndexChanged
        'Ensures that a type of memory is selected before refreshing display
        If lvwMemType.SelectedItems.Count <> 0 Then
            lastMemTypeSelected = CStr(lvwMemType.SelectedItems.Item(0).Tag)
            ChangeDisplay()
        End If
    End Sub

    Private Sub WriteTypeOfStorage(ByRef valueArray() As memoryStorageType, _
        ByRef buffer() As Byte, ByVal locationOffset As Integer, _
        ByRef bufCount As Integer, ByVal index As Integer, ByRef value As String)
        Dim strLength As Integer = valueArray(index).location.Length
        Dim count As Integer

        valueArray(index).value = value
        bufCount = 2 'first character is command character, 2nd is =
        'read the location and convert each character to ascii code
        For count = locationOffset To (strLength - 1)
            buffer(bufCount) = CByte(Asc(valueArray(index).location.Chars(count)))
            bufCount += 1
        Next
        buffer(bufCount) = 13  '13 = Return - Ends the command
    End Sub

    Private Sub lvwMemory_DoubleClick(ByVal sender As Object, _
        ByVal e As System.EventArgs) Handles lvwMemory.DoubleClick

        Dim hexStr As String = "&H"
        Dim intValue, index, bufCount As Integer
        Dim buffer(10) As Byte

        If (lastMemTypeSelected = "MCU") Then
            MsgBox("Can not edit MCU information.")
            Exit Sub
        End If

        'Input Box - Make sure can't change Certain R Values - Check to see if valid data
        If ((lastMemTypeSelected = "Data Register") AndAlso _
            (lvwMemory.SelectedItems.Item(0).Index >= 22)) Then
            MsgBox("Can not edit values for data registers R22-R31.")
            Exit Sub
        End If

        'If attempting to change memory - warn that there is no memory protection
        Dim value As String = InputBox(CStr(IIf((Not lastMemTypeSelected.Equals("Data" + _
            " Register")) And (Not lastMemTypeSelected.Equals("I/O Register")), _
            "Warning: There is no memory" + " protection" + Chr(10) + Chr(10), "")) + _
            "Enter New Value For " + lastMemTypeSelected + " " + _
            CStr(lvwMemory.SelectedItems.Item(0).Text) + ":")

        'If entered a data value
        If value.Length <> 0 Then
            Try
                intValue = CInt(hexStr + value)
                If ((intValue <= 255) AndAlso (intValue >= 0)) Then
                    'Valid value - Connect and Write the variable
                    If (MyCommPort.InitializeComm(commPort, True)) Then
                        index = lvwMemory.SelectedItems.Item(0).Index
                        buffer(1) = Asc(" ")
                        Select Case (lastMemTypeSelected) 'which type of memory selected
                            Case "Data Register"
                                buffer(0) = Asc("R")
                                WriteTypeOfStorage(dataRegisters, buffer, _
                                    dataPrefixSize, bufCount, index, value)
                            Case "I/O Register"
                                buffer(0) = Asc("I")
                                WriteTypeOfStorage(ioRegisters, buffer, memPrefixSize, _
                                    bufCount, index, value)
                            Case "All Memory Address"
                                buffer(0) = Asc("M")
                                WriteTypeOfStorage(memory, buffer, memPrefixSize, _
                                    bufCount, index, value)
                            Case Else
                                index = 0
                                'find the location of the register
                                While ((Not memory(index).location.Equals(lvwMemory.SelectedItems.Item(0).Text())) _
                                    AndAlso (index < MEMORY_SIZE))
                                    index += 1
                                End While
                                buffer(0) = Asc("M")
                                WriteTypeOfStorage(memory, buffer, memPrefixSize, _
                                    bufCount, index, value)
                        End Select

                        'Sending value in form [R,M,I] Location Value
                        buffer(bufCount) = Asc(" ")
                        bufCount += 1
                        If (value.Length > 1) Then
                            buffer(bufCount) = CByte(Asc(value.Chars(0)))
                            bufCount += 1
                            buffer(bufCount) = CByte(Asc(value.Chars(1)))
                        Else
                            buffer(bufCount) = CByte(Asc(value.Chars(0)))
                        End If
                        bufCount += 1

                        'Write new value to MCU
                        MyCommPort.WriteBytes(buffer, bufCount + 1)

                        ChangeDisplay()
                        MyCommPort.CloseComm()
                    End If
                Else
                    MsgBox("Invalid Entry - Enter New Value In Hex")
                End If
            Catch ex As Exception
                'Ignore Exception
            End Try
        End If
    End Sub

    Private Sub mnuExit_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuExit.Click
        MyCommPort.CloseComm()
        End
    End Sub

    Private Sub frmDebug_Closed(ByVal sender As Object, _
        ByVal e As System.EventArgs) Handles MyBase.Closed
        MyCommPort.CloseComm()
    End Sub

    Private Sub mnuIO_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuIO.Click
        Dim frm As New frmIO
        'Show form to change names of I/O Registers
        frm.ShowDialog()
        MyConfig.readIORegisterFile(ioRegisters, True, commPort)
        ChangeDisplay()
    End Sub

    Private Sub mnuComm_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuComm.Click

        'Show form to change communications settings
        Dim frm As New frmComm(commPort)
        frm.ShowDialog()
        commPort = frm.GetCommValue()
    End Sub

    Private Sub ReadTypeOfStorage(ByRef MyCommPort As SerialCommuication, _
        ByVal Size As Integer, ByVal valueArray() As memoryStorageType, _
        ByRef buffer() As Byte, ByVal offset As Integer, _
        ByVal checkUsed As Boolean, ByVal locationOffset As Integer, _
        ByVal multiplier As Integer)

        Dim count, count2, strLength, location As Integer
        Dim tempStr As String

        For count = 0 To Size
            If ((Not checkUsed) OrElse (checkUsed _
                AndAlso valueArray(count).byteLocation.Length > 0)) Then
                strLength = valueArray(count).location.Length
                location = 2 'first character is command character, 2nd is =
                'read the locationand convert each character to ascii code
                For count2 = locationOffset To (strLength - 1)
                    buffer(location) = CByte(Asc(valueArray(count).location.Chars(count2)))
                    location += 1
                Next

                buffer(location) = 13  '13 = Return - Ends the command
                MyCommPort.WriteBytes(buffer, location + 1)
                tempStr = ParseBuffer(MyCommPort)
                'No value received
                If tempStr.Length = 0 Then
                    'Try one more time to receive the data
                    'If still have problems then move on
                    MyCommPort.WriteBytes(buffer, location + 1)
                    tempStr = ParseBuffer(MyCommPort)
                End If
                valueArray(count).value = tempStr
            End If
            If (count Mod 5 = 0) Then
                barProgress.Value = offset + count * multiplier
            End If
        Next
    End Sub

    Private Sub ReadDataStorage(ByVal checkUsed As Boolean, ByVal readMem As Boolean)
        Dim count As Integer
        Dim buffer(10) As Byte
        Dim MyCommPort As SerialCommuication = New SerialCommuication

        If (MyCommPort.InitializeComm(commPort, True)) Then
            buffer(1) = Asc(" ")
            'max barProgress value = 2048+96
            If ((Not readMem) OrElse checkUsed) Then
                buffer(0) = Asc("r") 'Read data registers
                'IIf(readMem, 1, 30) - increases the progress bar more if memory is not read
                ReadTypeOfStorage(MyCommPort, DATA_REGS - 1, dataRegisters, buffer, 0, _
                    checkUsed, dataPrefixSize, CInt(IIf(readMem, 1, 20)))

                buffer(0) = Asc("i") 'Read I/O registers
                ReadTypeOfStorage(MyCommPort, IO_REGS - 1, ioRegisters, buffer, _
                    DATA_REGS + CInt(IIf(readMem, 0, 19 * DATA_REGS)), False, _
                    ioPrefixSize, CInt(IIf(readMem, 1, 20)))
            End If

            If readMem Then
                buffer(0) = Asc("m") 'Read memory
                ReadTypeOfStorage(MyCommPort, MEMORY_SIZE - 1, memory, buffer, _
                    DATA_REGS + IO_REGS, checkUsed, memPrefixSize, 1)
            End If

            'Get additional MCU info
            buffer(1) = 13
            buffer(0) = Asc("s")
            MyCommPort.WriteBytes(buffer, 2)
            mcuInfo(0).value = ParseBuffer(MyCommPort)
            buffer(0) = Asc("d")
            MyCommPort.WriteBytes(buffer, 2)
            mcuInfo(1).value = ParseBuffer(MyCommPort)
            buffer(0) = Asc("w")
            MyCommPort.WriteBytes(buffer, 2)
            mcuInfo(2).value = ParseBuffer(MyCommPort)

            If ((Not readMem) OrElse checkUsed) Then
                GetFullValues(dataRegisters, DATA_REGS)
            End If
            If readMem Then
                GetFullValues(memory, MEMORY_SIZE)
            End If
            ChangeDisplay()
            MyCommPort.CloseComm()
        End If
        barProgress.Visible = False
        EnableAction(True)
    End Sub

    Private Sub ReadAllReg()
        'Helper sub to prevent have to send variables when starting thread
        ReadDataStorage(False, False)
    End Sub

    Private Sub ReadAllMem()
        'Helper sub to prevent have to send variables when starting thread
        ReadDataStorage(False, True)
    End Sub

    Private Sub ReadUsed()
        'Helper sub to prevent have to send variables when starting thread
        If (openMapFile = False) Then
            If LoadMapFile() Then
                ReadDataStorage(True, True)
            Else
                EnableAction(True)
            End If
        Else
            ReadDataStorage(True, True)
        End If
    End Sub

    Private Sub btnRunTarget_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles btnRunTarget.Click
        Dim buffer() As Byte = {Asc("g"), 13}

        If (MsgBox("Are you sure you would like to run the target on the MCU?", _
            MsgBoxStyle.YesNo) = MsgBoxResult.Yes) Then
            'Attempt to run target application
            If (MyCommPort.InitializeComm(commPort, True)) Then
                MyCommPort.WriteBytes(buffer, 2)
                MyCommPort.CloseComm()
            End If
        End If
    End Sub

    Private Sub btnReset_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles btnReset.Click

        Dim buffer() As Byte = {Asc("x"), 13}

        If (MsgBox("Are you sure you would like to reset the MCU?", MsgBoxStyle.YesNo) = _
            MsgBoxResult.Yes) Then
            'Attempt to reset MCU
            If (MyCommPort.InitializeComm(commPort, True)) Then
                MyCommPort.WriteBytes(buffer, 2)
                MyCommPort.CloseComm()
            End If
        End If
    End Sub

    Private Sub mnuAbout_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuAbout.Click

        'Show the form with program information
        Dim frm As New frmAbout
        frm.ShowDialog()
    End Sub

    Private Sub btnStop_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles btnStop.Click


        Dim buffer() As Byte = {Asc("r"), Asc(" "), Asc("0"), 13}
        If (MsgBox("Are you sure you would like to stop the target on the MCU?", _
            MsgBoxStyle.YesNo) = MsgBoxResult.Yes) Then
            'Attempt to stop target application
            If (MyCommPort.InitializeComm(commPort, False)) Then
                MyCommPort.WriteBytes(buffer, 4)
                If ((MyCommPort.ReadBytes()).Length > 0) Then
                    MsgBox("Program already stopped.")
                    MyCommPort.CloseComm()
                    Return
                End If

                buffer(0) = 3 'Control-C = 3 in ASCII
                buffer(1) = 13
                MyCommPort.WriteBytes(buffer, 2)

                'Try sending a command to read register 0 and see if anything returned
                'This indicates if mcu successfully stopped
                buffer(0) = Asc("r")
                buffer(1) = Asc(" ")
                MyCommPort.WriteBytes(buffer, 4)

                If ((MyCommPort.ReadBytes()).Length <= 1) Then
                    MsgBox("Could not stop the target." + Chr(10) + Chr(10) + _
                    "NOTE: Must uncomment a define near the beginning" + _
                    Chr(10) + "of debugger.c for this command to work.")
                End If
                MyCommPort.CloseComm()
            End If
        End If
    End Sub

    Private Sub btnReadUsed_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles btnReadUsed.Click

        Dim rdThread As Thread
        'Thread is required so program can be terminated/minimized while reading values
        rdThread = New Thread(AddressOf ReadUsed)
        rdThread.IsBackground = True
        Cursor.Current = System.Windows.Forms.Cursors.WaitCursor

        EnableAction(False) 'Do not allow other actions while reading values
        barProgress.Value = 0
        barProgress.Visible = True
        rdThread.Start()
        Cursor.Current = System.Windows.Forms.Cursors.Default
    End Sub

    Private Sub mnuPrint_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuPrint.Click

        Try
            PrintDialog1.Document = PrintDocument1
            If (PrintDialog1.ShowDialog() = DialogResult.OK) Then
                'if pressed ok then attempt to print
                PrintDocument1.Print()
            End If

        Catch ex As Exception

        End Try
    End Sub

    Private Sub SetupPages(ByVal e As System.Drawing.Printing.PrintPageEventArgs, _
        ByVal heading As String, ByVal numItems As Integer, _
        ByVal valueArray() As memoryStorageType)

        'Use default margin settings to determine how much can print on page
        Dim y As Single = PrintDocument1.DefaultPageSettings.Margins.Top
        Dim new_x As Single = PrintDocument1.DefaultPageSettings.Margins.Top
        Dim x As Single = new_x
        Dim sectionFont As Font = New Font("Arial", 20, FontStyle.Bold)
        Dim textFont As Font = New Font("Arial", 10)
        Dim titleFont As Font = New Font("Arial", 10, FontStyle.Bold)
        Dim deltaY As Single = e.Graphics.MeasureString("test", textFont).Height + 1
        Dim titleDeltaY As Single = e.Graphics.MeasureString("test", titleFont).Height + 1
        Dim sectionDeltaY As Single = e.Graphics.MeasureString("test", sectionFont).Height + 1
        'Determine how much of page can print on
        Dim printWidth As Single = (PrintDocument1.DefaultPageSettings.PaperSize.Width - _
            PrintDocument1.DefaultPageSettings.Margins.Left - _
            PrintDocument1.DefaultPageSettings.Margins.Right)
        Dim startY As Single 'Line begin to print storage info on
        Dim counter As Integer
        Dim tempStr As String

        'Print the type of section in larger font
        e.Graphics.DrawString(heading, sectionFont, Brushes.Black, x + 20, y)
        Dim dateWidth As Single = e.Graphics.MeasureString(dateStr, textFont).Height()
        e.Graphics.DrawString(dateStr, textFont, Brushes.Black, new_x + _
            (printWidth - new_x - dateWidth) / 2, _
            PrintDocument1.DefaultPageSettings.PaperSize.Height - _
            PrintDocument1.DefaultPageSettings.Margins.Bottom)
        y += sectionDeltaY + 2

        'Print headings for each page
        e.Graphics.DrawString("Value", titleFont, Brushes.Black, x + 69, y)
        e.Graphics.DrawString("Variable", titleFont, Brushes.Black, x + 69 + 57, y)
        Select Case printType
            'Each time of printing has different headings
        Case Printing.dataRegisters
                e.Graphics.DrawString("Register", titleFont, Brushes.Black, x, y)
                e.Graphics.DrawString("Type", titleFont, Brushes.Black, x + 69 + 57 + _
                    138 + 103 + 161, y)
            Case Printing.ioRegisters
                e.Graphics.DrawString("Register", titleFont, Brushes.Black, x, y)
                e.Graphics.DrawString("Register", titleFont, Brushes.Black, x + _
                    (57 + 138 + 69), y)
                e.Graphics.DrawString("Value", titleFont, Brushes.Black, x + _
                    69 + (57 + 138 + 69), y)
                e.Graphics.DrawString("Variable", titleFont, Brushes.Black, _
                    x + 69 + 57 + (57 + 138 + 69), y)
            Case Printing.memory
                e.Graphics.DrawString("Address", titleFont, Brushes.Black, x, y)
        End Select

        If (printType <> Printing.ioRegisters) Then
            e.Graphics.DrawString("Byte Location", titleFont, Brushes.Black, _
                x + 69 + 57 + 138, y)
            e.Graphics.DrawString("Full Value", titleFont, Brushes.Black, _
                x + 69 + 57 + 138 + 103, y)
        End If

        startY = y - 1
        y += titleDeltaY + 2
        e.Graphics.DrawLine(New Pen(Color.Black, 2), x, y - 1, x + printWidth, y - 1)

        Select Case printType
            Case Printing.dataRegisters
                'Display all values for selected memory type
                For counter = 0 To (numItems - 1)
                    x = new_x
                    e.Graphics.DrawString(valueArray(counter).location, _
                        textFont, Brushes.Black, x, y)
                    x += 69
                    e.Graphics.DrawString(valueArray(counter).value, _
                        textFont, Brushes.Black, x, y)
                    x += 57
                    e.Graphics.DrawString(valueArray(counter).varName, _
                        textFont, Brushes.Black, x, y)
                    x += 138
                    e.Graphics.DrawString(valueArray(counter).byteLocation, _
                        textFont, Brushes.Black, x, y)
                    x += 103

                    'Check if can fit - if it can't then truncate
                    tempStr = valueArray(counter).fullValue
                    If (tempStr.Length > 20) Then
                        tempStr = tempStr.Remove(19, tempStr.Length - 20)
                    End If
                    e.Graphics.DrawString(tempStr, textFont, Brushes.Black, x, y)

                    x += 161
                    e.Graphics.DrawString(valueArray(counter).extra, _
                        textFont, Brushes.Black, x, y)
                    y += deltaY + 2
                    e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x, y - 1, _
                        new_x + printWidth, y - 1)
                Next counter

                'Print vertical lines marking different columns
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 69 + 57 + _
                    138 + 102, startY, new_x + 69 + 57 + 138 + 102, y - 1)
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 69 + 57 + _
                    138 + 103 + 160, startY, new_x + 69 + 57 + 138 + 103 + 160, y - 1)

                'Print Extra Information
                y += deltaY + 2 + deltaY
                x = new_x
                e.Graphics.DrawString("MCU Info", sectionFont, Brushes.Black, x + 20, y)
                y += sectionDeltaY + 2
                e.Graphics.DrawString("Type of Information", titleFont, Brushes.Black, _
                    x, y)
                e.Graphics.DrawString("Value", titleFont, Brushes.Black, x + 175, y)
                e.Graphics.DrawString("Additional Info", titleFont, Brushes.Black, _
                    x + 175 + 85, y)

                Dim startY2 As Single = y - 1 'Used to record where mcuinfo begins
                y += titleDeltaY + 2
                e.Graphics.DrawLine(New Pen(Color.Black, 2), x, y - 1, x + printWidth, _
                    y - 1)

                For counter = 0 To (INFO_SIZE - 1) 'Display all values for selected memory type
                    e.Graphics.DrawString(mcuInfo(counter).location, textFont, _
                        Brushes.Black, x, y)
                    e.Graphics.DrawString(mcuInfo(counter).value, textFont, _
                        Brushes.Black, x + 175, y)
                    e.Graphics.DrawString(mcuInfo(counter).varName, textFont, _
                        Brushes.Black, x + 175 + 85, y)
                    y += deltaY + 2
                    e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x, y - 1, _
                        new_x + printWidth, y - 1)
                Next counter
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 174, _
                    startY2, new_x + 174, y - 1)
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 175 + 84, _
                    startY2, new_x + 175 + 84, y - 1)
                y = startY2 - deltaY - 2 - deltaY - sectionDeltaY - 1

            Case Printing.ioRegisters
                'Prints two columns of I/O registers per page
                For counter = 0 To (numItems - 1) 'Display all values for selected memory type
                    x = new_x
                    e.Graphics.DrawString(valueArray(counter).location, textFont, _
                        Brushes.Black, x, y)
                    x += 69
                    e.Graphics.DrawString(valueArray(counter).value, textFont, _
                        Brushes.Black, x, y)
                    x += 57
                    e.Graphics.DrawString(valueArray(counter).varName, textFont, _
                        Brushes.Black, x, y)
                    x += 138
                    e.Graphics.DrawString(valueArray(counter + 32).location, textFont, _
                        Brushes.Black, x, y)
                    x += 69
                    e.Graphics.DrawString(valueArray(counter + 32).value, textFont, _
                        Brushes.Black, x, y)
                    x += 57
                    e.Graphics.DrawString(valueArray(counter + 32).varName, textFont, _
                        Brushes.Black, x, y)
                    y += deltaY + 2
                    e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x, y - 1, _
                        new_x + printWidth, y - 1)
                Next counter

                'Print vertical lines marking different columns
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 68 + _
                    (57 + 138 + 69), startY, new_x + 68 + (57 + 138 + 69), y - 1)
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 69 + 56 + _
                    (57 + 138 + 69), startY, new_x + 69 + 56 + (57 + 138 + 69), y - 1)
            Case Printing.memory
                counter = 0
                'Can only fit 40 entries per page
                While ((counter < 40) AndAlso (printIndex < numItems))
                    If (valueArray(printIndex).byteLocation.Length > 0) Then
                        x = new_x
                        e.Graphics.DrawString(valueArray(printIndex).location, _
                            textFont, Brushes.Black, x, y)
                        x += 69
                        e.Graphics.DrawString(valueArray(printIndex).value, _
                            textFont, Brushes.Black, x, y)
                        x += 57
                        e.Graphics.DrawString(valueArray(printIndex).varName, _
                            textFont, Brushes.Black, x, y)
                        x += 138
                        e.Graphics.DrawString(valueArray(printIndex).byteLocation, _
                            textFont, Brushes.Black, x, y)
                        x += 103

                        'Check if can fit - if it can't then truncate
                        tempStr = valueArray(printIndex).fullValue
                        If (tempStr.Length > 32) Then
                            tempStr = tempStr.Remove(31, tempStr.Length - 32)
                        End If
                        e.Graphics.DrawString(tempStr, textFont, Brushes.Black, x, y)

                        y += deltaY + 2
                        e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x, y - 1, new_x + printWidth, y - 1)
                        counter += 1
                    End If
                    printIndex += 1
                End While
                e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 69 + 57 + 138 + 102, _
                    startY, new_x + 69 + 57 + 138 + 102, y - 1)
        End Select

        'column lines that are on all types of printed pages
        e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 68, startY, _
            new_x + 68, y - 1)
        e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 69 + 56, _
            startY, new_x + 69 + 56, y - 1)
        e.Graphics.DrawLine(New Pen(Color.Black, 1), new_x + 69 + 57 + 137, _
            startY, new_x + 69 + 57 + 137, y - 1)
    End Sub

    Private Sub PrintDocument1_PrintPage(ByVal sender As System.Object, _
        ByVal e As System.Drawing.Printing.PrintPageEventArgs) _
        Handles PrintDocument1.PrintPage

        'Begin by printing dataRegisters, then I/O registers, and then all used memory
        Select Case printType
            Case Printing.dataRegisters
                dateStr = Now().ToString
                SetupPages(e, "Data Registers", DATA_REGS, dataRegisters)
                e.HasMorePages = True
                printType = Printing.ioRegisters
            Case Printing.ioRegisters
                SetupPages(e, "I/O Registers", CInt(IO_REGS / 2), ioRegisters)
                If (openMapFile = False) Then
                    printType = Printing.dataRegisters
                    e.HasMorePages = False
                Else
                    e.HasMorePages = True
                    printIndex = 0
                    printType = Printing.memory
                End If
            Case Printing.memory
                SetupPages(e, "Used Memory", MEMORY_SIZE, memory)
                If (printIndex = MEMORY_SIZE) Then
                    printType = Printing.dataRegisters
                    e.HasMorePages = False
                Else
                    e.HasMorePages = True
                End If
        End Select
    End Sub

    Private Sub mnuPreview_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles mnuPreview.Click

        Try
            PrintPreviewDialog1.Text = "Print Preview"
            PrintPreviewDialog1.WindowState = FormWindowState.Maximized
            PrintPreviewDialog1.Document = PrintDocument1
            PrintPreviewDialog1.ShowDialog()
        Catch ex As Exception

        End Try
    End Sub

    Private Sub btnAllReg_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles btnAllReg.Click

        Dim rdThread As Thread
        'Thread is required so program can be terminated/minimized while reading values
        rdThread = New Thread(AddressOf ReadAllReg)
        rdThread.IsBackground = True
        Cursor.Current = System.Windows.Forms.Cursors.WaitCursor

        EnableAction(False)
        barProgress.Value = 0
        barProgress.Visible = True
        rdThread.Start()

        Cursor.Current = System.Windows.Forms.Cursors.Default
    End Sub

    Private Sub btnAllMem_Click(ByVal sender As System.Object, _
        ByVal e As System.EventArgs) Handles btnAllMem.Click

        Dim rdThread As Thread
        'Thread is required so program can be terminated/minimized while reading values
        rdThread = New Thread(AddressOf ReadAllMem)
        rdThread.IsBackground = True
        Cursor.Current = System.Windows.Forms.Cursors.WaitCursor

        EnableAction(False)
        barProgress.Value = 0
        barProgress.Visible = True
        rdThread.Start()

        Cursor.Current = System.Windows.Forms.Cursors.Default
    End Sub

    Private Sub mnuUserMan_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles mnuUserMan.Click

        'Show the form with user manaul
        Dim frm As New frmUser
        frm.ShowDialog()
    End Sub
End Class
