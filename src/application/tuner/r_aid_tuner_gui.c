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
 * File Name    : r_aid_tuner_gui.c
 * Description  : This module solves all the world's problems
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
*          : 09.12.2021 1.00
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <stdbool.h>
#include "r_aid_config.h"
#include "r_aid_tuner_gui.h"
#include "r_aid_auto_identify.h"
#include "r_mtr_ics.h"
#include "r_aid_tuner_gui_user.h"

#include "hal_data.h"

/***********************************************************************************************************************
 Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
enum
{
    RMW_ACT_NONE = 0,
    RMW_ACT_TUNER_START = 1,
    RMW_ACT_TUNER_STOP = 2,
    RMW_ACT_TUNER_RESET = 3,
    RMW_ACT_APPLY_MOTOR_PARAMS = 4,
    RMW_ACT_TUNER_APPLY_PARAMS = 5
} gui_s4_rmw_action;    /*< Action code from Renesas motor workbench, will be set 0 after non 0 value written */

uint8_t  gui_u1_use_init_r = false;
uint8_t  gui_u1_use_init_ld = false;
uint8_t  gui_u1_use_init_lq = false;
uint8_t  gui_u1_use_init_ke = false;
uint8_t  gui_u1_only_clear_error = false;
uint8_t  gui_u1_params_applied = false;
uint16_t gui_u2_tuner_status;
float    gui_f4_init_r;
float    gui_f4_init_ld;
float    gui_f4_init_lq;
float    gui_f4_init_ke;
float    gui_f4_r;
float    gui_f4_ld;
float    gui_f4_lq;
float    gui_f4_ke;
float    gui_f4_j;
float    gui_f4_d;
st_aid_volterr_lut_t gui_st_volterr_lut;
float    gui_f4_volterr_table_current[5];
float    gui_f4_volterr_table_volterr[5];

st_aid_volterr_lut_t g_st_volterr_lut =
{
    .current_table =
    {
        AIDU_TBL_COMP_I0,
        AIDU_TBL_COMP_I1,
        AIDU_TBL_COMP_I2,
        AIDU_TBL_COMP_I3,
        AIDU_TBL_COMP_I4
    },
    .volterr_table =
    {
        AIDU_TBL_COMP_V0,
        AIDU_TBL_COMP_V1,
        AIDU_TBL_COMP_V2,
        AIDU_TBL_COMP_V3,
        AIDU_TBL_COMP_V4
    },
    .ref_voltage =
    {
        AIDU_REF_VOLTAGE
    }
};

uint16_t gui_u2_error_status;
uint8_t  gui_u1_tune_mode;
uint16_t gui_u2_measure_progress;
float    aid_f4_measure_progress;
uint8_t  gui_u1_flag_tune_result;
uint8_t  gui_u1_flg_reset = 0;
uint8_t  gui_u1_para_setting = 0;
uint8_t  gui_u1_flag_tune_mode = 1;
float    gui_f4_slide_parameter = 50;
uint8_t  gui_u1_active_gui;
uint8_t  gui_u1_flg_1para_init = 0;
float    gui_f4_1para_speed_omega;
float    gui_f4_1para_pos_omega;
extern uint8_t gui_u1_volterr_is_enabled;
/***********************************************************************************************************************
 Private global variables and functions
 **********************************************************************************************************************/
static uint8_t gs_u1_is_tuner_running = false;

static void tuner_act_ready(void);
static void tuner_act_measure(void);
static void tuner_act_error(void);
static void tuner_act_completed(void);
static void tuner_save_identified_params(void);
static void tuner_reset(void);
static void rmw_act_start(void);
static uint16_t rmw_get_progress(void);
static void r_aid_config_init(void);
static void r_aid_config_fp_init(void);

/***********************************************************************************************************************
* Function Name : R_TUNER_InitGUI
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void R_TUNER_InitGUI (void)
{
    gui_u1_volterr_is_enabled = 0;
    
    r_aid_config_init();
    r_aid_config_fp_init();
    
    R_AID_Init(AIDU_PWM_TICK_PER_IRQ, AIDU_SPEED_CTRL_PERIOD);
    R_AID_ConfigSetVolterrLUT(&g_st_volterr_lut);
} /* End of function R_TUNER_InitGUI() */

