/**
 * \file SSD1306.c
 * \brief Functions relating to SSD1306.
 * \author
 * \see glcd.h
 */ 
#if defined(GLCD_CONTROLLER_SSD1306)

#include "../glcd.h"

void glcd_command(uint8_t c)
{
	GLCD_DC_LOW();
	glcd_spi_write(c);	
}

void glcd_data(uint8_t c)
{
	GLCD_DC_HIGH();
	glcd_spi_write(c);	
}

void glcd_set_contrast(uint8_t val) {
	/* Can set a 6-bit value (0 to 63)  */

	/* Must send this command byte before setting the contrast */
	glcd_command(0x81);
	
	/* Set the contrat value ("electronic volumne register") */
	if (val > 63) {
		glcd_command(63);
	} else {
		glcd_command(val);
	}
	return;
}

void glcd_power_down(void)
{
	/* Not applicable */
	return;
	//asm("break");
}

void glcd_power_up(void)
{
	/* Not applicable */
	return;
	//asm("break");
}

void glcd_set_y_address(uint8_t y)
{
	glcd_command(0xB0 | y);
}

void glcd_set_x_address(uint8_t x)
{
//	glcd_set_column_upper(x);
//	glcd_set_column_lower(x);
	glcd_set_column_address(x,x);
}

void glcd_all_on(void)
{
	glcd_command(SSD1306_DISPLAYALLON);
}

void glcd_normal(void)
{
	glcd_command(SSD1306_NORMALDISPLAY);
}

void glcd_set_column_address(uint8_t lower_addr, uint8_t upper_addr){
	glcd_command(0x21);
	glcd_command(lower_addr);
	glcd_command(upper_addr);
}

/** Clear the display immediately, does not buffer */
void glcd_clear_now(void)
{
	uint8_t page;
	for (page = 0; page < GLCD_NUMBER_OF_BANKS; page++) {
		glcd_set_y_address(page);
		glcd_set_x_address(0);
		uint8_t col;
		for (col = 0; col < GLCD_NUMBER_OF_COLS; col++) {
			glcd_data(0);
		}			
	}
}

void glcd_pattern(void)
{
	uint8_t page;
	for (page = 0; page < GLCD_NUMBER_OF_BANKS; page++) {
		glcd_set_y_address(page);
		glcd_set_x_address(0);
		uint8_t col;
		for (col = 0; col < GLCD_NUMBER_OF_COLS; col++) {
			glcd_data( (col / 8 + 2) % 2 == 1 ? 0xff : 0x00 );
		}			
	}
}

void glcd_write()
{

#ifdef GLCD_CONTROLLER_SSD1306_DMA

	glcd_start_dma_transfer();
	while(DMA_CH5_BUSY_STATUS); //wait until transfer is done

#else

	uint8_t bank;

	for (bank = 0; bank < GLCD_NUMBER_OF_BANKS; bank++) {
		/* Each bank is a single row 8 bits tall */
		uint8_t column;		
		
		if (glcd_bbox_selected->y_min >= (bank+1)*8) {
			continue; /* Skip the entire bank */
		}
		
		if (glcd_bbox_selected->y_max < bank*8) {
			break;    /* No more banks need updating */
		}
		
		glcd_set_y_address(bank);
		glcd_set_x_address(glcd_bbox_selected->x_min);

		for (column = glcd_bbox_selected->x_min; column <= glcd_bbox_selected->x_max; column++)
		{
			glcd_data( glcd_buffer_selected[GLCD_NUMBER_OF_COLS * bank + column] );
		}
	}

#endif

	glcd_reset_bbox();

}

void glcd_SSD1306_init(void) {

	glcd_reset();

	glcd_set_display_on_off(0x00);		// Display Off (0x00/0x01)
	glcd_set_display_clock(0x80);		// Set Clock as 100 Frames/Sec
	glcd_set_multiplex_ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
	glcd_set_display_offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	glcd_set_start_line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	glcd_set_charge_pump(0x04);			// Enable Embedded DC/DC Converter (0x00/0x04)
	glcd_set_column_address(0x00,0x7F);	// Set start and end columns (for horz and vert addr only)
	glcd_set_page_address(0x00,0x07);	// Set start and end pages (for horz and vert addr only)
	glcd_set_addressing_mode(0x02);		// Set Addressing Mode (Horz=0x00/Vert=0x01/Page=0x02)
	glcd_set_segment_remap(0x01);		// Set SEG/Column Mapping (0x00/0x01)
	glcd_set_common_remap(0x08);			// Set COM/Row Scan Direction (0x00/0x08)
	glcd_set_common_config(0x10);		// Set Sequential Configuration (0x00/0x10)
	glcd_set_contrast(0x3F);	// Set SEG Output Current
	glcd_set_precharge_period(0xF1);		// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	glcd_set_VCOMH(0x40);			// Set VCOM Deselect Level
	glcd_set_entire_display(0x00);		// Disable Entire Display On (0x00/0x01)
	glcd_set_inverse_display(0x00);		// Disable Inverse Display On (0x00/0x01)

	glcd_clear_now();				// Clear Screen
	
	glcd_set_display_on_off(0x01);		// Display On (0x00/0x01)
	
	oled_is_ready = TRUE;

}

void glcd_set_addressing_mode(unsigned char d)
{
	glcd_command(0x20);			// Set Memory Addressing Mode
	glcd_command(d);			//   Default => 0x02
						//     0x00 => Horizontal Addressing Mode
						//     0x01 => Vertical Addressing Mode
						//     0x02 => Page Addressing Mode
}

void glcd_set_page_address(unsigned char a, unsigned char b)
{
	glcd_command(0x22);			// Set Page Address
	glcd_command(a);			//   Default => 0x00 (Page Start Address)
	glcd_command(b);			//   Default => 0x07 (Page End Address)
}

void glcd_set_start_line(unsigned char d)
{
	glcd_command(0x40|d);			// Set Display Start Line
						//   Default => 0x40 (0x00)
}

void glcd_set_charge_pump(unsigned char d)
{
	glcd_command(0x8D);			// Set Charge Pump
	glcd_command(0x10|d);			//   Default => 0x10
						//     0x10 (0x00) => Disable Charge Pump
						//     0x14 (0x04) => Enable Charge Pump
}

void glcd_set_segment_remap(unsigned char d)
{
	glcd_command(0xA0|d);			// Set Segment Re-Map
						//   Default => 0xA0
						//     0xA0 (0x00) => Column Address 0 Mapped to SEG0
						//     0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void glcd_set_entire_display(unsigned char d)
{
	glcd_command(0xA4|d);			// Set Entire Display On / Off
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On
}

void glcd_set_inverse_display(unsigned char d)
{
	glcd_command(0xA6|d);			// Set Inverse Display On/Off
						//   Default => 0xA6
						//     0xA6 (0x00) => Normal Display
						//     0xA7 (0x01) => Inverse Display On
}

void glcd_set_multiplex_ratio(unsigned char d)
{
	glcd_command(0xA8);			// Set Multiplex Ratio
	glcd_command(d);			//   Default => 0x3F (1/64 Duty)
}

void glcd_set_display_on_off(unsigned char d)
{
	glcd_command(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}

void glcd_set_start_page(unsigned char d)
{
	glcd_command(0xB0|d);			// Set Page Start Address for Page Addressing Mode
						//   Default => 0xB0 (0x00)
}

void glcd_set_common_remap(unsigned char d)
{
	glcd_command(0xC0|d);			// Set COM Output Scan Direction
						//   Default => 0xC0
						//     0xC0 (0x00) => Scan from COM0 to 63
						//     0xC8 (0x08) => Scan from COM63 to 0
}

void glcd_set_display_offset(unsigned char d)
{
	glcd_command(0xD3);			// Set Display Offset
	glcd_command(d);			//   Default => 0x00
}

void glcd_set_display_clock(unsigned char d)
{
	glcd_command(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	glcd_command(d);			//   Default => 0x80
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}

void glcd_set_precharge_period(unsigned char d)
{
	glcd_command(0xD9);			// Set Pre-Charge Period
	glcd_command(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void glcd_set_common_config(unsigned char d)
{
	glcd_command(0xDA);			// Set COM Pins Hardware Configuration
	glcd_command(0x02|d);			//   Default => 0x12 (0x10)
						//     Alternative COM Pin Configuration
						//     Disable COM Left/Right Re-Map
}

void glcd_set_VCOMH(unsigned char d)
{
	glcd_command(0xDB);			// Set VCOMH Deselect Level
	glcd_command(d);			//   Default => 0x20 (0.77*VCC)
}

void glcd_set_nop()
{
	glcd_command(0xE3);			// Command for No Operation
}

#endif
