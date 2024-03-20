/*******************************************************************************
 * File Name        : extdisplay.c
 *
 *
 ********************************************************************************
 * Copyright 2023-2024, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 *******************************************************************************/

/*******************************************************************************
 * Header Files
 *******************************************************************************/
// #include "app_mem_map.h"

#include "extdisplay.h"

uint8_t Hsync_H_last = 0x00;
uint8_t Hsync_L_last = 0x00;
uint8_t Vsync_H_last = 0x00;
uint8_t Vsync_L_last = 0x00;

uint8_t Hsync_L, Hsync_H, Vsync_L, Vsync_H;

bool suspend_on = 0;

uint8_t I2CADR = 0;
uint8_t Tx_HPD = 0;

// hfp, hs, hbp, hact, htotal, vfp, vs, vbp, vact, vtotal, hfp, hs, hbp, hact, htotal, vfp, vs, vbp, vact, vtotal, h_polary, v_polary, vic, aspact_ratio, pclk_khz

struct video_timing video_640x480_60Hz = {16, 96, 48, 640, 800, 10, 2, 33, 480, 525, 0, 0, 1, AR_4_3, 25000};
struct video_timing video_720x480_60Hz = {16, 62, 60, 720, 858, 9, 6, 30, 480, 525, 0, 0, 2, AR_4_3, 27000};
struct video_timing video_720x576_50Hz = {12, 64, 68, 720, 864, 5, 5, 39, 576, 625, 0, 0, 17, AR_4_3, 27000};
struct video_timing video_800x600_60Hz = {48, 128, 88, 800, 1056, 1, 4, 23, 600, 628, 1, 1, 0, AR_16_9, 40000};
struct video_timing video_1024x768_60Hz = {24, 136, 160, 1024, 1344, 3, 6, 29, 768, 806, 0, 0, 0, AR_16_9, 65000};
struct video_timing video_1280x720_60Hz = {110, 40, 220, 1280, 1650, 5, 5, 20, 720, 750, 1, 1, 4, AR_16_9, 74250};
struct video_timing video_1280x800_60Hz = {28, 32, 100, 1280, 1440, 2, 6, 15, 800, 823, 0, 0, 0, AR_16_9, 71000};
struct video_timing video_1920x1080_60Hz = {88, 44, 148, 1920, 2200, 4, 5, 36, 1080, 1125, 1, 1, 16, AR_16_9, 148500};

struct video_timing *video;

cyhal_i2c_t mI2C;

void handle_error(uint32_t status);
uint8_t LT8912_Get_HPD(void);

void ext_display_adapter_reset(void)
{
	/* Reset Board */
	cyhal_gpio_write(RESET_PIN, true);
	Cy_SysLib_Delay(100);				// 50ms ideal
	cyhal_gpio_write(RESET_PIN, false); // RESET active low
	Cy_SysLib_Delay(200);				// 200ms ideal
	cyhal_gpio_write(RESET_PIN, true);
	Cy_SysLib_Delay(200); // 200ms ideal
	printf("ext_display_adapter_reset done\r\n");
}

int ext_display_adapter_init(void)
{
	cy_rslt_t result;
	cyhal_i2c_cfg_t mI2C_cfg;

	result = cyhal_gpio_init(HPD_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLDOWN, 0); // HPD
	handle_error(result);
	result = cyhal_gpio_init(RESET_PIN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1); // RESET
	handle_error(result);

	printf("ext_display_adapter_init GPIO and I2C Master... ");
	mI2C_cfg.is_slave = false;
	mI2C_cfg.address = 0;
	mI2C_cfg.frequencyhal_hz = I2C_FREQ;

	/* Init I2C master */
	result = cyhal_i2c_init(&mI2C, ADAPTER_I2C_SDA, ADAPTER_I2C_SCL, NULL);
	handle_error(result);

	/* Configure I2C Master */
	result = cyhal_i2c_configure(&mI2C, &mI2C_cfg);
	handle_error(result);

	Cy_GPIO_SetDriveSel(CYHAL_GET_PORTADDR(ADAPTER_I2C_SCL),
						CYHAL_GET_PIN(ADAPTER_I2C_SCL), CY_GPIO_DRIVE_FULL);
	Cy_GPIO_SetDriveSel(CYHAL_GET_PORTADDR(ADAPTER_I2C_SDA),
						CYHAL_GET_PIN(ADAPTER_I2C_SDA), CY_GPIO_DRIVE_FULL);

	printf("done\r\n");
	return 0;
}

