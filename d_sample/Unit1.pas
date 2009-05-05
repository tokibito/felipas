unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, XPMan, felipas;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Edit1: TEdit;
    XPManifest1: TXPManifest;
    Button3: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
  private
    procedure FelicaCardFind(CardInfo: TFelicaCardInfo);
    procedure FelicaCardLose;
  public
    procedure DefaultHandler(var Message); override;
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
begin
  _LoadFelicaLibrary;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  _FreeFelicaLibrary;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  Edit1.Text := '';
  _SetCallback(Self.Handle);
  _StartPolling;
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
  _StopPolling;
end;

procedure TForm1.Button3Click(Sender: TObject);
begin
  Edit1.Text := '';
  Edit1.Text := PollingOnce.idx;
end;

procedure TForm1.FelicaCardFind(CardInfo: TFelicaCardInfo);
begin
  { カードを認識したとき }
  Edit1.Text := CardInfo.idx;
end;

procedure TForm1.FelicaCardLose;
begin
  { カードが離脱したとき }
  Edit1.Text := '';
end;

procedure TForm1.DefaultHandler(var Message);
begin
  inherited;
  with TWMNotify(Message) do
  begin
    if TWMNotify(Message).Msg = WM_CARDFIND then
      FelicaCardFind(PollingOnce)
    else if Msg = WM_CARDLOSE then
      FelicaCardLose;
  end;
end;

end.
