/******************************************************************************
 * @file     startup_S6E2CCAJ0A.c
 * @brief    CMSIS-Core(M) Device Startup File for
 *           Device S6E2CCAJ0A
 * @version  V1.0.0
 * @date     20. January 2021
 ******************************************************************************/
/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include "s6e2ccxj.h"

/*---------------------------------------------------------------------------
  External References
 *---------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint32_t __STACK_SEAL;
#endif

extern __NO_RETURN void __PROGRAM_START(void);



/*---------------------------------------------------------------------------
  Internal References
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
__NO_RETURN void Default_Handler(void);

/* ToDo: Add Cortex exception handler according to the used Cortex-Core */



/*---------------------------------------------------------------------------
  Exception / Interrupt Handler
 *---------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak));
void MemManage_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
void SecureFault_Handler    (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler       (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

/* ToDo: Add your device specific interrupt handler */
void <DeviceInterrupt first>_Handler     (void) __attribute__ ((weak, alias("Default_Handler")));
...
void <DeviceInterrupt last>_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));


/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

/* ToDo: Add Cortex exception vectors according the used Cortex-Core */
extern const VECTOR_TABLE_Type __VECTOR_TABLE[<Device vector table entries>];

const VECTOR_TABLE_Type __VECTOR_TABLE[<Device vector table entries>] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),  /*     Initial Stack Pointer */
  Reset_Handler,                       /*     Reset Handler */
  NMI_Handler,                         /* -14 NMI Handler */
  HardFault_Handler,                   /* -13 Hard Fault Handler */
  MemManage_Handler,                   /* -12 MPU Fault Handler */
  BusFault_Handler,                    /* -11 Bus Fault Handler */
  UsageFault_Handler,                  /* -10 Usage Fault Handler */
  SecureFault_Handler,                 /*  -9 Secure Fault Handler */
  0,                                   /*     Reserved */
  0,                                   /*     Reserved */
  0,                                   /*     Reserved */
  SVC_Handler,                         /*  -5 SVCall Handler */
  DebugMon_Handler,                    /*  -4 Debug Monitor Handler */
  0,                                   /*     Reserved */
  PendSV_Handler,                      /*  -2 PendSV Handler */
  SysTick_Handler,                     /*  -1 SysTick Handler */