/***********************************************************************************************************************
* Function Name : R_TUNER_MainLoop
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void R_TUNER_MainLoop (void)
{
    /* Start Button */
    if (gui_u1_tune_mode == AIDU_TUNE_MODE_START)
    {
        R_AID_Init(AIDU_PWM_TICK_PER_IRQ, AIDU_SPEED_CTRL_PERIOD);
        R_AID_ConfigSetVolterrLUT(&g_st_volterr_lut);
        gui_s4_rmw_action = RMW_ACT_TUNER_START;
        gui_u1_tune_mode = AIDU_TUNE_MODE_MAX;
    }

    /* Reset Button */
    if (gui_u2_tuner_status == AID_STATUS_RESET)
    {
        gui_s4_rmw_action = RMW_ACT_TUNER_RESET;
        rmw_apply_reset();
    }

    /* Stop Button */
    if (gui_u2_error_status == AIDU_ERROR_ASSERT_FAIL)
    {
        R_AID_UserError(AIDU_ERROR_STOP_BUTTON);
    }
    
    gui_u2_tuner_status = R_AID_GetSystemStatus();
    switch(gui_u2_tuner_status)
    {
        case AID_STATUS_READY:
        {
            gui_u2_measure_progress = 0;
            tuner_act_ready();
        }
        break;

        case AID_STATUS_MEASURE:
        {
            tuner_act_measure();
            gui_u2_measure_progress = rmw_get_progress();
        }
        break;

        case AID_STATUS_ERROR:
        {
            gui_u2_error_status = R_AID_GetErrorStatus();
            tuner_act_error();
        }
        break;

        case AID_STATUS_COMPLETED:
        {
            tuner_act_completed();
        }
        break;

        default:
        {
            /* Do nothing */
        }
    }

    /* Clear command/action to make sure the command is only executed once */
    gui_s4_rmw_action = RMW_ACT_NONE;
} /* End of function R_TUNER_MainLoop() */

/***********************************************************************************************************************
* Function Name : R_TUNER_IsRunning
* Description   : 
* Arguments     : None
* Return Value  : 
***********************************************************************************************************************/
uint8_t R_TUNER_IsRunning(void)
{
    return gs_u1_is_tuner_running;
} /* End of function R_TUNER_IsRunning() */


/***********************************************************************************************************************
* Function Name : R_TUNER_SetTuneResult
* Description   : Set gui_u1_flag_tune_result
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void R_TUNER_SetTuneResult(void)
{
    gui_u1_flag_tune_result = AIDU_FLG_SET;
} /* End of function R_TUNER_SetTuneResult() */

/***********************************************************************************************************************
* Function Name : R_TUNER_GetFlugReset
* Description   : Get gui_u1_flg_reset
* Arguments     : None
* Return Value  : 
***********************************************************************************************************************/
uint8_t R_TUNER_GetFlugReset(void)
{
    return gui_u1_flg_reset;
} /* End of function R_TUNER_GetFlugReset() */

/***********************************************************************************************************************
* Function Name : R_TUNER_SetFlugReset
* Description   : Set gui_u1_flg_reset
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void R_TUNER_SetFlugReset(uint8_t num)
{
    gui_u1_flg_reset = num;
} /* End of function R_TUNER_GetFlugReset() */

/***********************************************************************************************************************
* Function Name : tuner_act_ready
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void tuner_act_ready(void)
{
    switch(gui_s4_rmw_action)
    {
        case RMW_ACT_TUNER_START:
        {
            rmw_act_start();
            gs_u1_is_tuner_running = true;
        }
        break;

        default:
        {
            gs_u1_is_tuner_running = false;
        }
    }
} /* End of function tuner_act_ready() */

/***********************************************************************************************************************
* Function Name : tuner_act_measure
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void tuner_act_measure(void)
{
    switch(gui_s4_rmw_action)
    {
        case RMW_ACT_TUNER_STOP:
        {
            R_AID_CmdStop();
            gs_u1_is_tuner_running = false;
        }
        break;

        default:
        {
            /* Do nothing */
        }
    }
} /* End of function tuner_act_measure() */

