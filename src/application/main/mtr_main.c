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
* Copyright (C) 2023 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name   : mtr_main.c
* Description : The main function and the processes of motor control application layer
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version
*         : 23.05.2023 1.00
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdint.h>
#include "mtr_main.h"
#include "hal_data.h"
#include "ics2_RA6T3.h"
#include "r_mtr_ics.h"
#define     CONF_MOTOR_TYPE ("Brushless DC Motor")
#define     CONF_CONTROL ("Encoder vector control (Position control)")
#define     CONF_INVERTER ("MCI-LV-1")
#define     CONF_MOTOR_TYPE_LEN (18)
#define     CONF_CONTROL_LEN (41)
#define     CONF_INVERTER_LEN (8)

/* USE_RMW_TUNER */
#include "r_aid_tuner_gui.h"
#include "r_aid_auto_identify.h"
/* USE_RMW_TUNER */

/***********************************************************************************************************************
* Global variables
***********************************************************************************************************************/
float       g_f4_speed_ref = 0;
float       g_f_position_ref = 0;
uint8_t     g_u1_motor_status;            /* Motor status */
uint8_t     com_u1_sw_userif;             /* User interface switch */
uint8_t     g_u1_sw_userif;               /* User interface switch */
uint8_t     com_u1_mode_system;           /* System mode */
uint8_t     g_u1_mode_system;             /* System mode */
uint16_t    g_u2_max_speed_rpm;
uint8_t     g_u1_stop_req;
uint16_t    g_u2_chk_error;
uint16_t    g_u2_vr1_ad;
uint16_t    g_u2_conf_hw;
uint16_t    g_u2_conf_sw;
uint16_t    g_u2_conf_tool;
uint8_t     g_u1_conf_motor_type_len;
uint8_t     g_u1_conf_control_len;
uint8_t     g_u1_conf_inverter_len;
uint8_t     g_u1_conf_motor_type[CONF_MOTOR_TYPE_LEN];
uint8_t     g_u1_conf_control[CONF_CONTROL_LEN];
uint8_t     g_u1_conf_inverter[CONF_INVERTER_LEN];
uint8_t     g_u1_reset_req;             /* Reset request flag */
uint8_t     g_u1_sw_cnt;                /* Counter to remove chattering */
uint8_t     g_u1_loop_mode;
motor_cfg_t g_user_motor_cfg;
motor_encoder_extended_cfg_t g_user_motor_encoder_extended_cfg;
motor_speed_cfg_t g_user_motor_speed_cfg;
motor_speed_extended_cfg_t g_user_motor_speed_extended_cfg;
motor_position_cfg_t g_user_motor_position_cfg;
motor_position_extended_cfg_t g_user_motor_position_extended_cfg;
motor_current_cfg_t g_user_motor_current_cfg;
motor_current_extended_cfg_t g_user_motor_current_extended_cfg;
motor_angle_cfg_t g_user_motor_angle_cfg;
motor_sense_encoder_extended_cfg_t g_user_motor_sense_encoder_extended_cfg;
motor_driver_cfg_t g_user_motor_driver_cfg;
motor_driver_extended_cfg_t g_user_motor_driver_extended_cfg;
motor_current_motor_parameter_t g_user_motor_current_motor_parameter;
motor_current_design_parameter_t g_user_motor_current_design_parameter;

/***********************************************************************************************************************
* Private functions
***********************************************************************************************************************/
static void     motor_fsp_init (void);
static void     mtr_board_led_control (uint8_t u1_motor_status);
static uint8_t  mtr_remove_sw_chattering (uint8_t u1_sw, uint8_t u1_on_off);
static void     board_ui (void);           /* Board user interface */
static void     ics_ui (void);             /* ICS (Analyzer) user interface */
static void     software_init (void);      /* Software initialize */
static uint16_t get_vr1 (void);
static uint8_t  get_sw1 (void);
static uint8_t  get_sw2 (void);
static void     led1_on (void);
static void     led2_on (void);
static void     led3_on (void);
static void     led1_off (void);
static void     led2_off (void);
static void     led3_off (void);

/* USE_RMW_TUNER */
uint16_t g_u2_tune_mode = 0;
uint16_t g_u2_tune_mode_pre = 0;

