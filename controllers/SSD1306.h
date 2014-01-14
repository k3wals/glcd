/**
 * \file SSD1306.h
 * \brief Constants relating to SSD1306 OLED controller.
 * \author
 *
 * Constants and functions specific to SSD1306.
 * 
 * \todo Need to move functions to be controller independent
 * \todo Need to document controller functions
 *
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

#ifndef SSD1306_H_
#define SSD1306_H_

/* Commands */
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20 //0x20 = Horz. 0x21 = Vert. 0x22 = Paging

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
#define ST7565R_SET_START_LINE (1<<6)

/* These functions only available on SSD1306 implementation (for now) */

/* Private functions */
//void glcd_set_column_upper(uint8_t addr);
//void glcd_set_column_lower(uint8_t addr);
void glcd_set_column_address(uint8_t lower_addr, uint8_t upper_addr);

/** All display points on (native) */
void glcd_all_on(void);

/** Set to normal mode */
void glcd_normal(void);

/** Set start line/page */
void glcd_set_start_line(uint8_t addr);

/** Clear the display immediately, does not buffer */
void glcd_clear_now(void);

/** Show a black and white line pattern on the display */
void glcd_pattern(void);

/** Init SSD1306 controller / display */
void glcd_SSD1306_init(void);

void glcd_set_addressing_mode(uint8_t d);
void glcd_set_page_address(uint8_t a, uint8_t b);
void glcd_set_start_line(uint8_t d);
void glcd_set_charge_pump(uint8_t d);
void glcd_set_segment_remap(uint8_t d);
void glcd_set_entire_display(uint8_t d);
void glcd_set_inverse_display(uint8_t d);
void glcd_set_multiplex_ratio(uint8_t d);
void glcd_set_display_on_off(uint8_t d);
void glcd_set_start_page(uint8_t d);
void glcd_set_common_remap(uint8_t d);
void glcd_set_display_offset(uint8_t d);
void glcd_set_display_clock(uint8_t d);
void glcd_set_precharge_period(uint8_t d);
void glcd_set_common_config(uint8_t d);
void glcd_set_VCOMH(uint8_t d);
void glcd_set_nop(void);


#endif /* SSD1306_H_ */
