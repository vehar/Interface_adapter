unit form_0;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Buttons, Menus, ExtCtrls, clipbrd;

type
  TForm1 = class(TForm)
    GroupBox1: TGroupBox;
    RadioButton1: TRadioButton;
    Button1: TButton;
    Button2: TButton;
    GroupBox2: TGroupBox;
    RadioButton2: TRadioButton;
    Label2: TLabel;
    ListBox1: TListBox;
    ComboBox1: TComboBox;
    GroupBox3: TGroupBox;
    Button4: TButton;
    StaticText1: TStaticText;
    Button5: TButton;
    Button6: TButton;
    SpeedButton1: TSpeedButton;
    SpeedButton2: TSpeedButton;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    SpeedButton6: TSpeedButton;
    GroupBox4: TGroupBox;
    Button7: TButton;
    Label3: TLabel;
    Edit1: TEdit;
    BitBtn1: TBitBtn;
    RadioButton3: TRadioButton;
    RadioButton4: TRadioButton;
    PopupMenu1: TPopupMenu;
    Timer1: TTimer;
    ListBox2: TListBox;
    Button8: TButton;
    SpeedButton7: TSpeedButton;
    Label1: TLabel;
    Timer2: TTimer;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    SpeedButton8: TSpeedButton;
    Label9: TLabel;
    ComboBox2: TComboBox;
    Button3: TButton;
    Label10: TLabel;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    Edit2: TEdit;
    Savelog: TSaveDialog;
    Action_timer: TTimer;
    procedure ComboBox1CloseUp(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Refresh_right_panel(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure Correct_starts;
    procedure Button7Click(Sender: TObject);
    procedure SndCom(b: byte);
    procedure Button5Click(Sender: TObject);
    function  ConvertList: string;
    procedure Button4Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure RadioButton3Click(Sender: TObject);
    procedure RadioButton4Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure BitBtn1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure RadioButton2Click(Sender: TObject);
    function ReadStatus: byte;
    procedure SpeedButton7Click(Sender: TObject);
    procedure SpeedButton8Click(Sender: TObject);
    procedure RadioButton1Click(Sender: TObject);
    procedure ListBox1DblClick(Sender: TObject);
    procedure Edit2Change(Sender: TObject);
    procedure CheckBox2Click(Sender: TObject);
  private
    { Private declarations }
    procedure Loadlist_menu_click(Sender: TObject);
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  h: THandle;

  i2c_actions_list: TStringList;
  OneWire_actions_list: TStringList;
  GlobalDebuggerStatus: byte=255;

  LogFile: TFileName;

const
  modes: array[0..3] of string = ('Ждущий режим','i2c сниффер','Мастер','Мастер, выполнение списка');

implementation

uses add_byte, report;

{$R *.dfm}

procedure TForm1.BitBtn1Click(Sender: TObject);
begin
Timer1.Enabled := false;
SndCom($29);
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
Timer1.Enabled := true;
SndCom($30);
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
SndCom($36);
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
 Clipboard.SetTextBuf(ListBox2.Items.GetText);
end;

procedure TForm1.Button4Click(Sender: TObject);
var
c: byte;
n: cardinal;
begin
c:=$32 + Button4.Tag;
WriteFile(h,c,1,n,nil);
end;

procedure TForm1.Button5Click(Sender: TObject);
var
tmp_str: string;
i: integer;
c,c1: byte;
n: cardinal;
tmp_i: integer;
tmp_bool: boolean;
begin
Timer2.Enabled := false;

if CheckBox1.Checked then
 if SaveLog.Execute then
  LogFile := SaveLog.FileName
 else
  CheckBox1.Checked := false;  


repeat
ReadFile(h,c,1,n,nil);
Sleep(100);
tmp_str := ConvertList;
SndCom($31);
SndCom($34);
c := Length(tmp_Str);
WriteFile(h,c,1,n,nil);
for I := 1 to Length(tmp_Str) do
 begin
  c := Byte(tmp_str[i]);
  WriteFile(h,c,1,n,nil);
 end;
SndCom($35);

Sleep(200);
tmp_i := Length(tmp_Str);
tmp_Str := '';
for I := 0 to tmp_i do
 begin
  ReadFile(h,c,1,n,nil);
  tmp_str := tmp_str + Char(c);
 end;

Report_str := tmp_str;
until Form3.ShowModal<>mrRetry;
Timer2.Enabled := true;
end;

procedure TForm1.Button6Click(Sender: TObject);
begin
if MessageBox(Form1.Handle, 'Уверены?','Очистить список действий',MB_YESNO or MB_ICONQUESTION) = IDYES then
begin
 ListBox1.Clear;
 Refresh_right_panel(nil);
end;
end;

procedure TForm1.Button7Click(Sender: TObject);
var
c: _COMMTIMEOUTS;
dcb: _dcb;
begin
if Button7.Tag=0 then
begin

h := CreateFile(PChar(Combobox2.Text), GENERIC_READ+GENERIC_WRITE,0,nil,OPEN_EXISTING,0,0);

c.ReadIntervalTimeout := 25;
c.ReadTotalTimeoutMultiplier := 10;
c.ReadTotalTimeoutConstant := 10;
c.WriteTotalTimeoutMultiplier := 10;
c.WriteTotalTimeoutConstant := 10;
SetCommTimeouts(h,c);
SetupComm(h,4096,4096);


   GetCommState(h, dcb);
   dcb.BaudRate := StrToIntDef(Edit1.text,9600);     // set the baud rate
   dcb.ByteSize := 8;             // data size, xmit, and rcv
   dcb.Parity := NOPARITY;        // no parity bit
   dcb.StopBits := ONESTOPBIT;    // one stop bit
   SetCommState(h, dcb);

Button7.Tag := 1;
Button7.Caption := 'Закрыть COM порт';
end
else
begin
CloseHandle(h);
Button7.Tag := 0;
Button7.Caption := 'Открыть COM порт';
end;
end;

procedure TForm1.Button8Click(Sender: TObject);
begin
if MessageBox(Form1.Handle, 'Уверены?','Очистить лог',MB_YESNO or MB_ICONQUESTION) = IDYES then
begin
 ListBox2.Clear;
end;
end;

procedure TForm1.CheckBox2Click(Sender: TObject);
begin
Edit2.Enabled := CheckBox2.Checked;
end;

procedure TForm1.ComboBox1CloseUp(Sender: TObject);
var
tmpstr: string;
i: integer;
tmp_bool: boolean;
begin
if ComboBox1.ItemIndex=0 then exit;

if ComboBox1.Text='Start condition' then
begin
 tmp_bool := false;
 for I := 0 to ListBox1.Count - 1 do
  begin
   if (ListBox1.Items.Strings[i]='Старт') or
      (ListBox1.Items.Strings[i]='Повторный старт') then tmp_bool := true;
   if ListBox1.Items.Strings[i]='Стоп' then tmp_bool := false;
  end;
 if tmp_bool then ListBox1.Items.Add('Повторный старт');
 if not tmp_bool then ListBox1.Items.Add('Старт');
end;
if ComboBox1.Text='Stop condition' then ListBox1.Items.Add('Стоп');
if ComboBox1.Text='Передать байт' then
begin
Form2.Caption := 'Передать байт...';
Form2.Edit1.Text := '38';
Form2.Edit1Change(nil);
 if Form2.ShowModal = mrOk then
  if add_byte.Value<256 then
   ListBox1.Items.Add('Передать байт 0x'+IntToHex(Value,2));
end;

if ComboBox1.Text='Прочитать байт, ответить ACK' then ListBox1.Items.Add('Прочитать байт, ответить ACK');

if ComboBox1.Text='Прочитать байт, ответить NACK' then ListBox1.Items.Add('Прочитать байт, ответить NACK');


if ComboBox1.Text='RESET' then ListBox1.Items.Add('RESET');

if ComboBox1.Text='Подключить доп. питание' then ListBox1.Items.Add('Подключить доп. питание');

if ComboBox1.Text='Отключить доп. питание' then ListBox1.Items.Add('Отключить доп. питание');

if ComboBox1.Text='Послать байт' then
begin
Form2.Caption := 'Послать байт...';
Form2.Edit1.Text := '38';
Form2.Edit1Change(nil);
 if Form2.ShowModal = mrOk then
  if add_byte.Value<256 then
   ListBox1.Items.Add('Послать байт 0x'+IntToHex(Value,2));
end;

if ComboBox1.Text='Принять байт' then ListBox1.Items.Add('Принять байт');

ComboBox1.ItemIndex:=0;
end;

function TForm1.ConvertList: string;
var
Error_flag: boolean;
I: integer;
tmp_str: string;
begin
Result := '';


for I := 0 to ListBox1.Count - 1 do
 begin
  tmp_str := ListBox1.Items.Strings[i];
  Error_flag := true;

  if (tmp_str='Старт') or (tmp_str='Повторный старт') then
   begin
    Result := Result + #5 + #0;
    Error_flag := false;
   end;

  if tmp_str='Стоп' then
   begin
    Result := Result + #2 + #0;
    Error_flag := false;
   end;

  if tmp_str='RESET' then
   begin
    Result := Result + #1 + #0;
    Error_flag := false;
   end;

  if tmp_str='Подключить доп. питание' then
   begin
    Result := Result + #$12 + #0;
    Error_flag := false;
   end;

   if tmp_str='Отключить доп. питание' then
   begin
    Result := Result + #$13 + #0;
    Error_flag := false;
   end;

  if Copy(tmp_str,1,15)='Послать байт 0x' then
   begin
    Result := Result + #6 + Char(Byte(StrToInt('$'+Copy(tmp_str,16,2))));
    Error_flag := false;
   end;

  if tmp_str='Принять байт' then
   begin
    Result := Result + #3 + #0;
    Error_flag := false;
   end;

  if Copy(tmp_str,1,16)='Передать байт 0x' then
   begin
    Result := Result + #7 + Char(Byte(StrToInt('$'+Copy(tmp_str,17,2))));
    Error_flag := false;
   end;

  if tmp_str='Прочитать байт, ответить ACK' then
   begin
    Result := Result + #$10 + #0;
    Error_flag := false;
   end;

  if tmp_str='Прочитать байт, ответить NACK' then
   begin
    Result := Result + #$11 + #0;
    Error_flag := false;
   end;

 end; 
end;

procedure TForm1.Correct_starts;
var
i: integer;
tmp_bool: boolean;
begin
tmp_bool := false;
 for I := 0 to ListBox1.Count - 1 do
  begin
   if tmp_bool then
    if (ListBox1.Items.Strings[i]='Старт') then ListBox1.Items.Strings[i]:='Повторный старт';

  if not tmp_bool then
    if (ListBox1.Items.Strings[i]='Повторный старт') then ListBox1.Items.Strings[i]:='Старт';


   if (ListBox1.Items.Strings[i]='Старт') or
      (ListBox1.Items.Strings[i]='Повторный старт') then tmp_bool := true;
   if ListBox1.Items.Strings[i]='Стоп' then tmp_bool := false;
  end;
end;

procedure TForm1.Edit2Change(Sender: TObject);
begin
Action_timer.Interval := StrToIntDef(Edit2.text,5)*1000;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
i2c_actions_list := TStringList.Create;
OneWire_actions_list := TStringList.Create;
end;

procedure TForm1.ListBox1DblClick(Sender: TObject);
var
 tmp_str: string;
begin
if ListBox1.ItemIndex=-1 then exit;

if listbox1.Items.Strings[listbox1.ItemIndex] = 'Прочитать байт, ответить ACK' then
 listbox1.Items.Strings[listbox1.ItemIndex] := 'Прочитать байт, ответить NACK'
else
 if listbox1.Items.Strings[listbox1.ItemIndex] = 'Прочитать байт, ответить NACK' then
  listbox1.Items.Strings[listbox1.ItemIndex] := 'Прочитать байт, ответить ACK';

if listbox1.Items.Strings[listbox1.ItemIndex] = 'Подключить доп. питание' then
 listbox1.Items.Strings[listbox1.ItemIndex] := 'Отключить доп. питание'
else
 if listbox1.Items.Strings[listbox1.ItemIndex] = 'Отключить доп. питание' then
  listbox1.Items.Strings[listbox1.ItemIndex] := 'Подключить доп. питание';

if copy(listbox1.Items.Strings[listbox1.ItemIndex],1,16) = 'Передать байт 0x' then
 begin
 Form2.Caption := 'Передать байт...';
 Form2.Edit1.Text := copy(listbox1.Items.Strings[listbox1.ItemIndex],17,2);
 Form2.Edit1Change(nil);
 if Form2.ShowModal = mrOk then
  if add_byte.Value<256 then
   listbox1.Items.Strings[listbox1.ItemIndex] := 'Передать байт 0x'+IntToHex(Value,2);
 end;

if copy(listbox1.Items.Strings[listbox1.ItemIndex],1,15) = 'Послать байт 0x' then
 begin
 Form2.Caption := 'Послать байт...';
 Form2.Edit1.Text := copy(listbox1.Items.Strings[listbox1.ItemIndex],16,2);
 Form2.Edit1Change(nil);
 if Form2.ShowModal = mrOk then
  if add_byte.Value<256 then
   listbox1.Items.Strings[listbox1.ItemIndex] := 'Послать байт 0x'+IntToHex(Value,2);
 end;
end;

procedure TForm1.Loadlist_menu_click(Sender: TObject);
var
tmp_file_name: TFileName;
begin
tmp_file_name :=ExtractFilePath(Application.ExeName)+'Lists\'
+ TMenuItem(Sender).Caption+'.lst';

if not FileExists(tmp_file_name) then exit;

ListBox1.Items.LoadFromFile(tmp_file_name);
end;

procedure TForm1.RadioButton1Click(Sender: TObject);
begin
BitBtn1.Enabled := true;
Button1.Enabled := true;
Button2.Enabled := true;
Button8.Enabled := true;
ListBox2.Enabled := true;

RadioButton3.Enabled := false;
RadioButton4.Enabled := false;
SpeedButton1.Enabled := false;
SpeedButton2.Enabled := false;
SpeedButton3.Enabled := false;
SpeedButton4.Enabled := false;
SpeedButton5.Enabled := false;
SpeedButton6.Enabled := false;
SpeedButton8.Enabled := false;
ListBox1.Enabled := false;
Combobox1.Enabled := false;
Button5.Enabled := false;
Button6.Enabled := false;
end;

procedure TForm1.RadioButton2Click(Sender: TObject);
begin
Timer1.Enabled := false;

BitBtn1.Enabled := false;
Button1.Enabled := false;
Button2.Enabled := false;
Button8.Enabled := false;
ListBox2.Enabled := false;

RadioButton3.Enabled := true;
RadioButton4.Enabled := true;
SpeedButton1.Enabled := true;
SpeedButton2.Enabled := true;
SpeedButton3.Enabled := true;
SpeedButton4.Enabled := true;
SpeedButton5.Enabled := true;
SpeedButton6.Enabled := true;
SpeedButton8.Enabled := true;
ListBox1.Enabled := true;
Combobox1.Enabled := true;
Button5.Enabled := true;
Button6.Enabled := true;
end;

procedure TForm1.RadioButton3Click(Sender: TObject);
begin
OneWire_actions_list.Clear;
OneWire_actions_list.AddStrings(ListBox1.Items);

ListBox1.Clear;
ListBox1.Items.AddStrings(i2c_actions_list);

Combobox1.Clear;
Combobox1.Items.Add('Добавить действие...');
Combobox1.Items.Add('Start condition');
Combobox1.Items.Add('Stop condition');
Combobox1.Items.Add('Передать байт');
Combobox1.Items.Add('Прочитать байт, ответить ACK');
Combobox1.Items.Add('Прочитать байт, ответить NACK');
Combobox1.ItemIndex := 0;
end;

procedure TForm1.RadioButton4Click(Sender: TObject);
begin
i2c_actions_list.Clear;
i2c_actions_list.AddStrings(ListBox1.Items);

ListBox1.Clear;
ListBox1.Items.AddStrings(OneWire_actions_list);

Combobox1.Clear;
Combobox1.Items.Add('Добавить действие...');
Combobox1.Items.Add('RESET');
Combobox1.Items.Add('Послать байт');
Combobox1.Items.Add('Принять байт');
Combobox1.Items.Add('Подключить доп. питание');
Combobox1.Items.Add('Отключить доп. питание');
Combobox1.ItemIndex := 0;
end;

function TForm1.ReadStatus: byte;
var
c: byte;
n: cardinal;
begin
Result := 255;
c := $37;
WriteFile(h,c,1,n,nil);
Sleep(25);
ReadFile(h,c,1,n,nil);
if n=0 then exit;
Result := c;
end;

procedure TForm1.Refresh_right_panel(Sender: TObject);
begin
if ListBox1.ItemIndex=-1 then
begin
 SpeedButton4.Enabled := false;
 SpeedButton5.Enabled := false;
 SpeedButton6.Enabled := false;
end
else
begin
 SpeedButton4.Enabled := true;
 SpeedButton5.Enabled := true;
 SpeedButton6.Enabled := true;
end;
end;

procedure TForm1.SndCom(b: byte);
var
c: byte;
n: cardinal;
begin
 WriteFile(h,b,1,n,nil);
end;

procedure TForm1.SpeedButton2Click(Sender: TObject);
var
sr: TSearchRec;
tmp_i: TMenuItem;
p: TPoint;
begin
if not DirectoryExists(ExtractFilePath(Application.ExeName)+'Lists') then exit;

if FindFirst(ExtractFilePath(Application.ExeName)+'Lists\*.lst', faAnyFile, sr) = 0 then
  begin
   PopupMenu1.Items.Clear;
   repeat
    tmp_i := TMenuItem.Create(nil);
    tmp_i.Caption := Copy(sr.Name,1,Length(sr.Name)-4);
    tmp_i.OnClick := LoadList_Menu_click;
    PopupMenu1.Items.Add(tmp_i);
   until FindNext(sr) <> 0;
   GetCursorPos(p);
   PopupMenu1.Popup(p.X,p.y);
  end;
SysUtils.FindClose(sr);
end;

procedure TForm1.SpeedButton3Click(Sender: TObject);
var
tmp_name: string;
begin
tmp_name := InputBox('Сохранить список', 'Название списка:','');

if tmp_name<>'' then
ListBox1.Items.SaveToFile(ExtractFilePath(Application.ExeName)+'Lists\'
+tmp_name+'.lst');
end;

procedure TForm1.SpeedButton4Click(Sender: TObject);
begin
if ListBox1.ItemIndex = 0 then exit;
Listbox1.Items.Exchange(ListBox1.ItemIndex,ListBox1.ItemIndex-1);
Correct_starts;
end;

procedure TForm1.SpeedButton5Click(Sender: TObject);
begin
if ListBox1.ItemIndex = ListBox1.Count-1 then exit;
Listbox1.Items.Exchange(ListBox1.ItemIndex,ListBox1.ItemIndex+1);
Correct_starts;
end;

procedure TForm1.SpeedButton6Click(Sender: TObject);
begin
ListBox1.DeleteSelected;
Correct_starts;
Refresh_right_panel(nil);
end;

procedure TForm1.SpeedButton7Click(Sender: TObject);
var
tmp_B: byte;
begin
if Timer1.Enabled then exit;
GlobalDebuggerStatus := ReadStatus;

if GlobalDebuggerStatus=255 then
begin
 Label1.Font.Color := clRed;
 Label1.Caption := 'НЕ ПОДКЛЮЧЕН';

 Label4.Caption := '';

 Button4.Enabled := false;

 Label8.Caption := '';
 Label6.Caption := '';
end
else
begin
 Label1.Font.Color := clGreen;
 Label1.Caption := 'ПОДКЛЮЧЕН';

 tmp_B := GlobalDebuggerStatus shr 1;
 tmp_B := tmp_B and 3;
 Label4.Caption := modes[tmp_b];

 Button4.Enabled := true;
 if GlobalDebuggerStatus and 1 = 1 then
  begin
   Button4.Caption := 'Отключить';
   Button4.Tag := 1;
  end
 else
  begin
   Button4.Caption := 'Подключить';
   Button4.Tag := 0;
  end;

 Label8.Caption := 'LOW';
 Label6.Caption := 'LOW';
 Label8.Font.Color := clRed;
 Label6.Font.Color := clRed;
 if GlobalDebuggerStatus and 8 = 8 then
  begin
   Label8.Caption := 'HIGH';
   Label8.Font.Color := clGreen;
  end;
 if GlobalDebuggerStatus and 16 = 16 then
  begin
   Label6.Caption := 'HIGH';
   Label6.Font.Color := clGreen;
  end;

end;

end;

procedure TForm1.SpeedButton8Click(Sender: TObject);
begin
 Clipboard.SetTextBuf(ListBox1.Items.GetText);
end;

procedure TForm1.Timer1Timer(Sender: TObject);
var
c,c1: byte;
tmr: word;
n: cardinal;
tmr_string: string;
text_string: string;
begin
ReadFile(h,c,1,n,nil);
if n=0 then exit;
sleep(25);
ReadFile(h,c1,1,n,nil);
if n=0 then exit;
ReadFile(h,tmr,2,n,nil);
if n=0 then exit;

tmr_string := IntToStr(tmr)+'ms';
text_string := '';

if c=$1 then text_string := 'Старт';
if c=$2 then text_string := 'Стоп';
if c=$4 then text_string := 'Данные: 0x'+IntToHex(c1,2)+', ACK';
if c=$C then text_string := 'Данные: 0x'+IntToHex(c1,2)+', NACK';

if text_string<>'' then ListBox2.Items.Add(tmr_string + ' > ' + text_string);
end;

end.
