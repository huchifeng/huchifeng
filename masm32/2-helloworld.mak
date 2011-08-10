OUTDIR=./
 
all : test1.exe-cmd
 
test1.exe : test.obj-1
    link /nologo /DEBUG /incremental:no /subsystem:windows /out:$(OUTDIR)/test1.exe $(OUTDIR)/test.obj
 
test1.exe-cmd : test.obj-3
    link /nologo /DEBUG /incremental:no /subsystem:console /out:$(OUTDIR)/test1.exe $(OUTDIR)/test.obj
 
test.obj-1 : 1-helloworld.asm.cmd
    ml /nologo /coff /c /Fo $(OUTDIR)/test.obj 1-helloworld.asm.cmd

test.obj-2: 2-inline-asm.cpp
    cl /EHsc /c  2-inline-asm.cpp /Fo"$(OUTDIR)/test.obj" 

#	Missing "press any key to continue..." when lauching with CTRL+F5 
#	have to manually edit test1.vcxproj; add 
#	<ItemDefinitionGroup><Link><SubSystem>Console</SubSystem></Link></ItemDefinitionGroup>

#   cannot add block comment in makefile ?

test.obj-3 : *.asm *.cpp
#   ml /nologo /coff /c /Zi /Fo $(OUTDIR)/test.obj basic.asm
	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj call-dll-entry-static.asm
#	ml /nologo /coff /c /Zi /Fo $(OUTDIR)/test.obj call-dll-entry-dynamic.asm
#	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj test-masm-high-level.asm
#	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj test-masm-MACRO.asm
#	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj in-memory-dialog.asm
#	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj my-lib-dialog-caller.asm
# 	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj test-DateTime.asm
#   ml /nologo /coff /c /Fo $(OUTDIR)/test.obj test-FPU.asm
#	ml /nologo /coff /c /Fo $(OUTDIR)/test.obj test-vkdebug.asm
#	cl /EHsc /c /Zi 2-inline-asm.cpp /Fo"$(OUTDIR)/test.obj" 
#   添加 /Zi 可以调试, cpp/asm 皆可。