void rm_motor_driver_cyclic_tuner (adc_callback_args_t * p_args);
void rm_motor_speed_cyclic_tuner (timer_callback_args_t * p_args);
/* USE_RMW_TUNER */

/***********************************************************************************************************************
* Function Name : mtr_init
* Description   : Initialization for Motor Control
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void mtr_init(void)
{
    int i;
    uint8_t u1_conf_motor_type[] = CONF_MOTOR_TYPE;
    uint8_t u1_conf_control[] = CONF_CONTROL;
    uint8_t u1_conf_inverter[] = CONF_INVERTER;
    g_u1_conf_motor_type_len = CONF_MOTOR_TYPE_LEN;
    g_u1_conf_control_len    = CONF_CONTROL_LEN;
    g_u1_conf_inverter_len   = CONF_INVERTER_LEN;
    for (i = 0; i < g_u1_conf_motor_type_len; i++)
    {
        g_u1_conf_motor_type[i] = u1_conf_motor_type[i];
    }
    for (i = 0; i < g_u1_conf_control_len; i++)
    {
        g_u1_conf_control[i] = u1_conf_control[i];
    }
    for (i = 0; i < g_u1_conf_inverter_len; i++)
    {
        g_u1_conf_inverter[i] = u1_conf_inverter[i];
    }
    g_u2_conf_hw = 0x0008;                        /* 0000000000001000b */
/* USE_RMW_TUNER */
    g_u2_conf_sw   = 0x4081;                        /* 0100000010000001b */
    g_u2_conf_tool = 0x0600;      /* 0000011000000000b */
/* USE_RMW_TUNER */

    motor_fsp_init();
    software_init();                              /* Initialize private global variables */

#if USE_BUILT_IN
    ics2_init(ICS_SCI9_P110_P109, ICS_BRR, ICS_INT_MODE);
#else
    ics2_init(ICS_SCI0_P411_P410, ICS_BRR, ICS_INT_MODE);
#endif

    /* Execute reset event */
    g_motor_encoder0.p_api->reset(g_motor_encoder0.p_ctrl);

/* USE_RMW_TUNER */
    R_TUNER_InitGUI();
/* USE_RMW_TUNER */
} /* End of function mtr_init() */

/***********************************************************************************************************************
* Function Name : mtr_main
* Description   : Main routine for Motor Control
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
void mtr_main(void)
{
    /*** select user interfaces ***/
    if (g_u1_sw_userif != com_u1_sw_userif)
    {
        g_u1_sw_userif = com_u1_sw_userif;
        if (ICS_UI == g_u1_sw_userif)
        {
            g_u1_mode_system = g_u1_motor_status;
        }
    }

    if (BOARD_UI == g_u1_sw_userif)
    {
        board_ui();                           /* User interface control routine */
    }
    else if (ICS_UI == g_u1_sw_userif)
    {
        ics_ui();                             /* User interface using ICS */
/* USE_RMW_TUNER */
        R_TUNER_MainLoop();
        g_u2_tune_mode = R_TUNER_IsRunning();
        if (g_u2_tune_mode != g_u2_tune_mode_pre)
        {
            gpt_extended_cfg_t * p_gpt_exntend = (gpt_extended_cfg_t *) g_user_motor_sense_encoder_extended_cfg.p_input_capture_instance->p_cfg->p_extend;
            if (g_u2_tune_mode == 1)
            {
                R_BSP_IrqDisable(p_gpt_exntend->capture_a_irq);
                g_user_motor_driver_cfg.p_adc_instance->p_api->callbackSet(g_user_motor_driver_cfg.p_adc_instance->p_ctrl,
                                                          rm_motor_driver_cyclic_tuner,
                                                          &g_motor_driver0_ctrl,
                                                          &(g_motor_driver0_ctrl.adc_callback_args));
                g_user_motor_speed_cfg.p_timer_instance->p_api->callbackSet(g_user_motor_speed_cfg.p_timer_instance->p_ctrl,
                                                          rm_motor_speed_cyclic_tuner,
                                                          &g_motor_speed0,
                                                          &(g_motor_speed0_ctrl.timer_args));
            }
            else
            {
                g_user_motor_driver_cfg.p_adc_instance->p_api->callbackSet(g_user_motor_driver_cfg.p_adc_instance->p_ctrl,
                                                          rm_motor_driver_cyclic,
                                                          &g_motor_driver0_ctrl,
                                                          &(g_motor_driver0_ctrl.adc_callback_args));
                g_user_motor_speed_cfg.p_timer_instance->p_api->callbackSet(g_user_motor_speed_cfg.p_timer_instance->p_ctrl,
                                                          rm_motor_speed_cyclic,
                                                          &g_motor_speed0,
                                                          &(g_motor_speed0_ctrl.timer_args));
                g_motor_encoder0.p_api->stop(g_motor_encoder0.p_ctrl);
                R_BSP_IrqEnable(p_gpt_exntend->capture_a_irq);
            }
        g_u2_tune_mode_pre = g_u2_tune_mode;
        }
/* USE_RMW_TUNER */
    }
    else
    {
        /* Do Nothing */
    }
} /* End of function mtr_main() */

