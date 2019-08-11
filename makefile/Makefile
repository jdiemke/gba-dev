# Basic GBA Makefile Rules
#
# In order to use this makefile include it into your projects makefile
# and add the ROM_NAME and SOURCE_FILES variables.
#
# Example:
#
# ROM_NAME        = demo
# SOURCE_FILES    = main.cpp mode3.cpp
# include ../makefile/Makefile

PATH := $(DEVKITARM)/bin:$(PATH)

SOURCE			= src
BUILD			= build

OBJECT_FILES	= $(SOURCE_FILES:.cpp=.o)

CC      		= arm-none-eabi-gcc
OBJCOPY			= arm-none-eabi-objcopy

CFLAGS			= -mthumb -mthumb-interwork -O2 -Wall -fno-strict-aliasing
LDFLAGS			= -specs=gba.specs -mthumb -mthumb-interwork
LIBRARIES		= -lm

$(ROM_NAME).gba : $(ROM_NAME).elf
	$(OBJCOPY) -v -O binary $(BUILD)/$(ROM_NAME).elf $(BUILD)/$(ROM_NAME).gba
	gbafix $(BUILD)/$(ROM_NAME).gba

$(ROM_NAME).elf : $(OBJECT_FILES)
	$(CC) $(LDFLAGS) $(addprefix $(BUILD)/,$(OBJECT_FILES)) $(LIBRARIES) -o $(BUILD)/$(ROM_NAME).elf 

%.o : $(SOURCE)/%.cpp | createdir
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@

createdir :
	mkdir -p $(BUILD)

.PHONY : clean

clean :
	rm -rf build
