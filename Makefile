##########################################################################################################################
# SITL Makefile (GCC + FreeRTOS POSIX)
##########################################################################################################################

######################################
# target
######################################
TARGET = main

######################################
# building variables
######################################
DEBUG = 1
OPT = -std=gnu11 -Wall -fdata-sections -ffunction-sections

RM    ?= rm -f
RMDIR ?= rm -rf
MAKEFLAGS += --jobs=8

#######################################
# paths
#######################################
BUILD_DIR = build

# FreeRTOS kernel path (ajuste para o caminho correto no seu sistema)
FREERTOS_DIR = /home/pedro-bittencourt/Downloads/FreeRTOSv202406.04-LTS/FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel

######################################
# source
######################################

C_SOURCES =  \
main.c \
app/Src/app.c \
hal/Src/hal.c \
hal/Src/hal_app.c \
hal/Src/hal_cpu.c \
hal/Src/hal_gpio.c \
hal/Src/hal_rtc.c \
hal/Src/hal_ser.c \
hal/Src/hal_time.c \
utl/Src/utl_cbf.c \
utl/Src/utl_cobs.c \
utl/Src/utl_crc16.c \
utl/Src/utl_dbg.c \
utl/Src/utl_io.c \
port/pc_linux/time.c \
port/pc_linux/gpio.c \
$(FREERTOS_DIR)/tasks.c \
$(FREERTOS_DIR)/list.c \
$(FREERTOS_DIR)/queue.c \
$(FREERTOS_DIR)/timers.c \
$(FREERTOS_DIR)/event_groups.c \
$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/port.c \
$(FREERTOS_DIR)/portable/MemMang/heap_4.c \
$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c 

#######################################
# binaries
#######################################
CC = gcc
SZ = size

#######################################
# includes / defs
#######################################
C_DEFS = \
-DPLATFORM_PC_LINUX

C_INCLUDES = \
-I. \
-Iapp/Inc \
-Ihal/Inc \
-Iutl/Inc \
-I$(FREERTOS_DIR)/include \
-I$(FREERTOS_DIR)/portable/ThirdParty/GCC/Posix

CFLAGS = $(C_DEFS) $(C_INCLUDES) $(OPT)

ifeq ($(DEBUG),1)
CFLAGS += -g -O0
else
CFLAGS += -O2
endif

CFLAGS += -MMD -MP

#######################################
# linker
#######################################
LIBS = -lm -lpthread
LDFLAGS = $(LIBS)

#######################################
# objects (CORREÇÃO CRÍTICA)
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))

# resolve diretórios automaticamente
vpath %.c $(sort $(dir $(C_SOURCES)))

#######################################
# rules
#######################################

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

#######################################
# run
#######################################
run: all
	./$(BUILD_DIR)/$(TARGET)

#######################################
# clean
#######################################
clean:
	-$(RMDIR) $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(BUILD_DIR)/*.d

# *** EOF ***