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
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,             
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
/** \file wm8731.h
 **
 ** I2S codec support functions
 **
 ** History:
 ** - 2015-01-09  1.0   EZh  First version for FM universal PDL
 **
 *****************************************************************************/

#ifndef __I2S_CODEC_WG8731_H__
#define __I2S_CODEC_WG8731_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "pdl_user.h"
#include "mfs.h"


#if (PDL_UTILITY_ENABLE_I2S_CODEC_WM8731 == PDL_ON)   
   
/**
 ******************************************************************************
 ** \defgroup Wm8731Group 
 **
 ** Provided WM8731 module functions:
 **
 ** - Wm8731_Init()
 ** - Wm8731_SetHpVolume()
 **
 ** Wm8731_Init() initializes all of WM8731 registers 
 **
 ** Wm8731_SetHpVolume() adjusts the volume of headphone.
 ** 
 ******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
  
//@{  

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/  
#define WM8731_ADDRESS             (0x1A)       // WM8731 chip address on I2C bus

#define WM8731_REG_LLINE_IN        (0x00)       // Left Channel Line Input Volume Control
#define WM8731_REG_RLINE_IN        (0x01)       // Right Channel Line Input Volume Control
#define WM8731_REG_LHPHONE_OUT     (0x02)       // Left Channel Headphone Output Volume Control
#define WM8731_REG_RHPHONE_OUT     (0x03)       // Right Channel Headphone Output Volume Control
#define WM8731_REG_ANALOG_PATH     (0x04)       // Analog Audio Path Control
#define WM8731_REG_DIGITAL_PATH    (0x05)       // Digital Audio Path Control
#define WM8731_REG_PDOWN_CTRL      (0x06)       // Power Down Control Register
#define WM8731_REG_DIGITAL_IF      (0x07)       // Digital Audio Interface Format
#define WM8731_REG_SAMPLING_CTRL   (0x08)       // Sampling Control Register
#define WM8731_REG_ACTIVE_CTRL     (0x09)       // Active Control
#define WM8731_REG_RESET           (0x0F)       // Reset register  
  
/******************************************************************************
 * WM8731 registers bit field
 ******************************************************************************/    
typedef struct stc_wm8731_llin_field
{
    uint16_t LINVOL   : 5;
    uint16_t RESERVED : 2;
    uint16_t LINMUTE  : 1;
    uint16_t LRINBOTH : 1;
    
}stc_wm8731_llin_field_t;

typedef struct stc_wm8731_rlin_field
{
    uint16_t RINVOL   : 5;
    uint16_t RESERVED : 2;
    uint16_t RINMUTE  : 1;
    uint16_t RINBOTH  : 1;
    
}stc_wm8731_rlin_field_t;
 
typedef struct stc_wm8731_lhout_field
{
    uint16_t LHPVOL   : 7;
    uint16_t LZCEN    : 1;
    uint16_t LRHPBOTH : 1;
    
}stc_wm8731_lhout_field_t;

typedef struct stc_wm8731_rhout_field
{
    uint16_t RHPVOL   : 7;
    uint16_t RZCEN    : 1;
    uint16_t RLHPBOTH : 1;
    
}stc_wm8731_rhout_field_t;

typedef struct stc_wm8731_aapc_field
{
    uint16_t MICBOOST   : 1;
    uint16_t MUTEMIC    : 1;
    uint16_t INSEL      : 1;
    uint16_t BYPASS     : 1;
    uint16_t DACSEL     : 1;
    uint16_t SIDETONE   : 1; 
    uint16_t SIDEATT    : 2;        
    
}stc_wm8731_aapc_field_t;

typedef struct stc_wm8731_dapc_field
{
    uint16_t ADCHPD   : 1;
    uint16_t DEEMP    : 2;
    uint16_t DACMU    : 1;
    uint16_t HPOR     : 1;  
    
}stc_wm8731_dapc_field_t;

