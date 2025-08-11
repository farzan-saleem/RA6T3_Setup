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
 * File Name    : r_aid_tuner_gui_user.c
 **********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_aid_tuner_gui_user.h"
#include "r_aid_auto_identify.h"
#include "r_aid_tuner_gui.h"
#include "hal_data.h"

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
extern float    com_f4_speed_limit_rpm;               /* Over speed limit [rpm] */
extern uint16_t com_u2_mtr_pp;                        /* Pole pairs */
extern float    com_f4_mtr_r;                         /* Resistance [ohm] */
extern float    com_f4_mtr_ld;                        /* D-axis inductance [H] */
extern float    com_f4_mtr_lq;                        /* Q-axis inductance [H] */
extern float    com_f4_mtr_m;                         /* Permanent magnetic flux [Wb] */
extern float    com_f4_mtr_j;                         /* Rotor inertia [kgm^2] */
extern float    com_f4_ol_ref_id;                     /* Id reference when open loop [A] */
extern float    com_f4_current_omega;                 /* Natural frequency for current loop [Hz] */
extern float    com_f4_current_zeta;                  /* Damping ratio for current loop */
extern float    com_f4_speed_omega;                   /* Natural frequency for speed loop [Hz] */
extern float    com_f4_speed_zeta;                    /* Damping ratio for speed loop */
extern float    com_f4_pos_omega;
extern float    com_f4_sob_omega;
extern float    com_f4_sob_zeta;
extern float    com_f4_ref_speed_rpm;
extern float    com_f4_max_speed_rpm;
extern float    com_f4_overcurrent_limit;             /* Over current limit */
extern float    com_f4_iq_limit;                      /* Q-axis current limit */
extern int8_t   com_u1_enable_write;

extern float    gui_f4_r;
extern float    gui_f4_ld;
extern float    gui_f4_lq;
extern float    gui_f4_ke;
extern float    gui_f4_j;
extern uint8_t  gui_u1_flag_tune_mode;
extern float    gui_f4_slide_parameter;
extern uint8_t  gui_u1_active_gui;
extern uint8_t  gui_u1_flg_1para_init;
extern float    gui_f4_1para_speed_omega;
extern float    gui_f4_1para_pos_omega;

/***********************************************************************************************************************
* Function Name : aid_mtr_inv_set_uvw
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void aid_mtr_inv_set_uvw(float f4_duty_u, float f4_duty_v, float f4_duty_w, uint8_t u1_id)
{
    three_phase_duty_cycle_t temp_duty;

    uint16_t                 u2_temp_base  = g_motor_driver0_ctrl.u2_carrier_base;
    uint16_t                 u2_temp_deadt = g_motor_driver0_ctrl.u2_deadtime_count;

    temp_duty.duty[0] = (uint32_t) ((u2_temp_base * (1.0F - f4_duty_u)) + (u2_temp_deadt * 0.5F));
    temp_duty.duty[1] = (uint32_t) ((u2_temp_base * (1.0F - f4_duty_v)) + (u2_temp_deadt * 0.5F));
    temp_duty.duty[2] = (uint32_t) ((u2_temp_base * (1.0F - f4_duty_w)) + (u2_temp_deadt * 0.5F));
    g_motor_driver0_ctrl.p_cfg->p_three_phase_instance->p_api->dutyCycleSet(g_motor_driver0_ctrl.p_cfg->p_three_phase_instance->p_ctrl, &temp_duty);
}

/***********************************************************************************************************************
* Function Name : aid_mtr_get_current_iuiw
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void aid_mtr_get_current_iuiw(float *f4_iu_ad, float *f4_iw_ad, uint8_t u1_id)
{
    *f4_iu_ad = g_motor_driver0_ctrl.f_iu_ad;
    *f4_iw_ad = g_motor_driver0_ctrl.f_iw_ad;
}

/***********************************************************************************************************************
* Function Name : aid_mtr_get_vdc
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
float aid_mtr_get_vdc(uint8_t u1_id)
{
    return (g_motor_driver0_ctrl.f_vdc_ad);
}

/***********************************************************************************************************************
* Function Name : mtr_clear_oc_flag
* Description   : Clears forced cutoff flag (detect over current)
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void aid_mtr_clear_oc_flag(void)
{
    R_POEG_Reset(g_poeg0.p_ctrl);
} /* End of function mtr_clear_oc_flag */

/***********************************************************************************************************************
* Function Name : mtr_ctrl_start
* Description   : Starts PWM output
* Arguments     : u1_id - Motor ID
* Return Value  : None
***********************************************************************************************************************/
void aid_mtr_ctrl_start(uint8_t u1_id)
{
    g_motor_encoder0.p_api->run(g_motor_encoder0.p_ctrl);
} /* End of function mtr_ctrl_start */

