OUTDIR=./
 
all : VCHelloWorld.exe-1
 
VCHelloWorld.exe-1 : obj1
    link /nologo /DEBUG /incremental:no /subsystem:windows /out:$(OUTDIR)/test1.exe $(OUTDIR)/VCHelloWorld.obj
 
obj1 : 1-helloworld.asm.cmd
    ml /nologo /coff /c /Fo $(OUTDIR)/VCHelloWorld.obj 1-helloworld.asm.cmd

VCHelloWorld.exe : obj2
    link /nologo /DEBUG /incremental:no /subsystem:console /out:$(OUTDIR)/test1.exe $(OUTDIR)/VCHelloWorld.obj
 
obj2: 2-inline-asm.cpp
    cl /EHsc /c  2-inline-asm.cpp /Fo"$(OUTDIR)/VCHelloWorld.obj" 