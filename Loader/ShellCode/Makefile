ShellCode = LoaderShellCode.bin
ShellCodes = LoaderShellCode.s

all: clean $(ShellCode)

$(ShellCode):
	nasm -f bin -o $(ShellCode) $(ShellCodes)
	
.PHONY: clean
clean:
	rm -f $(ShellCode) 