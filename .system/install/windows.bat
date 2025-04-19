$HOME = git rev-parse --show-toplevel
$scriptPath = "$HOME/scripts/auto-start.go"

$startupFolder = [System.Environment]::GetFolderPath('Startup')
$shortcut = "$startupFolder\script.lnk"

$ws = New-Object -ComObject WScript.Shell
$lnk = $ws.CreateShortcut($shortcut)
$lnk.TargetPath = $scriptPath
$lnk.Save()