/***********************************************************************************************************************
* Function Name : board_ui
* Description   : User interface using board UI
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void board_ui(void)
{
    uint8_t u1_temp_sw_signal;
    motor_speed_position_data_t temp_position_data;
    float f4_temp;

    /*======================*/
    /*     Mode control     */
    /*======================*/

    /* Get status of motor control system */
    g_motor_encoder0.p_api->statusGet(g_motor_encoder0.p_ctrl, &g_u1_motor_status);
    switch (g_u1_motor_status)
    {
        case MOTOR_ENCODER_CTRL_STOP:
            u1_temp_sw_signal = get_sw1();

            /* Check SW1 */
            if ((MTR_FLG_SET == mtr_remove_sw_chattering(u1_temp_sw_signal, SW1_ON)) && (MTR_FLG_SET != g_u1_stop_req))
            {
                g_motor_encoder0.p_api->run(g_motor_encoder0.p_ctrl);
            }
            break;

        case MOTOR_ENCODER_CTRL_RUN:
            u1_temp_sw_signal = get_sw1();

            /* Check SW1 */
            if ((MTR_FLG_SET == mtr_remove_sw_chattering(u1_temp_sw_signal, SW1_OFF)) || (MTR_FLG_CLR != g_u1_stop_req))
            {
                g_motor_encoder0.p_api->stop(g_motor_encoder0.p_ctrl);
            }
            break;

        case MOTOR_ENCODER_CTRL_ERROR:

            /* check SW2 & reset request flag */
            u1_temp_sw_signal = get_sw2();
            if ((SW_OFF == g_u1_reset_req) && (MTR_FLG_SET == mtr_remove_sw_chattering(u1_temp_sw_signal, SW2_ON)))
            {
                g_u1_reset_req = SW_ON;
            }
            else if ((SW_ON == g_u1_reset_req) && (MTR_FLG_SET == mtr_remove_sw_chattering(u1_temp_sw_signal, SW2_OFF)))
            {
                g_u1_reset_req = SW_OFF;
                g_motor_encoder0.p_api->reset(g_motor_encoder0.p_ctrl);
            }
            else
            {
                ;
            }
            break;

        default: /* Do nothing */
            break;
    }

    if (g_u1_loop_mode == LOOP_SPEED)
    {
        /*=============================*/
        /*      Set speed reference    */
        /*=============================*/
        g_f4_speed_ref = ((float)g_u2_vr1_ad - ADJUST_OFFSET) * VR1_SCALING; /* Read speed reference from VR1 */
        g_motor_encoder0.p_api->speedSet(g_motor_encoder0.p_ctrl, g_f4_speed_ref);
        temp_position_data.e_step_mode= MOTOR_SPEED_STEP_DISABLE;
        temp_position_data.e_loop_mode = MOTOR_SPEED_LOOP_MODE_SPEED;
        temp_position_data.position_reference_degree = 0;
        g_motor_encoder0.p_api->positionSet(g_motor_encoder0.p_ctrl, &temp_position_data);

        if ((g_f4_speed_ref > (-STOP_RPM)) && (g_f4_speed_ref < STOP_RPM)) /* Check SW1 */
        {
            g_u1_stop_req = MTR_FLG_SET;
        }
        else
        {
            g_u1_stop_req = MTR_FLG_CLR;
        }
    }
    else if (g_u1_loop_mode == LOOP_POSITION)
    {
        if (g_user_motor_sense_encoder_extended_cfg.loop_mode == LOOP_SPEED)
        {
            g_user_motor_sense_encoder_extended_cfg.loop_mode = MOTOR_SPEED_LOOP_MODE_POSITION;
            g_motor_sense_encoder0.p_api->parameterUpdate(g_motor_sense_encoder0.p_ctrl, &g_user_motor_angle_cfg);
        }

        /*=============================*/
        /*   Set position reference    */
        /*=============================*/
        /* read position reference from VR1, remove offset */
        g_f_position_ref = (ADJUST_OFFSET - (float)g_u2_vr1_ad) * VR1_SCALING_POS;
        if(g_f_position_ref > VR1_180)
        {
            g_f_position_ref = VR1_180;
        }
        else if (g_f_position_ref < -VR1_180)
        {
            g_f_position_ref = -VR1_180;
        }
        /* limit dead-band  */
        if(g_f_position_ref <= VR1_POSITION_DEAD_BAND && g_f_position_ref >= - VR1_POSITION_DEAD_BAND)
        {
            g_f_position_ref = 0.0F;
        }
        g_motor_encoder0.p_api->speedSet(g_motor_encoder0.p_ctrl, 0.0F);
        temp_position_data.e_step_mode = MOTOR_SPEED_STEP_ENABLE;
        temp_position_data.e_loop_mode = MOTOR_SPEED_LOOP_MODE_POSITION;
        temp_position_data.position_reference_degree = (int16_t)(g_f_position_ref);
        g_motor_encoder0.p_api->positionSet(g_motor_encoder0.p_ctrl, &temp_position_data);
    }

    if(MOTOR_ENCODER_CTRL_RUN == g_u1_motor_status && g_u1_loop_mode == LOOP_POSITION)
    {
        /***** Positioning flag *****/
        f4_temp = fabsf(g_motor_position0_ctrl.st_variable.f4_pos_err_rad);
        if (g_user_motor_position_extended_cfg.u2_pos_band_limit * g_user_motor_position_extended_cfg.f_encd_angle_diff >= f4_temp)
        {
            led3_on();
        }
        else
        {
            led3_off();
        }
    }
    else
    {
        g_u1_stop_req = MTR_FLG_CLR;
    }

    /***** LED control *****/
    mtr_board_led_control(g_u1_motor_status);
} /* End of function board_ui */

