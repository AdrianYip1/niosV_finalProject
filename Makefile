INSTALL	:= C:/intelFPGA/QUARTUS_Lite_V23.1

MAIN	:= software/main.c
HDRS	:= address_map.h
SRCS := software/main.c \
        hardware/audio.c \
        hardware/keyboard.c \
        hardware/mouse.c \
        software/graphics/graphics.c \
        software/textinput/getTextFromUser.c \
        software/graphics/textbox/textBoxSprite.c \
		software/graphics/textbox/small_spacebar.c \
        software/graphics/mcWalkingDraw.c \
        software/graphics/map.c \
        software/graphics/predefined_graphics.c \
        software/graphics/tiles.c \
        software/graphics/backdrops/backdrop1_tiles.c \
        software/graphics/sprites/battleIcons/battle_icons.c \
        software/graphics/sprites/battleIcons/battleHp/burned.c \
        software/graphics/sprites/battleIcons/battleHp/caught.c \
        software/graphics/sprites/battleIcons/battleHp/frozen.c \
        software/graphics/sprites/battleIcons/battleHp/myHpEmpty.c \
        software/graphics/sprites/battleIcons/battleHp/oppHpEmpty.c \
        software/graphics/sprites/battleIcons/battleHp/para.c \
        software/graphics/sprites/battleIcons/battleHp/poison.c \
        software/graphics/sprites/battleIcons/battleHp/sleep.c \
        software/graphics/sprites/battleIcons/attackTypes/attackTypeSprites.c \
        software/graphics/sprites/mainCharacter/idle/mcIdle_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingEast_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingNorth_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingNorthEast_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingNorthWest_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingSouth_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingSouthEast_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingSouthWest_frames.c \
        software/graphics/sprites/mainCharacter/walking/mcWalkingWest_frames.c \
        software/graphics/sprites/staticSprite.c \
        software/graphics/sprites/pokemon/charizardSprite.c \
        software/graphics/sprites/pokemon/charmanderSprite.c \
        software/graphics/sprites/pokemon/charmeleonSprite.c \
        software/graphics/sprites/pokemon/pokemonSpriteInit.c \
        software/graphics/sprites/boxSprites/charizardBoxSprite.c \
        software/graphics/sprites/boxSprites/charmanderBoxSprite.c \
        software/graphics/sprites/boxSprites/charmeleonBoxSprite.c \
        software/graphics/sprites/boxSprites/pokemonBoxSpriteInit.c \
        software/graphics/sprites/statEffects/statUp.c \
        software/graphics/sprites/statEffects/statDown.c \
        software/graphics/sprites/battleParty/battlePartySprite.c \
        software/graphics/sprites/playerSprites.c \
        software/graphics/sprites/spacebar/spacebar_frames.c \
        software/graphics/sprites/arrowGif/arrowGif_frames.c \
        software/graphics/titleScreen/titleScreen_frames.c \
        software/graphics/titleScreen/titleScreenDraw.c \
        software/gameplayLogic/map_movement/mcDirectionChecker.c \
		
        software/gameplayLogic/map_movement/mcMoving.c \
        software/se/opening_audio.c \
        software/se/introduction_audio.c \
        software/se/battle_audio.c \
        software/se/map_audio.c \
        software/se/plink_audio.c \
        software/graphics/sprites/battleUIBackground/battleUIBackgroundSprite.c

SHELL	:= cmd.exe

# DE1-SoC
JTAG_INDEX_SoC	:= 2

# The following variables are set based on the value of the INSTALL variable
COMPILER		:= $(INSTALL)/fpgacademy/AMP/cygwin64/home/compiler/bin
BASH			:= $(INSTALL)/fpgacademy/AMP/cygwin64/bin/bash --noprofile -norc -c 
HW_DE1-SoC		:= "$(INSTALL)/fpgacademy/Computer_Systems/DE1-SoC/DE1-SoC_Computer/niosVg/DE1_SoC_Computer.sof"
HW_DE10-Lite	:= "$(INSTALL)/fpgacademy/Computer_Systems/DE10-Lite/DE10-Lite_Computer/niosVg/DE10_Lite_Computer.sof"

# for Quartus programmer (two possibilities exist for the path)
export PATH := $(INSTALL)/quartus/bin64/:$(PATH)
export PATH := $(INSTALL)/qprogrammer/quartus/bin64/:$(PATH)
# for GDB server
export PATH := $(INSTALL)/riscfree/debugger/gdbserver-riscv/:$(PATH)
# for GDB client
export PATH := $(INSTALL)/riscfree/toolchain/riscv32-unknown-elf/bin/:$(PATH)
# for the nios2-terminal
export PATH := $(INSTALL)/fpgacademy/AMP/bin/:$(PATH)
# for checking JTAG chain
export PATH := $(INSTALL)/quartus/sopc_builder/bin/:$(PATH)

CYGWIN_INSTALL := $(shell $(BASH) 'export PATH=/usr/local/bin:/usr/bin; cygpath $(INSTALL)')
CYGWIN_PATH := export PATH=/usr/local/bin:/usr/bin:$(CYGWIN_INSTALL)/fpgacademy/AMP/bin

