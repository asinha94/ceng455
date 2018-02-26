/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : fsl_mpu1.h
**     Project     : serial_echo
**     Processor   : MK64FN1M0VLL12
**     Component   : fsl_mpu
**     Version     : Component 1.3.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.3.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-25, 18:10, # CodeGen: 1
**     Contents    :
**         MPU_DRV_Init                      - mpu_status_t MPU_DRV_Init(uint32_t instance,const mpu_user_config_t *...
**         MPU_DRV_Deinit                    - void MPU_DRV_Deinit(uint32_t instance);
**         MPU_DRV_SetRegionConfig           - mpu_status_t MPU_DRV_SetRegionConfig(uint32_t instance,const...
**         MPU_DRV_SetRegionAddr             - void MPU_DRV_SetRegionAddr(uint32_t instance,mpu_region_num_t...
**         MPU_DRV_SetLowMasterAccessRights  - mpu_status_t MPU_DRV_SetLowMasterAccessRights(uint32_t...
**         MPU_DRV_SetHighMasterAccessRights - mpu_status_t MPU_DRV_SetHighMasterAccessRights(uint32_t...
**         MPU_DRV_SetRegionValidCmd         - void MPU_DRV_SetRegionValidCmd(uint32_t instance,mpu_region_num_t...
**         MPU_DRV_GetDetailErrorAccessInfo  - mpu_status_t MPU_DRV_GetDetailErrorAccessInfo(uint32_t...
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file fsl_mpu1.h
** @version 01.00
*/         
/*!
**  @addtogroup fsl_mpu1_module fsl_mpu1 module documentation
**  @{
*/         
#ifndef __fsl_mpu1_H
#define __fsl_mpu1_H
/* MODULE fsl_mpu1. */

/* Include inherited beans */
#include "clockMan1.h"
#include "Cpu.h"

/*! @brief Device instance number */
#define fsl_mpu1_IDX MPU_IDX
/*! @brief Device instance number for backward compatibility */
#define FSL_FSL_MPU1 fsl_mpu1_IDX


#endif
/* ifndef __fsl_mpu1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