/***********************************************************************************************************************
* Function Name : tuner_act_error
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void tuner_act_error(void)
{
    switch(gui_s4_rmw_action)
    {
        case RMW_ACT_TUNER_RESET:
        {
            tuner_reset();
            gs_u1_is_tuner_running = true;
        }
        break;

        default:
        {
            gs_u1_is_tuner_running = false;
        }
    }
} /* End of function tuner_act_error() */

/***********************************************************************************************************************
* Function Name : tuner_act_completed
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void tuner_act_completed(void)
{
    /* Store identified parameters,
     * variables inside the tuner may be reset next */
    if (gui_u1_params_applied == false)
    {
        tuner_save_identified_params();
        rmw_apply_identified_params();
        gui_u1_params_applied = true;
    }

    switch(gui_s4_rmw_action)
    {
        case RMW_ACT_TUNER_RESET:
        {
            tuner_reset();
            gs_u1_is_tuner_running = true;
        }
        break;

        case RMW_ACT_TUNER_START:         /* Start command is also available in COMPLETED state */
        {
            rmw_act_start();
            gs_u1_is_tuner_running = true;
        }
        break;

        case RMW_ACT_TUNER_APPLY_PARAMS:
        {
            rmw_apply_identified_params();
        }
        break;

        default:
        {
            gs_u1_is_tuner_running = false;
            gui_u2_tuner_status = AID_STATUS_READY;
            gui_u1_tune_mode = AIDU_TUNE_MODE_DEFAULT;
        }
    }
} /* End of function tuner_act_completed() */

/***********************************************************************************************************************
* Function Name : tuner_reset
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void tuner_reset(void)
{
    if (false == gui_u1_only_clear_error)
    {
        gui_f4_r = 0.0f;
        gui_f4_ld = 0.0f;
        gui_f4_lq = 0.0f;
        gui_f4_ke = 0.0f;
        gui_f4_j = 0.0f;
        gui_f4_d = 0.0f;
    }
    R_AID_CmdReset();
} /* End of function tuner_reset() */

/***********************************************************************************************************************
* Function Name : tuner_save_identified_params
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void tuner_save_identified_params(void)
{
    gui_f4_r = R_AID_GetResistance();
    gui_f4_ld = R_AID_GetLd();
    gui_f4_lq = R_AID_GetLq();
    gui_f4_ke = R_AID_GetKe();
    gui_f4_j = R_AID_GetInertia();
    gui_f4_d = R_AID_GetFriction();
    R_AID_GetVolterrLUT(&gui_st_volterr_lut);
} /* End of function tuner_save_identified_params() */

/***********************************************************************************************************************
* Function Name : rmw_act_start
* Description   : 
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void rmw_act_start(void)
{
    /* Identification configuration can be done by writing variables with gui_ prefix */

    /* gui_u1_use_init prefix variables shows whether the initial parameter told by GUI should be used */
    R_AID_SetInitElecParams(gui_u1_use_init_r  ? gui_f4_init_r  : 0.0f,
                            gui_u1_use_init_ld ? gui_f4_init_ld : 0.0f,
                            gui_u1_use_init_lq ? gui_f4_init_lq : 0.0f,
                            gui_u1_use_init_ke ? gui_f4_init_ke : 0.0f);
    R_AID_CmdStart();
    gui_u1_params_applied = false;
} /* End of function rmw_act_start() */

/***********************************************************************************************************************
* Function Name : rmw_get_progress
* Description   : 
* Arguments     : None
* Return Value  : 
***********************************************************************************************************************/
static uint16_t rmw_get_progress(void)
{
    float f4_temp;
    uint16_t u2_temp;
    
    f4_temp = R_AID_GetProgress();
    u2_temp = 0;
    if (f4_temp > AIDU_PROGRESS_55)
    {
        u2_temp = AIDU_MEASURE_PROGRESS_1;
    }
    if (f4_temp > AIDU_PROGRESS_64)
    {
        u2_temp = AIDU_MEASURE_PROGRESS_2;
    }
    if (f4_temp > AIDU_PROGRESS_69)
    {
        u2_temp = AIDU_MEASURE_PROGRESS_3;
    }
    if (f4_temp > AIDU_PROGRESS_75)
    {
        u2_temp = AIDU_MEASURE_PROGRESS_4;
    }
    return(u2_temp);
} /* End of function rmw_get_progress() */

