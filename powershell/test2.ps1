
$a=0;
$c=0;
dir ..\ -recurse -filter *.cpp | foreach{ $a++; $c += (type $_.FullName).Length }
[datetime]::Now
""+$a+" files,"+$c +" lines"

Write-Host "Press any key to continue ..."

$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

Write-Host
Write-Host "A"
Write-Host "B"
Write-Host "C"
Write-Host $x
$x = $host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")

