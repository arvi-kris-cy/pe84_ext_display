/*
 * extdisplay.h
 *
 *  Created on: 07-Feb-2024
 *      Author: RamanBalasat
 */

#ifndef EXTDISPLAY_H_
#define EXTDISPLAY_H_

/*******************************************************************************
* Macros
*******************************************************************************/

/* I2C bus frequency */
#define I2C_FREQ                 (100000UL)

#define BUFFER_SIZE              (2UL)
//#define CM55_BOOT_WAIT_TIME_USEC (10U)
#define ADAPTER_I2C_SCL          (P17_0)
#define ADAPTER_I2C_SDA          (P17_1)
#define HPD_PIN                  (P15_0) //D8
#define RESET_PIN                (P11_1) //D7

#define MIPI_LANE_INPUT_TWO      (2)
#define TEST_MODE                (0)
#define LINUX_REPO               (0)
#define ORANGE_PI                (0)
#define MSM                      (1)

#define MSM_RegWrite HDMI_Write


// 根据前端MIPI输入信号的Timing修改以下宏定义的值:
// #define MIPI_H_Active	800
// #define MIPI_V_Active	600

// #define MIPI_H_Total	1056
// #define MIPI_V_Total	628

// #define MIPI_H_FrontPorch	40
// #define MIPI_H_SyncWidth	128
// #define MIPI_H_BackPorch	88

// #define MIPI_V_FrontPorch	1
// #define MIPI_V_SyncWidth	4
// #define MIPI_V_BackPorch	23

/* 1024x600 panel
#define Panel_Pixel_CLK 5125 // 51.25MHZ

#define Panel_H_Active	1024
#define Panel_V_Active	600

#define Panel_H_Total	1344
#define Panel_V_Total	635

#define Panel_H_FrontPorch	160
#define Panel_H_SyncWidth	20
#define Panel_H_BackPorch	140

#define Panel_V_FrontPorch	12
#define Panel_V_SyncWidth	3
#define Panel_V_BackPorch	20
*/

/* 1024x768 panel
#define Panel_Pixel_CLK 6500    // 65MHz

#define Panel_H_Active	1024
#define Panel_V_Active	768

#define Panel_H_Total	1344
#define Panel_V_Total	806

#define Panel_H_FrontPorch	24
#define Panel_H_SyncWidth	136
#define Panel_H_BackPorch	160

#define Panel_V_FrontPorch	3
#define Panel_V_SyncWidth	6
#define Panel_V_BackPorch	29
*/

// #define Panel_Pixel_CLK 5125 // 51.25MHZ

// #define MIPI_H_Active	1024
// #define MIPI_V_Active	600

// #define MIPI_H_Total	1344
// #define MIPI_V_Total	635

// #define MIPI_H_FrontPorch	160
// #define MIPI_H_SyncWidth	20
// #define MIPI_H_BackPorch	140

// #define MIPI_V_FrontPorch	12
// #define MIPI_V_SyncWidth	3
// #define MIPI_V_BackPorch	20

#define MIPI_Lane 2

#define MIPI_H_Active	640
#define MIPI_V_Active	480

#define MIPI_H_Total	800
#define MIPI_V_Total	525

#define MIPI_H_FrontPorch	16
#define MIPI_H_SyncWidth	64
#define MIPI_H_BackPorch	80

#define MIPI_V_FrontPorch	3
#define MIPI_V_SyncWidth	4
#define MIPI_V_BackPorch	13

// #define MIPI_H_Active	640
// #define MIPI_V_Active	480

// #define MIPI_H_Total	800
// #define MIPI_V_Total	525

// #define MIPI_H_FrontPorch	16
// #define MIPI_H_SyncWidth	96
// #define MIPI_H_BackPorch	48

// #define MIPI_V_FrontPorch	10
// #define MIPI_V_SyncWidth	2
// #define MIPI_V_BackPorch	33

// #define MIPI_H_Active	1024
// #define MIPI_V_Active	600

// #define MIPI_H_Total	1414
// #define MIPI_V_Total	655

// #define MIPI_H_FrontPorch	160
// #define MIPI_H_SyncWidth	70
// #define MIPI_H_BackPorch	160

// #define MIPI_V_FrontPorch	12
// #define MIPI_V_SyncWidth	20
// #define MIPI_V_BackPorch	23

// #define MIPI_H_Active	1024
// #define MIPI_V_Active	600

// #define MIPI_H_Total	1104
// #define MIPI_V_Total	618

// #define MIPI_H_FrontPorch	8
// #define MIPI_H_SyncWidth	32
// #define MIPI_H_BackPorch	40

// #define MIPI_V_FrontPorch	4
// #define MIPI_V_SyncWidth	8
// #define MIPI_V_BackPorch	6

// #define MIPI_H_Active	800
// #define MIPI_V_Active	600

// #define MIPI_H_Total	1056
// #define MIPI_V_Total	628

// #define MIPI_H_FrontPorch	40
// #define MIPI_H_SyncWidth	128
// #define MIPI_H_BackPorch	88

// #define MIPI_V_FrontPorch	1
// #define MIPI_V_SyncWidth	4
// #define MIPI_V_BackPorch	23

// #define MIPI_H_Active	1024
// #define MIPI_V_Active	768

// #define MIPI_H_Total	1344
// #define MIPI_V_Total	806

// #define MIPI_H_FrontPorch	24
// #define MIPI_H_SyncWidth	136
// #define MIPI_H_BackPorch	160

// #define MIPI_V_FrontPorch	3
// #define MIPI_V_SyncWidth	6
// #define MIPI_V_BackPorch	29

// #define MIPI_H_Active	1280
// #define MIPI_V_Active	720

// #define MIPI_H_Total	1650
// #define MIPI_V_Total	750

// #define MIPI_H_FrontPorch	110
// #define MIPI_H_SyncWidth	40
// #define MIPI_H_BackPorch	220

// #define MIPI_V_FrontPorch	5
// #define MIPI_V_SyncWidth	5
// #define MIPI_V_BackPorch	20
/*******************************************************************************
* Function Definitions
*******************************************************************************/
int HDMI_Write(uint16_t i2c_slave,uint8_t reg_addr, uint8_t value);
int HDMI_Read(uint16_t i2c_slave, uint8_t reg_addr);
void test_1280x720(void);
int ext_adapter_display_init(void);
void extdisp_config(void);
void msm_display(void);
void read_test_data(void);

#endif /* EXTDISPLAY_H_ */
