#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
//第一个电机	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
//第二个电机	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure_B;
  GPIO_InitStructure_B.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入，匹配编码器输出
  GPIO_InitStructure_B.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure_B.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure_B);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure_TIM4;
  TIM_ICInitTypeDef TIM_ICInitStructure_TIM4;

	// 时基初始化
	TIM_TimeBaseInitStructure_TIM4.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure_TIM4.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure_TIM4.TIM_Period = 65536 - 1;  // ARR寄存器最大值
	TIM_TimeBaseInitStructure_TIM4.TIM_Prescaler = 1 - 1;   // PSC分频系数
	TIM_TimeBaseInitStructure_TIM4.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure_TIM4);

	// 输入捕获初始化（先重置默认参数）
	TIM_ICStructInit(&TIM_ICInitStructure_TIM4);
	// 配置通道1
	TIM_ICInitStructure_TIM4.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure_TIM4.TIM_ICFilter = 0xF;  // 输入滤波，减少噪声干扰
	TIM_ICInit(TIM4, &TIM_ICInitStructure_TIM4);
	// 配置通道2
	TIM_ICInitStructure_TIM4.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure_TIM4.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure_TIM4);

	// 编码器接口配置
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	// 使能TIM4
	TIM_Cmd(TIM4, ENABLE);
}

int16_t Encoder_Get1(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}

int16_t Encoder_Get2(void)
{
    int16_t Temp;
    Temp = TIM_GetCounter(TIM4); 
    TIM_SetCounter(TIM4, 0);   
    return Temp;
}