int ext_display_adapter_writebyte(uint8_t reg_addr, uint8_t value)
{
	uint8_t dataBuffer[2] = {reg_addr, value};
	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(&mI2C, (I2CADR >> 1), dataBuffer, 2, 0, true))
	{
		//  printf("writebuffer: 0x%02X \n\r",dataBuffer[1]);
		//  Adapter_I2C_Read((I2CADR >> 1),reg_addr,value);
	}
	else
	{
		printf("ERR writing to I2CADR 0x%02X reg=0x%02X val=0x%02X \n\r", I2CADR, dataBuffer[0], dataBuffer[1]);
		//  handle_error(1);
	}

	return CY_RSLT_SUCCESS;
}

uint8_t ext_display_adapter_readbyte(uint8_t reg_addr)
{
	uint8_t dataBuffer[1] = {reg_addr};
	uint8_t readBuffer[1];
	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(&mI2C, (I2CADR >> 1), dataBuffer, 1, 0, true))
	{
		if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(&mI2C, (I2CADR >> 1), readBuffer, 1, 0, true))
		{
			// printf("<Read from I2CADR 0x%02X reg=0x%02X val=0x%02X>\n\r", I2CADR, dataBuffer[0], readBuffer[0]);
		}
	}
	else
	{
		printf("ERR <reading from I2CADR 0x%02X reg=0x%02X val=0x%02X>\n\r", I2CADR, dataBuffer[0], readBuffer[0]);
	}
	return readBuffer[0];
}

void ext_display_adapter_configure()
{
	read_LT8912_chip_ID();

#ifdef _pattern_test_
	pattern_test();
	while (1)
		;
#else

	DigitalClockEn();
	TxAnalog();
	CbusAnalog();
	HDMIPllAnalog();
	MipiAnalog();
	MipiBasicSet();
	DDSConfig();
	MIPI_Video_Setup(&video_800x600_60Hz);
	MIPI_Input_det();
	AudioIIsEn();

#ifdef dds_debug
	lt8912_check_dds(); // can be removed when debug stage.
#endif

#ifdef _lvds_bypass
	lvds_output_cfg();
	LvdsOutput(1);
#endif

	/////////////////////////////////////////////////////////////	������
	HdmiOutput(1);
	while (1)
		;
		/////////////////////////////////////////////////////////////

#ifdef _hdmi_output
	if (LT8912_Get_HPD())
	{
		if (!Tx_HPD)
		{
			HdmiOutput(1);
			Tx_HPD = 1;
			printf("\r\nLT8912_Get_HPD: high");
		}
	}
	else
	{
		if (Tx_HPD)
		{
			LT8912B_Suspend(1);
			printf("\r\nLT8912 Suspend!");
			HdmiOutput(0);
			Tx_HPD = 0;
			printf("\r\nLT8912_Get_HPD: low");
		}
	}
#endif

#endif

	while (1)
	{
		MIPI_Input_det();
		Cy_SysLib_Delay(1000);

#ifdef _hdmi_output
		if (LT8912_Get_HPD())
		{
			if (!Tx_HPD)
			{
				LT8912B_Suspend(0);
				HdmiOutput(1);
				Tx_HPD = 1;
			}
		}
		else
		{
			if (Tx_HPD)
			{
				LT8912B_Suspend(1);
				HdmiOutput(0);
				Tx_HPD = 0;
			}
		}
#endif
	}
}

void DigitalClockEn(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x02, 0xf7);
	ext_display_adapter_writebyte(0x08, 0xff);
	ext_display_adapter_writebyte(0x09, 0xff);
	ext_display_adapter_writebyte(0x0a, 0xff);
	ext_display_adapter_writebyte(0x0b, 0x7c);
	ext_display_adapter_writebyte(0x0c, 0xff);
}
void TxAnalog(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x31, 0xE1);
	ext_display_adapter_writebyte(0x32, 0xE1);
	ext_display_adapter_writebyte(0x33, 0x0c); // en/disable hdmid output
	ext_display_adapter_writebyte(0x37, 0x00);
	ext_display_adapter_writebyte(0x38, 0x22);
	ext_display_adapter_writebyte(0x60, 0x82);
}

void CbusAnalog(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x39, 0x45);
	ext_display_adapter_writebyte(0x3a, 0x00); // 20180719
	ext_display_adapter_writebyte(0x3b, 0x00);
}

void HDMIPllAnalog(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x44, 0x31);
	ext_display_adapter_writebyte(0x55, 0x44);
	ext_display_adapter_writebyte(0x57, 0x01);
	ext_display_adapter_writebyte(0x5a, 0x02);
}

