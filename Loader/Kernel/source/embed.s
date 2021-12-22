.section .rodata

    .global LoaderShellCode
    .type   LoaderShellCode, @object
    .align  4

LoaderShellCode:
    .incbin "../ShellCode/LoaderShellCode.bin"

LoaderShellCodeEnd:
    .global LoaderShellCodeSize
    .type   LoaderShellCodeSize, @object
    .align  4

LoaderShellCodeSize:
    .int    LoaderShellCodeEnd - LoaderShellCode
    