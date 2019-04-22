DEMO_DIR = Demo
TARGET = FreeRTOSv9.0.0
CFLAGS += -Os -lm

BSP_BASE = ../../bsp


C_SRCS += Source/croutine.c  
C_SRCS += Source/list.c  
C_SRCS += Source/queue.c  
C_SRCS += Source/tasks.c  
C_SRCS += Source/timers.c  
C_SRCS += Source/event_groups.c  
C_SRCS += Source/portable/MemMang/heap_4.c  
C_SRCS += Source/portable/GCC/N200/port.c

C_SRCS += ${DEMO_DIR}/main.c
C_SRCS += ${DEMO_DIR}/adc/adc.c
C_SRCS += ${DEMO_DIR}/bldc/bldc.c
C_SRCS += ${DEMO_DIR}/ds18b20/ds18b20.c
C_SRCS += ${DEMO_DIR}/lcd/oled.c
C_SRCS += ${DEMO_DIR}/mpu6050/MPU6050.c
C_SRCS += ${DEMO_DIR}/mpu6050/ioi2c.c
C_SRCS += ${DEMO_DIR}/mpu6050/filter.c


INCLUDES += -ISource/include
INCLUDES += -ISource/portable/GCC/N200
INCLUDES += -I${DEMO_DIR}
INCLUDES += -I${DEMO_DIR}/mpu6050
INCLUDES += -I${DEMO_DIR}/lcd
INCLUDES += -I${DEMO_DIR}/bldc


ASM_SRCS += Source/portable/GCC/N200/portasm.S

include $(BSP_BASE)/$(BOARD)/n22/env/common.mk
