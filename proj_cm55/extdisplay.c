/*
 * extdisplay.c
 *
 *  Created on: 07-Feb-2024
 *      Author: RamanBalasat
 */


/*******************************************************************************
* Header Files
*******************************************************************************/
//#include "app_mem_map.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "extdisplay.h"

// #define _VesaJeidaMode 0x00
// #define _DE_Sync_mode 0x00

// #define _Hsync_polarity_active_high		0x02 // default, POSITIVE
// #define _Hsync_polarity_active_low		0x00 // NEGATIVE
// #define _Vsync_polarity_active_high		0x01 // default, POSITIVE
// #define _Vsync_polarity_active_low		0x00 // NEGATIVE
// #define _Hsync_polarity		_Hsync_polarity_active_high
// #define _Vsync_polarity		_Vsync_polarity_active_high

// #define _ColorDepth 0x10
/* I2C slave address */
#define I2C_ADDR_MAIN 0x48
#define I2C_ADDR_CEC_DSI 0x49

typedef enum
{
	I2C_MAIN,
	I2C_CEC_DSI,
	I2C_MAX_IDX
}i2c_addr_idx_e;

/* I2C Regmap */
uint32_t regmap[2] = {I2C_ADDR_MAIN, I2C_ADDR_CEC_DSI};

/*******************************************************************************
* Global Variables
*******************************************************************************/
cyhal_i2c_t mI2C;

enum
{
	H_act = 0,
	V_act,
	H_tol,
	V_tol,
	H_bp,
	H_sync,
	V_sync,
	V_bp
};

uint8_t	AVI_PB0	= 0x00;
uint8_t	AVI_PB1	= 0x00;
uint8_t	AVI_PB2	= 0x00;

uint8_t  vsync_setting;
uint8_t  hdmi_setting;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void read_test_data(void);

/*******************************************************************************
* Function Name: handle_error
********************************************************************************
* Summary:
* User defined error handling function
*
* Parameters:
*  uint32_t status - status indicates success or failure
*
* Return:
*  void
*
*******************************************************************************/
void handle_error(uint32_t status)
{
    if (status != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

int lt8912_write_lvds_config(void)
{
	int ret;
	// lvds power up
	ret=HDMI_Write(regmap[I2C_MAIN], 0x44, 0x30);
	ret=HDMI_Write(regmap[I2C_MAIN], 0x51, 0x05);

	// core pll bypass
	ret=HDMI_Write(regmap[I2C_MAIN], 0x50, 0x24); // cp=50uA
	ret=HDMI_Write(regmap[I2C_MAIN], 0x51, 0x2d); // Pix_clk as reference, second order passive LPF PLL
	ret=HDMI_Write(regmap[I2C_MAIN], 0x52, 0x04); // loopdiv=0, use second-order PLL
	ret=HDMI_Write(regmap[I2C_MAIN], 0x69, 0x0e); // CP_PRESET_DIV_RATIO
	ret=HDMI_Write(regmap[I2C_MAIN], 0x69, 0x8e);
	ret=HDMI_Write(regmap[I2C_MAIN], 0x6a, 0x00);
	ret=HDMI_Write(regmap[I2C_MAIN], 0x6c, 0xb8); // RGD_CP_SOFT_K_EN,RGD_CP_SOFT_K[13:8]
	ret=HDMI_Write(regmap[I2C_MAIN], 0x6b, 0x51);

	ret=HDMI_Write(regmap[I2C_MAIN], 0x04, 0xfb); // core pll reset
	ret=HDMI_Write(regmap[I2C_MAIN], 0x04, 0xff);

	// scaler bypass
	ret=HDMI_Write(regmap[I2C_MAIN], 0x7f, 0x00); // disable scaler
	ret=HDMI_Write(regmap[I2C_MAIN], 0xa8, 0x13); // 0x13: JEIDA, 0x33: VESA

	ret=HDMI_Write(regmap[I2C_MAIN], 0x02, 0xf7); // lvds pll reset
	ret=HDMI_Write(regmap[I2C_MAIN], 0x02, 0xff);
	ret=HDMI_Write(regmap[I2C_MAIN], 0x03, 0xcf);
	ret=HDMI_Write(regmap[I2C_MAIN], 0x03, 0xff);
    
	return ret;
}

int lt8912_write_rxlogicres_config(void)
{
	int ret;

	ret = HDMI_Write(regmap[I2C_MAIN], 0x03, 0x7f);
	Cy_SysLib_Delay(15);
	ret |= HDMI_Write(regmap[I2C_MAIN], 0x03, 0xff);

	return ret;
}

int lt8912_write_dds_config(void)
{
	int ret;

	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x4e, 0xff);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x4f, 0x56);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x50, 0x69);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x51, 0x80);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x1f, 0x5e);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x20, 0x01);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x21, 0x2c);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x22, 0x01);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x23, 0xfa);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x24, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x25, 0xc8);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x26, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x27, 0x5e);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x28, 0x01);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x29, 0x2c);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x2a, 0x01);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x2b, 0xfa);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x2c, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x2d, 0xc8);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x2e, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x42, 0x64);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x43, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x44, 0x04);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x45, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x46, 0x59);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x47, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x48, 0xf2);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x49, 0x06);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x4a, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x4b, 0x72);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x4c, 0x45);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x4d, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x52, 0x08);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x53, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x54, 0xb2);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x55, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x56, 0xe4);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x57, 0x0d);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x58, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x59, 0xe4);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x5a, 0x8a);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x5b, 0x00);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x5c, 0x34);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x1e, 0x4f);
	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x51, 0x00);

	return ret;
}

