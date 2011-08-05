OUTDIR=./
 
all : test1.exe
 
test1.exe : test.obj-2
    link /nologo /DEBUG /incremental:no /subsystem:windows /out:$(OUTDIR)/test1.exe $(OUTDIR)/test.obj
 
test1.exe-cmd : test.obj-1
    link /nologo /DEBUG /incremental:no /subsystem:console /out:$(OUTDIR)/test1.exe $(OUTDIR)/test.obj
 
test.obj-1: 2-inline-asm.cpp
    cl /EHsc /c  2-inline-asm.cpp /Fo"$(OUTDIR)/test.obj" 

test.obj-2 : basic.asm
    ml /nologo /coff /c /Fo $(OUTDIR)/test.obj basic.asm

