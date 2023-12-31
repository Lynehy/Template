
# 工程文件夹
TARGET = FreeRTOS
 
# C编译生成文件夹
BUILD_DIR = build
 
# C源文件

C_DIR = \
User/Src \
Drivers/CMSIS/Source \
FreeRTOS \
FreeRTOS/portable/ARM_CM3 \
Hardware/LED/Src

C_SOURCES =  $(foreach dir, $(C_DIR), $(wildcard $(dir)/*.c))
C_SOURCES += \
FreeRTOS/portable/MemMang/heap_4.c \
Drivers/STM32F10x_StdPeriph_Driver/Source/misc.c \
Drivers/STM32F10x_StdPeriph_Driver/Source/stm32f10x_rcc.c \
Drivers/STM32F10x_StdPeriph_Driver/Source/stm32f10x_gpio.c

######################################
 
 
 
# ASM sources
ASM_SOURCES =  \
Drivers/CMSIS/startup/startup_stm32f10x_md.s
 
######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og
 
 
#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3
 
# fpu
# NONE for Cortex-M0/M0+/M3
 
# float-abi
 
 
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
 
# macros for gcc
# AS defines
AS_DEFS = 
 
# C defines   宏定义标志
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F10X_MD
 
# AS includes
AS_INCLUDES = 
 
# C includes  C头文件路径
C_INCLUDES =  \
-IDrivers/CMSIS/Include \
-IDrivers/STM32F10x_StdPeriph_Driver/Include \
-IFreeRTOS/include \
-IFreeRTOS/portable/ARM_CM3 \
-IUser/Inc \
-IHardware/LED/Inc
 
# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
 
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
 
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
 
 
# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
 
 
#######################################
# LDFLAGS
#######################################
# link script  链接配置文件
LDSCRIPT = User/Lib/stm32_flash.ld
 
# libraries
#LIBS = -lc -lm -lnosys 
LIBS = -lc
LIBDIR = 
#LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
 
# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
 
 
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
 
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
 
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
 
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
 
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		
 
#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
 
# *** EOF ***