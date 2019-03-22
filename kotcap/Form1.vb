

Imports System.ComponentModel

Public Class Form1
    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        Dim ports As String() = System.IO.Ports.SerialPort.GetPortNames()
        Dim port As String
        For Each port In ports
            ComboBox1.Items.Add(port)
        Next port

        'ComboBox1.SelectedIndex = 0
        ' Array.Sort(ports)
        ' ComboBox1.Items.AddRange(ports)

        If ComboBox1.Items.Count >= 2 Then ComboBox1.SelectedIndex = 1
        If ComboBox1.Items.Count = 1 Then ComboBox1.SelectedIndex = 0

        GroupBox1.Enabled = False

        pos = 0

    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click

        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = ComboBox1.SelectedItem
            SerialPort1.Open()
            Button1.Enabled = False
            ComboBox1.Enabled = False
        End If


        SerialPort1.Write(180)
        pos = 180

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        SerialPort1.Close()
        Button1.Enabled = True
        ComboBox1.Enabled = True
        GroupBox1.Enabled = False
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = ComboBox1.SelectedItem
            SerialPort1.Open()
            Button1.Enabled = False
            ComboBox1.Enabled = False
        End If
        GroupBox1.Enabled = True
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click

        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = ComboBox1.SelectedItem
            SerialPort1.Open()
            Button1.Enabled = False
            ComboBox1.Enabled = False
        End If

        SerialPort1.Write(0)
        pos = 0
    End Sub

    Private Sub Button5_Click(sender As Object, e As EventArgs) Handles Button5.Click
        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = ComboBox1.SelectedItem
            SerialPort1.Open()
            Button1.Enabled = False
            ComboBox1.Enabled = False
        End If
        SerialPort1.Write(NumericUpDown1.Text)
        pos = NumericUpDown1.Text
    End Sub

    Private Sub Button6_Click(sender As Object, e As EventArgs) Handles Button6.Click
        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = ComboBox1.SelectedItem
            SerialPort1.Open()
            Button1.Enabled = False
            ComboBox1.Enabled = False
        End If


        SerialPort1.Write(90)
        pos = 180
    End Sub
End Class
