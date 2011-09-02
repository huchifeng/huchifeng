$p=get-wmiobject win32_process | where{$_.ProcessName -eq "notepad.exe"}
Write-Host "process ..."
$p[0]
Write-Host "owner ..."
$p[0].getowner()


Write-Host "Press any key to continue ..."

$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")