int lt8912_video_setup(void)
{
	uint32_t hactive, h_total, hpw, hfp, hbp;
	uint32_t vactive, v_total, vpw, vfp, vbp;
	uint8_t settle = 0x08;
	int ret, hsync_activehigh, vsync_activehigh;

	hactive = 800;
	hfp = 40;
	hpw = 128;
	hbp = 88;
	h_total = hactive + hfp + hpw + hbp;
	hsync_activehigh = 1;

	vactive = 600;
	vfp = 1;
	vpw = 4;
	vbp = 23;
	v_total = vactive + vfp + vpw + vbp;
	vsync_activehigh = 1;

	if (vactive <= 600)
		settle = 0x04;
	else if (vactive == 1080)
		settle = 0x0a;

	ret = HDMI_Write(regmap[I2C_CEC_DSI], 0x10, 0x01);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x11, settle);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x18, hpw);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x19, vpw);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x1c, hactive & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x1d, hactive >> 8);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x2f, 0x0c);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x34, h_total & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x35, h_total >> 8);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x36, v_total & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x37, v_total >> 8);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x38, vbp & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x39, vbp >> 8);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x3a, vfp & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x3b, vfp >> 8);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x3c, hbp & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x3d, hbp >> 8);

	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x3e, hfp & 0xff);
	ret |= HDMI_Write(regmap[I2C_CEC_DSI], 0x3f, hfp >> 8);

    HDMI_Read(regmap[I2C_MAIN], 0xab, vsync_setting);

    vsync_setting |= (vsync_activehigh ? BIT(0) : 0);
    vsync_setting |= (hsync_activehigh ? BIT(1) : 0);

    ret |= HDMI_Write(regmap[I2C_MAIN], 0xab, vsync_setting);
    HDMI_Read(regmap[I2C_MAIN], 0xb2, hdmi_setting);

    hdmi_setting |= BIT(0);
    ret |= HDMI_Write(regmap[I2C_MAIN], 0xb2, hdmi_setting);

	return ret;
}

int lt8912_video_on(void)
{
	int ret;

	ret = lt8912_video_setup();
	if (ret < 0)
		goto end;

	ret = lt8912_write_dds_config();
	if (ret < 0)
		goto end;

	ret = lt8912_write_rxlogicres_config();
	if (ret < 0)
		goto end;

	// ret = lt8912_write_lvds_config();
	// if (ret < 0)
	// 	goto end;

end:
	return ret;
}

void lt8912_write_mipi_basic_config()
{
	HDMI_Write(regmap[I2C_CEC_DSI], 0x12, 0x04);
	HDMI_Write(regmap[I2C_CEC_DSI], 0x14, 0x00);
	HDMI_Write(regmap[I2C_CEC_DSI], 0x15, 0x00);
	HDMI_Write(regmap[I2C_CEC_DSI], 0x1a, 0x03);
	HDMI_Write(regmap[I2C_CEC_DSI], 0x1b, 0x03);
}

void lt8912_write_init_config(void)
{
	/* Digital clock en*/
	HDMI_Write(regmap[I2C_MAIN], 0x08, 0xff);
	HDMI_Write(regmap[I2C_MAIN], 0x09, 0xff);
	HDMI_Write(regmap[I2C_MAIN], 0x0a, 0xff);
	HDMI_Write(regmap[I2C_MAIN], 0x0b, 0x7c);
	HDMI_Write(regmap[I2C_MAIN], 0x0c, 0xff);
	HDMI_Write(regmap[I2C_MAIN], 0x42, 0x04);

	/*Tx Analog*/
	HDMI_Write(regmap[I2C_MAIN], 0x31, 0xb1);
	HDMI_Write(regmap[I2C_MAIN], 0x32, 0xb1);
	HDMI_Write(regmap[I2C_MAIN], 0x33, 0x0e);
	HDMI_Write(regmap[I2C_MAIN], 0x37, 0x00);
	HDMI_Write(regmap[I2C_MAIN], 0x38, 0x22);
	HDMI_Write(regmap[I2C_MAIN], 0x60, 0x82);

	/*Cbus Analog*/
	HDMI_Write(regmap[I2C_MAIN], 0x39, 0x45);
	HDMI_Write(regmap[I2C_MAIN], 0x3a, 0x00);
	HDMI_Write(regmap[I2C_MAIN], 0x3b, 0x00);

	/*HDMI Pll Analog*/
	HDMI_Write(regmap[I2C_MAIN], 0x44, 0x31);
	HDMI_Write(regmap[I2C_MAIN], 0x55, 0x44);
	HDMI_Write(regmap[I2C_MAIN], 0x57, 0x01);
	HDMI_Write(regmap[I2C_MAIN], 0x5a, 0x02);

	/*MIPI Analog*/
	HDMI_Write(regmap[I2C_MAIN], 0x3e, 0xd6);
	HDMI_Write(regmap[I2C_MAIN], 0x3f, 0xd4);
	HDMI_Write(regmap[I2C_MAIN], 0x41, 0x3c);
	HDMI_Write(regmap[I2C_MAIN], 0xb2, 0x00);
}

