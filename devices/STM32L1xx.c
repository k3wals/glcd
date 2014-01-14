/**
	\file STM32L1xx.c
	\author Andy Gock
	\brief Functions specific to STM32 L1 ARM Cortex-M0 devices.
 */

/*
	Copyright (c) 2012, Andy Gock

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
		* Redistributions of source code must retain the above copyright
		  notice, this list of conditions and the following disclaimer.
		* Redistributions in binary form must reproduce the above copyright
		  notice, this list of conditions and the following disclaimer in the
		  documentation and/or other materials provided with the distribution.
		* Neither the name of Andy Gock nor the
		  names of its contributors may be used to endorse or promote products
		  derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL ANDY GOCK BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if defined(GLCD_DEVICE_STM32L1XX)

/* Includes from CMSIS and Peripheral Library */
#include "stm32l1xx.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_spi.h"
#include "stm32l1xx_rcc.h"
#include "misc.h"

/* Includes from GLCD */
#include "../glcd.h"
#include "inc/STM32L1xx.h"

void glcd_init(void)
{

#if defined(GLCD_CONTROLLER_SSD1306)
	/* Initialization for PCD8544 controller */

	/* Declare GPIO and SPI init structures */
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Initialise structures (which we will overide later) */
	GPIO_StructInit(&GPIO_InitStructure);
	SPI_StructInit(&SPI_InitStructure);
	
	/* Need to make start up the correct peripheral clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* DC pin */
	GPIO_InitStructure.GPIO_Pin = CONTROLLER_SPI_DC_PIN;
	GPIO_Init(CONTROLLER_SPI_DC_PORT, &GPIO_InitStructure);

	/* RESET pin */
	GPIO_InitStructure.GPIO_Pin = CONTROLLER_SPI_RST_PIN;
	GPIO_Init(CONTROLLER_SPI_RST_PORT, &GPIO_InitStructure);

	/* Make sure chip is de-selected by default */
//	GLCD_DESELECT();

	/* Set up GPIO for SPI pins */
	GPIO_InitStructure.GPIO_Pin   = CONTROLLER_SPI_SCK_PIN | CONTROLLER_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_Init(CONTROLLER_SPI_PORT, &GPIO_InitStructure);

	/* Configure alternate function mode for SPI pins */
	GPIO_PinAFConfig(GPIOA,CONTROLLER_SPI_SCK_PINSRC,CONTROLLER_SPI_AF);
	GPIO_PinAFConfig(GPIOA,CONTROLLER_SPI_MOSI_PINSRC,CONTROLLER_SPI_AF);

	/* Initialize SPI */
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; /* Set clock speed! */
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(CONTROLLER_SPI_NUMBER, &SPI_InitStructure);

	/* Enable SPI interrupts */
	/*
	SPI_I2S_ITConfig(CONTROLLER_SPI_NUMBER, SPI_I2S_IT_TXE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_Init(&NVIC_InitStructure);
	*/
	
	/* Enable SPI */
	SPI_Cmd(CONTROLLER_SPI_NUMBER, ENABLE);

	/* Initialization sequence of controller */
	glcd_reset();

	glcd_SSD1306_init();


	glcd_clear();


#else
	#error "Controller not supported by STM32L1xx"
#endif

}

void glcd_spi_write(uint8_t c)
{

////	GLCD_SELECT();
//	SPI_SendData8(CONTROLLER_SPI_NUMBER, (uint16_t) c);
//
//	/* Wait until entire byte has been read (which we discard anyway) */
//	while(SPI_I2S_GetFlagStatus(CONTROLLER_SPI_NUMBER, SPI_I2S_FLAG_BSY) != RESET);
	
	SPI_SendByte(c);
	SPI_Wait();

//	GLCD_DESELECT();
}

void glcd_start_dma_transfer(void){
	GLCD_DC_LOW(); //Set to command
	glcd_set_column_address(0x00,0x7F);	// Set start and end columns (for horz and vert addr mode only)
	glcd_set_page_address(0x00,0x07);	// Set start and end pages (for horz and vert addr mode only)

	GLCD_DC_HIGH(); //Set to data
	DMA_SSD_1306->CCR &= ((uint32_t)0xFFFFFFFE); // Disable DMA for channel
	DMA_SSD_1306->CNDTR = 1024; // 128x8
	DMA_SSD_1306->CMAR = (uint32_t)(glcd_buffer);
	DMA_SSD_1306->CCR |= ((uint32_t)0x00000001); // Enable DMA for channel
}

void glcd_reset(void)
{
	/* Toggle RST low to reset. Minimum pulse 100ns on datasheet. */
//	GLCD_SELECT();
	GLCD_RESET_HIGH();
	GLCD_RESET_LOW();
	delay_ms(GLCD_RESET_TIME);
	GLCD_RESET_HIGH();
//	GLCD_DESELECT();
}

#endif
