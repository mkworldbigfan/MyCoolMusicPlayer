
.SUFFIXES:


ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

TARGET          := mycool3dsmp
BUILD           := build
SOURCES         := source
DATA            := data
INCLUDES        := include
GRAPHICS        := gfx
GFXBUILD        := $(BUILD)
ROMFS           := romfs

APP_TITLE       := My Cool 3DS Music Player!
APP_DESCRIPTION := hiiii
APP_AUTHOR      := harina
ICON            := $(CURDIR)/meta/icon.png

ARCH    := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS  := -g -Wall -O2 -mword-relocations \
           -ffunction-sections \
           $(ARCH)

CFLAGS  += $(INCLUDE) -D__3DS__ -I$(PORTLIBS)/include/opus

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11

ASFLAGS := -g $(ARCH)
LDFLAGS  = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS    := -lcitro2d -lcitro3d -lctru -lopusfile -lopus -lvorbisidec -logg -lm

LIBDIRS := $(PORTLIBS) $(CTRULIB)


ifneq ($(BUILD),$(notdir $(CURDIR)))


export OUTPUT   := $(CURDIR)/$(TARGET)
export TOPDIR   := $(CURDIR)

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

cia: all
	@if [ -f $(CURDIR)/meta/banner.cgfx ]; then \
		bannertool makebanner -cgfx $(CURDIR)/meta/banner.cgfx -a $(CURDIR)/meta/sound.wav -o $(BUILD)/banner.bin; \
	else \
		bannertool makebanner -i $(CURDIR)/meta/logo.png -a $(CURDIR)/meta/sound.wav -o $(BUILD)/banner.bin; \
	fi
	@makerom -f cia -o $(CURDIR)/$(TARGET).cia -elf $(CURDIR)/$(TARGET).elf -rsf $(CURDIR)/meta/app.rsf -icon $(CURDIR)/$(TARGET).smdh -banner $(BUILD)/banner.bin

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
