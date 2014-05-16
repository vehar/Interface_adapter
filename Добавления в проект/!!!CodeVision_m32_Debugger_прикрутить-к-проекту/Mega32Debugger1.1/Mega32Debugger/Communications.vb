Option Strict On
Option Explicit On 

Public Class SerialCommuication
    Public Structure DCB
        Public DCBlength As Int32   'length of structure in bytes
        'Bps-110,300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200,128000,256000
        Public BaudRate As Int32
        'fBinary:1; fParity:1; fOutxCtsFlow:1; fOutxDsrFlow:1; fDtrControl:2; 
        'fDsrSensitivity:1; fTXContinueOnXoff:1; fOutX:1; fInX:1; fErrorChar:1; fNull:1; 
        'fRtsControl:2; fAbortOnError:1;fDummy2:17;
        Public fBitFields As Int32
        Public wReserved As Int16   'Reserverd, must be zero
        Public XonLim As Int16      'Max bytes in buffer before flow control prevents tx
        Public XoffLim As Int16     'Max bytes in buffer before flow control allows tx
        Public ByteSize As Byte     'Number of bits in bytes tx/rcvd
        Public Parity As Byte       '0=No,1=Odd,2=Even,3=Mark,4=Space
        Public StopBits As Byte     '0=0,1=1.5,2=2
        Public XonChar As Byte      'Value of Xon character
        Public XoffChar As Byte     'Value of Xoff character
        Public ErrorChar As Byte    'Character used to replace bytes rcvd w/ parity error
        Public EofChar As Byte      'End of data character 
        Public EvtChar As Byte      'End of event character
        Public wReserved1 As Int16  'Reserved; Do Not Use
    End Structure

    Public Structure COMMTIMEOUTS
        Public ReadIntervalTimeout As Int32
        Public ReadTotalTimeoutMultiplier As Int32
        Public ReadTotalTimeoutConstant As Int32
        Public WriteTotalTimeoutMultiplier As Int32
        Public WriteTotalTimeoutConstant As Int32
    End Structure

    'Reference unmanaged functions in kernel32
    Public Declare Auto Function CreateFile Lib "kernel32.dll" _
       (ByVal lpFileName As String, ByVal dwDesiredAccess As Int32, _
          ByVal dwShareMode As Int32, ByVal lpSecurityAttributes As IntPtr, _
             ByVal dwCreationDisposition As Int32, ByVal dwFlagsAndAttributes As Int32, _
                ByVal hTemplateFile As IntPtr) As IntPtr

    Public Declare Auto Function GetCommState Lib "kernel32.dll" (ByVal nCid As IntPtr, _
       ByRef lpDCB As DCB) As Boolean

    Public Declare Auto Function SetCommState Lib "kernel32.dll" (ByVal nCid As IntPtr, _
       ByRef lpDCB As DCB) As Boolean

    Public Declare Auto Function GetCommTimeouts Lib "kernel32.dll" (ByVal hFile As IntPtr, _
       ByRef lpCommTimeouts As COMMTIMEOUTS) As Boolean

    Public Declare Auto Function SetCommTimeouts Lib "kernel32.dll" (ByVal hFile As IntPtr, _
       ByRef lpCommTimeouts As COMMTIMEOUTS) As Boolean

    Public Declare Auto Function WriteFile Lib "kernel32.dll" (ByVal hFile As IntPtr, _
       ByVal lpBuffer As Byte(), ByVal nNumberOfBytesToWrite As Int32, _
          ByRef lpNumberOfBytesWritten As Int32, ByVal lpOverlapped As IntPtr) As Boolean

    Public Declare Auto Function ReadFile Lib "kernel32.dll" (ByVal hFile As IntPtr, _
       ByVal lpBuffer As Byte(), ByVal nNumberOfBytesToRead As Int32, _
          ByRef lpNumberOfBytesRead As Int32, ByVal lpOverlapped As IntPtr) As Boolean

    Public Declare Auto Function CloseHandle Lib "kernel32.dll" (ByVal hObject As IntPtr) As Boolean

    Private hSerialPort As IntPtr

    Public Function CloseComm() As Boolean
        ' Release the handle
        Return CloseHandle(hSerialPort)
    End Function

    Public Function InitializeComm(ByVal portNum As Integer, ByVal checkRcv As Boolean) As Boolean
        Dim success As Boolean
        Dim MyDCB As DCB
        Dim MyCommTimeouts As COMMTIMEOUTS
        Dim commPort As String = "COM"

        'Communications settings
        Const GENERIC_READ As Integer = &H80000000
        Const GENERIC_WRITE As Integer = &H40000000
        Const OPEN_EXISTING As Integer = 3
        Const FILE_ATTRIBUTE_NORMAL As Integer = &H80
        Const NO_PARITY As Integer = 0
        Const ONE_STOP_BIT As Integer = 0
        Const CARRIAGE_RETURN As Integer = 13

        Try
            ' Obtain a handle to the serial port.
            hSerialPort = CreateFile(commPort + CStr(portNum), GENERIC_READ Or GENERIC_WRITE, 0, _
                IntPtr.Zero, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, IntPtr.Zero)

            ' Verify that the obtained handle is valid.
            If hSerialPort.ToInt32 = -1 Then
                Throw New Exception("Unable to obtain a handle to the COM1 port")
            End If

            ' Retrieve the current control settings.
            success = GetCommState(hSerialPort, MyDCB)
            If success = False Then
                Throw New Exception("Unable to retrieve the current control settings")
            End If

            'Values used by the MCU in the debugger
            'MyDCB.BaudRate = 9600
            MyDCB.BaudRate = 19200
            MyDCB.ByteSize = 8
            MyDCB.Parity = NO_PARITY
            MyDCB.StopBits = ONE_STOP_BIT

            ' Reconfigure COM based on the properties of MyDCB.
            success = SetCommState(hSerialPort, MyDCB)
            If success = False Then
                Throw New Exception("Unable to reconfigure COM1")
            End If

            ' Retrieve the current time-out settings.
            success = GetCommTimeouts(hSerialPort, MyCommTimeouts)
            If success = False Then
                Throw New Exception("Unable to retrieve current time-out settings")
            End If

            'Best serial settings found in practice
            'Max time allowed between two bytes in milliseconds
            MyCommTimeouts.ReadIntervalTimeout = 13
            'MyCommTimeouts.ReadTotalTimeoutConstant = 35 for 9600
            'Total time out period for read operations
            MyCommTimeouts.ReadTotalTimeoutConstant = 20
            MyCommTimeouts.ReadTotalTimeoutMultiplier = 0
            MyCommTimeouts.WriteTotalTimeoutConstant = 0
            MyCommTimeouts.WriteTotalTimeoutMultiplier = 0

            ' Reconfigure the time-out settings, based on the properties of MyCommTimeouts.
            success = SetCommTimeouts(hSerialPort, MyCommTimeouts)
            If success = False Then
                Throw New Exception("Unable to reconfigure the time-out settings")
            End If

            If (checkRcv) Then
                'Test if the MCU is capable of receiving and replying to a message, 
                'i.e. set to debug mode
                Dim buffer() As Byte = {Asc("r"), Asc(" "), Asc("0"), 13}
                WriteBytes(buffer, 4)

                success = (ReadBytes()).Length > 1
                If success = False Then
                    Throw New Exception("Unable to receive a message")
                End If
            End If

        Catch ex As Exception
            MsgBox("Unable to connect to establish serial connection", MsgBoxStyle.OKOnly)
        End Try

        If (Not success) Then
            CloseComm()
        End If

        Return success
    End Function

    Public Function WriteBytes(ByVal buffer() As Byte, _
        ByVal bufferLength As Integer) As Boolean

        Dim success As Boolean
        Dim bytesWritten As Int32

        ' Write data to Serial Connection
        success = WriteFile(hSerialPort, buffer, bufferLength, bytesWritten, IntPtr.Zero)

        'Check if sent all bytes
        success = success And (bufferLength = bytesWritten)

        If success = False Then
            MsgBox("Unable to communicate with device", MsgBoxStyle.OKOnly)
        End If

        Return success
    End Function
    Public Function ReadBytes() As Byte()
        Dim buffer(50) As Byte
        Dim outBuffer() As Byte
        Dim success As Boolean = True
        Dim bytesRead As Integer = 1

        'determine if able to read from serial file
        success = ReadFile(hSerialPort, buffer, 50, bytesRead, IntPtr.Zero)

        If success = False Then
            MsgBox("Unable to communicate with device", MsgBoxStyle.OKOnly)
        End If

        ReDim outBuffer(bytesRead)
        buffer.Copy(buffer, outBuffer, bytesRead)

        Return outBuffer
    End Function
End Class
