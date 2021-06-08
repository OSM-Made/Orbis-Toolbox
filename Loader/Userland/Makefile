CC		:=	gcc
AS		:=	gcc
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
RDIR	:=  Resources
IDIRS	:=  -Iinclude -I$(FREEBSD_INCLUDES)
CFLAGS	:=	$(IDIRS) -O3 -s -w -std=gnu11 -fno-builtin -fno-exceptions -fno-asynchronous-unwind-tables -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -DVERSION_$(VERSION)
SFLAGS	:=	-nostartfiles -nostdlib -march=btver2 -mtune=btver2
LFLAGS	:=	-Xlinker -T linkfile -Wl,--build-id=none -Wl,--gc-sections
CFILES  :=  $(shell find $(SDIR) -name \*.c)
SFILES  :=  $(shell find $(SDIR) -name \*.s)
RFILES  :=  $(shell find $(RDIR) -name \*.bin)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES)) $(patsubst $(RDIR)/%.bin, $(ODIR)/%.o, $(RFILES))

TARGET = $(shell basename "$(CURDIR)").elf

$(TARGET): $(ODIR) $(OBJS)
	$(CC) crt0.s $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR)/%.o: $(RDIR)/%.bin
	mkdir -p $(dir $@)
	ld -r -b binary -o $@ $<
#	objdump -t $@

$(ODIR):
	@mkdir $@

.PHONY: clean

pack_resources:
	cp '../../Orbis Toolbox.sprx' '$(RDIR)/Orbis_Toolbox.bin'
	cp '../Kernel/Kernel.ELF' '$(RDIR)/Kernel.bin'

clean:
	rm -rf -v $(TARGET) $(ODIR)/*