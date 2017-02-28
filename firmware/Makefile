PROJECT=switchy
MCU=LPC1114
OPTS = -Os -fstrict-aliasing -fsingle-precision-constant -funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fno-common -fdata-sections -fomit-frame-pointer
CXX_OPTS = -Os -fstrict-aliasing -fsingle-precision-constant -funsigned-char -funsigned-bitfields -fshort-enums -ffunction-sections -fno-common -fdata-sections -fomit-frame-pointer -fno-threadsafe-statics -fno-exceptions -fno-rtti -finline-functions -finline-small-functions -findirect-inlining -finline -fpartial-inlining -freorder-blocks  -falign-loops -falign-jumps -funroll-loops -fpeel-loops
WARNINGS = -Wno-strict-aliasing -Wundef -Wall -Wextra -Wunreachable-code #-nostdlib
LDSCRIPT=core/$(MCU).ld

ASM_SOURCES=$(wildcard core/*.s)
C_SOURCES=$(wildcard src/*.c) $(wildcard core/*.c)
CXX_SOURCES=$(wildcard src/*.cpp) $(wildcard core/*.cpp)
ELF_NAME=$(PROJECT).elf
BIN_NAME=$(PROJECT).bin

OBJECTS=$(patsubst %,./%,$(C_SOURCES:.c=.o)) $(patsubst %,./%,$(CXX_SOURCES:.cpp=.o)) $(patsubst %,./%,$(ASM_SOURCES:.s=.o))
DEPS=$(patsubst %,./%,$(C_SOURCES:.c=.d)) $(patsubst %,./%,$(CXX_SOURCES:.cpp=.d)) $(patsubst %,./%,$(ASM_SOURCES:.s=.d)) 
LSTFILES=$(patsubst %,./%,$(C_SOURCES:.c=.lst)) $(patsubst %,./%,$(CXX_SOURCES:.cpp=.lst)) $(patsubst %,./%,$(ASM_SOURCES:.s=.lst))

AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
REMOVE = rm -f
SIZE = arm-none-eabi-size
CFLAGS = -std=gnu99 -mcpu=cortex-m0 -mthumb -I. -Icore -Iinclude $(OPTS) -Wstrict-prototypes $(WARNINGS) -Wa,-adhlns=./$(<:.c=.lst)
CXXFLAGS = -std=gnu++11 -mcpu=cortex-m0 -mthumb -I. -Icore -Iinclude $(CXX_OPTS) $(WARNINGS) -Wa,-adhlns=./$(<:.cpp=.lst)
	LDFLAGS = -T$(LDSCRIPT) -Wl,--gc-sections -specs=nosys.specs
	#LDFLAGS = -T$(LDSCRIPT) -Wl,--gc-sections

#########################################################################

all: .bin ./core $(BIN_NAME) stats

program: all
	lpc21isp -bin $(BIN_NAME) -control -controlswap /dev/ttyUSB0 115200 17476

.bin:
	mkdir .bin

./core:
	mkdir ./core

$(BIN_NAME): ./$(ELF_NAME) Makefile
	@$(OBJCOPY) -R .stack -O binary ./$(ELF_NAME) $(BIN_NAME)

./$(ELF_NAME): $(OBJECTS) Makefile
	@echo "  LD ($(OBJECTS)) -> $(ELF_NAME)"
	@$(CXX) -o ./$(ELF_NAME) $(LDFLAGS) -lc $(OBJECTS) $(CFLAGS)

stats: ./$(ELF_NAME) 
	@$(SIZE) ./$(ELF_NAME)

clean:
	@echo "  Cleanup $<"
	@$(REMOVE) $(OBJECTS)
	@$(REMOVE) $(DEPS)
	@$(REMOVE) $(LSTFILES)
	@$(REMOVE) $(BIN_NAME)
	@$(REMOVE) $(ELF_NAME)

#########################################################################

./%.o: %.c Makefile 
	@echo "  CC $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

./%.o: %.cpp Makefile 
	@echo "  CXX $<"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

./%.o: %.s Makefile 
	@echo "  AS $<"
	@$(AS) -o $@ -c $<

.PHONY : clean all stats program