void AviInfoframe(struct video_timing *video_format)
{
	uint8_t VIC = video_format->vic;
	uint8_t AR = video_format->aspact_ratio;
	uint8_t pb0, pb2, pb4, Sync_Polarity;

	I2CADR = 0x94;
	ext_display_adapter_writebyte(0x3c, 0x41); // enable null package

	Sync_Polarity = (video_format->h_polarity * 0x02) + (video_format->v_polarity * 0x01);
	pb2 = (AR << 4) + 0x08;
	pb4 = VIC;
	pb0 = (((pb2 + pb4) <= 0x5f) ? (0x5f - pb2 - pb4) : (0x15f - pb2 - pb4));

	I2CADR = 0x90;
	ext_display_adapter_writebyte(0xab, Sync_Polarity); // sync polarity

	I2CADR = 0x94;
	ext_display_adapter_writebyte(0x43, pb0);  // PB0:check sum
	ext_display_adapter_writebyte(0x44, 0x10); // PB1:RGB888
	ext_display_adapter_writebyte(0x45, pb2);  // PB2
	ext_display_adapter_writebyte(0x46, 0x00); // PB3
	ext_display_adapter_writebyte(0x47, pb4);  // PB4:vic
}

void MipiAnalog(void)
{
	I2CADR = 0x90;
#ifdef _pn_swap_
	ext_display_adapter_writebyte(0x3e, 0xf6); // P/N swap
#else
	ext_display_adapter_writebyte(0x3e, 0xd6); // if mipi pin map follow reference design, no need swap P/N.
#endif

	ext_display_adapter_writebyte(0x3f, 0xd4); // EQ
	ext_display_adapter_writebyte(0x41, 0x3c); // EQ
}

void MipiBasicSet(void)
{
	I2CADR = 0x92;
	ext_display_adapter_writebyte(0x10, 0x01); // term en
	ext_display_adapter_writebyte(0x11, 0x08); //0x10); // settle
	// ext_display_adapter_writebyte(0x12,0x08);  // trail
	ext_display_adapter_writebyte(0x13, lane_cnt); // 00 4 lane //01 1 lane //02 2 lane //03 3lane
	ext_display_adapter_writebyte(0x14, 0x00);	   // debug mux

#ifdef _lane_swap_							   // for EVB only, if mipi pin map follow reference design, no need swap lane.
	ext_display_adapter_writebyte(0x15, 0xa8); // lane swap:3210
	printf("\r\nmipi basic set: lane swap 3210, %bd", lane_cnt);
#else
	ext_display_adapter_writebyte(0x15, 0x00); // lane swap:0123
#endif

	ext_display_adapter_writebyte(0x1a, 0x03); // hshift 3
	ext_display_adapter_writebyte(0x1b, 0x03); // vshift 3
}

void MIPI_Video_Setup(struct video_timing *video_format)
{
	I2CADR = 0x92;
	ext_display_adapter_writebyte(0x18, (uint8_t)(video_format->hs % 256));		// hwidth
	ext_display_adapter_writebyte(0x19, (uint8_t)(video_format->vs % 256));		// vwidth
	ext_display_adapter_writebyte(0x1c, (uint8_t)(video_format->hact % 256));	// H_active[7:0]
	ext_display_adapter_writebyte(0x1d, (uint8_t)(video_format->hact / 256));	// H_active[15:8]
	ext_display_adapter_writebyte(0x2f, 0x0c);									// fifo_buff_length 12
	ext_display_adapter_writebyte(0x34, (uint8_t)(video_format->htotal % 256)); // H_total[7:0]
	ext_display_adapter_writebyte(0x35, (uint8_t)(video_format->htotal / 256)); // H_total[15:8]
	ext_display_adapter_writebyte(0x36, (uint8_t)(video_format->vtotal % 256)); // V_total[7:0]
	ext_display_adapter_writebyte(0x37, (uint8_t)(video_format->vtotal / 256)); // V_total[15:8]
	ext_display_adapter_writebyte(0x38, (uint8_t)(video_format->vbp % 256));	// VBP[7:0]
	ext_display_adapter_writebyte(0x39, (uint8_t)(video_format->vbp / 256));	// VBP[15:8]
	ext_display_adapter_writebyte(0x3a, (uint8_t)(video_format->vfp % 256));	// VFP[7:0]
	ext_display_adapter_writebyte(0x3b, (uint8_t)(video_format->vfp / 256));	// VFP[15:8]
	ext_display_adapter_writebyte(0x3c, (uint8_t)(video_format->hbp % 256));	// HBP[7:0]
	ext_display_adapter_writebyte(0x3d, (uint8_t)(video_format->hbp / 256));	// HBP[15:8]
	ext_display_adapter_writebyte(0x3e, (uint8_t)(video_format->hfp % 256));	// HFP[7:0]
	ext_display_adapter_writebyte(0x3f, (uint8_t)(video_format->hfp / 256));	// HFP[15:8]
	video = video_format;
}

void MIPIRxLogicRes(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x03, 0x7f); // mipi rx reset
	Cy_SysLib_Delay(10);
	ext_display_adapter_writebyte(0x03, 0xff);

	ext_display_adapter_writebyte(0x05, 0xfb); // dds reset
	Cy_SysLib_Delay(10);
	ext_display_adapter_writebyte(0x05, 0xff);
}

