OUTDIR=./
 
all : VCHelloWorld.exe
 
VCHelloWorld.exe : VCHelloWorld.obj
    link /nologo /DEBUG /incremental:no /subsystem:windows /out:$(OUTDIR)/test1.exe $(OUTDIR)/VCHelloWorld.obj
 
VCHelloWorld.obj : 1-helloworld.asm.cmd
    ml /nologo /coff /c /Fo $(OUTDIR)/VCHelloWorld.obj 1-helloworld.asm.cmd

