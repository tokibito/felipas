unit felipas;

interface

uses
  Windows, SysUtils;

const
  FELICA_IDM_L = 16;
  FELICA_PMM_L = 32;

type
  TFelicaCardInfo = record
    idx: String;
    pmx: String;
  end;

function _LoadFelicaLibrary: Integer; cdecl; external 'felipas.dll';
function _FreeFelicaLibrary: Integer; cdecl; external 'felipas.dll';
function _PollingOnce(idm: PByte; pmm: PByte): Integer; cdecl; external 'felipas.dll';
function _SetCallback(hWindow: HWND): Integer; cdecl; external 'felipas.dll';
function _StartPolling: Integer; cdecl; external 'felipas.dll';
function _StopPolling: Integer; cdecl; external 'felipas.dll';
function PollingOnce: TFelicaCardInfo;

var
  WM_CARDFIND: Word;
  WM_CARDLOSE: Word;

implementation

function PollingOnce: TFelicaCardInfo;
var
  idm: PByte;
  pmm: PByte;
  idHead: PByte;
  xbuf: String;
  i: Integer;
  success: Integer;
begin
  GetMem(idm, FELICA_IDM_L);
  GetMem(pmm, FELICA_PMM_L);
  try
    success := _PollingOnce(idm, pmm);

    if success < 0 then
    begin
      Result.idx := '';
      Result.pmx := '';
      Exit;
    end;

    xbuf := '';
    idHead := idm;
    for i := 0 to FELICA_IDM_L div 2 - 1 do
    begin
      xbuf := IntToHex(Byte(idHead^), 2) + xbuf;
      Inc(idHead);
    end; 
    Result.idx := xbuf;
    xbuf := '';
    idHead := pmm;
    for i := 0 to FELICA_PMM_L div 2 - 1 do
    begin
      xbuf := IntToHex(Byte(idHead^), 2) + xbuf;
      Inc(idHead);
    end; 
    Result.pmx := xbuf;
  finally
    FreeMem(idm);
    FreeMem(pmm);
  end;

end;

initialization

WM_CARDFIND := RegisterWindowMessage('CardFind');
WM_CARDLOSE := RegisterWindowMessage('CardLose');

end.
