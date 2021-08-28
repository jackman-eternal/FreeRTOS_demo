#  练习freeRTOS FreeRTOS_demo(参考正点原子的源码,采用的单片机是stm32f103c8t6)  
## 主函数内容:以下代码是测试任务的创建和删除,改代码执行和测试（主函数创建开始任务start_task,开始任务执行创建两个点灯任务,并且开始任务创建后删除start_task）,接着测试（任务一执行五次后删除任务二，此时任务二不再执行）

//开始任务的参数  
#define START_STK_SIZE 120   
#define START_TASK_PRIORITY 1  
void start_task( void * pvParameters );  
TaskHandle_t  StartTask_Handler;  
//任务一  
#define TASK1_STK_SIZE 100   
#define TASK1_TASK_PRIORITY 2  
void task1_task( void * pvParameters );  
TaskHandle_t Task1_Handler;  
//任务二  
#define TASK2_STK_SIZE 100   
#define TASK2_TASK_PRIORITY 3  
void task2_task( void * pvParameters );  
TaskHandle_t Task2_Handler;   
int main(void)   
{   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	   	 
	delay_init();	    				//延时函数初始化	     
	//uart_init(115200);					//初始化串口   
	LED_INIT(0);		  					//初始化LED   
	xTaskCreate( (TaskFunction_t) start_task,  
			     (char *) "start_task",   
			     (configSTACK_DEPTH_TYPE) START_STK_SIZE,  
			     ( void *) NULL,   
			     (UBaseType_t) START_TASK_PRIORITY,    
			     (TaskHandle_t *) &StartTask_Handler );   
    vTaskStartScheduler();          //开启任务调度    
}   
 void start_task ( void * pvParameters )   
 {   
	  //task 1   
    xTaskCreate( (TaskFunction_t) task1_task ,   
				 (char *)  "task1_task",    
				 (configSTACK_DEPTH_TYPE) TASK1_STK_SIZE ,   
				 (void *) NULL ,   
				 (UBaseType_t) TASK1_TASK_PRIORITY ,   
				 (TaskHandle_t*) &Task1_Handler );  
      //task 2    
    xTaskCreate( (TaskFunction_t) task2_task ,   
				 (char *) "",   
				 (configSTACK_DEPTH_TYPE) TASK2_STK_SIZE ,   
				 (void *) NULL ,   
				 (UBaseType_t) TASK2_TASK_PRIORITY ,  
				 (TaskHandle_t*) &Task2_Handler );   
    vTaskDelete(StartTask_Handler );    					  
 }   
 void task1_task( void * pvParameters )    
 {   
	 char i=0;     
	 while(1)   
	 {    
		 if(i==5)   
 		 {   
			 vTaskDelete(Task2_Handler );   
		 }    
		LED0 = ~LED0 ;   
		 i++;    
        vTaskDelay(500);   		 
	 }   
 }   
 void task2_task( void * pvParameters )   
 {    
	  while(1)   
	 {   
		 LED1 = 0;   
		 vTaskDelay(250);  
         LED1 = 1;  
         vTaskDelay(250);   		 
	 }   
 }    
