    .global LibraryFile
    .type   LibraryFile, @object
    .align  4

LibraryFile:
    .incbin "../../Orbis Toolbox.sprx"
LibraryFileEnd:

    .global LibraryFileSize
    .type   LibraryFileSize, @object
    .align  4

LibraryFileSize:
    .int    LibraryFileEnd - LibraryFile
    