#include "ht32.h"
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "pwm.h"
#include "GPIOinput.h"
#include "delay.h"

const char HelloString[] = "Hello,This is  test. \r\n";
int main(void)
{			
	int input=0;	  
	GPIOInout_CKCU_Configuration();              
  GPIO_IN_Configuration();                                                                    
  USART_Configuration();
	PWM_Init();
	
	
  printf("Loading Finished! %d\r\n", input);
	USART_Tx1(HelloString, sizeof(HelloString));
	USART_Tx0(HelloString, sizeof(HelloString));
  while (1)
  {	
		unsigned char i=1;
		GPIO_MainRoutine();
		if(i--)
		{
			PWM_SetFreq(PWM_FREQ_40K);					//Æô¶¯¶æ»ú£¬50Hz
			PWM_UpdateDuty(TM_CH_3, PWM_FREQ_40K * 0.125);
			PWM_UpdateDuty(TM_CH_2, PWM_FREQ_40K * 0.125);
			PWM_UpdateDuty(TM_CH_1, PWM_FREQ_40K * 0.125);
			PWM_UpdateDuty(TM_CH_0, PWM_FREQ_40K * 0.125);
			PWM_Cmd(ENABLE);
			delay_ms(1000);
			PWM_UpdateDuty(TM_CH_3, PWM_FREQ_40K * 0.075);
			PWM_UpdateDuty(TM_CH_2, PWM_FREQ_40K * 0.075);
			PWM_UpdateDuty(TM_CH_1, PWM_FREQ_40K * 0.075);
			PWM_UpdateDuty(TM_CH_0, PWM_FREQ_40K * 0.075);
			delay_ms(1000);
			
		}
		
		
	}
	
}