void DDSConfig(void)
{
	I2CADR = 0x92;
	ext_display_adapter_writebyte(0x4e, 0x93); // strm_sw_freq_word[ 7: 0]
	ext_display_adapter_writebyte(0x4f, 0x3E); // strm_sw_freq_word[15: 8]
	ext_display_adapter_writebyte(0x50, 0x29); // strm_sw_freq_word[23:16]
	ext_display_adapter_writebyte(0x51, 0x80); //[0]=strm_sw_freq_word[24]
	ext_display_adapter_writebyte(0x1e, 0x4f);
	ext_display_adapter_writebyte(0x1f, 0x5e); // full_value   464
	ext_display_adapter_writebyte(0x20, 0x01);
	ext_display_adapter_writebyte(0x21, 0x2c); // full_value1  416
	ext_display_adapter_writebyte(0x22, 0x01);
	ext_display_adapter_writebyte(0x23, 0xfa); // full_value2  400
	ext_display_adapter_writebyte(0x24, 0x00);
	ext_display_adapter_writebyte(0x25, 0xc8); // full_value3  384
	ext_display_adapter_writebyte(0x26, 0x00);
	ext_display_adapter_writebyte(0x27, 0x5e); // empty_value   464
	ext_display_adapter_writebyte(0x28, 0x01);
	ext_display_adapter_writebyte(0x29, 0x2c); // empty_value1  416
	ext_display_adapter_writebyte(0x2a, 0x01);
	ext_display_adapter_writebyte(0x2b, 0xfa); // empty_value2  400
	ext_display_adapter_writebyte(0x2c, 0x00);
	ext_display_adapter_writebyte(0x2d, 0xc8); // empty_value3  384
	ext_display_adapter_writebyte(0x2e, 0x00);
	ext_display_adapter_writebyte(0x42, 0x64); // tmr_set[ 7:0]:100us
	ext_display_adapter_writebyte(0x43, 0x00); // tmr_set[15:8]:100us
	ext_display_adapter_writebyte(0x44, 0x04); // timer step
	ext_display_adapter_writebyte(0x45, 0x00);
	ext_display_adapter_writebyte(0x46, 0x59);
	ext_display_adapter_writebyte(0x47, 0x00);
	ext_display_adapter_writebyte(0x48, 0xf2);
	ext_display_adapter_writebyte(0x49, 0x06);
	ext_display_adapter_writebyte(0x4a, 0x00);
	ext_display_adapter_writebyte(0x4b, 0x72);
	ext_display_adapter_writebyte(0x4c, 0x45);
	ext_display_adapter_writebyte(0x4d, 0x00);
	ext_display_adapter_writebyte(0x52, 0x08); // trend step
	ext_display_adapter_writebyte(0x53, 0x00);
	ext_display_adapter_writebyte(0x54, 0xb2);
	ext_display_adapter_writebyte(0x55, 0x00);
	ext_display_adapter_writebyte(0x56, 0xe4);
	ext_display_adapter_writebyte(0x57, 0x0d);
	ext_display_adapter_writebyte(0x58, 0x00);
	ext_display_adapter_writebyte(0x59, 0xe4);
	ext_display_adapter_writebyte(0x5a, 0x8a);
	ext_display_adapter_writebyte(0x5b, 0x00);
	ext_display_adapter_writebyte(0x5c, 0x34);
	ext_display_adapter_writebyte(0x51, 0x00);
}

void AudioIIsEn(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0xB2, 0x01); // DVI mode:0x00; HDMI mode:0x01;
	I2CADR = 0x94;
	ext_display_adapter_writebyte(0x06, 0x08);
	ext_display_adapter_writebyte(0x07, 0xF0);
	ext_display_adapter_writebyte(0x34, 0xD2); // 0xE2:32FS; 0xD2:64FS
	ext_display_adapter_writebyte(0x0F, 0x2B);
}

void AudioSpdifEn(void)
{
	//	I2CADR = 0x90;
	//	ext_display_adapter_writebyte(0xB2,0x01);
	I2CADR = 0x94;
	ext_display_adapter_writebyte(0x06, 0x0e);
	ext_display_adapter_writebyte(0x07, 0x00);
	ext_display_adapter_writebyte(0x34, 0xD2); // 0xE2:32FS; 0xD2:64FS
}

