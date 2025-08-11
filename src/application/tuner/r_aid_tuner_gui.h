/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_aid_tuner_gui.h
 * Description  : This module solves all the world's problems
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
*          : 09.12.2021 1.00
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

#ifndef APPLICATION_TUNER_R_AID_TUNER_GUI_H_
    #define APPLICATION_TUNER_R_AID_TUNER_GUI_H_
#include <stdint.h>

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#define AIDU_1_PARAMETER_SET         (1)
#define AIDU_SQRT_3                  (1.7320508f)
#define AIDU_SQRT_2                  (1.41421356f)
#define AIDU_TWOPI                   (3.14159265358979F * 2.0F)
#define AIDU_FLOAT_0_2               (0.2f)
#define AIDU_FLOAT_0_25              (0.25f)
#define AIDU_FLOAT_0_3               (0.3f)
#define AIDU_FLOAT_0_5               (0.5f)
#define AIDU_FLOAT_0_8               (0.8f)
#define AIDU_FLOAT_0_9               (0.9f)
#define AIDU_FLOAT_1_5               (1.5f)
#define AIDU_FLOAT_60_0              (60.0f)
#define AIDU_FLOAT_80_0              (80.0f)

#define AIDU_FLUG_RESET_MDOE0        (0)
#define AIDU_FLUG_RESET_MDOE1        (1)
#define AIDU_FLUG_RESET_MDOE2        (2)

#define AIDU_STATUS_READY            (0)
#define AIDU_STATUS_MEASURE          (1)
#define AIDU_STATUS_ERROR            (2)
#define AIDU_STATUS_RESET            (3)


#define AIDU_PWM_TICK_PER_IRQ        (AIDU_INT_DECIMATION + 1)

#define AIDU_TUNE_MODE_DEFAULT       (0)
#define AIDU_TUNE_MODE_START         (4)
#define AIDU_TUNE_MODE_MAX           (255)

#define AIDU_ERROR_STOP_BUTTON       (254)
#define AIDU_ERROR_ASSERT_FAIL       (255)

#define AIDU_PROGRESS_55             (0.55f)
#define AIDU_PROGRESS_64             (0.64f)
#define AIDU_PROGRESS_69             (0.69f)
#define AIDU_PROGRESS_75             (0.75f)

#define AIDU_MEASURE_PROGRESS_1      (1)
#define AIDU_MEASURE_PROGRESS_2      (2)
#define AIDU_MEASURE_PROGRESS_3      (3)
#define AIDU_MEASURE_PROGRESS_4      (4)

#define AIDU_TUNE_INERTIA_SETTING    (0.0f)
#define AIDU_TUNE_VOLTERR_SETTING    (2)
#define AIDU_INV_INFO_DUTY_MIN       (0.1f)
#define AIDU_INV_INFO_DUTY_MAX       (0.9f)

#define AIDU_TBL_COMP_V0             (0.477f)
#define AIDU_TBL_COMP_V1             (0.742f)
#define AIDU_TBL_COMP_V2             (0.892f)
#define AIDU_TBL_COMP_V3             (0.979f)
#define AIDU_TBL_COMP_V4             (1.009f)

#define AIDU_TBL_COMP_I0             (0.021f)
#define AIDU_TBL_COMP_I1             (0.034f)
#define AIDU_TBL_COMP_I2             (0.064f)
#define AIDU_TBL_COMP_I3             (0.158f)
#define AIDU_TBL_COMP_I4             (0.400f)

#define AIDU_REF_VOLTAGE             (24.0f)

#define AIDU_FLG_SET                 (1)

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void    R_TUNER_InitGUI (void);
void    R_TUNER_MainLoop (void);
uint8_t R_TUNER_IsRunning(void);
void    R_TUNER_SetTuneResult(void);
uint8_t R_TUNER_GetFlugReset(void);
void    R_TUNER_SetFlugReset(uint8_t);
void    rmw_apply_identified_params(void);
void    rmw_apply_reset(void);
#endif /* APPLICATION_TUNER_R_AID_TUNER_GUI_H_ */
