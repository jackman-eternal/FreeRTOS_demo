#include "stm32f10x.h" 
#include "delay.h" 
#include "led.h" 
#include "sys.h" 
#include "FreeRTOS.h" 
#include "task.h" 
#include "list.h"

//开始任务 
#define START_STK_SIZE 50   
#define START_TASK_PRIORITY 1  
void start_task(void * pvParameters );  
TaskHandle_t  StartTask_Handler; 
 //任务一
#define TASK1_STK_SIZE 128  
#define TASK1_TASK_PRIORITY 2  
void task1_task( void * pvParameters );  
TaskHandle_t Task1_Handler;  
 //查询
#define QUERY_STK_SIZE 256  
#define QUERY_TASK_PRIORITY 3  
void query_task( void * pvParameters );  
TaskHandle_t Query_Handler;  

int main(void)  
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4  	  	 
	delay_init();	    				            //延时函数初始化	      
	uart_init(9600);				            	//初始化串口    
	LED_INIT(0);		  				        	//初始化LED  
	xTaskCreate( (TaskFunction_t) start_task,       //任务函数指针
			     (char *) "start_task",             //任务函数名
			     (configSTACK_DEPTH_TYPE) START_STK_SIZE,   //任务堆栈大小
			     ( void *) NULL,                            //传递给任务函数的参数
			     (UBaseType_t) START_TASK_PRIORITY,         //任务优先级
			     (TaskHandle_t *) &StartTask_Handler );     //任务句柄
    vTaskStartScheduler();          //开启任务调度  
}   
 void start_task ( void * pvParameters )   
 { 	  
    taskENTER_CRITICAL();            //进入临界区	 
    //task 1 
    xTaskCreate( (TaskFunction_t) task1_task , 
				 (char *)  "task1_task", 
				 (configSTACK_DEPTH_TYPE) TASK1_STK_SIZE , 
				 (void *) NULL , 
				 (UBaseType_t) TASK1_TASK_PRIORITY , 
				 (TaskHandle_t*) &Task1_Handler ); 	 
    //query_task    
    xTaskCreate( (TaskFunction_t) query_task ,   
				 (char *)  "query_task",    
				 (configSTACK_DEPTH_TYPE) QUERY_STK_SIZE ,    
				 (void *) NULL ,    
				 (UBaseType_t) QUERY_TASK_PRIORITY ,    
				 (TaskHandle_t*) &Query_Handler );    
    vTaskDelete(StartTask_Handler );     
    taskEXIT_CRITICAL();             //退出临界区	  			 
 }   
 void task1_task(void * pvParameters)  
 {  
	 while(1)  
	 {  
        PCout(13)=0;  
        vTaskDelay(1000); 		 
        PCout(13)=1;  
        vTaskDelay(1000); 
	 }  
 }  
void query_task(void * pvParameters)
{
	
   while(1)
   {
     vTaskDelay(10); 	   
   }
}







/**********************************任务状态查询代码****************
uint32_t Total_time;
UBaseType_t ArraySize,x;
TaskStatus_t* StatusArray;
ArraySize = uxTaskGetNumberOfTasks();         //获取系统任务数量
StatusArray = pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//为这个数组申请内存,
//任务数*任务状态结构体的大小=任务状态结构体数组的大小,并返回申请是否成功
if(StatusArray!=NULL)
{
	 ArraySize = uxTaskGetSystemState((TaskStatus_t* )  StatusArray, 
									  (UBaseType_t   )   ArraySize, 
									  (uint32_t*     )   &Total_time );
	printf("TaskName\tTaskNumber\tCurrentPriority\tCurrentState\tRunTimeCounter\tStackHighWaterMark\r\n");
	for(x=0;x<=sizeof(ArraySize);x++)
	{
		printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\r\n",StatusArray[x].pcTaskName,
								(int)StatusArray[x].xTaskNumber,
								(int)StatusArray[x].uxCurrentPriority,
								StatusArray[x].eCurrentState,
								StatusArray[x].ulRunTimeCounter,
								StatusArray[x].usStackHighWaterMark);																										        	
	}
}
vPortFree(StatusArray);	//释放内存
*******************************************************
//printf("TaskName\t\tPriority\t\tTaskNumber\t\t\r\n");
for(x=0;x<ArraySize;x++)
{
//通过串口打印出获取到的系统任务的有关信息，比如任务名称、
//任务优先级和任务编号。
printf("%s\t\t%d\t\t\t%d\t\t\t\r\n",				
		StatusArray[x].pcTaskName,
		(int)StatusArray[x].uxCurrentPriority,
		(int)StatusArray[x].xTaskNumber);		
}
********************************************************
UBaseType_t  Priority;
Priority = uxTaskPriorityGet(Query_Handler); //获取任务优先级

*********************************************/


/******************************列表**********
#define LIST_STK_SIZE 128  
#define LIST_TASK_PRIORITY 3  
void list_task( void * pvParameters );  
TaskHandle_t List_Handler; 
//定义一个列表和三个列表项
List_t TestList;
ListItem_t ListItem1;
ListItem_t ListItem2;
ListItem_t ListItem3;
int main(void)  
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4  	  	 
	delay_init();	    				//延时函数初始化	      
	uart_init(9600);					//初始化串口    
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
    taskENTER_CRITICAL();            //进入临界区	 
    //task 1 
    xTaskCreate( (TaskFunction_t) task1_task , 
				 (char *)  "task1_task", 
				 (configSTACK_DEPTH_TYPE) TASK1_STK_SIZE , 
				 (void *) NULL , 
				 (UBaseType_t) TASK1_TASK_PRIORITY , 
				 (TaskHandle_t*) &Task1_Handler ); 	 
	//list_task    
    xTaskCreate( (TaskFunction_t) list_task ,   
				 (char *)  "list_task",    
				 (configSTACK_DEPTH_TYPE) LIST_STK_SIZE ,    
				 (void *) NULL ,    
				 (UBaseType_t) LIST_TASK_PRIORITY ,    
				 (TaskHandle_t*) &List_Handler );    
    vTaskDelete(StartTask_Handler );     
    taskEXIT_CRITICAL();             //退出临界区	  			 
 }   
 void task1_task(void * pvParameters)  
 {  
	 while(1)  
	 {  
		LED0 = ~LED0;   
        vTaskDelay(500);   		 
	 }  
 }  
 //初始化列表和列表项
 void list_task( void * pvParameters )
 {
	 vListInitialise(&TestList );//初始化列表
     vListInitialiseItem(&ListItem1);//初始化列表项
     vListInitialiseItem(&ListItem2);
     vListInitialiseItem(&ListItem3);
     ListItem1.xItemValue  = 40;
     ListItem2.xItemValue  = 60;
     ListItem3.xItemValue  = 50;
    //第二步：打印列表和其他列表项的地址
	printf("项目                              地址				    \r\n");
	printf("TestList                          %#x					\r\n",(int)&TestList);
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd                %#x					\r\n",(int)(&TestList.xListEnd));
	printf("ListItem1                         %#x					\r\n",(int)&ListItem1);
	printf("ListItem2                         %#x					\r\n",(int)&ListItem2);
	printf("ListItem3                         %#x					\r\n",(int)&ListItem3);
   } 
******************************************/

/************************************中断任务的屏蔽与解除
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
 **************************************************/
 
 /**********************测试任务的挂起和恢复******** 
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
**************************************/
