path %ProgramFiles%\gs\gs8.71\bin;%path%
path %ProgramFiles%\gs\gs8.71\lib;%path%
rem start ps2pdf.bat gscjk_ag.ps tmp.pdf
rem gswin32c --help

gswin32c -sDEVICE=pdfwrite -sOutputFile=tmp1.pdf -dBATCH -q -dNOPAUSE gscjk_ag.ps
gswin32c -sDEVICE=pdfwrite -sOutputFile=tmp2.pdf -dBATCH -q -dNOPAUSE image-qa.ps
pause