/***********************************************************************************************************************
* Function Name : mtr_ctrl_stop
* Description   : Stops PWM output disable for motor control
* Arguments     : u1_id - Motor ID
* Return Value  : None
***********************************************************************************************************************/
void aid_mtr_ctrl_stop(uint8_t u1_id)
{
    g_motor_encoder0.p_api->stop(g_motor_encoder0.p_ctrl);
} /* End of function mtr_ctrl_stop */

/***********************************************************************************************************************
* Function Name : rmw_apply_identified_params
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void rmw_apply_identified_params(void)
{
    st_aid_id_setting_t id_setting;
    float ia_max;
    float va_max;

    R_AID_GetIDSetting(&id_setting);
    com_f4_overcurrent_limit = id_setting.f4_rated_current;
    ia_max = com_f4_overcurrent_limit * (AIDU_SQRT_3 / AIDU_SQRT_2);
    va_max = AIDU_INPUT_V * (AIDU_SQRT_3 / AIDU_SQRT_2) * AIDU_FLOAT_0_5 * AIDU_FLOAT_0_9;
    com_u2_mtr_pp = id_setting.u2_num_pole_pairs;
    com_f4_mtr_r = gui_f4_r;
    com_f4_mtr_ld = gui_f4_ld;
    com_f4_mtr_lq = gui_f4_lq;
    com_f4_mtr_m = gui_f4_ke;
    com_f4_mtr_j = gui_f4_j;

    com_f4_max_speed_rpm = (va_max / gui_f4_ke) / (float)com_u2_mtr_pp / AIDU_TWOPI * AIDU_FLOAT_60_0;
    com_f4_speed_limit_rpm = com_f4_max_speed_rpm * AIDU_FLOAT_1_5;
    com_f4_ol_ref_id = ia_max * AIDU_FLOAT_0_8;
    com_f4_ref_speed_rpm = com_f4_max_speed_rpm / 2;
    com_f4_iq_limit = com_f4_overcurrent_limit;

    com_u1_enable_write ^= 1;
} /* End of function rmw_apply_identified_params() */

/***********************************************************************************************************************
* Function Name : rmw_apply_reset
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void rmw_apply_reset(void)
{
    g_motor_encoder0.p_api->reset(g_motor_encoder0.p_ctrl);
}

/***********************************************************************************************************************
* Function Name : mtr_ics_1_parameter
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void mtr_ics_1_parameter(void)
{
    uint8_t u1_temp;
    
    /* for 1 parameter */
    u1_temp = gui_u1_active_gui & 0x04;
    if ((0x04 == u1_temp) && (AIDU_1_PARAMETER_SET == gui_u1_flag_tune_mode))
    {
        if (gui_u1_flg_1para_init != AIDU_1_PARAMETER_SET)
        {
            gui_f4_1para_speed_omega = AIDU_SPEED_OMEGA;
            gui_f4_1para_pos_omega = AIDU_POS_OMEGA;
            gui_u1_flg_1para_init = AIDU_1_PARAMETER_SET;
        }
        if (gui_f4_slide_parameter > 100.0f)
        {
            gui_f4_slide_parameter = 100.0f;
        }
        else if (gui_f4_slide_parameter < 0)
        {
            gui_f4_slide_parameter = 0.0f;
        }
        com_f4_current_omega            = AIDU_CURRENT_OMEGA;
        com_f4_current_zeta             = AIDU_CURRENT_ZETA;
        com_f4_speed_omega              = 1.0f + ((gui_f4_1para_speed_omega - 1.0f) * 2) * gui_f4_slide_parameter / 100.0f;
        com_f4_speed_zeta               = AIDU_SPEED_ZETA;
        com_f4_pos_omega                = (gui_f4_1para_pos_omega - 5.0f) + (5.0f * 2) * gui_f4_slide_parameter / 100.0f;
        com_f4_sob_omega                = AIDU_SOB_OMEGA;
        com_f4_sob_zeta                 = AIDU_SOB_ZETA;
    }
} /* End of function mtr_ics_1_parameter */

/***********************************************************************************************************************
* Function Name : aid_mtr_ics_interrupt
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void aid_mtr_ics_interrupt(void)
{
    if (R_TUNER_GetFlugReset() == AIDU_FLUG_RESET_MDOE1)
    {
        g_motor_encoder0.p_api->reset(g_motor_encoder0.p_ctrl);
        R_TUNER_SetFlugReset(AIDU_FLUG_RESET_MDOE2);
    }
}