void Core_Pll_setup(struct panel_parameter *panel)
{
	uint8_t cpll_m, cpll_k1, cpll_k2; // cpll_k3;
	uint32_t temp;

	temp = (panel->pclk_khz * 7) / 25;
	cpll_m = temp / 1000;

	temp = (panel->pclk_khz * 7) / 25;
	temp = temp % 1000;
	temp = temp * 16.384;

	cpll_k1 = temp % 256;
	cpll_k2 = temp / 256;

	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x50, 0x24);	 // cp=50uA
	ext_display_adapter_writebyte(0x51, 0x05);	 // xtal_clk as reference,second order passive LPF PLL
	ext_display_adapter_writebyte(0x52, 0x14);	 // use second-order PLL
	ext_display_adapter_writebyte(0x69, cpll_m); // CP_PRESET_DIV_RATIO
	ext_display_adapter_writebyte(0x69, (cpll_m | 0x80));
	ext_display_adapter_writebyte(0x6c, (cpll_k2 | 0x80)); // RGD_CP_SOFT_K_EN,RGD_CP_SOFT_K[13:8]
	ext_display_adapter_writebyte(0x6b, cpll_k1);

	ext_display_adapter_writebyte(0x04, 0xfb); // core pll reset
	ext_display_adapter_writebyte(0x04, 0xff);
}

void Core_Pll_bypass(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x50, 0x24); // cp=50uA
	ext_display_adapter_writebyte(0x51, 0x2d); // Pix_clk as reference,second order passive LPF PLL
	ext_display_adapter_writebyte(0x52, 0x04); // loopdiv=0;use second-order PLL
	ext_display_adapter_writebyte(0x69, 0x0e); // CP_PRESET_DIV_RATIO
	ext_display_adapter_writebyte(0x69, 0x8e);
	ext_display_adapter_writebyte(0x6a, 0x00);
	ext_display_adapter_writebyte(0x6c, 0xb8); // RGD_CP_SOFT_K_EN,RGD_CP_SOFT_K[13:8]
	ext_display_adapter_writebyte(0x6b, 0x51);

	ext_display_adapter_writebyte(0x04, 0xfb); // core pll reset
	ext_display_adapter_writebyte(0x04, 0xff);
}

void Lvds_Pll_Reset(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x02, 0xf7); // lvds pll reset
	ext_display_adapter_writebyte(0x02, 0xff);
}

void Scaler_bypass(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x7f, 0x00); // disable scaler
	ext_display_adapter_writebyte(0xa8, 0x13);
}

void Scaler_setup(struct video_timing *input_video, struct panel_parameter *panel)
{
	// for example: 720P to 1280x800
	// These register base on MIPI resolution and LVDS panel resolution.
	uint32_t h_ratio, v_ratio;
	uint8_t i;
	uint32_t htotal;
	h_ratio = input_video->hact * 4096.00 / panel->hact;
	v_ratio = input_video->vact * 4096.00 / panel->vact;

	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x80, 0x00);
	ext_display_adapter_writebyte(0x81, 0xff);
	ext_display_adapter_writebyte(0x82, 0x03);
	ext_display_adapter_writebyte(0x83, (uint8_t)(input_video->hact % 256));
	ext_display_adapter_writebyte(0x84, (uint8_t)(input_video->hact / 256));
	ext_display_adapter_writebyte(0x85, 0x80);
	ext_display_adapter_writebyte(0x86, 0x10);
	ext_display_adapter_writebyte(0x87, (uint8_t)(panel->htotal % 256));
	ext_display_adapter_writebyte(0x88, (uint8_t)(panel->htotal / 256));
	ext_display_adapter_writebyte(0x89, (uint8_t)(panel->hs % 256));
	ext_display_adapter_writebyte(0x8a, (uint8_t)(panel->hbp % 256));
	ext_display_adapter_writebyte(0x8b, (uint8_t)(panel->vs % 256));
	ext_display_adapter_writebyte(0x8c, (uint8_t)(panel->hact % 256));
	ext_display_adapter_writebyte(0x8d, (uint8_t)(panel->vact % 256));
	ext_display_adapter_writebyte(0x8e, (uint8_t)(panel->vact / 256) * 16 + (panel->hact / 256));
	ext_display_adapter_writebyte(0x8f, (uint8_t)(h_ratio % 256));
	ext_display_adapter_writebyte(0x90, (uint8_t)(h_ratio / 256));
	ext_display_adapter_writebyte(0x91, (uint8_t)(v_ratio % 256));
	ext_display_adapter_writebyte(0x92, (uint8_t)(v_ratio / 256));
	ext_display_adapter_writebyte(0x7f, 0x96);
	// ext_display_adapter_writebyte(0x7f,0xb0);
	// ext_display_adapter_writebyte(0xa8,0x3b);
	ext_display_adapter_writebyte(0xa8, 0x13);

	ext_display_adapter_writebyte(0x02, 0xf7); // lvds pll reset
	ext_display_adapter_writebyte(0x02, 0xff);

	ext_display_adapter_writebyte(0x03, 0xcf); // scaler reset
	ext_display_adapter_writebyte(0x03, 0xff);

	ext_display_adapter_writebyte(0x7f, 0xb0);

	for (i = 0; i < 300; i++)
	{
		if (ext_display_adapter_readbyte(0xa7) & 0x20)
		{
			htotal = (ext_display_adapter_readbyte(0xa7) & 0x0f) * 0x100 + ext_display_adapter_readbyte(0xa6);
			printf("\r\n scaler setup htotal = ");
			printdec_u32(htotal);
			break;
			// please set "htotal" to panel_parameter's htotal.
		}
		printf("\r\n scaler loop = %d", i);
		Cy_SysLib_Delay(100);
	}
}
void LvdsPowerUp(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x44, 0x30);
	ext_display_adapter_writebyte(0x51, 0x05);
}
void LvdsPowerDown(void)
{
	I2CADR = 0x90;
	// ext_display_adapter_writebyte(0x44,0x31);
	ext_display_adapter_writebyte(0x51, 0x15); //
											   // ext_display_adapter_writebyte(0x03,0xfb);
}

