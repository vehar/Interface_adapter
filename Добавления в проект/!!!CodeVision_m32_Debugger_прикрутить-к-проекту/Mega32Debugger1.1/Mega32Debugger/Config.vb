Option Strict On
Option Explicit On 

Imports System.IO
Imports System.Xml

Public Class ConfigInfo
    Private defaultIOVarNames() As String = {"TWBR", "TWSR", "TWAR", "TWDR", "ADCL", "ADCH", "ADCSRA", "ADMUX", "ACSR", "UBRRL", "UCSRB", "UCSRA", "UDR", "SPCR", "SPSR", "SPDR", "PIND", "DDRD", "PORTD", "PINC", "DDRC", "PORTC", "PINB", "DDRB", "PORTB", "PINA", "DDRA", "PORTA", "EECR", "EEDR", "EEARL", "EEARH", "UBRRH", "WDTCR", "ASSR", "OCR2", "TCNT2", "TCCR2", "ICR1L", "ICR1H", "OCR1BL", "OCR1BH", "OCR1AL", "OCR1AH", "TCNT1L", "TCNT1H", "TCCR1B", "TCCR1A", "SFIOR", "OSCCAL", "TCNT0", "TCCR0", "MCUCSR", "MCUCR", "TWCR", "SPMCR", "TIFR", "TIMSK", "GIFR", "GICR", "OCR0", "SPL", "SPH", "SREG"}

    Public Const CONFIG_FILE As String = "config.xml"
    Public Sub saveConfigFile(ByVal commPort As Integer)
        Dim m_xmld As XmlDocument
        Dim m_node As XmlNode
        Dim textWriter As XmlTextWriter

        Try
            'Create the XML Document
            m_xmld = New XmlDocument
            'Load the Xml file
            m_xmld.Load(CONFIG_FILE)

            textWriter = New XmlTextWriter(CONFIG_FILE, Nothing)

            'Change the comm port value
            m_node = m_xmld.SelectSingleNode("/debugger/debugger/commPort")
            m_node.ChildNodes.Item(0).InnerText() = CStr(commPort)

            m_xmld.Save(textWriter)
            textWriter.Close()
        Catch e As Exception
            'MsgBox(e.ToString)
        End Try
    End Sub


    Public Sub readIORegisterFile(ByRef ioRegisters() As frmDebug.memoryStorageType, _
        ByVal firstTime As Boolean, ByRef commPort As Integer)
        Dim m_xmld As XmlDocument
        Dim m_nodelist As XmlNodeList
        Dim m_node As XmlNode

        Try
            'Create the XML Document
            m_xmld = New XmlDocument
            'Load the Xml file
            m_xmld.Load(CONFIG_FILE)

            'Get the list of name nodes 
            m_node = m_xmld.SelectSingleNode("/debugger/debugger/commPort")
            commPort = CInt(m_node.ChildNodes.Item(0).InnerText())

            'Get the list of name nodes 
            m_nodelist = m_xmld.SelectNodes("/debugger/debugger/ioRegisters/register")

            'Loop through the nodes
            For Each m_node In m_nodelist
                ioRegisters(CInt(m_node.Attributes.GetNamedItem("address").Value)).varName = _
                    m_node.ChildNodes.Item(0).InnerText()
            Next

        Catch e As FileNotFoundException
            If (firstTime) Then
                'write default file and re-read info
                writeDefaultIORegFile()
                readIORegisterFile(ioRegisters, False, commPort)
            End If
        Catch e As Exception
            'MsgBox(e.ToString)
        End Try
    End Sub

    Public Sub writeDefaultIORegFile()
        Dim count As Integer
        ' Create a new file in C:\\ dir
        Dim textWriter As XmlTextWriter = New XmlTextWriter(CONFIG_FILE, Nothing)

        ' Opens the document
        textWriter.WriteStartDocument()

        ' Write first element
        textWriter.WriteStartElement("debugger")
        textWriter.WriteStartElement("debugger")
        textWriter.WriteStartElement("commPort")
        textWriter.WriteString("1")
        textWriter.WriteEndElement()
        textWriter.WriteStartElement("ioRegisters")

        ' Write next element
        For count = 0 To (defaultIOVarNames.GetLength(0) - 1)
            textWriter.WriteStartElement("register", "")
            textWriter.WriteAttributeString("address", CStr(count))
            textWriter.WriteStartElement("name", "")
            textWriter.WriteString(defaultIOVarNames(count))
            textWriter.WriteEndElement()
            textWriter.WriteEndElement()
        Next count
        textWriter.WriteEndElement()
        textWriter.WriteEndElement()
        textWriter.WriteEndElement()

        ' Ends the document.
        textWriter.WriteEndDocument()
        ' close writer
        textWriter.Close()
    End Sub

End Class
