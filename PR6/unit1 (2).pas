unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    FileList: TMemo;
    ScanDir: TButton;
    DirList: TMemo;
    procedure FormCreate(Sender: TObject);
    procedure ScanDirClick(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;

  TScanDir = class(TThread)
    private
       _newPath : string;

    public
      procedure SetPath(path : string);

    protected
       procedure Execute(); override;
       function isPalindromString(str : string) : boolean;

  end;

var
  Form1: TForm1;
  sd : TScanDir;

implementation

{$R *.lfm}

{Thread}

procedure TScanDir.Execute();
var hFolder : TSearchRec;
    nt : TScanDir;
begin
      SetCurrentDir(self._newPath);

      if(FindFirst('*', faAnyFile, hFolder) = 0) then
      begin
          repeat

             if((hFolder.attr and faDirectory) = faDirectory) then
             begin
                 if((hFolder.Name <> '.') and (hFolder.Name <> '..')) then
                 begin
                      nt := TScanDir.Create(false);
                      nt.SetPath(self._newPath + '\' + hFolder.Name);
                      form1.DirList.Append('New path: ' + self._newPath + '\' + hFolder.Name);
                 end;
             end else begin

                  if(self.isPalindromString(hFolder.Name)) then
                  begin
                       form1.FileList.Append('Delete file: ' + self._newPath + '\' + hFolder.Name);
                  end;
             end;
          until FindNext(hFolder) <> 0;

        FindClose(hFolder);
      end;
end;

procedure TScanDir.SetPath(path : string);
begin
     self._newPath := path;
end;

function TScanDir.isPalindromString(str : string) : boolean;
var i, j : integer;
begin
     i := 1;
     j := length(str);
     while(str[i] <> str[j]) do
     begin

          inc(i);
          dec(j);
     end;

     isPalindromString := true;
end;

{ TForm1 }

procedure TForm1.ScanDirClick(Sender: TObject);
begin
     sd := TScanDir.Create(false);
    sd.SetPath('C:\Test');
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  DirList.Clear();
  FileList.Clear();
end;

end.