void LvdsBypass(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x50, 0x24); // cp=50uA
	ext_display_adapter_writebyte(0x51, 0x2d); // Pix_clk as reference,second order passive LPF PLL
	ext_display_adapter_writebyte(0x52, 0x04); // loopdiv=0;use second-order PLL
	ext_display_adapter_writebyte(0x69, 0x0e); // CP_PRESET_DIV_RATIO
	ext_display_adapter_writebyte(0x69, 0x8e);
	ext_display_adapter_writebyte(0x6a, 0x00);
	ext_display_adapter_writebyte(0x6c, 0xb8); // RGD_CP_SOFT_K_EN,RGD_CP_SOFT_K[13:8]
	ext_display_adapter_writebyte(0x6b, 0x51);

	ext_display_adapter_writebyte(0x04, 0xfb); // core pll reset
	ext_display_adapter_writebyte(0x04, 0xff);

	ext_display_adapter_writebyte(0x7f, 0x00); // disable scaler
	ext_display_adapter_writebyte(0xa8, 0x13); // 0x13:VSEA ; 0x33:JEIDA;
}

void LvdsOutput(bool on)
{
	if (on)
	{
		I2CADR = 0x90;
		ext_display_adapter_writebyte(0x02, 0xf7); // lvds pll reset
		ext_display_adapter_writebyte(0x02, 0xff); // scaler module reset
		ext_display_adapter_writebyte(0x03, 0xcb); // lvds tx module reset
		ext_display_adapter_writebyte(0x03, 0xfb);
		ext_display_adapter_writebyte(0x03, 0xff);

		ext_display_adapter_writebyte(0x44, 0x30); // enbale lvds output

		printf("\r\nLT8912_lvds_output_enable!\r\n");
	}
	else
	{
		I2CADR = 0x90;
		ext_display_adapter_writebyte(0x44, 0x31);
	}
}

void HdmiOutput(bool on)
{
	if (on)
	{
		I2CADR = 0x90;
		ext_display_adapter_writebyte(0x33, 0x0e); // enable hdmi output
		printf("\r\nLT8912_hdmi_output_enable!\r\n");
	}
	else
	{
		I2CADR = 0x90;
		ext_display_adapter_writebyte(0x33, 0x0c); // disable hdmi output
		printf("\r\nLT8912_hdmi_output_disable!\r\n");
	}
}

void LvdsScalerResult(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x7f, 0xb0);
}

void ScalerReset(void)
{
	I2CADR = 0x90;
	ext_display_adapter_writebyte(0x03, 0xcf);
	ext_display_adapter_writebyte(0x03, 0xff);
}

void lt8912_check_dds(void)
{
	uint8_t reg_920c, reg_920d, reg_920e, reg_920f;
	uint8_t i;
	for (i = 0; i < 10; i++)
	{
		I2CADR = 0x92;
		reg_920c = ext_display_adapter_readbyte(0x0c);
		reg_920d = ext_display_adapter_readbyte(0x0d);
		reg_920e = ext_display_adapter_readbyte(0x0e);
		reg_920f = ext_display_adapter_readbyte(0x0f);
		printf("Check DDS 0x0c~0f = 0x%02X, 0x%02X, 0x%02X, 0x%02X \r\n", reg_920c, reg_920d, reg_920e, reg_920f);
		//		if((reg_920e == 0xd2)&&(reg_920d < 0xff)&&(reg_920d > 0xd0)) //shall update threshold here base on actual dds result.
		//		{
		//			printf("\r\nlvds_check_dds: stable!");
		//			break;
		//		}
		Cy_SysLib_Delay(500);
	}
}

void lvds_output_cfg(void)
{
	LvdsPowerUp();
#ifdef _lvds_bypass
	LvdsBypass();
#else
	// Core_Pll_setup(&AA070M0C1);
	Core_Pll_setup(&KD101FM_1_C001A);
	Scaler_setup(&video_1280x720_60Hz, &KD101FM_1_C001A);
#endif
}

