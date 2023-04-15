
#include "ht32.h"
#include "pwm.h"
#include "ht32_board_config.h"
#include "ht32f5xxxx_gpio.h"

/* Settings ------------------------------------------------------------------------------------------------*/
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

/* Private variables ---------------------------------------------------------------------------------------*/
s32 gIsTMStop = 0;
static u32 k;
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Init PWM function.
  * @retval None
  ***********************************************************************************************************/
void PWM_Init(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.GPTM0 = 1;
    CKCUClock.Bit.AFIO             = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure AFIO mode as TM function                                                                     */
  #if (HTCFG_PWM0_EN == 1)
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_MODE_4);		//AFIO_FUN_PWM
  #endif
  #if (HTCFG_PWM1_EN == 1)
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_5, AFIO_MODE_4);		//AFIO_FUN_PWM
  #endif
  #if (HTCFG_PWM2_EN == 1)
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_8, AFIO_MODE_4);		//AFIO_FUN_PWM
  #endif
  #if (HTCFG_PWM3_EN == 1)
  AFIO_GPxConfig(GPIO_PC, AFIO_PIN_9, AFIO_MODE_4);		//AFIO_FUN_PWM
  #endif


  { 
    TM_TimeBaseInitTypeDef TimeBaseInit;

    TimeBaseInit.Prescaler = 1000 - 1;            							// Timer clock = CK_AHB / HTCFG_PWM_TM_PRESCALER
    TimeBaseInit.CounterReload = 48000000/1/HTCFG_PWM_FREQ_HZ - 1;			//设置成50K，再分频1000，为50hz
    TimeBaseInit.RepetitionCounter = 0;
    TimeBaseInit.CounterMode = TM_CNT_MODE_UP;
    TimeBaseInit.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_GPTM0, &TimeBaseInit);
    TM_CRRPreloadCmd(HT_GPTM0, ENABLE);

    /* Clear Update Event Interrupt flag since the "TM_TimeBaseInit()" writes the UEV1G bit                 */
    TM_ClearFlag(HT_GPTM0, TM_FLAG_UEV);
  }

  { /* Channel n output configuration                                                                       */

    TM_OutputInitTypeDef OutInit;

    OutInit.OutputMode = TM_OM_PWM1;
    OutInit.Control = TM_CHCTL_ENABLE;
    OutInit.ControlN = TM_CHCTL_DISABLE;
    OutInit.Polarity = TM_CHP_NONINVERTED;
    OutInit.PolarityN = TM_CHP_NONINVERTED;
    OutInit.IdleState = MCTM_OIS_LOW;
    OutInit.IdleStateN = MCTM_OIS_HIGH;
    OutInit.Compare = HTCFG_PWM_IDLE_DUTY;
    OutInit.AsymmetricCompare = 0;

    #if (HTCFG_PWM0_EN == 1)
    OutInit.Channel = TM_CH_0;
    TM_OutputInit(HT_GPTM0, &OutInit); // CH0 Start Output as default value
    TM_CHCCRPreloadConfig(HT_GPTM0, TM_CH_0, ENABLE);
    #endif

    #if (HTCFG_PWM1_EN == 1)
    OutInit.Channel = TM_CH_1;
    TM_OutputInit(HT_GPTM0, &OutInit); // CH1 Start Output as default value
    TM_CHCCRPreloadConfig(HT_GPTM0, TM_CH_1, ENABLE);
    #endif

    #if (HTCFG_PWM2_EN == 1)
    OutInit.Channel = TM_CH_2;
    TM_OutputInit(HT_GPTM0, &OutInit); // CH2 Start Output as default value
    TM_CHCCRPreloadConfig(HT_GPTM0, TM_CH_2, ENABLE);
    #endif

    #if (HTCFG_PWM3_EN == 1)
    OutInit.Channel = TM_CH_3;
    TM_OutputInit(HT_GPTM0, &OutInit); // CH3 Start Output as default value
    TM_CHCCRPreloadConfig(HT_GPTM0, TM_CH_3, ENABLE);
    #endif
  }

  #if (LIBCFG_MCTM0)
  if (HT_GPTM0 == HT_MCTM0)
  {
    // Only MCTM need OE Enable
    MCTM_CHMOECmd(HT_GPTM0, ENABLE);

    // Enable MT_CHnN
    //MCTM_ChannelNConfig(HTCFG_PWM_TM_PORT, HTCFG_PWM_TM_CH, TM_CHCTL_ENABLE);
  }
  #endif

  /* Enable Update Event interrupt                                                                          */
  NVIC_EnableIRQ(GPTM0_IRQn);

  #if 0 // Default enable or disable
  TM_Cmd(HTCFG_PWM_TM_PORT, ENABLE);
  #endif
}

/*********************************************************************************************************//**
  * @brief  This function handles TM interrupt.
  * @retval None
  ***********************************************************************************************************/
void HTCFG_PWM_TM_IRQHandler(void)
{
  TM_ClearFlag(HT_GPTM0, TM_INT_UEV);
  k++;
	
	
  if (gIsTMStop == 1)
  {
    gIsTMStop = 2;
  }
  else if (gIsTMStop == 2)
  {
    TM_Cmd(HT_GPTM0, DISABLE);

    TM_IntConfig(HT_GPTM0, TM_INT_UEV, DISABLE);

    gIsTMStop = 0;
  }
}

/*********************************************************************************************************//**
  * @brief Enable or Disable PWM.
  * @param NewState: This parameter can be ENABLE or DISABLE.
  * @retval None
  ***********************************************************************************************************/
void PWM_Cmd(ControlStatus NewState)
{
  if (NewState == ENABLE)
  {
    TM_Cmd(HT_GPTM0, NewState);
  }
  else
  {
    // Set the flag to stop timer after update event.
    gIsTMStop = 1;

    // Set PWM as IDLE duty before stop it
    #if (HTCFG_PWM0_EN == 1)
    PWM_UpdateDuty(TM_CH_0, HTCFG_PWM_IDLE_DUTY);
    #endif
    #if (HTCFG_PWM1_EN == 1)
    PWM_UpdateDuty(TM_CH_1, HTCFG_PWM_IDLE_DUTY);
    #endif
    #if (HTCFG_PWM2_EN == 1)
    PWM_UpdateDuty(TM_CH_2, HTCFG_PWM_IDLE_DUTY);
    #endif
    #if (HTCFG_PWM3_EN == 1)
    PWM_UpdateDuty(TM_CH_3, HTCFG_PWM_IDLE_DUTY);
    #endif

    TM_IntConfig(HT_GPTM0, TM_INT_UEV, ENABLE);
  }
}

#if 0
/*********************************************************************************************************//**
  * @brief  Set PWM Frequency
  * @param  uReload: Reload value of timer)
  * @retval None
  ***********************************************************************************************************/
void PWM_SetFreq(u32 uReload)
{
  TM_SetCounterReload(HT_GPTM0, uReload);
}

/*********************************************************************************************************//**
  * @brief  Update PWM Duty
  * @param  TM_CH_n: Specify the TM channel.
  * @param  uCompare: PWM duty (Compare value of timer)
  * @retval None
  ***********************************************************************************************************/
void PWM_UpdateDuty(TM_CH_Enum TM_CH_n, u32 uCompare)
{
  TM_SetCaptureCompare(HT_GPTM0, TM_CH_n, uCompare);
}
#endif
