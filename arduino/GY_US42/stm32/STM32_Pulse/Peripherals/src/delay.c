#include "delay.h"
static u8  Times_us=0;//1us需要计的次数
static u16 Times_ms=0;//1ms需要计的次数
//SYSCLK=72M,SYSCLK/8=9M
void delay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	Times_us=SYSCLK/8;	//Times_us=9次	    
	Times_ms=(u16)Times_us*1000;//Times_ms=9000次
}
//SYSCLK=72M,SYSCLK/8=9M,即计一次数为9分之一 微秒，1us需要计数9次
//所以SysTick->LOAD=nus*SYSCLK/8=nus*9=Count
//Count<2^24，Max_Count=16777215
//最大nus=Max_Count/9=16777215/9=1864135us
//即最大nus为1864135
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=(nus*Times_us); //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 	
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//SYSCLK=72M,SYSCLK/8=9M,1微秒需计数9次，则1毫秒需计数9000次
//所以SysTick->LOAD=nms*9000=Count
//Count<2^24，Max_Count=16777215
//最大nms=Max_Count/9000=16777215/9000=1864.135ms，
//即最大nms=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(nms*Times_ms);//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;   
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));  
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       	    
}
void Time_Begin(void)
{
  SysTick->CTRL=0x00; //关闭计数器  
	SysTick->LOAD=(60*Times_ms);//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;     //清16位，使用外部时钟，无中断，开启定时器计数

}
void Get_Time(float *time)//返回秒
{
	  SysTick->CTRL=0x00; //关闭计数器    
    time[0]=((float)(60*Times_ms-SysTick->VAL))/(Times_ms*1000);//获取时间
	 // SysTick->LOAD=(Times_us*20000);//20ms定时
  //	SysTick->VAL =0x00;          //清计数器 
	 // SysTick->CTRL=0x01 ;
}
void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);	


/*--------------------------------------------------配置定时器5输入捕获--------------------------------------------*/	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	//初始化TIM5输入捕获参数
	 TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	 
	

   	TIM_Cmd(TIM2,DISABLE ); 	//使能定时器5

}