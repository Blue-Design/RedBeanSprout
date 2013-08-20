/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f1_system.h"
#include "stm32f1_adc.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu16 ADC_DMA_Buf[ADC_Sample][ADC_Channel] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : ADC_Average
**�\�� : �N ADC �ഫ�᪺��ƨ�����
**��J : ADC_AveTr
**��X : None
**�ϥ� : ADC_Average(ADC_AveTr);
**=====================================================================================================*/
/*=====================================================================================================*/
void ADC_Config( void )
{
	DMA_InitTypeDef DMA_InitStruct;
	ADC_InitTypeDef ADC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);

	/* ADC_1 PA4 */	/* ADC_2 PA5 */	/* ADC_3 PA6 */	/* ADC_4 PA7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC DMA Init *************************************************************/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStruct.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;                    // Peripheral address
  DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&ADC_DMA_Buf;                      // DMA���s�a�}
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;                             // ���s�@���ƾڶǿ骺�ت��a
	DMA_InitStruct.DMA_BufferSize = ADC_Sample*ADC_Channel;                     // DMA�q�D��DMA�w�s���j�p
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;               // �~�]�a�}�H�s������
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                        // ���s�a�}�H�s�����W
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    // �ƾڼe�׬�16��
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;            // �ƾڼe�׬�16��
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                                // �`���Ҧ��}�ҡABuffer�g����A�۰ʦ^���l�a�}�}�l�ǿ�
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;                            // DMA�q�Dx�֦����u����
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                                   // DMA�q�Dx�S���]�m�����s�줺�s�ǿ�
  DMA_Init(DMA1_Channel1, &DMA_InitStruct);
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* ADC Init *****************************************************************/
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                             // ADC1 �M ADC2 �u�@�b�W�߼Ҧ� 
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;                                   // ���y�Ҧ� 
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                             // �s���ഫ�Ҧ� 
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;            // �~��Ĳ�o����
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;                         // ADC�ƾڥk���
  ADC_InitStruct.ADC_NbrOfChannel = ADC_Channel;                              // �ഫADC�q�D�ƥ�
  ADC_Init(ADC1, &ADC_InitStruct);

  /* ADC Regular Config *******************************************************/
	// �]�m���wADC���W�h�ճq�D�A�]�m���̪���ƶ��ǩM���ˮɶ�
	// ADC1, ADC�q�Dx, �W�h���˶��ǭȬ�y, ���ˮɶ���239.5�g��
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_55Cycles5);

  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**��� : ADC_Average
**�\�� : �N ADC �ഫ�᪺��ƨ�����
**��J : ADC_AveTr
**��X : None
**�ϥ� : ADC_Average(ADC_AveTr);
**=====================================================================================================*/
/*=====================================================================================================*/
void ADC_Average( u16* pADC_AveTr )
{
  u8 i, j;
  u32 ADC_DMA_Tmp[ADC_Channel] = {0};

  for(i=0; i<ADC_Channel; i++) {
    for(j=0; j<ADC_Sample; j++)
      ADC_DMA_Tmp[i] += ADC_DMA_Buf[j][i];
    pADC_AveTr[i] = (u16)(ADC_DMA_Tmp[i] / ADC_Sample);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