/***********************************************************************************************************************
* Function Name : ics_ui
* Description   : User interface using ICS
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void ics_ui(void)
{
    uint8_t u1_temp;
    float f4_temp;

    mtr_set_com_variables();

    /*============================*/
    /*        Execute event       */
    /*============================*/
    u1_temp = com_u1_mode_system;

    if (g_u1_mode_system != u1_temp)
    {
        if (u1_temp > MOTOR_ENCODER_CTRL_EVENT_RESET)
        {
            com_u1_mode_system = g_u1_mode_system;
        }
        else
        {
            g_u1_mode_system = u1_temp;
            switch (g_u1_mode_system)
            {
                case MOTOR_ENCODER_CTRL_EVENT_STOP:
                    g_motor_encoder0.p_api->stop(g_motor_encoder0.p_ctrl);
                break;

                case MOTOR_ENCODER_CTRL_EVENT_RUN:
                    g_motor_encoder0.p_api->run(g_motor_encoder0.p_ctrl);
                break;

                case MOTOR_ENCODER_CTRL_EVENT_RESET:
                    g_motor_encoder0.p_api->reset(g_motor_encoder0.p_ctrl);
                break;

                default:
                    /* Do nothing */
                break;
            }
        }
    }

    g_motor_encoder0.p_api->statusGet(g_motor_encoder0.p_ctrl, &g_u1_motor_status);

    if (MOTOR_ENCODER_CTRL_EVENT_RESET == g_u1_mode_system)
    {
        if (MOTOR_ENCODER_CTRL_STOP == g_u1_motor_status)
        {
            software_init();                            /* Initialize private global variables for reset event */
        }
        else if (MOTOR_ENCODER_CTRL_ERROR == g_u1_motor_status)
        {
            g_u1_mode_system   = MOTOR_ENCODER_CTRL_EVENT_ERROR;
            com_u1_mode_system = MOTOR_ENCODER_CTRL_EVENT_ERROR;
        }
        else
        {
            /* Do nothing */
        }
    }

    if(MOTOR_ENCODER_CTRL_RUN == g_u1_motor_status && g_u1_loop_mode == LOOP_POSITION)
    {
        /***** Positioning flag *****/
        f4_temp = fabsf(g_motor_position0_ctrl.st_variable.f4_pos_err_rad);
        if (g_user_motor_position_extended_cfg.u2_pos_band_limit * g_user_motor_position_extended_cfg.f_encd_angle_diff >= f4_temp)
        {
            led3_on();
        }
        else
        {
            led3_off();
        }
    }
    else
    {
        led3_off();
    }

    /***** LED control *****/
    mtr_board_led_control(g_u1_motor_status);
} /* End of function ics_ui */

