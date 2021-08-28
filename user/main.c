#include "stm32f10x.h" 
#include "delay.h" 
#include "led.h" 
#include "sys.h" 
#include "FreeRTOS.h" 
#include "task.h" 
#include "timer.h" 

//��ʼ���� 
#define START_STK_SIZE 128   
#define START_TASK_PRIORITY 1  
void start_task(void * pvParameters );  
TaskHandle_t  StartTask_Handler;  
//�ж����� 
#define INT_STK_SIZE 128    
#define INT_TASK_PRIORITY 2  
void interrupt_task(void * pvParameters );  
TaskHandle_t  Interrupt_Handler;  
int main(void)  
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4  	  	 
	delay_init();	    				//��ʱ������ʼ��	      
	uart_init(9600);					//��ʼ������    
	LED_INIT(0);		  					//��ʼ��LED  
    TIM2_INIT(); 	 
   	TIM3_INIT();     

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
    taskENTER_CRITICAL();            //�����ٽ���	   
	  //interrupt_task    
    xTaskCreate( (TaskFunction_t) interrupt_task ,   
				 (char *)  "interrupt_task",    
				 (configSTACK_DEPTH_TYPE) INT_STK_SIZE ,    
				 (void *) NULL ,    
				 (UBaseType_t) INT_TASK_PRIORITY ,    
				 (TaskHandle_t*) &Interrupt_Handler );    
    vTaskDelete(StartTask_Handler );     
    taskEXIT_CRITICAL();             //�˳��ٽ���	  			 
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
		 //  vTaskDelay(5000);  �����ô˺������ᵼ�����Ͻ�����Σ�������Ч��
		   printf("turn on \r\n");   
           portENABLE_INTERRUPTS();    		   
	   }  
	   vTaskDelay(1000);   
	 }   
 }   
 
 
 /***��������Ĺ���ͻָ�******** 
 //����һ
#define TASK1_STK_SIZE 128  
#define TASK1_TASK_PRIORITY 2  
void task1_task( void * pvParameters );  
TaskHandle_t Task1_Handler;  
//�����
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
      //task 2   ����2���ȼ��ߣ���������1����
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
			vTaskDelay(10); //���� 
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
