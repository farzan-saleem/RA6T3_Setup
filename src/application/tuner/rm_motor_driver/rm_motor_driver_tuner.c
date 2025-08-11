/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <math.h>
#include <stdint.h>
#include "rm_motor_driver.h"
#include "r_gpt.h"

/* USE_RMW_TUNER */
#include "r_aid_tuner_gui.h"
#include "r_aid_auto_identify.h"
/* USE_RMW_TUNER */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#define MOTOR_DRIVER_IO_PORT_CFG_LOW            (0x3000004) /* Set I/O port with low output */
#define MOTOR_DRIVER_IO_PORT_CFG_HIGH           (0x3000005) /* Set I/O port with high output*/
#define MOTOR_DRIVER_IO_PORT_PERIPHERAL_MASK    (0x0010000) /* Mask for pin to operate as a peripheral pin */
#define MOTOR_DRIVER_IO_PORT_GPIO_MASK          (0xFFEFFFF) /* Mask for pin to operate as a GPIO pin */

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
void rm_motor_driver_cyclic_tuner(adc_callback_args_t * p_args);
static void rm_motor_driver_current_get(motor_driver_instance_ctrl_t * p_ctrl);
static void rm_motor_driver_pin_cfg (bsp_io_port_pin_t pin, uint32_t cfg);
static void rm_motor_driver_output_enable (motor_driver_instance_ctrl_t * p_ctrl);
static void rm_motor_driver_output_disable (motor_driver_instance_ctrl_t * p_ctrl);

/***********************************************************************************************************************
 * Function Name : rm_motor_driver_current_get
 * Description   : Get Iu/Iw & Vdc process
 * Arguments     : p_ctrl - The pointer to the motor driver module instance
 * Return Value  : None
 **********************************************************************************************************************/
static void rm_motor_driver_current_get (motor_driver_instance_ctrl_t * p_ctrl)
{
    uint16_t u2_addata[3]                      = {0U};
    float    f_addata[2]                       = {0.0F};
    motor_driver_cfg_t const    * p_cfg        = p_ctrl->p_cfg;
    motor_driver_extended_cfg_t * p_extend_cfg = (motor_driver_extended_cfg_t *) p_cfg->p_extend;

    /* Read A/D converted data */
    if (p_cfg->p_adc_instance != NULL)
    {
        p_cfg->p_adc_instance->p_api->read(p_cfg->p_adc_instance->p_ctrl, p_cfg->iu_ad_ch, &u2_addata[0]);
        p_cfg->p_adc_instance->p_api->read(p_cfg->p_adc_instance->p_ctrl, p_cfg->iw_ad_ch, &u2_addata[1]);
        p_cfg->p_adc_instance->p_api->read(p_cfg->p_adc_instance->p_ctrl, p_cfg->vdc_ad_ch, &u2_addata[2]);
    }

    f_addata[0] = (float) u2_addata[0];
    f_addata[1] = (float) u2_addata[1];
    f_addata[0] = f_addata[0] - p_extend_cfg->f_ad_current_offset;
    f_addata[1] = f_addata[1] - p_extend_cfg->f_ad_current_offset;

    p_ctrl->f_iu_ad = -(f_addata[0]) * (p_extend_cfg->f_current_range / p_extend_cfg->f_ad_resolution);
    p_ctrl->f_iw_ad = -(f_addata[1]) * (p_extend_cfg->f_current_range / p_extend_cfg->f_ad_resolution);

    p_ctrl->f_vdc_ad = (float) u2_addata[2] * (p_extend_cfg->f_vdc_range / p_extend_cfg->f_ad_resolution) *
                       p_extend_cfg->f_ad_voltage_conversion;
}                                      /* End of function rm_motor_driver_current_get */

/***********************************************************************************************************************
 * Function Name : rm_motor_driver_cyclic
 * Description   : Cyclic process for driver accsess (Call at A/D conversion finish interrupt)
 * Arguments     : p_args - The pointer to arguments of A/D conversion finish intterupt callback
 * Return Value  : None
 **********************************************************************************************************************/
void rm_motor_driver_cyclic_tuner (adc_callback_args_t * p_args)
{
    motor_driver_instance_ctrl_t * p_instance = (motor_driver_instance_ctrl_t *) p_args->p_context;
    motor_driver_callback_args_t   temp_args_t;
    motor_driver_extended_cfg_t * p_extended_cfg =
        (motor_driver_extended_cfg_t *) p_instance->p_cfg->p_extend;

    /* Enable PWM output */
    if (p_extended_cfg->port_up != 0)
    {
        rm_motor_driver_output_enable(p_instance);
    }

    /* Get A/D converted data (Phase Current & Main Line Voltage) */
    rm_motor_driver_current_get(p_instance);

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance->p_cfg->p_callback)
    {
        temp_args_t.event     = MOTOR_DRIVER_EVENT_FORWARD;
        temp_args_t.p_context = p_instance->p_cfg->p_context;
        (p_instance->p_cfg->p_callback)(&temp_args_t);
    }

/* USE_RMW_TUNER */
    R_AID_CurrentCtrlISR();
/* USE_RMW_TUNER */

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance->p_cfg->p_callback)
    {
        temp_args_t.event     = MOTOR_DRIVER_EVENT_BACKWARD;
        temp_args_t.p_context = p_instance->p_cfg->p_context;
        (p_instance->p_cfg->p_callback)(&temp_args_t);
    }
}