/***********************************************************************************************************************
* Function Name : software_init
* Description   : Initialize private global variables
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void software_init(void)
{
    g_u1_motor_status            = MOTOR_ENCODER_CTRL_STOP;
    g_u2_max_speed_rpm           = MTR_MAX_SPEED_RPM;
    g_u1_sw_userif               = CONFIG_DEFAULT_UI;
    g_u1_mode_system             = MOTOR_ENCODER_CTRL_EVENT_STOP;
    g_u1_reset_req               = SW_OFF;
    g_u1_loop_mode               = CONFIG_LOOP_MODE;

    /* ICS variables initialization */
    com_u1_sw_userif             = CONFIG_DEFAULT_UI;
    com_u1_mode_system           = MOTOR_ENCODER_CTRL_EVENT_STOP;
    mtr_ics_variables_init();
    mtr_set_com_variables();
} /* End of function software_init */

/***********************************************************************************************************************
* Function Name : g_poe_overcurrent
* Description   : POEG2 Interrupt callback function
* Arguments     : p_args - Callback argument
* Return Value  : None
***********************************************************************************************************************/
void g_poe_overcurrent(poeg_callback_args_t *p_args)
{
/* USE_RMW_TUNER */
    if (R_TUNER_IsRunning())
    {
        R_AID_UserError(1);
    }
/* USE_RMW_TUNER */

    if (p_args != NULL)
    {
        R_POEG_Reset(g_poeg0.p_ctrl);
        g_motor_encoder0.p_api->errorSet(g_motor_encoder0.p_ctrl, MOTOR_ERROR_OVER_CURRENT_HW);
        g_u2_chk_error |= MOTOR_ERROR_OVER_CURRENT_HW;
    }
} /* End of function g_poe_overcurrent */

