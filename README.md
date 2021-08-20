# FreeRTOS_demo   参考正点原子的源码   采用的单片机是stm32f103c8t6  
## 主函数内容     以下代码是测试任务的创建和删除  改代码执行和测试（主函数创建开始任务start_task,开始任务执行创建两个点灯任务）

//开始任务的属性
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

### 以下代码 测试任务挂起和恢复实验

