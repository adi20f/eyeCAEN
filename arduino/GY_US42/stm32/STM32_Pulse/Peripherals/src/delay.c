#include "delay.h"
static u8  Times_us=0;//1us��Ҫ�ƵĴ���
static u16 Times_ms=0;//1ms��Ҫ�ƵĴ���
//SYSCLK=72M,SYSCLK/8=9M
void delay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	Times_us=SYSCLK/8;	//Times_us=9��	    
	Times_ms=(u16)Times_us*1000;//Times_ms=9000��
}
//SYSCLK=72M,SYSCLK/8=9M,����һ����Ϊ9��֮һ ΢�룬1us��Ҫ����9��
//����SysTick->LOAD=nus*SYSCLK/8=nus*9=Count
//Count<2^24��Max_Count=16777215
//���nus=Max_Count/9=16777215/9=1864135us
//�����nusΪ1864135
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=(nus*Times_us); //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 	
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//SYSCLK=72M,SYSCLK/8=9M,1΢�������9�Σ���1���������9000��
//����SysTick->LOAD=nms*9000=Count
//Count<2^24��Max_Count=16777215
//���nms=Max_Count/9000=16777215/9000=1864.135ms��
//�����nms=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(nms*Times_ms);//ʱ�����(SysTick->LOADΪ24bit)
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
  SysTick->CTRL=0x00; //�رռ�����  
	SysTick->LOAD=(60*Times_ms);//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;     //��16λ��ʹ���ⲿʱ�ӣ����жϣ�������ʱ������

}
void Get_Time(float *time)//������
{
	  SysTick->CTRL=0x00; //�رռ�����    
    time[0]=((float)(60*Times_ms-SysTick->VAL))/(Times_ms*1000);//��ȡʱ��
	 // SysTick->LOAD=(Times_us*20000);//20ms��ʱ
  //	SysTick->VAL =0x00;          //������� 
	 // SysTick->CTRL=0x01 ;
}
void TIM2_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);	


/*--------------------------------------------------���ö�ʱ��5���벶��--------------------------------------------*/	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//��ʼ��TIM5���벶�����
	 TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
		TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	 
	

   	TIM_Cmd(TIM2,DISABLE ); 	//ʹ�ܶ�ʱ��5

}