void lt8912_soft_power_on(void)
{
	lt8912_write_init_config();
	HDMI_Write(regmap[I2C_CEC_DSI], 0x13, MIPI_LANE_INPUT_TWO & 3);

	lt8912_write_mipi_basic_config();
}

void lt8912_hard_power_on(void)
{
	cyhal_gpio_write(RESET_PIN,false);
	Cy_SysLib_Delay(20);
	cyhal_gpio_write(RESET_PIN, true);
}

void reset_board(void)
{
	/* Reset Board */
	cyhal_gpio_write(RESET_PIN, true);
	//cyhal_gpio_write(HPD_PIN, true);
	Cy_SysLib_Delay(50);
	cyhal_gpio_write(RESET_PIN, false);
	//cyhal_gpio_write(HPD_PIN, false);
	Cy_SysLib_Delay(150);
	cyhal_gpio_write(RESET_PIN, true);
	//cyhal_gpio_write(HPD_PIN, true);
	Cy_SysLib_Delay(120);
	//HDMI_Read(0x49,0x15,0x00); //lane swap
}

int display_init(void){
	    cy_rslt_t result;
	    cyhal_i2c_cfg_t mI2C_cfg;

   		/* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
   		printf("\x1b[2J\x1b[;H");
   		printf("I2C Master ->");
   		mI2C_cfg.is_slave = false;
   		mI2C_cfg.address = 0;
   		mI2C_cfg.frequencyhal_hz = I2C_FREQ;

   		/* Init I2C master */
   		result = cyhal_i2c_init(&mI2C, CYBSP_I2C_SDAA, CYBSP_I2C_SCLL, NULL);
   		/* I2C master init failed. Stop program execution */
        handle_error(result);

   		/* Configure I2C Master */
   		result = cyhal_i2c_configure(&mI2C, &mI2C_cfg);
   		/* I2C master configuration failed. Stop program execution */
   		handle_error(result);

   		Cy_GPIO_SetDriveSel(CYHAL_GET_PORTADDR(CYBSP_I2C_SCLL),
   				CYHAL_GET_PIN(CYBSP_I2C_SCLL), CY_GPIO_DRIVE_FULL);
   		Cy_GPIO_SetDriveSel(CYHAL_GET_PORTADDR(CYBSP_I2C_SDAA),
   				CYHAL_GET_PIN(CYBSP_I2C_SDAA), CY_GPIO_DRIVE_FULL);

  		result = cyhal_gpio_init(HPD_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false); //HPD
  		handle_error(result);
  		result = cyhal_gpio_init(RESET_PIN, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false); //RESET
  		handle_error(result);
		
   		printf("Done\r\n\n");

#if LINUX_REPO
	reset_board();
	//lt8912_hard_power_on();
	lt8912_soft_power_on();
	//test_1280x720();
	lt8912_video_on();
	//read_test_data();
#endif  

#if ORANGE_PI
     reset_board();
	 extdisp_config();
#endif

#if MSM
    reset_board();
    msm_display();
#endif

#if TEST_MODE
   reset_board();
   test_1280x720();
#endif

   	return 0;
}

int HDMI_Write( uint16_t i2c_slave,uint8_t reg_addr, uint8_t value)
{
	uint8_t dataBuffer[BUFFER_SIZE]= {reg_addr,value};
	 if (CY_RSLT_SUCCESS == cyhal_i2c_master_write(&mI2C, i2c_slave, dataBuffer, BUFFER_SIZE, 0, true))
	 {
		 printf("writebuffer: 0x%02X \n\r",dataBuffer[1]);

		//  HDMI_Read(i2c_slave,reg_addr,value);
	 }
	 else
	 {
		 handle_error(1);
	 }

	 return CY_RSLT_SUCCESS;
}

int HDMI_Read(uint16_t i2c_slave,uint8_t reg_addr,uint8_t value)
{

	uint8_t dataBuffer[BUFFER_SIZE]= {reg_addr};
	uint8_t readBuffer[1];
	 if (CY_RSLT_SUCCESS ==cyhal_i2c_master_write(&mI2C,i2c_slave, dataBuffer, 1, 0, true)){
	        	if (CY_RSLT_SUCCESS == cyhal_i2c_master_read(&mI2C,i2c_slave,readBuffer, 1, 0, true)){
	        	        	printf("readbuffer: 0x%02X \n\r",readBuffer[0]);
	        	}

	 }
	 else{
	 		 handle_error(1);
	 }
	 return CY_RSLT_SUCCESS;
}

