
.SUFFIXES:


ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

TARGET          := mycool3dsmp
BUILD           := build
SOURCES         := source third_party/faad2/libfaad
DATA            := data
INCLUDES        := include third_party/faad2/include third_party/faad2/libfaad
GRAPHICS        := gfx
GFXBUILD        := $(BUILD)
ROMFS           := romfs

APP_TITLE       := My Cool 3DS Music Player!
APP_DESCRIPTION := MCMP!
APP_AUTHOR      := harina
ICON            := meta/icon.png

ARCH    := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS  := -g -Wall -O2 -mword-relocations \
           -ffunction-sections \
           $(ARCH)

CFLAGS  += $(INCLUDE) -D__3DS__ -I$(PORTLIBS)/include/opus -DHAVE_STDINT_H -DHAVE_INTTYPES_H -DHAVE_MEMCPY -DHAVE_STRING_H -DPACKAGE_VERSION=\"2.11.4\" -Wno-unused-but-set-variable -Wno-unused-variable

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS := -g $(ARCH)
LDFLAGS  = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS    := -lcitro2d -lcitro3d -lctru -lopusfile -lopus -lvorbisidec -logg -lm

LIBDIRS := $(PORTLIBS) $(CTRULIB)


ifneq ($(BUILD),$(notdir $(CURDIR)))


export OUTPUT   := $(CURDIR)/$(TARGET)
export TOPDIR   := $(CURDIR)
export APP_ICON := $(TOPDIR)/$(ICON)

export VPATH    := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                   $(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir)) \
                   $(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR  := $(CURDIR)/$(BUILD)

CFILES          := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES        := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES          := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

ifeq ($(strip $(CPPFILES)),)
    export LD   := $(CC)
else
    export LD   := $(CXX)
endif

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export OFILES := $(OFILES_SOURCES)

export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export _3DSXDEPS := $(if $(NO_SMDH),,$(OUTPUT).smdh)

ifeq ($(strip $(NO_SMDH)),)
    export _3DSXFLAGS += --smdh=$(CURDIR)/$(TARGET).smdh
endif

ifneq ($(ROMFS),)
    export _3DSXFLAGS += --romfs=$(CURDIR)/$(ROMFS)
endif

.PHONY: all clean cia

all: $(BUILD) $(DEPSDIR)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	
cia:
	@test -f $(TOPDIR)/$(ICON) || { echo "ERROR: falta $(ICON) (PNG de 48x48)"; exit 1; }
	@test -f $(TOPDIR)/meta/sound.wav || { echo "ERROR: falta meta/sound.wav (WAV PCM 16-bit, max 3 s)"; exit 1; }
	@test -f $(TOPDIR)/meta/app.rsf || { echo "ERROR: falta meta/app.rsf"; exit 1; }
	@rm -f $(OUTPUT).smdh $(BUILD)/banner.bin
	@$(MAKE) --no-print-directory all
	@if [ -f $(TOPDIR)/meta/banner.cgfx ]; then \
		bannertool makebanner -cgfx $(TOPDIR)/meta/banner.cgfx -a $(TOPDIR)/meta/sound.wav -o $(BUILD)/banner.bin; \
	else \
		bannertool makebanner -i $(TOPDIR)/meta/logo.png -a $(TOPDIR)/meta/sound.wav -o $(BUILD)/banner.bin; \
	fi
	@test -f $(BUILD)/banner.bin || { echo "ERROR: bannertool no genero banner.bin"; exit 1; }
	@makerom -f cia -o $(TOPDIR)/$(TARGET).cia -elf $(TOPDIR)/$(TARGET).elf -rsf $(TOPDIR)/meta/app.rsf -icon $(TOPDIR)/$(TARGET).smdh -banner $(BUILD)/banner.bin
	@echo built ... $(TARGET).cia

$(BUILD):
	@mkdir -p $@

ifneq ($(DEPSDIR),$(BUILD))
$(DEPSDIR):
	@mkdir -p $@
endif

clean:
	@echo Limpiando proyecto...
	@rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).cia $(OUTPUT).smdh $(TARGET).elf

else

$(OUTPUT).3dsx  : $(OUTPUT).elf $(_3DSXDEPS)
$(OFILES_SOURCES) : $(HFILES)
$(OUTPUT).elf   : $(OFILES)

-include $(DEPSDIR)/*.d

endif
