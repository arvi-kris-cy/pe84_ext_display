/*******************************************************************************
 * File Name        : infineon_logo_paths.h
 *
 * Description      : This header file contains vectored data path of Infineon
 *                    logo to be drawn by vglite API(s).
 *
 * Related Document : See README.md
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


#ifndef __INFINEON_LOGO_PATHS_H__
#define __INFINEON_LOGO_PATHS_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "vg_lite.h"


/*******************************************************************************
 * Macros
 *******************************************************************************/
#define PATH_COUNT                          (2U)
#define LEFT_COORD                          (0U)
#define TOP_COORD                           (0U)
#define RIGHT_COORD                         (100U)
#define BOTTOM_COORD                        (100U)


/*******************************************************************************
 * Global Variables
 *******************************************************************************/

/* path_data0 arc===========================================*/
int path_data0[] =
{
        3, 305.2, 372.9,
        9, 0, 0, -23.65, 3.864, -48.92, 10.39,
        9, -19.11, 4.908, -32.32, 9.033, -37.28, 10.6,
        5, 0, -0.1566,
        9, -7.257, 2.245, -14.31, 4.647, -21.25, 7.048,
        9, 7.048, 6.944, 11.49, 16.5, 11.85, 27.05,
        9, 5.639, -2.454, 11.33, -4.856, 17.18, -7.205,
        5, 0, -0.0522,
        9, 6.526, -2.35, 24.17, -8.771, 39.37, -13.89,
        9, 18.95, -6.422, 44.69, -14.31, 44.69, -14.31,
        9, 74.92, -20.68, 157.9, -32.68, 235.2, -32.11,
        9, -89.12, -5.326, -170.2, -0.1566, -240.8, 12.64,
        3, 709.3, 281.9,
        9, -80.2, 72.83, -262.9, 115.3, -471.6, 115.3,
        9, -283.5, 0, -466.7, -83.17, -479.5, -193.2,
        9, -4.751, -41.35, 27.83, -81.34, 81.97, -115.5,
        9, -9.555, -7.205, -15.77, -18.53, -16.03, -31.33,
        9, -78.16, 40.99, -122.4, 94.92, -122.4, 149.7,
        9, 0, 121.2, 216.8, 224, 538.9, 219.5,
        9, 219.1, -3.028, 385.6, -65.52, 469.8, -142,
        5, -1.253, -2.454,
        0
};


