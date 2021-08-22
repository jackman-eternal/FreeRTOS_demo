#include "led.h"
//PC 13

void LED_INIT(int flag) 
{
	GPIO_InitTypeDef GPIO_LED; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE ); 
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_LED.GPIO_Pin  = GPIO_Pin_13 ;
	GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC ,&GPIO_LED);
	GPIO_LED.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_LED.GPIO_Pin  = GPIO_Pin_8 |GPIO_Pin_2 ;
    GPIO_LED.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_LED ); 
    GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_2 );  
    GPIO_LED.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;
    GPIO_LED.GPIO_Pin   = GPIO_Pin_3 ;
    GPIO_Init(GPIOA,&GPIO_LED); 	
	if(flag == 1)
	{
		GPIO_ResetBits(GPIOC ,GPIO_Pin_13 ); 
	} 
	else if(flag == 0) 
	{ 
		GPIO_SetBits(GPIOC,GPIO_Pin_13 );  
	} 
} 
void EXTI_PA4_init(void)  //PA4 触发中断实现任务的恢复    
{ 
	GPIO_InitTypeDef GPIO_EXTI_PA4;     
	EXTI_InitTypeDef EXTI_GPIO_Typedef;     
	NVIC_InitTypeDef NVIC_GPIO_Typedef;     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE );     
	GPIO_EXTI_PA4.GPIO_Mode  = GPIO_Mode_IPU ; // 上拉   
	GPIO_EXTI_PA4.GPIO_Pin   = GPIO_Pin_4 ;    
	GPIO_Init(GPIOA ,&GPIO_EXTI_PA4);   
    EXTI_GPIO_Typedef.EXTI_Line  = EXTI_Line4 ;   
	EXTI_GPIO_Typedef.EXTI_LineCmd = ENABLE ;   
	EXTI_GPIO_Typedef.EXTI_Mode =EXTI_Mode_Interrupt ;    
	EXTI_GPIO_Typedef.EXTI_Trigger =EXTI_Trigger_Falling ;  //下降沿触发   
	EXTI_Init(&EXTI_GPIO_Typedef);   
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA ,GPIO_PinSource4 );   
    NVIC_GPIO_Typedef.NVIC_IRQChannel = EXTI4_IRQn ;    
    NVIC_GPIO_Typedef.NVIC_IRQChannelCmd = ENABLE ;   
	NVIC_GPIO_Typedef.NVIC_IRQChannelPreemptionPriority  = 6;   
	NVIC_Init(& NVIC_GPIO_Typedef) ;
}   
void EXTI4_IRQHandler(void)   
{  
	 BaseType_t YieldRequie;  
     delay_xms(10);  
	 if(EXTI_GetITStatus(EXTI_Line4 )!=RESET)   
	 {   
		    YieldRequie = xTaskResumeFromISR(Task2_Handler);   
		    if(YieldRequie == pdTRUE )  
		    {     
			 portYIELD_FROM_ISR(YieldRequie); //任务切换  
		    }   
	 }   
	 EXTI_ClearITPendingBit(EXTI_Line4);  
}


 
 
 


