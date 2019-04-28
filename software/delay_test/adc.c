


#include <stdio.h>
#include <stdlib.h>
#include "n22/drivers/n22_func.h"
#include "soc/drivers/soc.h"
#include <string.h>
#include "n22/drivers/n22_clic.h"
#include "n22/drivers/riscv_encoding.h"
#include <unistd.h>
#include "soc/drivers/board.h"
#define SPI1_MISO_GPIO_OFFSET 4
#define SPI1_CS_GPIO_OFFSET 2
#define SPI1_SCK_GPIO_OFFSET 5
#define SPI1_SCKMODE_CPOL 1
#define SPI1_SCKMODE_CPHA 0
void Adc_Init(void)
{
    GPIO_REG(GPIO_IOF_EN) |= ((0x1 << SPI1_MISO_GPIO_OFFSET)|(0x1 << SPI1_CS_GPIO_OFFSET) |  (0x1 << SPI1_SCK_GPIO_OFFSET));     //SPI2   IOF
	GPIO_REG(GPIO_IOF_SEL)  &= ~ ((0x1 << SPI1_MISO_GPIO_OFFSET) |(0x1 << SPI1_CS_GPIO_OFFSET)|  (0x1 << SPI1_SCK_GPIO_OFFSET));   //SPI2   IOF0

	GPIO_REG(GPIO_INPUT_EN)    &= ~( (0x1 << SPI1_CS_GPIO_OFFSET)|  (0x1 << SPI1_SCK_GPIO_OFFSET));
	GPIO_REG(GPIO_OUTPUT_EN) |= ( (0x1 << SPI1_CS_GPIO_OFFSET)|(0x1 << SPI1_SCK_GPIO_OFFSET));

	SPI1_REG(SPI_REG_SCKDIV)=0x09;
	SPI1_REG(SPI_REG_SCKMODE) &= ~((0x1 << SPI1_SCKMODE_CPOL)|(0x1 << SPI1_SCKMODE_CPHA));
}
