CC        	:=    g++
AS        	:=    g++
OBJCOPY   	:=    objcopy
ODIR    	:=    build
SDIR    	:=    source
IDIRS    	:=    -Iinclude -I$(FREEBSD_INCLUDES)
CFLAGS    	:=    $(IDIRS) -O3 -s -w -std=gnu++11 -fno-builtin -fno-exceptions -fno-asynchronous-unwind-tables -nostartfiles -nostdlib -w -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -mstackrealign -fPIE -DVERSION_$(VERSION)
LFLAGS    	:=    -Xlinker -T Linker -Wl,--build-id=none -mstackrealign -pie
SFLAGS  	:=    -nostartfiles -nostdlib -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small
CFILES    	:=    $(shell find $(SDIR) -name \*.cpp)
SFILES    	:=    $(shell find $(SDIR) -name \*.s)
OBJS    	:=    $(patsubst $(SDIR)/%,$(ODIR)/%,$(CFILES:.cpp=.o)) $(patsubst $(SDIR)/%,$(ODIR)/%,$(SFILES:.s=.o))

TARGET = $(shell basename "$(CURDIR)").ELF

$(TARGET): $(ODIR) $(OBJS)
	$(CC) crt0.s $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o