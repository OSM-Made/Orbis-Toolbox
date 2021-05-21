CC		:=	gcc
AS		:=	gcc
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
RDIR	:=  Resources
IDIRS	:=  -Iinclude -I$(FREEBSD_INCLUDES)
CFLAGS	:=	$(IDIRS) -Os -std=gnu11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -DVERSION_$(VERSION)
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

$(ODIR):
	@mkdir $@

.PHONY: clean

pack_resources:
	cp '../../Orbis Toolbox.sprx' '$(RDIR)/Orbis_Toolbox.bin'
	cp '../Kernel/Kernel.ELF' '$(RDIR)/Kernel.bin'

	ld -r -b binary -o $(ODIR)/Orbis_Toolbox.o '$(RDIR)/Orbis_Toolbox.bin'
	ld -r -b binary -o $(ODIR)/Kernel.o '$(RDIR)/Kernel.bin'

	

clean:
	rm -f $(TARGET) $(ODIR)/*.o