# Programs
CC	:= $(COMPILER)/riscv32-unknown-elf-gcc.exe
LD	:= $(CC)
OD	:= $(COMPILER)/riscv32-unknown-elf-objdump.exe
NM	:= $(COMPILER)/riscv32-unknown-elf-nm.exe
RM	:= /usr/bin/rm -f

# Flags
USERCCFLAGS	:= -g -O1 -ffunction-sections -fverbose-asm -fno-inline -gdwarf-2 
USERLDFLAGS	:= -Wl,--defsym=__stack_pointer$$=0x4000000 -Wl,--defsym  -Wl,JTAG_UART_BASE=0xff201000 -lm
ARCHCCFLAGS	:= -march=rv32im_zicsr -mabi=ilp32
ARCHLDFLAGS	:= -march=rv32im_zicsr -mabi=ilp32
CCFLAGS		:= -Wall -c $(USERCCFLAGS) $(ARCHCCFLAGS)
LDFLAGS		:= $(USERLDFLAGS) $(ARCHLDFLAGS)

# Files
OBJS		:= $(patsubst %, %.o, $(SRCS))

############################################
# GDB Macros

# Programs
GDB_SERVER		:= ash-riscv-gdb-server.exe
GDB_CLIENT		:= riscv32-unknown-elf-gdb.exe

############################################
# System Macros

# Programs
QP_PROGRAMMER	:= quartus_pgm.exe

# Flags
# DE10-Lite
SYS_FLAG_CABLE_Lite		:= -c "USB-Blaster [USB-0]"
# SYS_FLAG_USB_Lite		:= "USB-0"
# DE1-SoC
SYS_FLAG_CABLE_SoC 		:= -c "DE-SoC [USB-1]"
# SYS_FLAG_USB_SoC		:= "USB-1"

# DE10-Lite
JTAG_INDEX_Lite	:= 1
RED_TEXT		:= @$(BASH) 'printf "\033[31m"'
GREEN_TEXT		:= @$(BASH) 'printf "\033[32m"'
CYAN_TEXT		:= @$(BASH) 'printf "\033[36m"'
YELLOW_TEXT		:= @$(BASH) 'printf "\033[33m"'
DEF_TEXT		:= @$(BASH) 'printf "\033[0m"'

############################################
# Compilation Targets

COMPILE: $(basename $(MAIN)).elf

$(basename $(MAIN)).elf: $(OBJS)
	@$(BASH) 'cd "$(CURDIR)"; $(RM) $@'
	$(CYAN_TEXT)
	@echo Linking
	@$(BASH) 'printf "$(LD) "'
	$(DEF_TEXT)
	@echo $(LDFLAGS) $(OBJS) -o $@
	@$(BASH) 'printf "\n"'
	@$(BASH) 'cd "$(CURDIR)"; $(CYGWIN_PATH); $(LD) $(LDFLAGS) $(OBJS) -o $@'

%.c.o: %.c $(HDRS)
	@$(BASH) 'cd "$(CURDIR)"; $(RM) $@'
	$(GREEN_TEXT)
	@echo Compiling
	@$(BASH) 'printf "$(CC) "'
	$(DEF_TEXT)
	@echo $(CCFLAGS) $< -o $@
	@$(BASH) 'cd "$(CURDIR)"; $(CYGWIN_PATH); $(CC) $(CCFLAGS) $< -o $@'

SYMBOLS: $(basename $(MAIN)).elf
	@echo $(NM) -p $<
	@$(BASH) 'cd "$(CURDIR)"; $(CYGWIN_PATH); $(NM) -p $<'

OBJDUMP: $(basename $(MAIN)).elf
	@echo $(OD) -d -S $<
	@$(BASH) 'cd "$(CURDIR)"; $(CYGWIN_PATH); $(OD) -d -S $<'

CLEAN: 
	$(RED_TEXT)
	@$(BASH) 'printf "$(RM) "'
	$(DEF_TEXT)
	@echo $(basename $(MAIN)).elf $(OBJS)
	@$(BASH) 'cd "$(CURDIR)"; $(RM) $(basename $(MAIN)).elf $(OBJS)'

############################################
# System Targets

DETECT_DEVICES:
	$(QP_PROGRAMMER) $(SYS_FLAG_CABLE) --auto

DE1-SoC:
	$(QP_PROGRAMMER) $(SYS_FLAG_CABLE_SoC) -m jtag -o "P;$(HW_DE1-SoC)@$(JTAG_INDEX_SoC)"

DE10-Lite:
	$(QP_PROGRAMMER) $(SYS_FLAG_CABLE_Lite) -m jtag -o "P;$(HW_DE10-Lite)@$(JTAG_INDEX_Lite)"

TERMINAL:
	nios2-terminal.exe --instance 0

############################################
# GDB Targets

GDB_SERVER: 
	$(GDB_SERVER) --device 02D120DD --gdb-port 2454 --instance 1 --probe-type USB-Blaster-2 --transport-type jtag --auto-detect true

GDB_CLIENT: 
	$(GDB_CLIENT) -silent -ex "target remote:2454" -ex "set $$mstatus=0" -ex "set $$mtvec=0" -ex "load" -ex "set $$pc=_start" -ex "info reg pc" "$(basename $(MAIN)).elf"

############################################
# EXTRAS

.SILENT: SYMBOLS OBJDUMP
