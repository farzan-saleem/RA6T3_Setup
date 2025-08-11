/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = poeg_event_isr, /* POEG1 EVENT (Port Output disable 1 interrupt) */
            [1] = agt_int_isr, /* AGT0 INT (AGT interrupt) */
            [2] = adc_scan_end_isr, /* ADC0 SCAN END (End of A/D scanning operation) */
            [3] = gpt_capture_compare_a_isr, /* GPT5 CAPTURE COMPARE A (Capture/Compare match A) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_POEG1_EVENT,GROUP0), /* POEG1 EVENT (Port Output disable 1 interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_AGT0_INT,GROUP1), /* AGT0 INT (AGT interrupt) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_ADC0_SCAN_END,GROUP2), /* ADC0 SCAN END (End of A/D scanning operation) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_GPT5_CAPTURE_COMPARE_A,GROUP3), /* GPT5 CAPTURE COMPARE A (Capture/Compare match A) */
        };
        #endif
        #endif