int path_data1[] =
{
        /* path_data1 i===========================================*/
        3, 140, 429.3,
        9, 0, -16.34, 13.21, -29.6, 29.55, -29.6,
        9, 16.34, 0, 29.6, 13.26, 29.6, 29.6,
        9, 0, 16.34, -13.26, 29.55, -29.6, 29.55,
        9, -16.34, 0, -29.55, -13.21, -29.55, -29.55,
        2, 156.5, 651.8,
        5, 0, -169.6,
        5, 27.88, -3.707,
        5, 0, 173.3,
        5, -27.88, 0,

        /* path2 n===========================================*/
        2, 278.1, 651.8,
        5, 0, -97.06,
        9, 0, -11.07, -0.992, -15.61, -3.185, -18.74,
        9, -1.984, -2.819, -5.43, -3.655, -9.137, -3.655,
        9, -5.378, 0, -13.47, 4.542, -20.15, 12.74,
        5, 0, 106.7,
        5, -27.72, 0,
        5, 0, -105.8,
        9, 0, -17.33, -2.245, -26.99, -3.707, -34.93,
        5, 26.05, -6.787,
        9, 0.731, 2.558, 3.185, 9.607, 3.446, 17.02,
        9, 7.101, -8.824, 20.15, -16.19, 32.89, -16.19,
        9, 9.868, 0, 17.7, 3.968, 22.61, 10.23,
        9, 5.691, 7.101, 6.631, 16.71, 6.631, 26.37,
        5, 0, 110.1,
        5, -27.72, 0,

        /* path3 f===========================================*/
        2, 396.5, 472.4,
        9, -3.185, -2.558, -8.354, -4.229, -13.26, -4.229,
        9, -6.892, 0, -11.07, 6.526, -11.07, 17.6,
        5, 0, 22.09,
        5, 26.05, 0,
        5, -5.169, 24.43,
        5, -20.88, 0,
        5, 0, 119.5,
        5, -28.51, 0,
        5, 0, -119.5,
        5, -9.555, 0,
        5, 0, -24.43,
        5, 9.555, 0,
        9, 0, 0, -0.4699, -9.92, -0.4699, -19,
        9, 0, -21.56, 7.101, -32.89, 13.73, -38.58,
        9, 6.683, -5.378, 14.25, -7.101, 21.41, -7.101,
        9, 10.29, 0, 18.69, 2.297, 26.05, 7.675,
        5, -7.884, 21.56,

        /* path4 i===========================================*/
        2, 419.6, 651.8,
        5, 0, -143,
        5, 28.25, -3.707,
        5, 0, 146.7,
        5, -28.25, 0,
        3, 13.52, -164,
        9, -10.08, 0, -18.17, -8.197, -18.17, -18.43,
        9, 0, -10.18, 8.354, -18.38, 18.64, -18.38,
        9, 10.08, 0, 18.22, 8.197, 18.22, 18.38,
        9, 0, 10.23, -8.354, 18.43, -18.69, 18.43,

        /* path5 n===========================================*/
        2, 543.6, 651.8,
        5, 0, -97.06,
        9, 0, -11.07, -0.992, -15.61, -3.185, -18.74,
        9, -1.984, -2.819, -5.43, -3.655, -9.137, -3.655,
        9, -5.378, 0, -13.47, 4.542, -20.15, 12.74,
        5, 0, 106.7,
        5, -27.72, 0,
        5, 0, -105.8,
        9, 0, -17.33, -2.245, -26.99, -3.707, -34.93,
        5, 26.05, -6.787,
        9, 0.731, 2.558, 3.185, 9.607, 3.446, 17.02,
        9, 7.101, -8.824, 20.15, -16.19, 32.89, -16.19,
        9, 9.868, 0, 17.7, 3.968, 22.61, 10.23,
        9, 5.691, 7.101, 6.631, 16.71, 6.631, 26.37,
        5, 0, 110.1,
        5, -27.72, 0,

        /* path6 e===========================================*/
        2, 660, 538,
        9, -2.715, -6.526, -7.153, -10.49, -14.25, -10.49,
        9, -12.53, 0, -17.96, 11.64, -18.43, 37.17,
        5, 35.87, 0,
        9, 0, -11.33, -0.992, -20.99, -3.185, -26.68,
        3, -32.68, 49.65,
        5, 0, 2.872,
        9, 0, 9.346, 0.7309, 17.86, 4.177, 25.27,
        9, 4.177, 8.771, 11.28, 13.63, 19.89, 13.63,
        9, 10.08, 0, 18.69, -5.117, 27.31, -14.2,
        5, 11.02, 20.68,
        9, -12.27, 13.37, -25.53, 19.32, -40.78, 19.32,
        9, -32.68, 0, -51.38, -28.09, -51.38, -75.76,
        9, 0, -24.7, 4.229, -42.87, 13.31, -56.49,
        9, 8.563, -12.74, 20.36, -19, 34.62, -19,
        9, 13.52, 0, 24.33, 5.43, 31.95, 15.35,
        9, 10.08, 13.05, 13.99, 30.65, 13.99, 62.13,
        5, 0, 6.213,
        5, -64.12, 0,

        /* path7 o===========================================*/
        2, 758.7, 529.5,
        9, -18.69, 0, -18.69, 34.04, -18.69, 48.82,
        9, 0, 15.61, -0.4699, 51.64, 19.42, 51.64,
        9, 18.43, 0, 18.43, -36.03, 18.43, -51.06,
        9, 0, -15.61, 0.731, -49.39, -19.16, -49.39,
        3, 0, 126,
        9, -38.32, 0, -48.4, -40.83, -48.66, -76.02,
        9, -0.2611, -35.5, 12.53, -75.5, 48.92, -75.5,
        9, 28.72, 0, 49.34, 25.84, 49.34, 74.92,
        9, 0, 36.6, -12.01, 76.59, -49.6, 76.59,

        /* path8 n===========================================*/
        2, 891.9, 651.8,
        5, 0, -97.06,
        9, 0, -11.07, -0.992, -15.61, -3.185, -18.74,
        9, -1.984, -2.819, -5.43, -3.655, -9.137, -3.655,
        9, -5.378, 0, -13.47, 4.542, -20.15, 12.74,
        5, 0, 106.7,
        5, -27.72, 0,
        5, 0, -105.8,
        9, 0, -17.33, -2.245, -26.99, -3.707, -34.93,
        5, 26.05, -6.787,
        9, 0.731, 2.558, 3.185, 9.607, 3.446, 17.02,
        9, 7.101, -8.824, 20.15, -16.19, 32.89, -16.19,
        9, 9.868, 0, 17.7, 3.968, 22.61, 10.23,
        9, 5.691, 7.101, 6.631, 16.71, 6.631, 26.37,
        5, 0, 110.1,
        5, -27.72, 0,
        0
};

vg_lite_path_t path[PATH_COUNT] = {
        {
                /* left, top, right, bottom */
                {LEFT_COORD, TOP_COORD, RIGHT_COORD, BOTTOM_COORD},
                VG_LITE_HIGH,       /* quality */
                VG_LITE_S32,        /* -128 to 127 coordinate range */
                {0},                /* uploaded */
                sizeof(path_data0), /* path length */
                path_data0,         /* path data */
                1                   /* initially, path is changed for uploaded */
        },
        {
                {LEFT_COORD, TOP_COORD, RIGHT_COORD, BOTTOM_COORD},
                VG_LITE_HIGH,
                VG_LITE_S32,
                {0},
                sizeof(path_data1),
                path_data1,
                1
        }
};

uint32_t color_data[] =
{
        /* path_data0 */   0xff4018ec,
        /* path_data1 */   0xffb36600,
};

#if defined(__cplusplus)
}
#endif

#endif  /* __INFINEON_LOGO_PATHS_H__ */

/* [] END OF FILE */
