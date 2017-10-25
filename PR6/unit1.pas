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
    procedure FormDestroy(Sender: TObject);
    procedure ScanDirClick(Sender: TObject);
  private

  public
  end;

  TScanDir = class(TThread)
    private
       _newPath : string;
       currentFilePath : string;
       currentDirPath : string;

    public
      procedure SetPath(path : string);
      procedure AddToDirMemo();
      procedure AddToFileMemo();

    protected
       procedure Execute(); override;
       function isFileForDelete(str : string) : boolean;

  end;

var
  Form1: TForm1;
  sd : TScanDir;
  handles : TList;

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
                      handles.Add(nt);

                      self.currentDirPath := 'New path: ' + self._newPath + '\' + hFolder.Name;
                      Synchronize(@AddToDirMemo);
                 end;
             end else begin

                  if(self.isFileForDelete(hFolder.Name)) then
                  begin
                      self.currentFilePath := 'Delete file: ' + self._newPath + '\' + hFolder.Name;
                      Synchronize(@AddToFileMemo);
                      Deletefile(self._newPath + '\' + hFolder.Name);
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

function TScanDir.isFileForDelete(str : string) : boolean;
var i, j : integer;
    sogl : set of char;
begin
     sogl := ['b', 'c','d','f','g','h','j','k','l','m','n','p','q','r','s','t','v','w','x','z'];

     for i := 0 to length(str) do
     begin
             if (str[i] in sogl) then inc(j);
     end;

     if (j >= 2) then isFileForDelete := true
     else isFileForDelete := false;
end;

{ TForm1 }

procedure TScanDir.AddToDirMemo();
begin
     Form1.DirList.Append(self.currentDirPath);
end;

procedure TScanDir.AddToFileMemo();
begin
     Form1.FileList.Append(self.currentFilePath);
end;

procedure TForm1.ScanDirClick(Sender: TObject);
begin
     sd := TScanDir.Create(false);
    sd.SetPath('C:\Test');
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  DirList.Clear();
  FileList.Clear();
     handles := TList.Create;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  handles.Free;
end;

end.