/***********************************************************************************************************************
* Function Name : motor_fsp_init
* Description   : Initialize Motor FSP modules
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void motor_fsp_init(void)
{
    motor_speed_position_data_t temp_position_data;

    R_POEG_Open(g_poeg0.p_ctrl, g_poeg0.p_cfg);
    g_motor_encoder0.p_api->open(g_motor_encoder0.p_ctrl, g_motor_encoder0.p_cfg);
    g_elc.p_api->open(g_elc.p_ctrl, g_elc.p_cfg);
    g_elc.p_api->enable(g_elc.p_ctrl);

    g_user_motor_cfg = *(g_motor_encoder0_ctrl.p_cfg);
    g_user_motor_encoder_extended_cfg = *(motor_encoder_extended_cfg_t *)g_user_motor_cfg.p_extend;
    g_user_motor_cfg.p_extend = &g_user_motor_encoder_extended_cfg;
    g_motor_encoder0_ctrl.p_cfg = &g_user_motor_cfg;

    g_user_motor_speed_cfg = *(g_motor_speed0_ctrl.p_cfg);
    g_user_motor_speed_extended_cfg = *(motor_speed_extended_cfg_t *)g_user_motor_speed_cfg.p_extend;
    g_user_motor_speed_cfg.p_extend = &g_user_motor_speed_extended_cfg;
    temp_position_data.e_step_mode = MOTOR_SPEED_STEP_DISABLE;
    temp_position_data.e_loop_mode = MOTOR_SPEED_LOOP_MODE_SPEED;
    temp_position_data.position_reference_degree = 0;
    g_motor_speed0.p_api->positionReferenceSet(g_motor_speed0.p_ctrl, &temp_position_data);

    g_user_motor_position_cfg = *(g_motor_position0_ctrl.p_cfg);
    g_user_motor_position_extended_cfg = *(motor_position_extended_cfg_t *)g_user_motor_position_cfg.p_extend;
    g_user_motor_position_cfg.p_extend = &g_user_motor_position_extended_cfg;

    g_user_motor_current_cfg = *(g_motor_current0_ctrl.p_cfg);
    g_user_motor_current_extended_cfg = *(motor_current_extended_cfg_t *)g_user_motor_current_cfg.p_extend;
    g_user_motor_current_motor_parameter = *(g_user_motor_current_extended_cfg.p_motor_parameter);
    g_user_motor_current_design_parameter = *(g_user_motor_current_extended_cfg.p_design_parameter);
    g_user_motor_current_extended_cfg.p_motor_parameter = &g_user_motor_current_motor_parameter;
    g_user_motor_current_extended_cfg.p_design_parameter = &g_user_motor_current_design_parameter;
    g_user_motor_current_cfg.p_extend = &g_user_motor_current_extended_cfg;

    g_user_motor_angle_cfg = *(g_motor_sense_encoder0_ctrl.p_cfg);
    g_user_motor_sense_encoder_extended_cfg
        = *(motor_sense_encoder_extended_cfg_t *)g_user_motor_angle_cfg.p_extend;
    g_user_motor_angle_cfg.p_extend = &g_user_motor_sense_encoder_extended_cfg;
    g_motor_sense_encoder0.p_api->parameterUpdate(g_motor_sense_encoder0.p_ctrl, &g_user_motor_angle_cfg);

    g_user_motor_driver_cfg = *(g_motor_driver0_ctrl.p_cfg);
    g_user_motor_driver_extended_cfg = *(motor_driver_extended_cfg_t *)g_user_motor_driver_cfg.p_extend;
    g_user_motor_driver_cfg.p_extend = &g_user_motor_driver_extended_cfg;
} /* End of function motor_fsp_init */

/***********************************************************************************************************************
* Function Name : mtr_callback_encoder
* Description   : Callback function of Encoder Control
* Arguments     : p_args - Callback argument
* Return Value  : None
***********************************************************************************************************************/
void mtr_callback_encoder(motor_callback_args_t * p_args)
{
    switch (p_args->event)
    {
        case MOTOR_CALLBACK_EVENT_SPEED_FORWARD:
            break;

        case MOTOR_CALLBACK_EVENT_SPEED_BACKWARD:
        {
            g_u2_vr1_ad = get_vr1();
            break;
        }

        case MOTOR_CALLBACK_EVENT_CURRENT_FORWARD:
        {
            if (MOTOR_ENCODER_CTRL_ERROR != g_u1_motor_status)
            {
                g_motor_encoder0.p_api->errorCheck(g_motor_encoder0.p_ctrl, &g_u2_chk_error);
            }
            break;
        }

        case MOTOR_CALLBACK_EVENT_CURRENT_BACKWARD:
        {
            mtr_ics_interrupt_process();
            break;
        }

        default:
            break;
    }
} /* End of function mtr_callback_encoder */

/***********************************************************************************************************************
* Function Name : mtr_board_led_control
* Description   : Set LED pattern depend on motor status
* Arguments     : u1_motor_status - Motor control status
* Return Value  : None
***********************************************************************************************************************/
static void mtr_board_led_control(uint8_t u1_motor_status)
{
    /***** LED control *****/
    if (MOTOR_ENCODER_CTRL_STOP == u1_motor_status)         /* Check motor status */
    {
        led1_off();                                     /* LED1 off */
        led2_off();                                     /* LED2 off */
    }
    else if (MOTOR_ENCODER_CTRL_RUN == u1_motor_status)      /* Check motor status */
    {
        led1_on();                                      /* LED1 on */
        led2_off();                                     /* LED2 off */
    }
    else if (MOTOR_ENCODER_CTRL_ERROR == u1_motor_status)       /* Check motor status */
    {
        led1_off();                                     /* LED1 off */
        led2_on();                                      /* LED2 on */
    }
    else
    {
        led1_on();                                      /* LED1 on */
        led2_on();                                      /* LED2 on */
        led3_on();                                      /* LED3 on */
    }
} /* End of function mtr_board_led_control */

