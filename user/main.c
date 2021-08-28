#include "stm32f10x.h" 
#include "delay.h" 
#include "led.h" 
#include "sys.h" 
#include "FreeRTOS.h" 
#include "task.h" 
#include "timer.h" 

//开始任务 
#define START_STK_SIZE 128   
#define START_TASK_PRIORITY 1  
void start_task(void * pvParameters );  
TaskHandle_t  StartTask_Handler;  
//中断任务 
#define INT_STK_SIZE 128    
#define INT_TASK_PRIORITY 2  
void interrupt_task(void * pvParameters );  
TaskHandle_t  Interrupt_Handler;  
int main(void)  
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4  	  	 
	delay_init();	    				//延时函数初始化	      
	uart_init(9600);					//初始化串口    
	LED_INIT(0);		  					//初始化LED  
    TIM2_INIT(); 	 
   	TIM3_INIT();     

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
    taskENTER_CRITICAL();            //进入临界区	   
	  //interrupt_task    
    xTaskCreate( (TaskFunction_t) interrupt_task ,   
				 (char *)  "interrupt_task",    
				 (configSTACK_DEPTH_TYPE) INT_STK_SIZE ,    
				 (void *) NULL ,    
				 (UBaseType_t) INT_TASK_PRIORITY ,    
				 (TaskHandle_t*) &Interrupt_Handler );    
    vTaskDelete(StartTask_Handler );     
    taskEXIT_CRITICAL();             //退出临界区	  			 
 }   
 void interrupt_task( void * pvParameters )   
 {   
	 static u32 num = 0;   
	 while(1)  
	 {   
	   num ++ ;  
	   if(num == 10)   
	   {  
		   printf("turn off \r\n");   
		   portDISABLE_INTERRUPTS();   
           delay_xms(5000);  
		 //  vTaskDelay(5000);  这里用此函数，会导致马上解除屏蔽，看不到效果
		   printf("turn on \r\n");   
           portENABLE_INTERRUPTS();    		   
	   }  
	   vTaskDelay(1000);   
	 }   
 }   
 
 
 /***测试任务的挂起和恢复******** 
 //任务一
#define TASK1_STK_SIZE 128  
#define TASK1_TASK_PRIORITY 2  
void task1_task( void * pvParameters );  
TaskHandle_t Task1_Handler;  
//任务二
#define TASK2_STK_SIZE 128 
#define TASK2_TASK_PRIORITY 3
void task2_task( void * pvParameters );
TaskHandle_t Task2_Handler;
//key_task 
#define KEY_STK_SIZE 128   
#define KEY_TASK_PRIORITY 4
void key_task( void * pvParameters );  
TaskHandle_t Key_Handler; 
 void start_task ( void * pvParameters ) 
 { 		 
	  //task 1 
    xTaskCreate( (TaskFunction_t) task1_task , 
				 (char *)  "task1_task", 
				 (configSTACK_DEPTH_TYPE) TASK1_STK_SIZE , 
				 (void *) NULL , 
				 (UBaseType_t) TASK1_TASK_PRIORITY , 
				 (TaskHandle_t*) &Task1_Handler ); 
      //task 2   任务2优先级高，先于任务1运行
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
 void task1_task(void * pvParameters)  
 {  
	 while(1)  
	 {  
		LED0 = ~LED0;   
        vTaskDelay(500);   		 
	 }  
 }  
 void task2_task(void * pvParameters)  
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
				vTaskSuspend(Task2_Handler); 
				vTaskDelay(20);  
			}  
		}  
		else 
		{
			vTaskDelay(5); 
		}
	 } 
 } 
*******/
