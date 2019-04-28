TARGET = delay_test
CFLAGS += -O2  \
	

BSP_BASE = ../../bsp

C_SRCS = delay_test.c \
	bldc.c	\
	ioi2c.c	\
	ds18b20.c	\
	MPU6050.c	\
	filter.c	\
	adc.c 		\
	DataScope_DP.c	\
	oled.c		\

HEADERS=bldc.h	\
	ioi2c.h	\
	MPU6050.h	\
	filter.h	\
	DataScope_DP.h	\
	oled.h		\
	oledfont.h	\



include $(BSP_BASE)/$(BOARD)/n22/env/common.mk
