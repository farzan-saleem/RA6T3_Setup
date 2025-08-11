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
#include "rm_motor_speed.h"

/* USE_RMW_TUNER */
#include "r_aid_tuner_gui.h"
#include "r_aid_auto_identify.h"
/* USE_RMW_TUNER */

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
void rm_motor_speed_cyclic_tuner(timer_callback_args_t * p_args);

/***********************************************************************************************************************
 * Function Name : rm_motor_speed_cyclic
 * Description   : Cyclic process of Speed Control (Call at timer interrupt)
 * Arguments     : p_args - The pointer to used arguments
 * Return Value  : None
 **********************************************************************************************************************/
void rm_motor_speed_cyclic_tuner (timer_callback_args_t * p_args)
{
    motor_speed_instance_t      * p_instance      = (motor_speed_instance_t *) p_args->p_context;
    motor_speed_callback_args_t   temp_args_t;

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance->p_cfg->p_callback)
    {
        temp_args_t.event     = MOTOR_SPEED_EVENT_FORWARD;
        temp_args_t.p_context = p_instance->p_cfg->p_context;
        (p_instance->p_cfg->p_callback)(&temp_args_t);
    }

/* USE_RMW_TUNER */
    R_AID_SpeedCtrlISR();
/* USE_RMW_TUNER */

    /* Invoke the callback function if it is set. */
    if (NULL != p_instance->p_cfg->p_callback)
    {
        temp_args_t.event     = MOTOR_SPEED_EVENT_BACKWARD;
        temp_args_t.p_context = p_instance->p_cfg->p_context;
        (p_instance->p_cfg->p_callback)(&temp_args_t);
    }
}