/***********************************************************************************************************************
* Function Name : r_aid_config_init
* Description   : Init
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void r_aid_config_init(void)
{
    g_f4_aid_r_diff_vd_step               = AID_R_DIFF_VD_STEP;
    g_f4_aid_r_diff_measure_current1      = AID_R_DIFF_MEASURE_CURRENT1;
    g_f4_aid_r_diff_measure_current2      = AID_R_DIFF_MEASURE_CURRENT2;
    g_f4_aid_r_diff_measure_current3      = AID_R_DIFF_MEASURE_CURRENT3;
    g_f4_aid_r_diff_measure_current4      = AID_R_DIFF_MEASURE_CURRENT4;
    g_f4_aid_r_diff_vd_step_wait          = AID_R_DIFF_VD_STEP_WAIT;
    g_f4_aid_r_diff_stab_wait             = AID_R_DIFF_STAB_WAIT;
    g_f4_aid_r_diff_measure_wait          = AID_R_DIFF_MEASURE_WAIT;
    g_f4_aid_r_diff_measure_time          = AID_R_DIFF_MEASURE_TIME;
    g_f4_aid_r_diff_reset_time            = AID_R_DIFF_RESET_TIME;
    g_u4_aid_r_diff_vd_adjust_timeout     = AID_R_DIFF_VD_ADJUST_TIMEOUT;
    g_f4_aid_rld_dft_freq                 = (float)AID_RLD_DFT_FREQ;
    g_f4_aid_rld_dft_vd_amp_coef          = AID_RLD_DFT_VD_AMP_COEF;
    g_f4_aid_rld_dft_vd_offset_coef       = AID_RLD_DFT_VD_OFFSET_COEF;
    g_f4_aid_rld_dft_measure_num          = (float)AID_RLD_DFT_MEASURE_NUM;
    g_f4_aid_rld_dft_reset_time           = AID_RLD_DFT_RESET_TIME;
    g_f4_aid_rld_dft_stab_coef            = (float)AID_RLD_DFT_STAB_COEF;
    g_f4_aid_rld_rls_vd_amp_coef          = AID_RLD_RLS_VD_AMP_COEF;
    g_f4_aid_rld_rls_vd_offset_coef       = AID_RLD_RLS_VD_OFFSET_COEF;
    g_f4_aid_rld_rls_init                 = (float)AID_RLD_RLS_INIT;
    g_f4_aid_rld_rls_forget_k             = AID_RLD_RLS_FORGET_K;
    g_f4_aid_rld_rls_vd_dc_wait_time      = AID_RLD_RLS_VD_DC_WAIT_TIME;
    g_f4_aid_rld_rls_measure_offset_time  = AID_RLD_RLS_MEASURE_OFFSET_TIME;
    g_f4_aid_rld_rls_vd_sin_wait_time     = AID_RLD_RLS_VD_SIN_WAIT_TIME;
    g_f4_aid_rld_rls_measure_num          = (float)AID_RLD_RLS_MEASURE_NUM;
    g_f4_aid_rld_rls_reset_time           = AID_RLD_RLS_RESET_TIME;
    g_f4_aid_lq_dft_freq                  = (float)AID_LQ_DFT_FREQ;
    g_f4_aid_lq_dft_vq_amp_coef           = AID_LQ_DFT_VQ_AMP_COEF;
    g_f4_aid_lq_dft_vd_offset_coef        = AID_LQ_DFT_VD_OFFSET_COEF;
    g_f4_aid_lq_dft_measure_num           = (float)AID_LQ_DFT_MEASURE_NUM;
    g_f4_aid_lq_dft_reset_time            = AID_LQ_DFT_RESET_TIME;
    g_f4_aid_lq_rls_freq                  = AID_LQ_RLS_FREQ;
    g_f4_aid_lq_rls_vq_amp_coef           = AID_LQ_RLS_VQ_AMP_COEF;
    g_f4_aid_lq_rls_vd_offset_coef        = AID_LQ_RLS_VD_OFFSET_COEF;
    g_f4_aid_lq_rls_init                  = (float)AID_LQ_RLS_INIT;
    g_f4_aid_lq_rls_forget_k              = AID_LQ_RLS_FORGET_K;
    g_f4_aid_lq_rls_vd_dc_wait_time       = AID_LQ_RLS_VD_DC_WAIT_TIME;
    g_f4_aid_lq_rls_vq_sin_wait_time      = AID_LQ_RLS_VQ_SIN_WAIT_TIME;
    g_f4_aid_lq_rls_measure_num           = (float)AID_LQ_RLS_MEASURE_NUM;
    g_f4_aid_lq_rls_reset_time            = AID_LQ_RLS_RESET_TIME;
    g_f4_aid_ke_id_up_time                = AID_KE_ID_UP_TIME;
    g_f4_aid_ke_id_ref_coef               = AID_KE_ID_REF_COEF;
    g_f4_aid_ke_freq                      = AID_KE_FREQ;
    g_f4_aid_ke_omega_up_time             = g_f4_aid_ke_freq * AIDU_FLOAT_80_0;
    g_f4_aid_ke_measure_time              = AID_KE_MEASURE_TIME;
    g_f4_aid_ke_measure_id_stab_wait      = AID_KE_MEASURE_ID_STAB_WAIT;
    g_f4_aid_ke_reset_time                = AID_KE_RESET_TIME;
    g_f4_aid_ke_assumed_inertia           = AID_KE_ASSUMED_INERTIA;
    g_f4_aid_jd_speedpi_omega             = AID_JD_SPEEDPI_OMEGA;
    g_f4_aid_jd_id_ref_coef               = AID_JD_ID_REF_COEF;
    g_f4_aid_jd_rls_freq                  = AID_JD_RLS_FREQ;
    g_f4_aid_jd_speed_amp_coef            = AID_JD_SPEED_AMP_COEF;
    g_f4_aid_jd_speed_amp_coef_min        = AID_JD_SPEED_AMP_COEF_MIN;
    g_f4_aid_jd_speed_offset_coef         = AID_JD_SPEED_OFFSET_COEF;
    g_f4_aid_jd_rls_init                  = (float)AID_JD_RLS_INIT;
    g_f4_aid_jd_rls_forget_k              = AID_JD_RLS_FORGET_K;
    g_f4_aid_jd_limit_speed_change        = AID_JD_LIMIT_SPEED_CHANGE;
    g_f4_aid_jd_dft_num                   = (float)AID_JD_DFT_NUM;
    g_f4_aid_jd_measure_start_amp_rate    = AID_JD_MEASURE_START_AMP_RATE;
    g_f4_aid_jd_stab_wait                 = (float)AID_JD_STAB_WAIT;
    g_f4_aid_jd_stop_time                 = (float)AID_JD_STOP_TIME;
    g_f4_aid_jd_assumed_inertia           = AID_JD_ASSUMED_INERTIA;
    g_u1_aid_volterr_enable               = AID_VOLTERR_ENABLE;
    g_f4_aid_volterr_rls_fgt_factor       = AID_VOLTERR_RLS_FGT_FACTOR;
    g_f4_aid_volterr_avg_sample           = (float)AID_VOLTERR_AVG_SAMPLE;
    g_f4_aid_volterr_avg_sample_fast      = (float)AID_VOLTERR_AVG_SAMPLE_FAST;
    g_f4_aid_volterr_avg_wait_time        = AID_VOLTERR_AVG_WAIT_TIME;
    g_u4_aid_volterr_point_num            = AID_VOLTERR_POINT_NUM;
    g_f4_aid_volterr_min_current_step_lsb = (float)AID_VOLTERR_MIN_CURRENT_STEP_LSB;
    g_u4_aid_volterr_iteration_num        = AID_VOLTERR_ITERATION_NUM;
    g_f4_aid_volterr_rls_upper_current    = AID_VOLTERR_RLS_UPPER_CURRENT;
    g_f4_aid_volterr_rls_lower_current    = AID_VOLTERR_RLS_LOWER_CURRENT;
    g_f4_aid_volterr_init_r               = AID_VOLTERR_INIT_R;
    g_f4_aid_current_omega                = AID_CURRENT_OMEGA;
    g_f4_aid_current_zeta                 = AID_CURRENT_ZETA;
    g_f4_aid_e_obs_omega                  = AID_E_OBS_OMEGA;
    g_f4_aid_e_obs_zeta                   = AID_E_OBS_ZETA;
    g_f4_aid_pll_est_omega                = AID_PLL_EST_OMEGA;
    g_f4_aid_pll_est_zeta                 = AID_PLL_EST_ZETA;
    g_f4_aid_offset_lpf_k                 = AID_OFFSET_LPF_K;
    g_f4_aid_offset_calc_time             = AID_OFFSET_CALC_TIME;
    g_f4_aid_overcurrent_limit_mult       = AID_OVERCURRENT_LIMIT_MULT;
    g_f4_aid_ol2cl_crnt_inc_mult          = AID_OL2CL_CRNT_INC_MULT;
    g_f4_aid_ol2cl_inciq_phaseerr_dec_th  = AID_OL2CL_INCIQ_PHASEERR_DEC_TH;
    g_f4_aid_ol2cl_phaseerr_max_dec       = AID_OL2CL_PHASEERR_MAX_DEC;
    g_f4_aid_ol2cl_bemf_th_coef           = AID_OL2CL_BEMF_TH_COEF;
    g_f4_aid_cl2ol_bemf_th_coef           = AID_CL2OL_BEMF_TH_COEF;
    g_f4_aid_speed_change_rate_limit      = AID_SPEED_CHANGE_RATE_LIMIT;
    g_f4_aid_iq_down_time                 = AID_IQ_DOWN_TIME;
    g_f4_aid_id_up_time                   = AID_ID_UP_TIME;
    g_f4_aid_id_down_time                 = AID_ID_DOWN_TIME;
    g_f4_aid_phaseerr_lpf_band_hz         = AID_PHASEERR_LPF_BAND_HZ;
    g_f4_aid_r_min                        = AID_R_MIN;
    g_f4_aid_num_pole_pair_limit          = (float)AID_NUM_POLE_PAIR_LIMIT;
    
    g_f4_tune_inertia_setting             = AIDU_TUNE_INERTIA_SETTING;
    g_u2_tune_volterr_setting             = AIDU_TUNE_VOLTERR_SETTING;
    g_u1_volterr_is_enabled               = AIDU_TUNE_VOLTERR_ENABLE;
    
    g_f4_aid_inv_info_duty_min            = AIDU_INV_INFO_DUTY_MIN;
    g_f4_aid_inv_info_duty_max            = AIDU_INV_INFO_DUTY_MAX;
    g_f4_ip_current_limit                 = AIDU_INV_INFO_OVERCURRENT_TH;
    g_f4_aid_inv_info_overvoltage_th      = AIDU_INV_INFO_OVERVOLTAGE_TH;
    g_f4_aid_inv_info_undervoltage_th     = AIDU_INV_INFO_UNDERVOLTAGE_TH;
    g_f4_aid_inv_info_pwm_cycle_s         = AIDU_INV_INFO_PWM_CYCLE_S;
    g_f4_aid_inv_info_pwm_deadtime_s      = AIDU_INV_INFO_PWM_DEADTIME_S;
    g_f4_mtr_carrier_set_base             = (float)AIDU_CARRIER_SET_BASE;
    g_f4_ip_current_range                 = AIDU_INV_INFO_CURRENT_RANGE;
    g_f4_mtr_deadtime_set                 = (float)AIDU_DEADTIME_SET;
    g_u4_u_count_reg                      = AIDU_PWM_TIMER_U_REG;
    g_u4_v_count_reg                      = AIDU_PWM_TIMER_V_REG;
    g_u4_w_count_reg                      = AIDU_PWM_TIMER_W_REG;
} /* End of function r_aid_config_init() */

/***********************************************************************************************************************
* Function Name : r_aid_config_fp_init
* Description   : function pointer Init
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void r_aid_config_fp_init(void)
{
    g_fp_aid_internal_clear_oc_flag       = aid_mtr_clear_oc_flag;
    g_fp_aid_internal_ctrl_start          = aid_mtr_ctrl_start;
    g_fp_aid_internal_ctrl_stop           = aid_mtr_ctrl_stop;
    g_fp_aid_internal_get_vdc             = aid_mtr_get_vdc;
    g_fp_aid_internal_get_current_iuiw    = aid_mtr_get_current_iuiw;
    g_fp_aid_internal_inv_set_uvw         = aid_mtr_inv_set_uvw;
} /* End of function r_aid_config_fp_init() */
