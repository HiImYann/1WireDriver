/****************************************************************************
* Prototype 1WireDriver.h													*
*                                                                         	*
* Lieu      : ETML - Labo uC                                              	*
* Auteur    : Yann Dos Santos						                        *
* Date      : 25.11.2022					                               	*
*                                                                         	*
* Modifications                                                           	*
*   Auteur  :           					                              	*
*   Date    : 																*
*   Raisons : 																*
*****************************************************************************/

#ifndef SRC_1WIREDRIVER_H_
#define SRC_1WIREDRIVER_H_


/* ---------- Includes ----------*/
#include "stm32f0xx_hal.h"


/* ---------- Prototypes ----------*/
void usDelay(uint16_t Microseconds);
uint8_t Initialization(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void WriteHigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void WriteLow(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t Read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void CommandGenerator(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t command);
void SearchROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint64_t ReadROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void MatchROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint64_t DeviceROMcode);
void SkipROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void AlarmSearch(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void ConvertT(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void WriteScratchpad(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t THreg, uint8_t TLreg, uint8_t Configreg);
void ReadScratchpad(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t nBytes, uint8_t MemoryArray[]);
void CopyScratchpad(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void RecallE2(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t ReadPSU(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* SRC_1WIREDRIVER_H_ */
