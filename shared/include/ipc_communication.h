/******************************************************************************
* File Name:   ipc_communication.h
*
* Description: This file contains definitions of constants and structures for
*              setting up user pipe and function prototypes for configuring
*              system and user IPC pipe.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2023, Cypress Semiconductor Corporation (an Infineon company) or
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
******************************************************************************/

#ifndef SOURCE_IPC_COMMUNICATION_H
#define SOURCE_IPC_COMMUNICATION_H

#define ENABLE_IPC_CM33_CM55

/* QUEUE1- command queue */
/* QUEUE2- msg queue */

#define CMD_CHANNEL_NUM                 CYHAL_IPC_CHAN_8
#define MSG_CHANNEL_NUM                 CYHAL_IPC_CHAN_9
#define QUEUE1_NUM                      (1UL)
#define QUEUE1_ITEMS                    (4UL)
#define QUEUE2_NUM                      (2UL)
#define QUEUE2_ITEMS                    (8UL)
#define IPC_QUEUE_IRQ_PRIORITY          (2U)

#define IPC_CMD_DEFAULT                 (0x80)
#define IPC_CMD_INIT                    (0x81)
#define IPC_CMD_START                   (0x82)
#define IPC_CMD_STOP                    (0x83)
#define IPC_CMD_STATUS                  (0x41)


#endif /* SOURCE_IPC_COMMUNICATION_H */

/* [] END OF FILE */
