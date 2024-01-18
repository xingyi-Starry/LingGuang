/*传感器输入模块, 光敏传感器从PA5口输入, 红外传感器从PA4口输入*/

#include "stm32f10x.h"

/*Sensor_Value[0]为红外传感器数值, Sensor_Value[1]为光敏传感器数值*/
uint16_t Sensor_Value[2];

void Sensor_Init(void)
{
    //开启外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //配置ADC预分频器
    RCC_ADCCLKConfig(6);

    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //配置多路开关
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);

    //配置ADC转换器
    ADC_InitTypeDef ADC_Initstructure;
    ADC_StructInit(&ADC_Initstructure);
    ADC_Initstructure.ADC_ContinuousConvMode = ENABLE;
    ADC_Initstructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Initstructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_Initstructure.ADC_Mode = ADC_Mode_Independent;
    ADC_Initstructure.ADC_NbrOfChannel = 2;
    ADC_Initstructure.ADC_ScanConvMode = ENABLE;
    ADC_Init(ADC1, &ADC_Initstructure);

    //DMA初始化
    DMA_InitTypeDef DMAInitStructure;
    DMA_StructInit(&DMAInitStructure);
    /*外设*/
    DMAInitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; 
    DMAInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMAInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //是否自增
    /*内存*/
    DMAInitStructure.DMA_MemoryBaseAddr = (uint32_t)Sensor_Value; 
    DMAInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMAInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //是否自增
    /*其它参数*/
    DMAInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设作为SRC(source), 由外设传输到存储器
    DMAInitStructure.DMA_BufferSize = 2; //传输计数器(转运次数)
    DMAInitStructure.DMA_Mode = DMA_Mode_Circular; //是否启用自动重装
    DMAInitStructure.DMA_M2M = DMA_M2M_Disable; //是否使用软件触发
    DMAInitStructure.DMA_Priority = DMA_Priority_Medium; //转运优先级，多通道DMA时有用
    DMA_Init(DMA1_Channel1, &DMAInitStructure);

    //开启ADC向DMA输出(作为DMA触发源)
    ADC_DMACmd(ADC1, ENABLE);

    //使能DMA, ADC
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    //校准ADC
    ADC_ResetCalibration(ADC1); //复位校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET); //等待复位完成

    ADC_StartCalibration(ADC1); //启动校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET); //等待校准完成

    //启动转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
}
