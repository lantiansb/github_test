
#ifndef __PWM_H
#define __PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Settings ------------------------------------------------------------------------------------------------*/
#define HTCFG_PWM_TM_PRESCALER                    (1)         // 1 ~ 65535
#define HTCFG_PWM_FREQ_HZ                         (50000)
#define HTCFG_PWM_IDLE_STATE                      (0)         // 0: 0 duty, 1: 100 duty
#define  HTCFG_PWM_TM_PCLK                       (48000000)
#define HTCFG_PWM_TM_RELOAD                       (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / HTCFG_PWM_FREQ_HZ)

#define HTCFG_PWM0_EN                             (1)
#define HTCFG_PWM1_EN                             (1)
#define HTCFG_PWM2_EN                             (1)
#define HTCFG_PWM3_EN                             (1)

/* Exported constants --------------------------------------------------------------------------------------*/
#define PWM_DUTY_0       (0)
#define PWM_DUTY_25      (HTCFG_PWM_TM_RELOAD * 0.25)
#define PWM_DUTY_50      (HTCFG_PWM_TM_RELOAD * 0.5)
#define PWM_DUTY_75      (HTCFG_PWM_TM_RELOAD * 0.75)
#define PWM_DUTY_100     (0xFFFF)

#define PWM_FREQ_50K     (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 50000)
#define PWM_FREQ_40K     (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 40000)
#define PWM_FREQ_12K     (HTCFG_PWM_TM_PCLK / HTCFG_PWM_TM_PRESCALER / 12000)



//TM_CH_3   PC9
//TM_CH_2   PC8
//TM_CH_1   PC5
//TM_CH_0   PC4


#define HTCFG_PWM_TM_PORT                         STRCAT2(HT_,       HTCFG_PWM_TM_IPN)
#define HTCFG_PWM_TM_AFIO_FUN                     STRCAT2(AFIO_FUN_, HTCFG_PWM_TM_IPN)
#define HTCFG_PWM_TM_IRQn                         STRCAT2(HTCFG_PWM_TM_IPN,  _IRQn)
#define HTCFG_PWM_TM_IRQHandler                   STRCAT2(HTCFG_PWM_TM_IPN,  _IRQHandler)

#if (HTCFG_PWM_IDLE_STATE == 0)
#define HTCFG_PWM_IDLE_DUTY PWM_DUTY_0
#else
#define HTCFG_PWM_IDLE_DUTY PWM_DUTY_100
#endif

#if (HTCFG_PWM0_EN == 1)
#define HTCFG_PWM0_GPIO_ID                        STRCAT2(GPIO_P,    HTCFG_PWM0_GPIOX)
#define HTCFG_PWM0_AFIO_PIN                       STRCAT2(AFIO_PIN_, HTCFG_PWM0_GPION)
#define PWM_CH0                                   STRCAT2(TM_CH_,    HTCFG_PWM0_CHN)
#endif

#if (HTCFG_PWM1_EN == 1)
#define HTCFG_PWM1_GPIO_ID                        STRCAT2(GPIO_P,    HTCFG_PWM1_GPIOX)
#define HTCFG_PWM1_AFIO_PIN                       STRCAT2(AFIO_PIN_, HTCFG_PWM1_GPION)
#define PWM_CH1                                   STRCAT2(TM_CH_,    HTCFG_PWM1_CHN)
#endif

#if (HTCFG_PWM2_EN == 1)
#define HTCFG_PWM2_GPIO_ID                        STRCAT2(GPIO_P,    HTCFG_PWM2_GPIOX)
#define HTCFG_PWM2_AFIO_PIN                       STRCAT2(AFIO_PIN_, HTCFG_PWM2_GPION)
#define PWM_CH2                                   STRCAT2(TM_CH_,    HTCFG_PWM2_CHN)
#endif

#if (HTCFG_PWM3_EN == 1)
#define HTCFG_PWM3_GPIO_ID                        STRCAT2(GPIO_P,    HTCFG_PWM3_GPIOX)
#define HTCFG_PWM3_AFIO_PIN                       STRCAT2(AFIO_PIN_, HTCFG_PWM3_GPION)
#define PWM_CH3                                   STRCAT2(TM_CH_,    HTCFG_PWM3_CHN)
#endif







/* Exported functions --------------------------------------------------------------------------------------*/
void PWM_Init(void);
void PWM_Cmd(ControlStatus NewState);

#if 1
#define PWM_SetFreq(uReload)     TM_SetCounterReload(HT_GPTM0, uReload)
#define PWM_UpdateDuty(CH, CMP)  TM_SetCaptureCompare(HT_GPTM0, CH, CMP)
#else
void PWM_SetFreq(u64 uReload);
void PWM_UpdateDuty(TM_CH_Enum TM_CH_n, u64 uCompare);
#endif

#ifdef __cplusplus
}
#endif

#endif