/***********************************************************************************************************************
 * Function Name: rm_motor_driver_pin_cfg
 * Description  : Configure a pin
 * Arguments    : pin -
 *                    The pin
 *                cfg -
 *                    Configuration for the pin (PmnPFS register setting)
 * Return Value : None
 **********************************************************************************************************************/
static void rm_motor_driver_pin_cfg (bsp_io_port_pin_t pin, uint32_t cfg)
{
    R_BSP_PinAccessEnable();
    R_BSP_PinCfg(pin, cfg);
    R_BSP_PinAccessDisable();
}                                      /* End of function rm_motor_driver_pin_cfg() */

/***********************************************************************************************************************
 * Function Name : rm_motor_driver_output_enable
 * Description   : Enable PWM output
 * Arguments     : p_ctrl - The pointer to the motor driver module instance
 * Return Value  : None
 **********************************************************************************************************************/
static void rm_motor_driver_output_enable (motor_driver_instance_ctrl_t * p_ctrl)
{
    motor_driver_cfg_t           * p_cfg         = (motor_driver_cfg_t *) p_ctrl->p_cfg;
    motor_driver_extended_cfg_t  * p_extended    = (motor_driver_extended_cfg_t *) p_cfg->p_extend;
    three_phase_instance_t const * p_three_phase = p_cfg->p_three_phase_instance;
    timer_instance_t const       * p_u_phase_gpt = p_three_phase->p_cfg->p_timer_instance[0];
    timer_instance_t const       * p_v_phase_gpt = p_three_phase->p_cfg->p_timer_instance[1];
    timer_instance_t const       * p_w_phase_gpt = p_three_phase->p_cfg->p_timer_instance[2];

    p_ctrl->u4_gtioca_low_cfg |= MOTOR_DRIVER_IO_PORT_PERIPHERAL_MASK;
    p_ctrl->u4_gtiocb_low_cfg |= MOTOR_DRIVER_IO_PORT_PERIPHERAL_MASK;

    /* Set pin function */
    rm_motor_driver_pin_cfg(p_extended->port_up, p_ctrl->u4_gtioca_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_un, p_ctrl->u4_gtiocb_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_vp, p_ctrl->u4_gtioca_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_vn, p_ctrl->u4_gtiocb_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_wp, p_ctrl->u4_gtioca_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_wn, p_ctrl->u4_gtiocb_low_cfg);

    /* PWM output enable */
    R_GPT_OutputEnable(p_u_phase_gpt->p_ctrl, GPT_IO_PIN_GTIOCA_AND_GTIOCB);
    R_GPT_OutputEnable(p_v_phase_gpt->p_ctrl, GPT_IO_PIN_GTIOCA_AND_GTIOCB);
    R_GPT_OutputEnable(p_w_phase_gpt->p_ctrl, GPT_IO_PIN_GTIOCA_AND_GTIOCB);
}                                      /* End of function rm_motor_driver_output_enable */

/***********************************************************************************************************************
 * Function Name : rm_motor_driver_output_disable
 * Description   : Disable PWM output
 * Arguments     : p_ctrl - The pointer to the motor driver module instance
 * Return Value  : None
 **********************************************************************************************************************/
static void rm_motor_driver_output_disable (motor_driver_instance_ctrl_t * p_ctrl)
{
    motor_driver_cfg_t           * p_cfg         = (motor_driver_cfg_t *) p_ctrl->p_cfg;
    motor_driver_extended_cfg_t  * p_extended    = (motor_driver_extended_cfg_t *) p_cfg->p_extend;
    three_phase_instance_t const * p_three_phase = p_cfg->p_three_phase_instance;
    timer_instance_t const       * p_u_phase_gpt = p_three_phase->p_cfg->p_timer_instance[0];
    timer_instance_t const       * p_v_phase_gpt = p_three_phase->p_cfg->p_timer_instance[1];
    timer_instance_t const       * p_w_phase_gpt = p_three_phase->p_cfg->p_timer_instance[2];

    /* PWM output disable */
    R_GPT_OutputDisable(p_u_phase_gpt->p_ctrl, GPT_IO_PIN_GTIOCA_AND_GTIOCB);
    R_GPT_OutputDisable(p_v_phase_gpt->p_ctrl, GPT_IO_PIN_GTIOCA_AND_GTIOCB);
    R_GPT_OutputDisable(p_w_phase_gpt->p_ctrl, GPT_IO_PIN_GTIOCA_AND_GTIOCB);

    p_ctrl->u4_gtioca_low_cfg &= MOTOR_DRIVER_IO_PORT_GPIO_MASK;
    p_ctrl->u4_gtiocb_low_cfg &= MOTOR_DRIVER_IO_PORT_GPIO_MASK;

    rm_motor_driver_pin_cfg(p_extended->port_up, p_ctrl->u4_gtioca_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_un, p_ctrl->u4_gtiocb_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_vp, p_ctrl->u4_gtioca_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_vn, p_ctrl->u4_gtiocb_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_wp, p_ctrl->u4_gtioca_low_cfg);
    rm_motor_driver_pin_cfg(p_extended->port_wn, p_ctrl->u4_gtiocb_low_cfg);
}                                      /* End of function rm_motor_driver_output_disable */