### 以下代码 测试任务挂起和恢复实验  （基于上面任务的创建和恢复，首先创建开始任务start_task,其次开始任务函数执行，创建三个任务，点灯任务一和点灯任务二，还有一个key_task(设置PA3 输入为 ： 1->执行挂起任务一)；输入  ：  0->恢复任务一 ） 代码如下：
#include "stm32f10x.h"   
#include "delay.h"    
#include "led.h"      
#include "sys.h"     
#include "FreeRTOS.h"    
#include "task.h"    
//开始任务    
#define START_STK_SIZE 120    
#define START_TASK_PRIORITY 1  
void start_task( void * pvParameters );   
TaskHandle_t  StartTask_Handler;    
//任务一    
#define TASK1_STK_SIZE 100      
#define TASK1_TASK_PRIORITY 2    
void task1_task( void * pvParameters );    
TaskHandle_t Task1_Handler;    
//任务二    
#define TASK2_STK_SIZE 100      
#define TASK2_TASK_PRIORITY 3    
void task2_task( void * pvParameters );     
TaskHandle_t Task2_Handler;    
//key_task     
#define KEY_STK_SIZE 100     
#define KEY_TASK_PRIORITY 4    
void key_task( void * pvParameters );   
TaskHandle_t Key_Handler;    
int main(void)     
{   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4      	 	 
	delay_init();	    				//延时函数初始化	      
	//uart_init(115200);					//初始化串口    
	LED_INIT(0);		  					//初始化LED    
	xTaskCreate( (TaskFunction_t) start_task,   
			     (char *) "start_task",   
			     (configSTACK_DEPTH_TYPE) START_STK_SIZE,  
			     ( void *) NULL,  
			     (UBaseType_t) START_TASK_PRIORITY,  
			     (TaskHandle_t *) &StartTask_Handler );  
    vTaskStartScheduler();          //开启任务调度  
}  
 void start_task ( void * pvParameters )  
 {  
	  //task 1  
    xTaskCreate( (TaskFunction_t) task1_task ,  
				 (char *)  "task1_task",  
				 (configSTACK_DEPTH_TYPE) TASK1_STK_SIZE ,  
				 (UBaseType_t) TASK1_TASK_PRIORITY ,  
				 (TaskHandle_t*) &Task1_Handler );  
      //task 2   
    xTaskCreate( (TaskFunction_t) task2_task ,  
				 (char *) "task2_task",  
				 (configSTACK_DEPTH_TYPE) TASK2_STK_SIZE ,   
				 (void *) NULL ,  
				 (UBaseType_t) TASK2_TASK_PRIORITY ,  
				 (TaskHandle_t*) &Task2_Handler );  
	  //key_task		   
	 xTaskCreate( (TaskFunction_t) key_task ,  
				 (char *) "key_task",  
				 (configSTACK_DEPTH_TYPE) KEY_STK_SIZE ,  
				 (void *) NULL ,  
				 (UBaseType_t) KEY_TASK_PRIORITY ,  
				 (TaskHandle_t*) &Key_Handler );	  		 
    vTaskDelete(StartTask_Handler ); 	 	 			  						   
 }   
 void task1_task( void * pvParameters )  
 {  
//	 char i=0;   
	 while(1)  
	 {  
//		 if(i==5)  
//		 {  
//			 vTaskDelete(Task2_Handler );   
//		 }  
		LED0 = ~LED0 ;  
//		 i++;  
        vTaskDelay(500);  		  
	 }  
 }  
 void task2_task( void * pvParameters )  
 {  
	  while(1)  
	 {  
		 LED1 = 0;  
		 vTaskDelay(250);  
         LED1 = 1;  
         vTaskDelay(250); 	 	  
	 }    
 }  
 void key_task( void * pvParameters )   
 {   
	  while(1)  
	 {   
		 if(PAin(3)== 1)    
		{  
			vTaskDelay(10); //防抖  
			if(PAin(3)== 1)  
			{  
				vTaskSuspend(Task1_Handler);  
				vTaskDelay(20);   
			}   
		}   
		if(PAin(3)==0)  
		{  
			vTaskDelay(10);  
			if(PAin(3)==0)  
			{  
				vTaskResume(Task1_Handler);  
				vTaskDelay(20);   
			}   
		}   
	 }   
 }        
####  以下代码是基于上面的代码进一步修改   (测试从中断服务函数中恢复任务,PA2 控制 led1  PA8 控制 le0 PA3控制任务一的挂起和恢复和任务二的挂起，PA4的中断服务函数中实现任务二的恢复)
注意： 
注意事项(避免程序卡死)！！！ 
中断函数中不可以使用vTaskDelay()！  
##### 所以FreeRTOS的API函数只有带FromISR后缀的才能在中断函数中使用，而**vTaskDelay()**好像也没有FromISR版本，所以就不能使用！推而广之，其它不带FromISR后缀的API函数也不能在中断函数中使用！  
中断函数中必须使用带FromISR后缀的API函数！   
这一条和上一条其实是一个意思，实验中在中断函数中对信号量进行释放，使用的是xTaskResumeFromISR()函数，如果改成vTaskResume()，实测发现程序同样会卡死在这里。   
加入以下代码  ：
这里添加在led.c中，实现在中断里恢复一次任务 
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
 ###### 如果在中断函数中使用了FreeRTOS的API函数，当然前提也是使用带FromISR后缀的，中断的优先级不能高于宏定义configMAX_SYSCALL_INTERRUPT_PRIORITY，这个宏定义在FreeRTOSConfig.h中：即中断优先级设置范围为5~15(0xf)。当然，如果中断函数中没有使用FreeRTOS的API，那么中断的优先级就不受限制。
 [https://blog.csdn.net/hqy450665101/article/details/113283748?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163006782716780269811042%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=163006782716780269811042&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v29_ecpm-1-113283748.first_rank_v2_pc_rank_v29&utm_term=freeRTOS%E5%9C%A8%E4%B8%AD%E6%96%AD%E5%87%BD%E6%95%B0%E6%81%A2%E5%A4%8D%E4%BB%BB%E5%8A%A1&spm=1018.2226.3001.4187]  

 FreeRTOS关于任务调度的函数  

 32423

 


 
 







 


