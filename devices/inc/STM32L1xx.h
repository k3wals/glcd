/**
 * \file STM32L1xx.h
 * \brief Device implementation for ST STM32L1xx ARM Cortex-M3 MCUs
 *        Requires the use of ST's Standard Peripheral Library
 * \author Andy Gock
 *
 * \todo Code is untested!
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
#ifndef STM32L1XX_H_
#define STM32L1XX_H_

#if defined(GLCD_DEVICE_STM32L1XX)

/** SPI port number e.g SPI1, SPI2 (not to be confused with GPIOA, GPIOB, etc) */
#define CONTROLLER_SPI_NUMBER   SPI2
#define CONTROLLER_SPI_AF	GPIO_AF_SPI2
#define CONTROLLER_SPI_PORT     GPIOB
#define CONTROLLER_SPI_SCK_PIN  GPIO_Pin_13
#define CONTROLLER_SPI_SCK_PINSRC  GPIO_PinSource13
#define CONTROLLER_SPI_MISO_PIN GPIO_Pin_xx
#define CONTROLLER_SPI_MISO_PINSRC GPIO_PinSourcexx
#define CONTROLLER_SPI_MOSI_PIN GPIO_Pin_15
#define CONTROLLER_SPI_MOSI_PINSRC GPIO_PinSource15

#define CONTROLLER_SPI_SS_PORT  GPIOB
#define CONTROLLER_SPI_SS_PIN   GPIO_Pin_xx
#define CONTROLLER_SPI_DC_PORT  GPIOB
#define CONTROLLER_SPI_DC_PIN   GPIO_Pin_14
#define CONTROLLER_SPI_RST_PORT GPIOB
#define CONTROLLER_SPI_RST_PIN  GPIO_Pin_12

#define GLCD_SELECT()     GPIO_ResetBits(CONTROLLER_SPI_SS_PORT,CONTROLLER_SPI_SS_PIN)
#define GLCD_DESELECT()   GPIO_SetBits(CONTROLLER_SPI_SS_PORT,CONTROLLER_SPI_SS_PIN)
#define GLCD_DC_LOW()     GPIO_ResetBits(CONTROLLER_SPI_DC_PORT,CONTROLLER_SPI_DC_PIN)
#define GLCD_DC_HIGH()    GPIO_SetBits(CONTROLLER_SPI_DC_PORT,CONTROLLER_SPI_DC_PIN)
#define GLCD_RESET_LOW()  GPIO_ResetBits(CONTROLLER_SPI_RST_PORT,CONTROLLER_SPI_RST_PIN)
#define GLCD_RESET_HIGH() GPIO_SetBits(CONTROLLER_SPI_RST_PORT,CONTROLLER_SPI_RST_PIN)

_Bool dma_ch5_busy;

#define   DMA_SSD_1306    DMA1_Channel5
#define DMA_CH5_BUSY_SET 	dma_ch5_busy=TRUE
#define DMA_CH5_BUSY_RESET 	dma_ch5_busy=FALSE
#define DMA_CH5_BUSY_STATUS	dma_ch5_busy


#define   SPI_SendByte(data)  SPI2->DR = (data)
#define   SPI_Wait()	while(!(SPI2->SR&SPI_I2S_FLAG_TXE));while(SPI2->SR&SPI_I2S_FLAG_BSY);

#else
	#error "Controller not supported by STM32L1XX"
#endif

/** \name Base Functions
 *  @{
 */

/**
 * Kickoff DMA transfer
 *
 * Description here
 *
 */
void glcd_start_dma_transfer(void);

/** @}*/

#endif /* STM32L1XX_H_ */