/* ToDo: Add your device specific interrupt vectors */
  /* Interrupts */

  CSV_IRQHandler,
  SWDT_IRQHandler,		
  LVD_IRQHandler,		
  IRQ003SEL_IRQHandler,
  IRQ004SEL_IRQHandler,
  IRQ005SEL_IRQHandler,
  IRQ006SEL_IRQHandler,
  IRQ007SEL_IRQHandler,
  IRQ008SEL_IRQHandler,
  IRQ009SEL_IRQHandler,
  IRQ010SEL_IRQHandler,
  EXINT0_IRQHandler,	
  EXINT1_IRQHandler,
  EXINT2_IRQHandler,	
  EXINT3_IRQHandler,	
  EXINT4_IRQHandler,	
  EXINT5_IRQHandler,	
  EXINT6_IRQHandler,	
  EXINT7_IRQHandler,	
  QPRC0_IRQHandler,	
  QPRC1_IRQHandler,	
  MFT0_WFG_DTIF_IRQHandle,
  MFT1_WFG_DTIF_IRQHandle,
  MFT2_WFG_DTIF_IRQHandle,
  MFT0_FRT_PEAK_IRQHandle,
  MFT0_FRT_ZERO_IRQHandle,
  MFT0_ICU_IRQHandler,
  MFT0_OCU_IRQHandler,	
  MFT1_FRT_PEAK_IRQHandle,
  MFT1_FRT_ZERO_IRQHandle,
  MFT1_ICU_IRQHandler,
  MFT1_OCU_IRQHandler,	
  MFT2_FRT_PEAK_IRQHandle,
  MFT2_FRT_ZERO_IRQHandle,
  MFT2_ICU_IRQHandler,
  MFT2_OCU_IRQHandler,	
  PPG00_02_04_IRQHandler,
  PPG08_10_12_IRQHandler,
  PPG16_18_20_IRQHandler,
  BT0_IRQHandler,
  BT1_IRQHandler,		
  BT2_IRQHandler,		
  BT3_IRQHandler,		
  BT4_IRQHandler,		
  BT5_IRQHandler,		
  BT6_IRQHandler,		
  BT7_IRQHandler,		
  DT_IRQHandler,	
  WC_IRQHandler,	
  EXTBUS_ERR_IRQHandler,
  RTC_IRQHandler,
  EXINT8_IRQHandler,
  EXINT9_IRQHandler,
  EXINT10_IRQHandler,	
  EXINT11_IRQHandler,	
  EXINT12_IRQHandler,	
  EXINT13_IRQHandler,	
  EXINT14_IRQHandler,	
  EXINT15_IRQHandler,	
  TIM_IRQHandler,	
  MFS0_RX_IRQHandler,
  MFS0_TX_IRQHandler,	
  MFS1_RX_IRQHandler,	
  MFS1_TX_IRQHandler,	
  MFS2_RX_IRQHandler,	
  MFS2_TX_IRQHandler,	
  MFS3_RX_IRQHandler,	
  MFS3_TX_IRQHandler,	
  MFS4_RX_IRQHandler,	
  MFS4_TX_IRQHandler,	
  MFS5_RX_IRQHandler,	
  MFS5_TX_IRQHandler,	
  MFS6_RX_IRQHandler,	
  MFS6_TX_IRQHandler,	
  MFS7_RX_IRQHandler,	
  MFS7_TX_IRQHandler,	
  ADC0_IRQHandler,	
  ADC1_IRQHandler,		
  USB0_F_IRQHandler,
  USB0_H_F_IRQHandler,
  CAN0_IRQHandler,	
  CAN1_CANFD0_IRQHandler,
  ETHER0_IRQHandler,
  DMAC0_IRQHandler,
  DMAC1_IRQHandler,	
  DMAC2_IRQHandler,	
  DMAC3_IRQHandler,	
  DMAC4_IRQHandler,	
  DMAC5_IRQHandler,	
  DMAC6_IRQHandler,	
  DMAC7_IRQHandler,	
  DSTC_IRQHandler,	
  EXINT16_19_IRQHandler,
  EXINT20_23_IRQHandler,
  EXINT24_27_IRQHandler,
  EXINT28_31_IRQHandler,
  QPRC2_IRQHandler,
  QPRC3_IRQHandler,	
  BT8_IRQHandler,	
  BT9_IRQHandler,		
  BT10_IRQHandler,		
  BT11_IRQHandler,		
  BT12_15_IRQHandler,
  MFS8_RX_IRQHandler,	
  MFS8_TX_IRQHandler,	
  MFS9_RX_IRQHandler,	
  MFS9_TX_IRQHandler,	
  MFS10_RX_IRQHandler,	
  MFS10_TX_IRQHandler,	
  MFS11_RX_IRQHandler,	
  MFS11_TX_IRQHandler,	
  ADC2_IRQHandler,	
  DSTC_HW_IRQHandler,
  USB1_F_IRQHandler,
  USB1_H_F_IRQHandler,
  HSSPI_IRQHandler,
  Default_Handler,	
  PCRC_I2S0_1_IRQHandler,
  SD_IRQHandler,
  FLASHIF_IRQHandler,
  MFS12_RX_IRQHandler,	
  MFS12_TX_IRQHandler,	
  MFS13_RX_IRQHandler,	
  MFS13_TX_IRQHandler,	
  MFS14_RX_IRQHandler,	
  MFS14_TX_IRQHandler,	
  MFS15_RX_IRQHandler,	
  MFS15_TX_IRQHandler

};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif



/*---------------------------------------------------------------------------
  Reset Handler called on controller reset
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
  __set_PSP((uint32_t)(&__INITIAL_SP));

/* ToDo: Initialize stack limit register for Armv8-M Main Extension based processors*/
  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
  __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

/* ToDo: Add stack sealing for Armv8-M based processors */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
#endif

  SystemInit();                    /* CMSIS System Initialization */
  __PROGRAM_START();               /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif



/*---------------------------------------------------------------------------
  Hard Fault Handler
 *---------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
  while(1);
}



/*---------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *---------------------------------------------------------------------------*/
void Default_Handler(void)
{
  while(1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif
