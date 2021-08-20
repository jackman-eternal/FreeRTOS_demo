#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"


#define START_STK_SIZE 120 
#define START_TASK_PRIORITY 1
void start_task( void * pvParameters );
TaskHandle_t  StartTask_Handler;

#define TASK1_STK_SIZE 100 
#define TASK1_TASK_PRIORITY 2
void task1_task( void * pvParameters );
TaskHandle_t Task1_Handler;

#define TASK2_STK_SIZE 100 
#define TASK2_TASK_PRIORITY 3
void task2_task( void * pvParameters );
TaskHandle_t Task2_Handler;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 	 
	delay_init();	    				//��ʱ������ʼ��	  
	//uart_init(115200);					//��ʼ������
	LED_INIT(0);		  					//��ʼ��LED
	xTaskCreate( (TaskFunction_t) start_task,
			     (char *) "start_task",
			     (configSTACK_DEPTH_TYPE) START_STK_SIZE,
			     ( void *) NULL,
			     (UBaseType_t) START_TASK_PRIORITY,
			     (TaskHandle_t *) &StartTask_Handler );
    vTaskStartScheduler();          //�����������
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
 


 