/***********************************************************************************************************************
* Function Name : mtr_remove_sw_chattering
* Description   : Get switch status with removing chattering
* Arguments     : u1_sw - Board interface switch signal
*                 u1_on_off - Detected status (ON/OFF)
* Return Value  : u1_remove_chattering_flg - Detection result
***********************************************************************************************************************/
static uint8_t mtr_remove_sw_chattering(uint8_t u1_sw, uint8_t u1_on_off)
{
    uint8_t u1_remove_chattering_flg;

    u1_remove_chattering_flg = 0;
    if (u1_on_off == u1_sw)
    {
        g_u1_sw_cnt++;
        if (CHATTERING_CNT < g_u1_sw_cnt)
        {
            u1_remove_chattering_flg = MTR_FLG_SET;
            g_u1_sw_cnt = 0;
        }
    }
    else
    {
        g_u1_sw_cnt = 0;
    }

    return (u1_remove_chattering_flg);
} /* End of function mtr_remove_sw_chattering */

/***********************************************************************************************************************
* Function Name : get_vr1
* Description   : Get A/D converted value of VR1
* Arguments     : None
* Return Value  : A/D converted value of VR1
***********************************************************************************************************************/
static uint16_t get_vr1(void)
{
    uint16_t ad_data;

    g_adc0.p_api->read(g_adc0.p_ctrl, MTR_ADCH_VR1, &ad_data);

    return (ad_data);
} /* End of function get_vr1 */

/***********************************************************************************************************************
* Function Name : get_sw1
* Description   : Get state of SW1
* Arguments     : None
* Return Value  : State of SW1
***********************************************************************************************************************/
static uint8_t get_sw1(void)
{
    uint8_t tmp_port;

    R_IOPORT_PinRead(&g_ioport_ctrl, MTR_PORT_SW1, &tmp_port);

    return (tmp_port);
} /* End of function get_sw1 */

/***********************************************************************************************************************
* Function Name : get_sw2
* Description   : Get state of SW2
* Arguments     : None
* Return Value  : State of SW2
***********************************************************************************************************************/
static uint8_t get_sw2(void)
{
    uint8_t tmp_port;

    R_IOPORT_PinRead(&g_ioport_ctrl, MTR_PORT_SW2, &tmp_port);

    return (tmp_port);
} /* End of function get_sw2 */

/***********************************************************************************************************************
* Function Name : led1_on
* Description   : Turn on LED1
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void led1_on(void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, MTR_PORT_LED1, MTR_LED_ON);
} /* End of function led1_on */

/***********************************************************************************************************************
* Function Name : led2_on
* Description   : Turn on LED2
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void led2_on(void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, MTR_PORT_LED2, MTR_LED_ON);
} /* End of function led2_on */

/***********************************************************************************************************************
* Function Name : led3_on
* Description   : Turn on LED3
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void led3_on(void)
{
    // LED3 is not assigned on MCB-RA6T3/RA4T1.
    // Do nothing.
} /* End of function led3_on */

/***********************************************************************************************************************
* Function Name : led1_off
* Description   : Turn off LED1
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void led1_off(void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, MTR_PORT_LED1, MTR_LED_OFF);
} /* End of function led1_off */

/***********************************************************************************************************************
* Function Name : led2_off
* Description   : Turn off LED2
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void led2_off(void)
{
    R_IOPORT_PinWrite(&g_ioport_ctrl, MTR_PORT_LED2, MTR_LED_OFF);
} /* End of function led2_off */

/***********************************************************************************************************************
* Function Name : led3_off
* Description   : Turn off LED3
* Arguments     : None
* Return Value  : None
***********************************************************************************************************************/
static void led3_off(void)
{
    // LED3 is not assigned on MCB-RA6T3/RA4T1.
    // Do nothing.
} /* End of function led3_off */