void MIPI_Input_det(void)
{
	I2CADR = 0x90;
	Hsync_L = ext_display_adapter_readbyte(0x9c);
	Hsync_H = ext_display_adapter_readbyte(0x9d);
	Vsync_L = ext_display_adapter_readbyte(0x9e);
	Vsync_H = ext_display_adapter_readbyte(0x9f);

	if ((Hsync_H != Hsync_H_last) || (Vsync_H != Vsync_H_last)) /*hiht byte changed*/
	{
		printf("\r\nMIPI_Input_det 0x9c~9f = Hsync_H: 0x%02X, Hsync_L: 0x%02X, Vsync_H: 0x%02X, Vsync_L: 0x%02X\r\n", Hsync_H, Hsync_L, Vsync_H, Vsync_L);

		if (Vsync_H == 0x02 && Vsync_L <= 0xef && Vsync_L >= 0xec) // 0x2EE
		{
			video = &video_1280x720_60Hz;
			printf("\r\nvideoformat = VESA_1280x720_60\r\n");
		}
		else if (Vsync_H == 0x03 && Vsync_L <= 0x3a && Vsync_L >= 0x34) // 0x337
		{
			video = &video_1280x800_60Hz;
			printf("\r\nvideoformat = VESA_1280x800_60\r\n");
		}
		else if (Vsync_H == 0x04 && Vsync_L <= 0x67 && Vsync_L >= 0x63) // 0x465
		{
			video = &video_1920x1080_60Hz;
			printf("\r\nvideoformat = VESA_1920x1080_60\r\n");
		}
		else
		{
			printf("\r\nvideoformat = video_none\r\n");
		}

		Hsync_L_last = Hsync_L;
		Hsync_H_last = Hsync_H;
		Vsync_L_last = Vsync_L;
		Vsync_H_last = Vsync_H;

		MIPI_Video_Setup(video);
		AviInfoframe(video);
		MIPIRxLogicRes();
	}
}

// void dds_clock_debug(void)
//{
//	#ifdef dds_debug
//	uint8_t reg_920c, reg_920d, reg_920e, reg_920f;
//
//	while(1)
//	{
//		I2CADR = 0x92;
//		reg_920c = ext_display_adapter_readbyte(0x0c);
//		reg_920d = ext_display_adapter_readbyte(0x0d);
//		reg_920e = ext_display_adapter_readbyte(0x0e);
//		reg_920f = ext_display_adapter_readbyte(0x0f);
//
//		printf("\r\n0x0c~0e = %02bx, %02bx, %02bx",reg_920c, reg_920d, reg_920e);
//		printf("\r\nEnter the dds_clock_debug cycle");
//
//		Cy_SysLib_Delay(1000);
//	}
//	#endif
// }

