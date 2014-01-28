/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f1_system.h"
#include "stm32f1_usart.h"
#include "module_rs232.h"
/*=====================================================================================================*/
/*=====================================================================================================*/

/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_Config
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
//void RS232_Config( void )
//{
//  GPIO_InitTypeDef GPIO_Struct;
//  USART_InitTypeDef USART_Struct;

//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);

//  /* USART1 Tx PA9 */
//  GPIO_Struct.GPIO_Pin = GPIO_Pin_9;
//  GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_Struct);
//  /* USART1 Rx PA10 */
//  GPIO_Struct.GPIO_Pin = GPIO_Pin_10;
//  GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOA, &GPIO_Struct);

//  USART_Struct.USART_BaudRate = 9600;
//  USART_Struct.USART_WordLength = USART_WordLength_8b;
//  USART_Struct.USART_StopBits = USART_StopBits_1;
//  USART_Struct.USART_Parity = USART_Parity_No ;
//  USART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  USART_Init(USART1, &USART_Struct);
//  USART_Cmd(USART1, ENABLE);

//  USART_ClearFlag(USART1, USART_FLAG_TC);
//}
void RS232_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  USART_InitTypeDef USART_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  /* USART1 Tx PA9 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* USART1 Rx PA10 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No ;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStruct);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);

  USART_ClearFlag(USART1, USART_FLAG_TC);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_SendStr
**功能 : 發送字串
**輸入 : USARTx, *pWord
**輸出 : None
**使用 : RS232_SendStr(USART1, (u8*)"Hellow World!");
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendStr( USART_TypeDef* USARTx, uc8 *pWord )
{
  while(*pWord != '\0') {
    USART_SendByte(USARTx, (u8*)pWord);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_SendData
**功能 : 發送資料
**輸入 : USARTx, *SendData, DataLen
**輸出 : None
**使用 : RS232_SendData(USART1, SendData, DataLen);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendData( USART_TypeDef* USARTx, u8 *SendData, u16 DataLen )
{
  do {
    USART_SendByte(USARTx, SendData);
    SendData++;
    DataLen--;
  } while(DataLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_RecvStr
**功能 : 接收字串
**輸入 : USARTx, *pWord
**輸出 : None
**使用 : RS232_RecvStr(USART1, Stirng);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_RecvStr( USART_TypeDef* USARTx, u8 *pWord )
{
  do {
    *pWord = USART_RecvByte(USARTx);
    pWord++;
  } while(*(pWord-1) != '\0');
  *pWord = '\0';
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_RecvData
**功能 : 接收資料
**輸入 : USARTx, *RecvData, DataLen
**輸出 : None
**使用 : RS232_RecvData(USART1, RecvData, DataLen);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_RecvData( USART_TypeDef* USARTx, u8 *RecvData, u16 DataLen )
{
  do {
    *RecvData = USART_RecvByte(USARTx);
    RecvData++;
    DataLen--;
  } while(DataLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_VisualScope_CRC16
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )
{
  u16 USART_IX, USART_IY, USART_CRC;

  USART_CRC = 0xffff;
  for(USART_IX=0; USART_IX<Len; USART_IX++) {
    USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
    for(USART_IY=0; USART_IY<=7; USART_IY++) {
      if((USART_CRC&1)!=0)
        USART_CRC = (USART_CRC>>1)^0xA001;
      else
        USART_CRC = USART_CRC>>1;
    }
  }
  return(USART_CRC);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : USART_VisualScope
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
  u8 i = 0;
  u16 Temp = 0;

  Temp = RS232_VisualScope_CRC16(pWord, Len);
  pWord[8] = Temp&0x00ff;
  pWord[9] = (Temp&0xff00)>>8;

  for(i=0; i<10; i++) {
    USART_SendData(USARTx, *pWord);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
