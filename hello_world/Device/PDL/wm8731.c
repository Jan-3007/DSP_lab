/*************************************************************************************
* Copyright (C) 2013-2015, Cypress Semiconductor Corporation. All rights reserved.    
*                                                                                     
* This software, including source code, documentation and related                     
* materials ( "Software" ), is owned by Cypress Semiconductor                         
* Corporation ( "Cypress" ) and is protected by and subject to worldwide              
* patent protection (United States and foreign), United States copyright              
* laws and international treaty provisions. Therefore, you may use this               
* Software only as provided in the license agreement accompanying the                 
* software package from which you obtained this Software ( "EULA" ).                  
* If no EULA applies, Cypress hereby grants you a personal, non exclusive,
* non-transferable license to copy, modify, and compile the                           
* Software source code solely for use in connection with Cypress's                    
* integrated circuit products. Any reproduction, modification, translation,           
* compilation, or representation of this Software except as specified                 
* above is prohibited without the express written permission of Cypress.              
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                                
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                                
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                        
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                                     
* PARTICULAR PURPOSE. Cypress reserves the right to make                              
* changes to the Software without notice. Cypress does not assume any                 
* liability arising out of the application or use of the Software or any              
* product or circuit described in the Software. Cypress does not                      
* authorize its products for use in any products where a malfunction or               
* failure of the Cypress product may reasonably be expected to result in              
* significant property damage, injury or death ( "High Risk Product" ). By            
* including Cypress's product in a High Risk Product, the manufacturer                
* of such system or application assumes all risk of such use and in doing             
* so agrees to indemnify Cypress against all liability.                               
*/

/*****************************************************************************/
/** \file wm8731.c
 **
 ** I2S codec WM8731 driver API functions
 **
 ** History:
 ** - 2015-01-09  1.0   EZh  First version for FM universal PDL
 **
 *****************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "wm8731.h"
#include <string.h>   

#if (PDL_UTILITY_ENABLE_I2S_CODEC_WM8731 == PDL_ON)   
   
/**
 ******************************************************************************
 ** \addtogroup Wm8731Group
 ******************************************************************************/
//@{   

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/
volatile stc_mfsn_i2c_t* pstcI2cInstance;
uint16_t u16LeftHpOut, u16RightHpOut;
   
/******************************************************************************/
/* Local function prototypes                                                  */
/******************************************************************************/
   
/**
 ******************************************************************************
 ** \brief   Check the error status of I2C
 **
 ** \retval  TRUE                       No error
 ** \retval  FALSE                      I2C error occurs
 **                                     
 ******************************************************************************/
static boolean_t I2cCheckErrorStatus(void)
{    
    if(TRUE == Mfs_I2c_GetStatus(pstcI2cInstance, I2cBusErr))
    {
        return TRUE; /* Bus error occurs? */
    }
    
    if(TRUE == Mfs_I2c_GetStatus(pstcI2cInstance, I2cOverrunError))
    {
        return TRUE; /* Overrun error occurs? */
    }
    
    return FALSE;
}

/**
 ******************************************************************************
 ** \brief   Generate I2C start signal and send device address
 **
 ** \param   [in]  u8DevAddr          Device address
 **
 ** \retval  Ok                       I2C start normally
 ** \retval  ErrorTimeout             I2C timeout 
 ** \retval  Error                    Other errors
 **                                     
 ******************************************************************************/