typedef struct stc_wm8731_pdc_field
{
    uint16_t LINEINPD   : 1;
    uint16_t MICPD      : 1;
    uint16_t ADCPD      : 1;
    uint16_t DACPD      : 1;
    uint16_t OUTPD      : 1;
    uint16_t OSCPD      : 1;
    uint16_t CLKOUTPD   : 1;
    uint16_t POWEROFF   : 1;
    
}stc_wm8731_pdc_field_t;

typedef struct stc_wm8731_daif_field
{
    uint16_t FORMAT     : 2;
    uint16_t IWL        : 2;
    uint16_t LRP        : 1;
    uint16_t LRSWAP     : 1;
    uint16_t MS         : 1;
    uint16_t BCLKINV    : 1;
    
}stc_wm8731_daif_field_t;

typedef struct stc_wm8731_sc_field
{
    uint16_t NORMAL_USB : 1;
    uint16_t BOSR       : 1;
    uint16_t SR         : 4;
    uint16_t CLKDIV2    : 1;
    uint16_t CLKODIV2   : 1;
    
}stc_wm8731_sc_field_t;

typedef struct stc_wm8731_ac_field
{
    uint16_t ACTIVE : 1;
    
}stc_wm8731_ac_field_t;

/******************************************************************************
 * WM8731 registers structure
 ******************************************************************************/  
typedef struct stc_wm8731_reg
{
  union {
    __IO  uint16_t LLIN;                    ///< Left line in register
    stc_wm8731_llin_field_t LLIN_f;
  };
  
  union {
    __IO  uint16_t RLIN;                   ///< Right line in register
    stc_wm8731_rlin_field_t RLIN_f;
  };
    
  union {
    __IO  uint16_t LHOUT;                 ///< Left headphone out register
    stc_wm8731_lhout_field_t LHOUT_f;
  };
  
  union {
    __IO  uint16_t RHOUT;                 ///< Right headphone out register
    stc_wm8731_rhout_field_t RHOUT_f;
  };
  
  union {
    __IO  uint16_t AAPC;                  ///< Analog audio path control register
    stc_wm8731_aapc_field_t AAPC_f;
  };
  
  union {
    __IO  uint16_t DAPC;                 ///< Digital audio path control register
    stc_wm8731_dapc_field_t DAPC_f;
  };
  
  union {
    __IO  uint16_t PDC;                  ///< Power down control register
    stc_wm8731_pdc_field_t PDC_f;
  };
  
  union {
    __IO  uint16_t DAIF;                 ///< Digital audio interface format register
    stc_wm8731_daif_field_t DAIF_f;
  };
  
  union {
    __IO  uint16_t SC;                   ///< Sampling control register
    stc_wm8731_sc_field_t SC_f;
  };
  
  union {
    __IO  uint16_t AC;                   ///< Active control register
    stc_wm8731_ac_field_t AC_f;
  };
  
  __IO  uint16_t RESET;                ///< Reset register
  
}stc_wm8731_reg_t;
  
/**
 *****************************************************************************
 ** \defgroup I2sGroup Inter IC Sound (I2S)
 **
 ** \brief This section describes the interface for the I2S codec WM8731
 **
 ** This Codec must be configured via I2C before I2S can be used.
 **
 ** Provided functions of I2S module:
 ** - I2scodec_Init()
 *****************************************************************************/
//@{

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/**
 *****************************************************************************
 ** \brief I2S start condition type (read, write)
 *****************************************************************************/

/**
 *****************************************************************************
 ** \brief I2S sub frame configuration
 *****************************************************************************/

/**
 *****************************************************************************
 ** \brief I2S configuration
 *****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
en_result_t Wm8731_Init(volatile stc_mfsn_i2c_t* pstcI2c,
                        stc_wm8731_reg_t* pstcReg) ;
en_result_t Wm8731_SetHpVolume(uint16_t u16LeftVol, uint16_t u16RightVol);

void Wm8731_SendCmd(uint8_t u8RegAddr, uint16_t u16Cmd);


//@}
#ifdef __cplusplus
}
#endif

#endif

#endif /* __I2S_CODEC_WG8731_H__ */
/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
