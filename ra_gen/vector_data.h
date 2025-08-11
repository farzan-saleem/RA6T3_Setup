/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (4)
#endif
/* ISR prototypes */
void poeg_event_isr(void);
void agt_int_isr(void);
void adc_scan_end_isr(void);
void gpt_capture_compare_a_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_POEG1_EVENT ((IRQn_Type) 0) /* POEG1 EVENT (Port Output disable 1 interrupt) */
#define POEG1_EVENT_IRQn          ((IRQn_Type) 0) /* POEG1 EVENT (Port Output disable 1 interrupt) */
#define VECTOR_NUMBER_AGT0_INT ((IRQn_Type) 1) /* AGT0 INT (AGT interrupt) */
#define AGT0_INT_IRQn          ((IRQn_Type) 1) /* AGT0 INT (AGT interrupt) */
#define VECTOR_NUMBER_ADC0_SCAN_END ((IRQn_Type) 2) /* ADC0 SCAN END (End of A/D scanning operation) */
#define ADC0_SCAN_END_IRQn          ((IRQn_Type) 2) /* ADC0 SCAN END (End of A/D scanning operation) */
#define VECTOR_NUMBER_GPT5_CAPTURE_COMPARE_A ((IRQn_Type) 3) /* GPT5 CAPTURE COMPARE A (Capture/Compare match A) */
#define GPT5_CAPTURE_COMPARE_A_IRQn          ((IRQn_Type) 3) /* GPT5 CAPTURE COMPARE A (Capture/Compare match A) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
