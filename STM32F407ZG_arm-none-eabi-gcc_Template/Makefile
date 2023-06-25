# STM32F4 Makefile file

# 输出文件名
TARGET = Template

# 编译文件夹
BUILD_DIR = build


###############################################################
# 调试优化参数
###############################################################
# 开启调试
DEBUG = 1

# 编译优化
# 优化参数有 -O0 -O1 -O2 -O3 -Og -Os -Ofast
# 如果编译时没有优化参数，则默认使用 -O0 进行优化，即不优化
# 参数 -O1、-O2、-O3 中，随着数字变大，代码的优化程度也越高，不过这在某种意义上来说，也是以牺牲程序的可调试性为代价的
# 参数 -Og 是在 -O1 的基础上，去掉了那些影响调试的优化，所以如果最终是为了调试程序，可以使用这个参数
# 参数 -Os 是在 -O2 的基础上，去掉了那些会导致最终可执行程序增大的优化，如果想要更小的可执行程序，可选择这个参数
# 参数 -Ofast 是在 -O3 的基础上，添加了一些非常规优化，这些优化是通过打破一些国际标准（比如一些数学函数的实现标准）来实现的，所以一般不推荐使用该参数
ifeq ($(DEBUG), 1)
OPT = -Og -g -gdwarf-2
else
OPT = -Os
endif
###############################################################


###############################################################
# 编译文件
###############################################################
# C define 宏定义
C_DEFS = \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F40_41xxx

# C include 头文件
C_INCLUDES = \
-IDrivers/CMSIS/Include \
-IDrivers/STM32F4xx_DSP_StdPeriph_Driver/Include \
-IUser/Inc \
-IBsp/Inc

# C source 源文件
SRC_DIR = \
User/Src \
Drivers/CMSIS/Source \
Bsp/Src

C_SOURCES = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
C_SOURCES += \
Drivers/STM32F4xx_DSP_StdPeriph_Driver/Source/misc.c \
Drivers/STM32F4xx_DSP_StdPeriph_Driver/Source/stm32f4xx_rcc.c \
Drivers/STM32F4xx_DSP_StdPeriph_Driver/Source/stm32f4xx_gpio.c

# 启动文件
STARTUP = \
Drivers/CMSIS/startup/startup_stm32f40_41xxx.s
###############################################################


###############################################################
# 编译器配置
###############################################################
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
###############################################################


###############################################################
# 编译参数
###############################################################
# cpu
CPU = -mcpu=cortex-m4

#fpu
FPU = -mfpu=fpv4-sp-d16

#float-abi
FLOAT-ABI = -mfloat-abi=hard

#mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# ASM flag
ASFLAGS = $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections

# C flag
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
###############################################################


###############################################################
# 链接参数
###############################################################
# link script 链接配置文件
LDSCRIPT = User/STM32F407ZG_FLASH.ld

#libraries
#libs = -lc -lm -lnosys 
LIBS = -lc
LIBDIR = 

# link clag
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
###############################################################


###############################################################
# 编译过程
###############################################################
# 生成文件
all : $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# list of ASM program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(STARTUP:.s=.o)))
vpath %.s $(sort $(dir $(STARTUP)))
# list of objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo Compiling $(notdir $<)...
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	@echo Compile finish.

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo Compiling $(notdir $<)...
	@$(CC) -c $(ASFLAGS) $< -o $@
	@echo Compile finish.

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo Linking $(notdir $@)...
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(HEX) $< $@
	@echo Generic $(notdir $@).

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@$(BIN) $< $@
	@echo Generic $(notdir $@).
	@echo Done.

$(BUILD_DIR):
	@mkdir $@
###############################################################


###############################################################
# 下载程序
###############################################################
download:
	openocd -f "daplink.cfg"

###############################################################


###############################################################
# 清理编程文件
###############################################################
clean:
	@rmdir /s /q $(BUILD_DIR)
	@echo Clean finish.
#-rm -fR $(BUILD_DIR)

###############################################################


# end