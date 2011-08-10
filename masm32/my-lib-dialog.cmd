

	ml /nologo /coff /c /Fo test.obj my-lib-dialog.asm
	lib test.obj /out:my-lib-dialog.lib
	pause

