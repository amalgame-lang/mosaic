; ═══════════════════════════════════════════════════════════
;  Mosaic — Inno Setup Script
;
;  Produces a Windows .exe installer that drops the Mosaic CLI
;  under <prefix>\bin\ and the livereload daemon + docs under
;  <prefix>\share\mosaic\.
;
;  Most of mosaic is bash scripts (mosaic-routes / build / dev /
;  new) — these REQUIRE a POSIX shell on Windows.  The installer
;  shows a final-page note telling the user to install either
;  Git for Windows (which ships Git Bash) or WSL2.  `mosaic-
;  supervise.exe` is native and works from any cmd.
;
;  Requirements: Inno Setup 6+
;
;  Build:
;    iscc /DMosaicVersion=0.5.4 ^
;         /DMosaicStageDir=..\..\dist\mosaic-0.5.4-windows-x86_64 ^
;         mosaic.iss
;
;  Output: Output\mosaic-<version>-setup.exe
; ═══════════════════════════════════════════════════════════

#ifndef MosaicVersion
  #define MosaicVersion "0.5.4"
#endif
#ifndef MosaicStageDir
  #define MosaicStageDir "..\..\dist\mosaic-" + MosaicVersion + "-windows-x86_64"
#endif

#define AppName      "Mosaic"
#define AppPublisher "Bastien MOUGET"
#define AppURL       "https://github.com/amalgame-lang/mosaic"
#define AppExe       "mosaic-supervise.exe"

[Setup]
AppId={{B2A1C3D4-5E6F-4A7B-8C9D-0E1F2A3B4C5D}
AppName={#AppName}
AppVersion={#MosaicVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#AppURL}
DefaultDirName={autopf}\Mosaic
DefaultGroupName={#AppName}
UninstallDisplayName={#AppName} {#MosaicVersion}
UninstallDisplayIcon={app}\bin\{#AppExe}
Compression=lzma2
SolidCompression=yes
OutputBaseFilename=mosaic-{#MosaicVersion}-setup
LicenseFile={#MosaicStageDir}\share\mosaic\LICENSE
ArchitecturesInstallIn64BitMode=x64
WizardStyle=modern
DisableProgramGroupPage=yes
DisableDirPage=no
DisableReadyPage=no
ShowLanguageDialog=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "addtopath"; Description: "Add {app}\bin to PATH"; GroupDescription: "Integration:"

[Files]
; ── Native binary — works from cmd, PowerShell, Git Bash, WSL ──
Source: "{#MosaicStageDir}\bin\mosaic-supervise.exe"; DestDir: "{app}\bin"; Flags: ignoreversion

; ── Bash scripts — need a POSIX shell at runtime ──
Source: "{#MosaicStageDir}\bin\mosaic";              DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#MosaicStageDir}\bin\mosaic-routes.sh";    DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#MosaicStageDir}\bin\mosaic-build.sh";     DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#MosaicStageDir}\bin\mosaic-dev.sh";       DestDir: "{app}\bin"; Flags: ignoreversion
Source: "{#MosaicStageDir}\bin\mosaic-new.sh";       DestDir: "{app}\bin"; Flags: ignoreversion

; ── Assets (livereload daemon + docs + license) ──
Source: "{#MosaicStageDir}\share\mosaic\*"; DestDir: "{app}\share\mosaic"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType: expandsz; ValueName: "PATH"; \
    ValueData: "{olddata};{app}\bin"; \
    Check: NeedsAddPath('{app}\bin'); Tasks: addtopath

[Run]
; Final page action: open the developer guide in the user's browser.
Filename: "https://github.com/amalgame-lang/mosaic/blob/main/docs/guide.md"; \
    Description: "Open the developer guide"; \
    Flags: shellexec postinstall nowait skipifsilent unchecked

[Code]
function NeedsAddPath(Param: string): Boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'PATH', OrigPath) then
  begin
    Result := True;
    exit;
  end;
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    { Heads-up about the POSIX-shell requirement.  Shown once at the
      end of install, not silently — this is the biggest gotcha for
      Windows users coming from `choco install` of a normal CLI. }
    MsgBox(
      'Mosaic CLI installed at ' + ExpandConstant('{app}\bin\') + #13#13 +
      'mosaic-supervise.exe runs from any cmd / PowerShell.' + #13 +
      '' + #13 +
      'mosaic, mosaic-build.sh, mosaic-dev.sh, mosaic-new.sh, and' + #13 +
      'mosaic-routes.sh are bash scripts and need a POSIX shell:' + #13 +
      '' + #13 +
      '  - Recommended: Git for Windows (ships Git Bash):' + #13 +
      '      https://gitforwindows.org/' + #13 +
      '' + #13 +
      '  - Or WSL2:' + #13 +
      '      wsl --install -d Ubuntu' + #13 +
      '' + #13 +
      'Open Git Bash (or WSL) and run `mosaic version` to verify.',
      mbInformation, MB_OK);
  end;
end;
