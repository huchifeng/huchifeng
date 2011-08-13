path %ProgramFiles%\gs\gs8.71\bin;%path%
path %ProgramFiles%\gs\gs8.71\lib;%path%
start ps2pdf.bat gscjk_ag.ps tmp.pdf
gswin32c --help

gswin32c -sDEVICE=pdfwrite -sOutputFile=tmp2.pdf -dBATCH -q -dNOPAUSE gscjk_ag.ps
pause