void pattern_test(void)
{
	uint32_t DDS_initial_value;
	uint8_t pclk_Mhz;

	//	video = &video_640x480_60Hz;
	//	video = &video_720x480_60Hz;
		video = &video_800x600_60Hz;
	// video = &video_1024x768_60Hz;
	//	video = &video_1280x720_60Hz;
	// video = &video_1920x1080_60Hz;
	

	DigitalClockEn();
	TxAnalog();
	CbusAnalog();
	HDMIPllAnalog();
	AudioIIsEn();
	AviInfoframe(video);
	lvds_output_cfg();

	I2CADR = 0x92;
	//////////////////Pattern resolution set////////////////////
	ext_display_adapter_writebyte(0x72, 0x12);
	ext_display_adapter_writebyte(0x73, (uint8_t)((video->hs + video->hbp) % 256));									   // RGD_PTN_DE_DLY[7:0]
	ext_display_adapter_writebyte(0x74, (uint8_t)((video->hs + video->hbp) / 256));									   // RGD_PTN_DE_DLY[11:8]
	ext_display_adapter_writebyte(0x75, (uint8_t)((video->vs + video->vbp) % 256));									   // RGD_PTN_DE_TOP[6:0]
	ext_display_adapter_writebyte(0x76, (uint8_t)(video->hact % 256));												   // RGD_PTN_DE_CNT[7:0]
	ext_display_adapter_writebyte(0x77, (uint8_t)(video->vact % 256));												   // RGD_PTN_DE_LIN[7:0]
	ext_display_adapter_writebyte(0x78, ((((uint8_t)(video->vact / 256)) << 4) + ((uint8_t)(video->hact / 256))));	   // RGD_PTN_DE_LIN[10:8],RGD_PTN_DE_CNT[11:8]
	ext_display_adapter_writebyte(0x79, (uint8_t)(video->htotal % 256));											   // RGD_PTN_H_TOTAL[7:0]
	ext_display_adapter_writebyte(0x7a, (uint8_t)(video->vtotal % 256));											   // RGD_PTN_V_TOTAL[7:0]
	ext_display_adapter_writebyte(0x7b, ((((uint8_t)(video->vtotal / 256)) << 4) + ((uint8_t)(video->htotal / 256)))); // RGD_PTN_V_TOTAL[10:8],RGD_PTN_H_TOTAL[11:8]
	ext_display_adapter_writebyte(0x7c, (uint8_t)(video->hs % 256));												   // RGD_PTN_HWIDTH[7:0]
	ext_display_adapter_writebyte(0x7d, ((((uint8_t)(video->hs / 256)) << 6) + ((uint8_t)(video->vs % 256))));		   // RGD_PTN_HWIDTH[9:8],RGD_PTN_VWIDTH[5:0]
	ext_display_adapter_writebyte(0x70, 0x80);																		   // pattern enable
	ext_display_adapter_writebyte(0x71, 0x51);
	ext_display_adapter_writebyte(0x42, 0x12);
	///////////////////////////////////////////////////////////

	ext_display_adapter_writebyte(0x1e, 0x67); // h v d pol hdmi sel pll sel

	//////////////////pattern pixl clock set///////////////////
	pclk_Mhz = (uint8_t)(video->pclk_khz / 1000);
	DDS_initial_value = (uint32_t)(pclk_Mhz * 0x16C16);
	ext_display_adapter_writebyte(0x4e, (uint8_t)(DDS_initial_value & 0x000000ff));			// strm_sw_freq_word[ 7: 0]
	ext_display_adapter_writebyte(0x4f, (uint8_t)((DDS_initial_value & 0x0000ff00) >> 8));	// strm_sw_freq_word[15: 8]
	ext_display_adapter_writebyte(0x50, (uint8_t)((DDS_initial_value & 0x00ff0000) >> 16)); // strm_sw_freq_word[23:16]
	ext_display_adapter_writebyte(0x51, 0x80);

	//  ext_display_adapter_writebyte(0x4e,0x3E); //strm_sw_freq_word[ 7: 0]
	//	ext_display_adapter_writebyte(0x4f,0xE9); //strm_sw_freq_word[15: 8]
	//	ext_display_adapter_writebyte(0x50,0xD3); //strm_sw_freq_word[23:16]
	//	ext_display_adapter_writebyte(0x51,0x80); //pattern en
	///////////////////////////////////////////////////////////

	HdmiOutput(1);
	LvdsOutput(1);
}

uint8_t LT8912_Get_HPD(void)
{
	//(0x90)0xc1[bit7]������ǰHPD�Ǹ߻��ǵͣ�ʵʱ��⡣
	I2CADR = 0x90;
	if ((ext_display_adapter_readbyte(0xc1) & 0x80) == 0x80)
	{
		printf("\r\nLT8912_Get_HPD: high\r\n");
		return 1;
	}
	else
	{
		printf("\r\nLT8912_Get_HPD: low\r\n");
		return 0;
	}
}

void read_LT8912_chip_ID(void)
{
	I2CADR = 0x90;
	uint8_t chip_ID0 = ext_display_adapter_readbyte(0x00);
	uint8_t chip_ID1 = ext_display_adapter_readbyte(0x01);

	printf("\r\nLT8912 chip ID: 0x%02X, 0x%02X\r\n", chip_ID0, chip_ID1);
}

void LT8912B_Suspend(bool on)
{
	// 9mA,HPD detect is normal.
	if (on)
	{
		if (!suspend_on)
		{
			// enter suspend mode
			I2CADR = 0x90;
			ext_display_adapter_writebyte(0x54, 0x1d);
			ext_display_adapter_writebyte(0x51, 0x15);
			ext_display_adapter_writebyte(0x44, 0x31);
			ext_display_adapter_writebyte(0x41, 0xbd);
			ext_display_adapter_writebyte(0x5c, 0x11);
			suspend_on = 1;
			printf("\r\nsuspend on");
		}
	}
	else
	{
		if (suspend_on)
		{
			// exist suspend mode
			I2CADR = 0x90;
			ext_display_adapter_writebyte(0x5c, 0x10);
			ext_display_adapter_writebyte(0x54, 0x1c);
			ext_display_adapter_writebyte(0x51, 0x2d);
			ext_display_adapter_writebyte(0x44, 0x30);
			ext_display_adapter_writebyte(0x41, 0xbc);

			Cy_SysLib_Delay(10);
			ext_display_adapter_writebyte(0x03, 0x7f);
			Cy_SysLib_Delay(10);
			ext_display_adapter_writebyte(0x03, 0xff);

			ext_display_adapter_writebyte(0x05, 0xfb);
			Cy_SysLib_Delay(10);
			ext_display_adapter_writebyte(0x05, 0xff);
			suspend_on = 0;
			printf("\r\nsuspend off");
		}
	}
}

void handle_error(uint32_t status)
{
	if (status != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}
}

/* [] END OF FILE */
