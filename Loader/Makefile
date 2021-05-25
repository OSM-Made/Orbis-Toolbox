SHELL=/bin/bash
PDIR	:=	../Icons
PFILES  :=  $(shell find $(PDIR) -name \*.png)

none:
	printf "\e[1m\e[31m[Please Choose a firmware!! EG. \"make 505\" \033[37m\n"

build:
	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C ShellCode
	+$(MAKE) -C Kernel clean
	+$(MAKE) -C Kernel
	+$(MAKE) -C Userland clean
	+$(MAKE) -C Userland pack_resources
	+$(MAKE) -C Userland

	objcopy -O binary Userland/Userland.elf ../Orbis-Toolbox-$(VERSION).bin

	printf "\e[1m\e[32m[%0.2f Build Sucess!]\033[37m\n" "$$(($(VERSION) * 1))e-2"

505:
	+$(MAKE) build VERSION=505

672:
	+$(MAKE) build VERSION=672

702:
	+$(MAKE) build VERSION=702

755:
	+$(MAKE) build VERSION=755

all:
	+$(MAKE) build VERSION=505
	+$(MAKE) build VERSION=672
	+$(MAKE) build VERSION=702
	+$(MAKE) build VERSION=755

	@/bin/echo -e "\e[1m\e[32m[All Build Sucess!]\033[37m"

clean:
	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C Kernel clean
	+$(MAKE) -C Userland clean
