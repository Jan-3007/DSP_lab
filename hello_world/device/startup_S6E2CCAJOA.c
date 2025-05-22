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
#include "system_s6e2cc.h"

/*---------------------------------------------------------------------------
  External References
 *---------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);



/*---------------------------------------------------------------------------
  Internal References
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
__NO_RETURN void Default_Handler(void);



/*---------------------------------------------------------------------------
  Exception / Interrupt Handler
 *---------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler                (void) __attribute__ ((weak, alias("Default_Handler"), noreturn)); // why has arm left out "noreturn" ??? see https://github.com/ARM-software/CMSIS_6/issues/234
void HardFault_Handler          (void) __attribute__ ((weak));
void MemManage_Handler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BusFault_Handler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void UsageFault_Handler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void SVC_Handler                (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DebugMon_Handler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void PendSV_Handler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void SysTick_Handler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));

/* device specific interrupt handler */
void CSV_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void SWDT_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void LVD_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ003SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ004SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ005SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ006SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ007SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ008SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ009SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void IRQ010SEL_IRQHandler       (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT0_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT1_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT2_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT3_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT4_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT5_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT6_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT7_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void QPRC0_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void QPRC1_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT0_WFG_DTIF_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT1_WFG_DTIF_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT2_WFG_DTIF_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT0_FRT_PEAK_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT0_FRT_ZERO_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT0_ICU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT0_OCU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT1_FRT_PEAK_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT1_FRT_ZERO_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT1_ICU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT1_OCU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT2_FRT_PEAK_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT2_FRT_ZERO_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT2_ICU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFT2_OCU_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void PPG00_02_04_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void PPG08_10_12_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void PPG16_18_20_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT0_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT1_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT2_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT3_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT4_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT5_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT6_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT7_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DT_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void WC_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXTBUS_ERR_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void RTC_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT8_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT9_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT10_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT11_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT12_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT13_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT14_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT15_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void TIM_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS0_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS0_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS1_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS1_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS2_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS2_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS3_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS3_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS4_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS4_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS5_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS5_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS6_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS6_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS7_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS7_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void ADC0_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void ADC1_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void USB0_F_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void USB0_H_F_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void CAN0_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void CAN1_CANFD0_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void ETHER0_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC0_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC1_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC2_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC3_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC4_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC5_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC6_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DMAC7_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DSTC_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT16_19_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT20_23_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT24_27_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void EXINT28_31_IRQHandler      (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void QPRC2_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void QPRC3_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT8_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT9_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT10_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT11_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void BT12_15_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS8_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS8_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS9_RX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS9_TX_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS10_RX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS10_TX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS11_RX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS11_TX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void ADC2_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void DSTC_HW_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void USB1_F_IRQHandler          (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void USB1_H_F_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void HSSPI_IRQHandler           (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void PCRC_I2S0_1_IRQHandler     (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void SD_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void FLASHIF_IRQHandler         (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS12_RX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS12_TX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS13_RX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS13_TX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS14_RX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS14_TX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS15_RX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));
void MFS15_TX_IRQHandler        (void) __attribute__ ((weak, alias("Default_Handler"), noreturn));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

/* Cortex exception vectors according to the used Cortex-Core */

const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),  /*     Initial Stack Pointer */
  Reset_Handler,                       /*     Reset Handler */
  NMI_Handler,                         /* -14 NMI Handler */
  HardFault_Handler,                   /* -13 Hard Fault Handler */
  MemManage_Handler,                   /* -12 MPU Fault Handler */
  BusFault_Handler,                    /* -11 Bus Fault Handler */
  UsageFault_Handler,                  /* -10 Usage Fault Handler */
  0,                                   /*     Reserved */
  0,                                   /*     Reserved */
  0,                                   /*     Reserved */
  0,                                   /*     Reserved */
  SVC_Handler,                         /*  -5 SVCall Handler */
  DebugMon_Handler,                    /*  -4 Debug Monitor Handler */
  0,                                   /*     Reserved */
  PendSV_Handler,                      /*  -2 PendSV Handler */
  SysTick_Handler,                     /*  -1 SysTick Handler */

/* device specific interrupt vectors */

  CSV_IRQHandler,                      /* IRQ #0 */
  SWDT_IRQHandler,                     /* IRQ #1 */
  LVD_IRQHandler,                      /* IRQ #2 */
  IRQ003SEL_IRQHandler,                /* IRQ #3 */
  IRQ004SEL_IRQHandler,                /* IRQ #4 */
  IRQ005SEL_IRQHandler,                /* IRQ #5 */
  IRQ006SEL_IRQHandler,                /* IRQ #6 */
  IRQ007SEL_IRQHandler,                /* IRQ #7 */
  IRQ008SEL_IRQHandler,                /* IRQ #8 */
  IRQ009SEL_IRQHandler,                /* IRQ #9 */
  IRQ010SEL_IRQHandler,                /* IRQ #10 */
  EXINT0_IRQHandler,                   /* IRQ #11 */
  EXINT1_IRQHandler,                   /* IRQ #12 */
  EXINT2_IRQHandler,                   /* IRQ #13 */
  EXINT3_IRQHandler,                   /* IRQ #14 */
  EXINT4_IRQHandler,                   /* IRQ #15 */
  EXINT5_IRQHandler,                   /* IRQ #16 */
  EXINT6_IRQHandler,                   /* IRQ #17 */
  EXINT7_IRQHandler,	               /* IRQ #18 */
  QPRC0_IRQHandler,	                   /* IRQ #19 */
  QPRC1_IRQHandler,	                   /* IRQ #20 */
  MFT0_WFG_DTIF_IRQHandler,            /* IRQ #21 */
  MFT1_WFG_DTIF_IRQHandler,            /* IRQ #22 */
  MFT2_WFG_DTIF_IRQHandler,            /* IRQ #23 */
  MFT0_FRT_PEAK_IRQHandler,            /* IRQ #24 */
  MFT0_FRT_ZERO_IRQHandler,            /* IRQ #25 */
  MFT0_ICU_IRQHandler,                 /* IRQ #26 */
  MFT0_OCU_IRQHandler,	               /* IRQ #27 */
  MFT1_FRT_PEAK_IRQHandler,            /* IRQ #28 */
  MFT1_FRT_ZERO_IRQHandler,            /* IRQ #29 */
  MFT1_ICU_IRQHandler,                 /* IRQ #30 */
  MFT1_OCU_IRQHandler,	               /* IRQ #31 */
  MFT2_FRT_PEAK_IRQHandler,            /* IRQ #32 */
  MFT2_FRT_ZERO_IRQHandler,            /* IRQ #33 */
  MFT2_ICU_IRQHandler,                 /* IRQ #34 */
  MFT2_OCU_IRQHandler,	               /* IRQ #35 */
  PPG00_02_04_IRQHandler,              /* IRQ #36 */
  PPG08_10_12_IRQHandler,              /* IRQ #37 */
  PPG16_18_20_IRQHandler,              /* IRQ #38 */
  BT0_IRQHandler,                      /* IRQ #39 */
  BT1_IRQHandler,		               /* IRQ #40 */
  BT2_IRQHandler,		               /* IRQ #41 */
  BT3_IRQHandler,		               /* IRQ #42 */
  BT4_IRQHandler,		               /* IRQ #43 */
  BT5_IRQHandler,		               /* IRQ #44 */
  BT6_IRQHandler,		               /* IRQ #45 */
  BT7_IRQHandler,		               /* IRQ #46 */
  DT_IRQHandler,	                   /* IRQ #47 */
  WC_IRQHandler,	                   /* IRQ #48 */
  EXTBUS_ERR_IRQHandler,               /* IRQ #49 */
  RTC_IRQHandler,                      /* IRQ #50 */
  EXINT8_IRQHandler,                   /* IRQ #51 */
  EXINT9_IRQHandler,                   /* IRQ #52 */
  EXINT10_IRQHandler,	               /* IRQ #53 */
  EXINT11_IRQHandler,	               /* IRQ #54 */
  EXINT12_IRQHandler,	               /* IRQ #55 */
  EXINT13_IRQHandler,	               /* IRQ #56 */
  EXINT14_IRQHandler,	               /* IRQ #57 */
  EXINT15_IRQHandler,	               /* IRQ #58 */
  TIM_IRQHandler,	                   /* IRQ #59 */
  MFS0_RX_IRQHandler,                  /* IRQ #60 */
  MFS0_TX_IRQHandler,	               /* IRQ #61 */
  MFS1_RX_IRQHandler,	               /* IRQ #62 */
  MFS1_TX_IRQHandler,	               /* IRQ #63 */
  MFS2_RX_IRQHandler,	               /* IRQ #64 */
  MFS2_TX_IRQHandler,	               /* IRQ #65 */
  MFS3_RX_IRQHandler,	               /* IRQ #66 */
  MFS3_TX_IRQHandler,	               /* IRQ #67 */
  MFS4_RX_IRQHandler,	               /* IRQ #68 */
  MFS4_TX_IRQHandler,	               /* IRQ #69 */
  MFS5_RX_IRQHandler,	               /* IRQ #70 */
  MFS5_TX_IRQHandler,	               /* IRQ #71 */
  MFS6_RX_IRQHandler,	               /* IRQ #72 */
  MFS6_TX_IRQHandler,	               /* IRQ #73 */
  MFS7_RX_IRQHandler,	               /* IRQ #74 */
  MFS7_TX_IRQHandler,	               /* IRQ #75 */
  ADC0_IRQHandler,	                   /* IRQ #76 */
  ADC1_IRQHandler,		               /* IRQ #77 */
  USB0_F_IRQHandler,                   /* IRQ #78 */
  USB0_H_F_IRQHandler,                 /* IRQ #79 */
  CAN0_IRQHandler,	                   /* IRQ #80 */
  CAN1_CANFD0_IRQHandler,              /* IRQ #81 */
  ETHER0_IRQHandler,                   /* IRQ #82 */
  DMAC0_IRQHandler,                    /* IRQ #83 */
  DMAC1_IRQHandler,	                   /* IRQ #84 */
  DMAC2_IRQHandler,	                   /* IRQ #85 */
  DMAC3_IRQHandler,	                   /* IRQ #86 */
  DMAC4_IRQHandler,	                   /* IRQ #87 */
  DMAC5_IRQHandler,	                   /* IRQ #88 */
  DMAC6_IRQHandler,	                   /* IRQ #89 */
  DMAC7_IRQHandler,	                   /* IRQ #90 */
  DSTC_IRQHandler,	                   /* IRQ #91 */
  EXINT16_19_IRQHandler,               /* IRQ #92 */
  EXINT20_23_IRQHandler,               /* IRQ #93 */
  EXINT24_27_IRQHandler,               /* IRQ #94 */
  EXINT28_31_IRQHandler,               /* IRQ #95 */
  QPRC2_IRQHandler,                    /* IRQ #96 */
  QPRC3_IRQHandler,	                   /* IRQ #97 */
  BT8_IRQHandler,	                   /* IRQ #98 */
  BT9_IRQHandler,		               /* IRQ #99 */
  BT10_IRQHandler,		               /* IRQ #100 */
  BT11_IRQHandler,		               /* IRQ #101 */
  BT12_15_IRQHandler,                  /* IRQ #102 */
  MFS8_RX_IRQHandler,	               /* IRQ #103 */
  MFS8_TX_IRQHandler,	               /* IRQ #104 */
  MFS9_RX_IRQHandler,	               /* IRQ #105 */
  MFS9_TX_IRQHandler,	               /* IRQ #106 */
  MFS10_RX_IRQHandler,	               /* IRQ #107 */
  MFS10_TX_IRQHandler,	               /* IRQ #108 */
  MFS11_RX_IRQHandler,	               /* IRQ #109 */
  MFS11_TX_IRQHandler,	               /* IRQ #110 */
  ADC2_IRQHandler,	                   /* IRQ #111 */
  DSTC_HW_IRQHandler,                  /* IRQ #112 */
  USB1_F_IRQHandler,                   /* IRQ #113 */
  USB1_H_F_IRQHandler,                 /* IRQ #114 */
  HSSPI_IRQHandler,                    /* IRQ #115 */
  Default_Handler,	                   /* IRQ #116 - Reserved*/
  PCRC_I2S0_1_IRQHandler,              /* IRQ #117 */
  SD_IRQHandler,                       /* IRQ #118 */
  FLASHIF_IRQHandler,                  /* IRQ #119 */
  MFS12_RX_IRQHandler,	               /* IRQ #120 */
  MFS12_TX_IRQHandler,	               /* IRQ #121 */
  MFS13_RX_IRQHandler,	               /* IRQ #122 */
  MFS13_TX_IRQHandler,	               /* IRQ #123 */
  MFS14_RX_IRQHandler,	               /* IRQ #124 */
  MFS14_TX_IRQHandler,	               /* IRQ #125 */
  MFS15_RX_IRQHandler,	               /* IRQ #126 */
  MFS15_TX_IRQHandler                  /* IRQ #127 */
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