void read_test_data(void)
{
	HDMI_Write(0x48,0xc1,0xc1);
	HDMI_Write(0x49,0xc1,0x00);
	HDMI_Read(0x48,0x9c,0x00);
	HDMI_Read(0x48,0x9d,0x00);
	HDMI_Read(0x48,0x9e,0x00);
	HDMI_Read(0x48,0x9f,0x00);
	HDMI_Read(0x49,0x0c,0x00);
	HDMI_Read(0x49,0x0d,0x00);
	HDMI_Read(0x49,0x0e,0x00);
	HDMI_Read(0x49,0x0f,0x00);
	HDMI_Read(0x49,0x13,0x00); //mipi lane 
	HDMI_Read(0x49,0x15,0x00); //lane swap
}

///////////////////////////////////////////////////////// TEST DISPLAY ////////////////////////////////////////////////////////////////////////
void test_1280x720(void)
{
	  	HDMI_Write(0x48,0x08,0xff);
		HDMI_Write(0x48,0x09,0xff);
		HDMI_Write(0x48,0x0a,0xff);
		HDMI_Write(0x48,0x0b,0xff);
		HDMI_Write(0x48,0x0c,0xff);

	//	TxAnalog(void)
		//I2CADR = 0x48;
		HDMI_Write(0x48,0x31,0xa1);
		HDMI_Write(0x48,0x32,0xa1);
		HDMI_Write(0x48,0x33,0x03);
		HDMI_Write(0x48,0x37,0x00);
		HDMI_Write(0x48,0x38,0x22);
		HDMI_Write(0x48,0x60,0x82);

	//	CbusAnalog(void)
		//I2CADR = 0x48;
		HDMI_Write(0x48,0x39,0x45);
		HDMI_Write(0x48,0x3b,0x00);


	//	HDMIPllAnalog(void)
		//I2CADR = 0x48;
		HDMI_Write(0x48,0x44,0x31);
		HDMI_Write(0x48,0x55,0x44);
		HDMI_Write(0x48,0x57,0x01);
		HDMI_Write(0x48,0x5a,0x02);

		HDMI_Write(0x48,0xb2,0x01);
	//--------------1080P test pattern------------------------//

		// Test pattern 1080P 60Hz
		//I2CADR = 0x49;
		HDMI_Write(0x49,0x72,0x12);
		HDMI_Write(0x49,0x73,0xc0);//RGD_PTN_DE_DLY[7:0]
		HDMI_Write(0x49,0x74,0x00);//RGD_PTN_DE_DLY[11:8]  192
		HDMI_Write(0x49,0x75,0x29);//RGD_PTN_DE_TOP[6:0]  41
		HDMI_Write(0x49,0x76,0x80);//RGD_PTN_DE_CNT[7:0]
		HDMI_Write(0x49,0x77,0x38);//RGD_PTN_DE_LIN[7:0]
		HDMI_Write(0x49,0x78,0x47);//RGD_PTN_DE_LIN[10:8],RGD_PTN_DE_CNT[11:8]
		HDMI_Write(0x49,0x79,0x98);//RGD_PTN_H_TOTAL[7:0]
		HDMI_Write(0x49,0x7a,0x65);//RGD_PTN_V_TOTAL[7:0]
		HDMI_Write(0x49,0x7b,0x48);//RGD_PTN_V_TOTAL[10:8],RGD_PTN_H_TOTAL[11:8]
		HDMI_Write(0x49,0x7c,0x2c);//RGD_PTN_HWIDTH[7:0]
		HDMI_Write(0x49,0x7d,0x05);//RGD_PTN_HWIDTH[9:8],RGD_PTN_VWIDTH[5:0]


		HDMI_Write(0x49,0x70,0x80);
		HDMI_Write(0x49,0x71,0x76);

		//148.5M CLK

    HDMI_Write(0x49,0x4e,0x33);
	HDMI_Write(0x49,0x4f,0x33);
	HDMI_Write(0x49,0x50,0xd3);
	HDMI_Write(0x49,0x51,0x80);
    Cy_SysLib_Delay(1000);

 }

///////////////////////////////////////////////////////// MSM DISPLAY ////////////////////////////////////////////////////////////////////////
void msm_display(void){

		//I2CADR = 0x90;                      // IIC address

		/* LVDS 配置 */
		HDMI_Write(0X48,0x08, 0xff );   // Register address : 0x08;	 Value : 0xff
		HDMI_Write(0X48,0x09, 0xff );
		HDMI_Write(0X48,0x0a, 0xff );
		HDMI_Write(0X48,0x0b, 0x7c );   //
		HDMI_Write(0X48,0x0c, 0xff );

		HDMI_Write(0X48,0x51, 0x15 );

		//I2CADR = 0x90;

		HDMI_Write(0X48,0x31, 0xa1 );
		HDMI_Write(0X48,0x32, 0xbf );
		HDMI_Write(0X48,0x33, 0x17 ); // bit0/bit1 =1 Turn On HDMI Tx；  bit0/bit1 = 0 Turn Off HDMI Tx
		HDMI_Write(0X48,0x37, 0x00 );
		HDMI_Write(0X48,0x38, 0x22 );
		HDMI_Write(0X48,0x60, 0x82 );

		//I2CADR = 0x90;
		HDMI_Write(0X48,0x39, 0x45 );
		HDMI_Write(0X48,0x3a, 0x00 );
		HDMI_Write(0X48,0x3b, 0x00 );

		//I2CADR = 0x90;
	//	HDMI_Write(0X48,0x3e, 0x96 + MIPI_Lane_PN_Swap );   //
		HDMI_Write(0X48,0x41, 0x7c );   //HS_eq current

		//I2CADR = 0x90;
		HDMI_Write(0X48,0x44, 0x31 );
		HDMI_Write(0X48,0x55, 0x44 );
		HDMI_Write(0X48,0x57, 0x01 );
		HDMI_Write(0X48,0x5a, 0x02 );

		//I2CADR = 0x49;
		HDMI_Write(0X49,0x10, 0x01 );               // 0x05
		HDMI_Write(0X49,0x11, 0x08 );               // 0x12
		HDMI_Write(0X49,0x12, 0x04 );
		HDMI_Write(0X49,0x13, MIPI_Lane % 0x04 );   // 00 4 lane  // 01 lane // 02 2lane                                            //03 3 lane
		HDMI_Write(0X49,0x14, 0x00 );

		HDMI_Write(0X49,0x15,0x00);  // 00: 0123 normal ; a8 : 3210 swap

		HDMI_Write(0X49,0x1a, 0x03 );
		HDMI_Write(0X49,0x1b, 0x03 );

		// MIPI DIGITAL
		//I2CADR = 0x49;
			HDMI_Write(0X49,0x18, (uint8_t)( MIPI_H_SyncWidth % 256 ) );     // hwidth
			HDMI_Write(0X49,0x19, (uint8_t)( MIPI_V_SyncWidth % 256 ) );     // vwidth
			HDMI_Write(0X49,0x1c, (uint8_t)( MIPI_H_Active % 256 ) );        // H_active[7:0]
			HDMI_Write(0X49,0x1d, (uint8_t)( MIPI_H_Active / 256 ) );        // H_active[15:8]

			HDMI_Write(0X49,0x1e, 0x67 );
			HDMI_Write(0X49,0x2f, 0x0c );

			HDMI_Write(0X49,0x34, (uint8_t)( MIPI_H_Total % 256 ) );         // H_total[7:0]
			HDMI_Write(0X49,0x35, (uint8_t)( MIPI_H_Total / 256 ) );         // H_total[15:8]

			HDMI_Write(0X49,0x36, (uint8_t)( MIPI_V_Total % 256 ) );         // V_total[7:0]
			HDMI_Write(0X49,0x37, (uint8_t)( MIPI_V_Total / 256 ) );         // V_total[15:8]

			HDMI_Write(0X49,0x38, (uint8_t)( MIPI_V_BackPorch % 256 ) );     // VBP[7:0]
			HDMI_Write(0X49,0x39, (uint8_t)( MIPI_V_BackPorch / 256 ) );     // VBP[15:8]
			HDMI_Write(0X49,0x3a, (uint8_t)( MIPI_V_FrontPorch % 256 ) );    // VFP[7:0]
			HDMI_Write(0X49,0x3b, (uint8_t)( MIPI_V_FrontPorch / 256 ) );    // VFP[15:8]
			HDMI_Write(0X49,0x3c, (uint8_t)( MIPI_H_BackPorch % 256 ) );     // HBP[7:0]
			HDMI_Write(0X49,0x3d, (uint8_t)( MIPI_H_BackPorch / 256 ) );     // HBP[15:8]
			HDMI_Write(0X49,0x3e, (uint8_t)( MIPI_H_FrontPorch % 256 ) );    // HFP[7:0]
			HDMI_Write(0X49,0x3f, (uint8_t)( MIPI_H_FrontPorch / 256 ) );    // HFP[15:8]

			//DDS CONFIG
			//I2CADR = 0x49;
			 HDMI_Write(0x49,0x4e, 0x52 );
	         HDMI_Write(0x49,0x4f, 0xde );
	         HDMI_Write(0x49,0x50, 0xc0 );
	         HDMI_Write(0x49,0x51, 0x80 );
			 HDMI_Write(0X49,0x51, 0x00 );

				HDMI_Write(0X49,0x1e, 0x4f );
				HDMI_Write(0X49,0x1f, 0x5e );
				HDMI_Write(0X49,0x20, 0x01 );
				HDMI_Write(0X49,0x21, 0x2c );
				HDMI_Write(0X49,0x22, 0x01 );
				HDMI_Write(0X49,0x23, 0xfa );
				HDMI_Write(0X49,0x24, 0x00 );
				HDMI_Write(0X49,0x25, 0xc8 );
				HDMI_Write(0X49,0x26, 0x00 );

				HDMI_Write(0X49,0x27, 0x5e );
				HDMI_Write(0X49,0x28, 0x01 );
				HDMI_Write(0X49,0x29, 0x2c );
				HDMI_Write(0X49,0x2a, 0x01 );
				HDMI_Write(0X49,0x2b, 0xfa );
				HDMI_Write(0X49,0x2c, 0x00 );
				HDMI_Write(0X49,0x2d, 0xc8 );
				HDMI_Write(0X49,0x2e, 0x00 );

				HDMI_Write(0X49,0x42, 0x64 );
				HDMI_Write(0X49,0x43, 0x00 );
				HDMI_Write(0X49,0x44, 0x04 );
				HDMI_Write(0X49,0x45, 0x00 );
				HDMI_Write(0X49,0x46, 0x59 );
				HDMI_Write(0X49,0x47, 0x00 );
				HDMI_Write(0X49,0x48, 0xf2 );
				HDMI_Write(0X49,0x49, 0x06 );
				HDMI_Write(0X49,0x4a, 0x00 );
				HDMI_Write(0X49,0x4b, 0x72 );
				HDMI_Write(0X49,0x4c, 0x45 );
				HDMI_Write(0X49,0x4d, 0x00 );
				HDMI_Write(0X49,0x52, 0x08 );
				HDMI_Write(0X49,0x53, 0x00 );
				HDMI_Write(0X49,0x54, 0xb2 );
				HDMI_Write(0X49,0x55, 0x00 );
				HDMI_Write(0X49,0x56, 0xe4 );
				HDMI_Write(0X49,0x57, 0x0d );
				HDMI_Write(0X49,0x58, 0x00 );
				HDMI_Write(0X49,0x59, 0xe4 );
				HDMI_Write(0X49,0x5a, 0x8a );
				HDMI_Write(0X49,0x5b, 0x00 );
				HDMI_Write(0X49,0x5c, 0x34 );

				HDMI_Write(0X49,0x51, 0x00 );

				//AVI CONFIG
				//I2CADR = 0x94;
					HDMI_Write(0X4A,0x3e, 0x0A );

					// 0x43寄存器是checksums，改变了0x45或者0x47 寄存器的值，0x43寄存器的值也要跟着变，
					// 0x43，0x44，0x45，0x47四个寄存器值的总和是0x6F

					uint8_t HDMI_VIC = 0x04; // 720P 60; Corresponding to the resolution to be output
					// HDMI_VIC = 0x10;                        // 1080P 60
					// HDMI_VIC = 0x1F; // 1080P 50
					// HDMI_VIC = 0x00; // If the resolution is non-standard, set to 0x00
					AVI_PB1 = 0x10;                         // PB1,color space: YUV444 0x70;YUV422 0x30; RGB 0x10
					AVI_PB2 = 0x2A;                         // PB2; picture aspect rate: 0x19:4:3 ;     0x2A : 16:9
					AVI_PB0 = ( ( AVI_PB1 + AVI_PB2 + HDMI_VIC ) <= 0x6f ) ? ( 0x6f - AVI_PB1 - AVI_PB2 - HDMI_VIC ) : ( 0x16f - AVI_PB1 - AVI_PB2 - HDMI_VIC );

					HDMI_Write(0X4A,0x43, AVI_PB0 );    //avi packet checksum ,avi_pb0
					HDMI_Write(0X4A,0x44, AVI_PB1 );    //avi packet output RGB 0x10
					HDMI_Write(0X4A,0x45, AVI_PB2 );    //0x19:4:3 ; 0x2A : 16:9
					HDMI_Write(0X4A,0x47, HDMI_VIC );   //VIC(as below);1080P60 : 0x10



					//I2CADR = 0x90;
						HDMI_Write(0X48,0x03, 0x7f );       // mipi rx reset
						Cy_SysLib_Delay(20);
						HDMI_Write(0X48,0x03, 0xff );
						HDMI_Write(0X48,0x05, 0xfb );       // DDS reset
						Cy_SysLib_Delay(20);
						HDMI_Write(0X48,0x05, 0xff );
}

///////////////////////////////////////////////////////// EXTERNAL DISPLAY ////////////////////////////////////////////////////////////////////////
void extdisp_config(void)
{
    static int MIPI_Timing[] = 
//	H_act	V_act	H_total	V_total	H_BP	H_sync	V_sync	V_BP
	{800,	600,	1056,	628,	88,	    128, 	4,	    23}; // 1280x720 Timing	
	//I2CADR = 0x90; // IIC address
	HDMI_Write(0x48,0x08,0xff);// Register address : 0x08; 	Value : 0xff
	HDMI_Write(0x48,0x09,0x81);
	HDMI_Write(0x48,0x0a,0xff);
	HDMI_Write(0x48,0x0b,0x64);//
	HDMI_Write(0x48,0x0c,0xff);
	
	HDMI_Read(0x48,0x08,0x00);
	HDMI_Read(0x48,0x09,0x00);
	HDMI_Read(0x48,0x0a,0x00);
	HDMI_Read(0x48,0x0b,0x00);
	HDMI_Read(0x48,0x0c,0x00);
				

	HDMI_Write(0x48,0x44,0x31);// Close LVDS ouput
	HDMI_Write(0x48,0x51,0x1f);

	//I2CADR = 0x90;
	HDMI_Write(0x48,0x31,0xa1);
	HDMI_Write(0x48,0x32,0xa1);
	HDMI_Write(0x48,0x33,0x03);// 0x03 Open HDMI Tx； 0x00 Close HDMI Tx
	HDMI_Write(0x48,0x37,0x00);
	HDMI_Write(0x48,0x38,0x22);
	HDMI_Write(0x48,0x60,0x82);

//------------------------------------------//
	//I2CADR = 0x90;
	HDMI_Write(0x48,0x39,0x45);
	HDMI_Write(0x48,0x3b,0x00);

//------------------------------------------//
	//I2CADR = 0x90;
	HDMI_Write(0x48,0x44,0x31);
	HDMI_Write(0x48,0x55,0x44);
	HDMI_Write(0x48,0x57,0x01);
	HDMI_Write(0x48,0x5a,0x02);

//------------------------------------------//
//	MipiBasicSet();
	//I2CADR = 0x92;
	HDMI_Write(0x49,0x10,0x01); // 0x05 
	HDMI_Write(0x49,0x11,0x08); // 0x12 
	HDMI_Write(0x49,0x12,0x04);  
	HDMI_Write(0x49,0x13,MIPI_Lane%0x04);  // 00 4 lane  // 01 lane // 02 2 lane //03 3 lane
	HDMI_Write(0x49,0x14,0x00);  
	HDMI_Write(0x49,0x15,0x00);
	HDMI_Write(0x49,0x1a,0x03);  
	HDMI_Write(0x49,0x1b,0x03);  

//------------------------------------------//
//	设置 MIPI Timing
	//I2CADR = 0x92;
	HDMI_Write(0x49,0x18,(uint8_t)(MIPI_Timing[H_sync]%256)); // hwidth
	HDMI_Write(0x49,0x19,(uint8_t)(MIPI_Timing[V_sync]%256)); // vwidth
	HDMI_Write(0x49,0x1c,(uint8_t)(MIPI_Timing[H_act]%256)); // H_active[7:0]
	HDMI_Write(0x49,0x1d,(uint8_t)(MIPI_Timing[H_act]/256)); // H_active[15:8]
                        
	HDMI_Write(0x49,0x1e,0x67); // hs/vs/de pol hdmi sel pll sel
	HDMI_Write(0x49,0x2f,0x0c); // fifo_buff_length 12
                       
	HDMI_Write(0x49,0x34,(uint8_t)(MIPI_Timing[H_tol]%256)); // H_total[7:0]
	HDMI_Write(0x49,0x35,(uint8_t)(MIPI_Timing[H_tol]/256)); // H_total[15:8]
	HDMI_Write(0x49,0x36,(uint8_t)(MIPI_Timing[V_tol]%256)); // V_total[7:0]
	HDMI_Write(0x49,0x37,(uint8_t)(MIPI_Timing[V_tol]/256)); // V_total[15:8]
	HDMI_Write(0x49,0x38,(uint8_t)(MIPI_Timing[V_bp]%256)); // VBP[7:0]
	HDMI_Write(0x49,0x39,(uint8_t)(MIPI_Timing[V_bp]/256)); // VBP[15:8]
	HDMI_Write(0x49,0x3a,(uint8_t)((MIPI_Timing[V_tol]-MIPI_Timing[V_act]-MIPI_Timing[V_bp]-MIPI_Timing[V_sync])%256)); // VFP[7:0]
	HDMI_Write(0x49,0x3b,(uint8_t)((MIPI_Timing[V_tol]-MIPI_Timing[V_act]-MIPI_Timing[V_bp]-MIPI_Timing[V_sync])/256)); // VFP[15:8]
	HDMI_Write(0x49,0x3c,(uint8_t)(MIPI_Timing[H_bp]%256)); // HBP[7:0]
	HDMI_Write(0x49,0x3d,(uint8_t)(MIPI_Timing[H_bp]/256)); // HBP[15:8]
	HDMI_Write(0x49,0x3e,(uint8_t)((MIPI_Timing[H_tol]-MIPI_Timing[H_act]-MIPI_Timing[H_bp]-MIPI_Timing[H_sync])%256)); // HFP[7:0]
	HDMI_Write(0x49,0x3f,(uint8_t)((MIPI_Timing[H_tol]-MIPI_Timing[H_act]-MIPI_Timing[H_bp]-MIPI_Timing[H_sync])/256)); // HFP[15:8]

//------------------------------------------//

	//I2CADR = 0x92;
	HDMI_Write(0x49,0x4e,0x52);
	HDMI_Write(0x49,0x4f,0xde);
	HDMI_Write(0x49,0x50,0xc0);
	HDMI_Write(0x49,0x51,0x80);
	HDMI_Write(0x49,0x51,0x00);

	HDMI_Write(0x49,0x1f,0x5e);
	HDMI_Write(0x49,0x20,0x01);
	HDMI_Write(0x49,0x21,0x2c);
	HDMI_Write(0x49,0x22,0x01);
	HDMI_Write(0x49,0x23,0xfa);
	HDMI_Write(0x49,0x24,0x00);
	HDMI_Write(0x49,0x25,0xc8);
	HDMI_Write(0x49,0x26,0x00);
	HDMI_Write(0x49,0x27,0x5e);
	HDMI_Write(0x49,0x28,0x01);
	HDMI_Write(0x49,0x29,0x2c);
	HDMI_Write(0x49,0x2a,0x01);
	HDMI_Write(0x49,0x2b,0xfa);
	HDMI_Write(0x49,0x2c,0x00);
	HDMI_Write(0x49,0x2d,0xc8);
	HDMI_Write(0x49,0x2e,0x00);

	//I2CADR = 0x90;
	HDMI_Write(0x48,0x03,0x7f);
	Cy_SysLib_Delay(10);
	HDMI_Write(0x48,0x03,0xff);

	//I2CADR = 0x92;
	HDMI_Write(0x49,0x51,0x00);

	HDMI_Write(0x49,0x42,0x64);
	HDMI_Write(0x49,0x43,0x00);
	HDMI_Write(0x49,0x44,0x04);
	HDMI_Write(0x49,0x45,0x00);
	HDMI_Write(0x49,0x46,0x59);
	HDMI_Write(0x49,0x47,0x00);
	HDMI_Write(0x49,0x48,0xf2);
	HDMI_Write(0x49,0x49,0x06);
	HDMI_Write(0x49,0x4a,0x00);
	HDMI_Write(0x49,0x4b,0x72);
	HDMI_Write(0x49,0x4c,0x45);
	HDMI_Write(0x49,0x4d,0x00);
	HDMI_Write(0x49,0x52,0x08);
	HDMI_Write(0x49,0x53,0x00);
	HDMI_Write(0x49,0x54,0xb2);
	HDMI_Write(0x49,0x55,0x00);
	HDMI_Write(0x49,0x56,0xe4);
	HDMI_Write(0x49,0x57,0x0d);
	HDMI_Write(0x49,0x58,0x00);
	HDMI_Write(0x49,0x59,0xe4);
	HDMI_Write(0x49,0x5a,0x8a);
	HDMI_Write(0x49,0x5b,0x00);
	
	HDMI_Write(0x49,0x5c,0x34);
	HDMI_Write(0x49,0x1e,0x4f);
	HDMI_Write(0x49,0x51,0x00);
	
	HDMI_Read(0x49,0x5c,0x00);
	HDMI_Read(0x49,0x1e,0x00);
	HDMI_Read(0x49,0x51,0x00);


//------------------------------------------//

//	AudioIIsEn(); // IIS Input
	//I2CADR = 0x90;
	HDMI_Write(0x48,0xB2,0x01);
	//I2CADR = 0x94;
	HDMI_Write(0x4a,0x06,0x08);
	HDMI_Write(0x4a,0x07,0xF0);
	HDMI_Write(0x4a,0x34,0xD2);
	
	HDMI_Read(0x4a,0x06,0x00);
	HDMI_Read(0x4a,0x07,0x00);
	HDMI_Read(0x4a,0x34,0x00);


//	AudioSPDIFEn(); // SPDIF Input
//	I2CADR = 0x90; 
//	HDMI_WriteI2C_Byte(0xB2,0x01);
//	I2CADR = 0x94;
//	HDMI_WriteI2C_Byte(0x06,0x0e);
//	HDMI_WriteI2C_Byte(0x07,0x00);
//	HDMI_WriteI2C_Byte(0x34,0xD2);

//------------------------------------------//

//	MIPIRxLogicRes();
	//I2CADR = 0x90;
	HDMI_Write(0x48,0x03,0x7f);
	Cy_SysLib_Delay(10);
	HDMI_Write(0x48,0x03,0xff);
//------------------------------------------//

#if 1//add by jst for test
	HDMI_Read(0x48,0x9c,0x00);
	HDMI_Read(0x48,0x9d,0x00);
	HDMI_Read(0x48,0x9e,0x00);
	HDMI_Read(0x48,0x9f,0x00);

	
#endif

#if 0
//check hpd
  HDMI_Write(0x48,0x0b,0x7c);// 要关掉一个clk

  //(0x90)0xc1[bit7]代表当前HPD是高还是低，实时检测。
  //I2CADR = 0x90;
  HDMI_Read(0x48,0xc1,0x00);
  
printk("HDMI_ReadI2C_Byte [0x90] 0x00[0]=%x\n",0x00[0]);  

  if((0x00[0] & 0x80)==0x80)
    printk("HDMI_ReadI2C_Byte check hpd ok!!!\n");
  else
    printk("HDMI_ReadI2C_Byte check hpd fail!!!\n");
    
  	HDMI_Write(0x48,0x0b,0x64);//  
#endif
  
}
/* [] END OF FILE */

