program i2c_1w_debug;

uses
  Forms,
  form_0 in 'form_0.pas' {Form1},
  add_byte in 'add_byte.pas' {Form2},
  report in 'report.pas' {Form3};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TForm2, Form2);
  Application.CreateForm(TForm3, Form3);
  Application.Run;
end.
