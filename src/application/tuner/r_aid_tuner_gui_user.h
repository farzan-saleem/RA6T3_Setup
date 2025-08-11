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
 * File Name    : r_aid_tuner_gui_user.h
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <stdint.h>
#include "hal_data.h"

extern motor_cfg_t g_user_motor_cfg;
extern motor_encoder_extended_cfg_t g_user_motor_encoder_extended_cfg;
extern motor_speed_cfg_t g_user_motor_speed_cfg;
extern motor_speed_extended_cfg_t g_user_motor_speed_extended_cfg;
extern motor_current_cfg_t g_user_motor_current_cfg;
extern motor_current_extended_cfg_t g_user_motor_current_extended_cfg;
extern motor_angle_cfg_t g_user_motor_angle_cfg;
extern motor_sense_encoder_extended_cfg_t g_user_motor_sense_encoder_extended_cfg;
extern motor_driver_cfg_t g_user_motor_driver_cfg;
extern motor_driver_extended_cfg_t g_user_motor_driver_extended_cfg;
extern motor_position_cfg_t g_user_motor_position_cfg;
extern motor_position_extended_cfg_t g_user_motor_position_extended_cfg;

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/
#define AIDU_CURRENT_OMEGA               (g_user_motor_current_extended_cfg.p_design_parameter->f_current_omega)           /* Natural frequency of current loop */
#define AIDU_CURRENT_ZETA                (g_user_motor_current_extended_cfg.p_design_parameter->f_current_zeta)            /* Damping ratio of current loop */
#define AIDU_SPEED_OMEGA                 (g_user_motor_speed_extended_cfg.d_param.f_speed_omega)             /* Natural frequency of speed loop */
#define AIDU_SPEED_ZETA                  (g_user_motor_speed_extended_cfg.d_param.f_speed_zeta)              /* Damping ratio of speed loop */
#define AIDU_POS_OMEGA                   (g_user_motor_position_extended_cfg.f_pos_omega)
#define AIDU_SOB_OMEGA                   (g_user_motor_speed_extended_cfg.d_param.f_observer_omega)
#define AIDU_SOB_ZETA                    (g_user_motor_speed_extended_cfg.d_param.f_observer_zeta)
#define AIDU_INPUT_V                     (g_user_motor_driver_extended_cfg.mod_param.f4_vdc)            /* Damping ratio of PLL Speed estimate loop */
#define AIDU_SPEED_CTRL_PERIOD           (g_user_motor_speed_extended_cfg.f_speed_ctrl_period)                    /* The speed control period[s] */
#define AIDU_INT_DECIMATION              (0)
#define AIDU_INV_INFO_OVERVOLTAGE_TH     (g_user_motor_encoder_extended_cfg.f_overvoltage_limit)
#define AIDU_INV_INFO_UNDERVOLTAGE_TH    (g_user_motor_encoder_extended_cfg.f_lowvoltage_limit)
#define AIDU_INV_INFO_PWM_CYCLE_S        (g_user_motor_current_extended_cfg.f_current_ctrl_period/1000.0f)
#define AIDU_INV_INFO_PWM_DEADTIME_S     (g_user_motor_driver_extended_cfg.u2_deadtime/1000000.0f)
#define AIDU_CARRIER_SET_BASE            (g_user_motor_driver_extended_cfg.u2_pwm_timer_freq * 1000 / g_user_motor_driver_extended_cfg.u2_pwm_carrier_freq / 2)
#define AIDU_DEADTIME_SET                (g_user_motor_driver_extended_cfg.u2_deadtime)

#define AIDU_INV_INFO_OVERCURRENT_TH     (g_user_motor_encoder_extended_cfg.f_overcurrent_limit)
#define AIDU_INV_INFO_CURRENT_RANGE      (g_user_motor_driver_extended_cfg.f_current_range)

#define AIDU_PWM_TIMER_U_REG             (&(R_GPT1->GTCCR[2]))
#define AIDU_PWM_TIMER_V_REG             (&(R_GPT2->GTCCR[2]))
#define AIDU_PWM_TIMER_W_REG             (&(R_GPT3->GTCCR[2]))

#define AIDU_TUNE_VOLTERR_ENABLE         (true)

/***********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void aid_mtr_inv_set_uvw(float f4_duty_u, float f4_duty_v, float f4_duty_w, uint8_t u1_id);
void aid_mtr_get_current_iuiw(float *f4_iu_ad, float *f4_iw_ad, uint8_t u1_id);
float aid_mtr_get_vdc(uint8_t u1_id);
void aid_mtr_clear_oc_flag(void);
void aid_mtr_ctrl_start(uint8_t u1_id);
void aid_mtr_ctrl_stop(uint8_t u1_id);
void mtr_ics_1_parameter(void);
void aid_mtr_ics_interrupt(void);
