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
###  以下代码是基于上面的代码进一步修改   (测试从中断服务函数中恢复任务,PA2 控制 led1  PA8 控制 le0 PA3控制任务一的挂起和恢复和任务二的挂起，PA4的中断服务函数中实现任务二的恢复)
注意： 
注意事项(避免程序卡死)！！！ 
中断函数中不可以使用vTaskDelay()！  
### 所以FreeRTOS的API函数只有带FromISR后缀的才能在中断函数中使用，而**vTaskDelay()**好像也没有FromISR版本，所以就不能使用！推而广之，其它不带FromISR后缀的API函数也不能在中断函数中使用！  
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
 ### 如果在中断函数中使用了FreeRTOS的API函数，当然前提也是使用带FromISR后缀的，中断的优先级不能高于宏定义configMAX_SYSCALL_INTERRUPT_PRIORITY，这个宏定义在FreeRTOSConfig.h中：即中断优先级设置范围为5~15(0xf)。当然，如果中断函数中没有使用FreeRTOS的API，那么中断的优先级就不受限制。
 [https://blog.csdn.net/hqy450665101/article/details/113283748?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163006782716780269811042%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=163006782716780269811042&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v29_ecpm-1-113283748.first_rank_v2_pc_rank_v29&utm_term=freeRTOS%E5%9C%A8%E4%B8%AD%E6%96%AD%E5%87%BD%E6%95%B0%E6%81%A2%E5%A4%8D%E4%BB%BB%E5%8A%A1&spm=1018.2226.3001.4187]  
 FreeRTOS关于任务调度的函数  
 ###　以下代码是测试freertos的中断的屏蔽实验（主要实现的功能，利用定时器2，3更新中断，并且在中断服务函数中打印相关提示，由于TIM2的中断优先级是4，故无法屏蔽，而TIM3的中断优先级为5，可以屏蔽）代码如下：
 主函数中代码：  
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
	 static u32 num = 0 ;       
	 while(1)     
	 {      
	   num ++ ;      
	   if(num == 10)      
	   {     
		   printf("turn off \r\n");      
		   portDISABLE_INTERRUPTS();      
           delay_xms(5000);     //使用该函数可能不会引起任务调度，影响较小
		   printf("turn on \r\n");       
           portENABLE_INTERRUPTS();       		   
	   }     
	   vTaskDelay(1000); //任务应用函数可用，在中断服务函数不可用未带ISR后缀的FreeRTOS的API函数      
	 }       
 }       
### 对中断屏蔽实验的重要补充，由于解除中断只需要调用函数（vPortSetBASEPRI( 0 )），而其他的FreeRTOS的函数也可能会调用此函数，因此，285行的延时函数不用freeRTOS自带的延时函数，这里补充下，如果使用则屏蔽中断后马上解除，看不到效果。
UBaseType_t  Priority;   
Priority = uxTaskPriorityGet(Query_Handler);   
调用此函数可查询优先级 
delay_xms(10);						// 延时10ms 这个延时函数不会引起任务调度的    
注意上面的小细节   
### 关于时间片--任务调度 (对于FreeRTOS 允许同等任务优先级存在, 那么对于多个同等优先级的任务运行,FreeRTOS 的机制就是对于同等优先级任务来说, 每个任务允许运行一个时间片.这个任务消耗完一个时间片,那么CPU的使用权,将会移交给同等优先级下的另一个任务.使用,如此反复, 直到次优先级完全对CPU使用权进行释放.概念: 时间片是由 configTICK_RATE_HZ 这个宏定义决定的. 在平常设置为1000 表示 时间片的长度为1/1000 S 相当于1ms.)[https://blog.csdn.net/longjingcha110/article/details/86509489]
遇到的问题，在测试查询系统任务时，低优先级的任务一直没有执行,原因高优先级使用while(1),导致低优先级的任务不运行
### 注意动态内存申请，调用函数 pvPortMalloc() 和 vPortFree()，调用格式参考函数定义                 eg:      定义 TaskStatus_t* StatusArray;                                               StatusArray = pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//为这个数组申请内存                 if(StatusArray!=NULL){}//判断内存是否申请成功                                                  释放内存     vPortFree(StatusArray); [https://blog.csdn.net/Hxj_CSDN/article/details/86816688?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163030473816780255275485%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=163030473816780255275485&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v29_ecpm-1-86816688.first_rank_v2_pc_rank_v29&utm_term=FreeRTOS+pvportmall&spm=1018.2226.3001.4187]学习FreeRTOS的总结博客
###　　　freertos编程标准和风格[https://freertos.blog.csdn.net/article/details/50057531?utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-1.control]  注：对任务创建函数里的一个“数据”的理解   typedef void (*TaskFunction_t)( void * ); typedef ： （复杂的变量声明理解复杂声明可用的“右左法则”：从变量名看起，先往右，再往左，碰到一个圆括号就调转阅读的方向；括号内分析完就跳出括号，还是按先右后左的顺序，如此循环，直到整个声明分析完。示例： int ( * func)(int * p) ;   首先找到变量名func,外面有一对括号，且左边有 * ，说明func是一个指针；跳出这个括号，看右边又有括号，说明（ * func）是一个函数，func是一个指向这类函数的指针，具有int *的形参，返回值是int)
###  typedef void ( * TaskFunction_t)( void * ); typedef(定义一个参数为void * 类型,返回值为 void的函数指针类型.可以用此类型声明指针变量,存储函数的地址,用指针变量跳转到函数执行.)例子分析2  int ( * func[ 5])(int * );  func 右边是一个[]运算符，说明func是具有5个元素的数组；func的左边有一个*，说明func的元素是指针（注意这里的 *  不是修饰func，而是修饰 func[ 5]的，原因是[ ]运算符优先级比 * 高，func先跟[]结合）。跳出这个括号，看右边，又遇到圆括号，说明func数组的元素是函数类型的指 针，它指向的函数具有int   * 类型的形参，返回值类型为int。    type (* (....)函数指针    type ( *)[]数组指针. 常常会遇到比较复杂的变量声明,使用typedef作简化自有其价值。参考链接[https://www.amobbs.com/thread-5688405-1-1.html] 
下面是三个变量的声明，我想使用typdef分别给它们定义一个别名. (对复杂变量建立一个类型别名的方法很简单，你只要在传统的变量声明表达式里用类型名替代变量名，然后把关键字typedef加在该语句的开头就行了。)       
int *(*a[5])(int, char*);  
//pFun是我们建的一个类型别名
typedef int *(*pFun)(int, char*); //使用定义的新类型来声明对象，等价于int* (*a[5])(int, char*);  
pFun a[5];    
2：void (*b[10]) (void (*)());   //首先为上面表达式蓝色部分声明一个新类型      
typedef void (*pFunParam)();    //整体声明一个新类型    
typedef void (*pFun)(pFunParam); //使用定义的新类型来声明对象，等价于void (*b[10]) (void (*)()); 
pFun b[10];
3. double(*[1]  (*pa)[9])()[2]  ;  //首先为上面表达式蓝色部分声明一个新类型   
typedef double(*pFun)(); //整体声明一个新类型   
typedef pFun (*pFunParam)[9];   //使用定义的新类型来声明对象，等价于double(*(*pa)[9])();  
pFunParam pa;  
pa是一个指针，指针指向一个数组，这个数组有9个元素，每一个元素都是“doube(*)()”--也即一个指针，指向一个函数，函数参数为空，返回值是“double”。    



 


 
 







 