static en_result_t I2cStart(uint8_t u8DevAddr)
{
    /* Prepare I2C device address */
    Mfs_I2c_SendData(pstcI2cInstance, u8DevAddr);
    
    /* Enable ACK */
    Mfs_I2c_ConfigAck(pstcI2cInstance, I2cAck);
    
    /* Generate I2C start signal */
    if(Ok != Mfs_I2c_GenerateStart(pstcI2cInstance))
    {
        return ErrorTimeout; /* Timeout or other error */
    }

    /* Wait until data transfer finish */
    while(1)
    {
        if(TRUE != Mfs_I2c_GetStatus(pstcI2cInstance, I2cRxTxIrq))
        {
            break;
        }
    }
    
    /* Check ACK */
    if(I2cNAck == Mfs_I2c_GetAck(pstcI2cInstance))
    {
        return Error;   /* NACK */
    }
    
    /* Check error status of I2C */
    if(TRUE == I2cCheckErrorStatus())
    {
        return Error;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Send I2C data
 **
 ** \param   [in]  pu8Data            Pointer to data address
 ** \param   [in]  u8Size             Data size
 **
 ** \retval  Ok                       I2C data send normally
 ** \retval  Error                    I2C NACK, bus error, overrun error
 **                                     
 ******************************************************************************/
static en_result_t I2cSendData(uint8_t* pu8Data, uint8_t u8Size) 
{
    uint8_t i = 0;
    
    for(i=0;i<u8Size;i++)
    {
        /* Wait for end of transmission */
        while(1)
        {
            if(TRUE == Mfs_I2c_GetStatus(pstcI2cInstance, I2cRxTxIrq))
            {
                break;
            }
        }
      
        /* Transmit the data */
        Mfs_I2c_SendData(pstcI2cInstance, pu8Data[i]);
        Mfs_I2c_ClrStatus(pstcI2cInstance, I2cRxTxIrq);
        
        /* Check until TX finish */
        while(1)
        {
            if(TRUE == Mfs_I2c_GetStatus(pstcI2cInstance, I2cTxEmpty))
            {
                break;
            }
        }
       
        /* Check ACK */
        if(I2cNAck == Mfs_I2c_GetAck(pstcI2cInstance))
        {
            return Error;   /* NACK */
        }
        
        if(TRUE == I2cCheckErrorStatus())
        {
            return Error;
        }
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Generate I2C stop signal
 **
 ** \retval  Ok                       I2C stop normally
 ** \retval  ErrorTimeout             I2C timeout 
 ** \retval  Error                    Other errors
 **                                     
 ******************************************************************************/
static en_result_t I2cStop(void)
{
    /* Generate I2C start signal */
    if(Ok != Mfs_I2c_GenerateStop(pstcI2cInstance))
    {
        return ErrorTimeout; /* Timeout or other error */
    }
    /* Clear Stop condition flag */
    while(1)
    {
        if(TRUE == Mfs_I2c_GetStatus(pstcI2cInstance, I2cStopDetect))
        {
            break;
        }
    }
   
    if(TRUE == Mfs_I2c_GetStatus(pstcI2cInstance, I2cBusErr))
    {
        return Error;
    }
   
    Mfs_I2c_ClrStatus(pstcI2cInstance, I2cStopDetect);
    Mfs_I2c_ClrStatus(pstcI2cInstance, I2cRxTxIrq);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Send command to WM8731
 **
 ** \param u8RegAddr Regist address
 ** \param u16Cmd    Command data
 **
 ** \return   Ok   Coded initialized
 ******************************************************************************/
void Wm8731_SendCmd(uint8_t u8RegAddr, uint16_t u16Cmd){

  // B[15:9] Are Control Address Bits
  // B[8:0]  Are Control Data Bits
  uint8_t u8Data[2];
  
  u8Data[0] = (uint8_t)((u8RegAddr << 1) | ((u16Cmd>>8) & 0x1));
  u8Data[1] = (uint8_t)(u16Cmd & 0xFF);
  
  I2cStart(WM8731_ADDRESS << 1 ); // Start I2C2 module
  I2cSendData(u8Data, 2);         // Write data
  I2cStop();                      // Stop I2C2 module
}

/**
 ******************************************************************************
 ** \brief Initialize WM8731 via I2C
 **
 ** \return   Ok   Codec initialized
 ******************************************************************************/
en_result_t Wm8731_Init(volatile stc_mfsn_i2c_t* pstcI2c,
                        stc_wm8731_reg_t* pstcReg)
{
    stc_mfs_i2c_config_t stcI2cConfig;
    
    if ((NULL == pstcI2c) || (NULL == pstcReg))
    {
        return ErrorInvalidParameter;
    }
    
    PDL_ZERO_STRUCT(stcI2cConfig);
    
    // Store I2C channel
    pstcI2cInstance = pstcI2c;
    
    stcI2cConfig.enMsMode = I2cMaster;
    stcI2cConfig.u32BaudRate = 400000u;
    stcI2cConfig.bWaitSelection = FALSE;
    stcI2cConfig.bDmaEnable = FALSE;
    stcI2cConfig.pstcFifoConfig = NULL;
    
    if(Ok != Mfs_I2c_Init(pstcI2cInstance, &stcI2cConfig))
    {
        return Error;
    }  
    
    Wm8731_SendCmd(WM8731_REG_RESET,         pstcReg->RESET);   // Reset module
    Wm8731_SendCmd(WM8731_REG_PDOWN_CTRL,    0x12);      		// Power down control, all active except line out and mic
    Wm8731_SendCmd(WM8731_REG_LLINE_IN,      pstcReg->LLIN);    // Left line in settings
    Wm8731_SendCmd(WM8731_REG_RLINE_IN,      pstcReg->RLIN);    // Right line in settings
    Wm8731_SendCmd(WM8731_REG_LHPHONE_OUT,   pstcReg->LHOUT);   // Left headphone out settings
    Wm8731_SendCmd(WM8731_REG_RHPHONE_OUT,   pstcReg->RHOUT);   // Right headphone out settings
    Wm8731_SendCmd(WM8731_REG_ANALOG_PATH,   pstcReg->AAPC);    // Analog paths
    Wm8731_SendCmd(WM8731_REG_DIGITAL_PATH,  pstcReg->DAPC);    // Digital paths
    Wm8731_SendCmd(WM8731_REG_DIGITAL_IF,    pstcReg->DAIF);    // Digital interface
    Wm8731_SendCmd(WM8731_REG_SAMPLING_CTRL, pstcReg->SC);      // Sampling control
    Wm8731_SendCmd(WM8731_REG_ACTIVE_CTRL,   pstcReg->AC);      // Activate module
    Wm8731_SendCmd(WM8731_REG_PDOWN_CTRL,    pstcReg->PDC);     // Power down control
    
    u16LeftHpOut = pstcReg->LHOUT;   // Store left headphone out data
    u16RightHpOut = pstcReg->RHOUT;  // Store right headphone out data
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set volume of headphone
 **
 ** \return   Ok   Codec initialized
 ******************************************************************************/
en_result_t Wm8731_SetHpVolume(uint16_t u16LeftVol, uint16_t u16RightVol)
{
    u16LeftHpOut = u16LeftHpOut & 0xC0u;
    u16RightHpOut = u16RightHpOut & 0xC0u;
    
    u16LeftHpOut |= u16LeftVol;
    u16RightHpOut |= u16RightVol;
  
    Wm8731_SendCmd(WM8731_REG_LHPHONE_OUT, u16LeftHpOut);
    Wm8731_SendCmd(WM8731_REG_RHPHONE_OUT, u16RightHpOut); 

    return Ok;
}


//@} // Wm8731Group  

#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
