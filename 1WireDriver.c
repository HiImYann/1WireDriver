/****************************************************************************
* PROGRAMME 1WireDriver.c													*
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

/****************************************************************************
* DESCRIPTION :																*
* Driver pour le protocole 1-Wire de Maxim Integrated. Concu pour les 		*
* capteurs temperature comme le DS18B20 ou le DS1822.						*
* ATTENTION : Les fonctions SearchROM et AlarmSearch ne sont PAS completes.	*
* 			  Ce driver n'inclut pas le cas du parasite power				*
*****************************************************************************/


////////////////////////
///     INCLUDES     ///
////////////////////////

#include "1WireDriver.h"


///////////////////////
///    VARIABLES    ///
///////////////////////

uint8_t i = 0, j = 0;
extern TIM_HandleTypeDef htim17;


///////////////////////
///    FUNCTIONS    ///
///////////////////////

/****************************************************************************
* Fonction usDelay															*
*																			*
* Parametres : 																*
*		uint16_t Microseconds : temps d'attente en microsecondes			*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Fonction d'attente active en microsecondes pour les timings du		*
* 		protocole 1-Wire													*
****************************************************************************/
void usDelay(uint16_t Microseconds){
	__HAL_TIM_SET_COUNTER(&htim17, 0);
	while(__HAL_TIM_GET_COUNTER(&htim17) < Microseconds);
}


/****************************************************************************
* Fonction Initialization													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Fonction d'initialisation des capteurs 1-Wire						*
* 																			*
* Retourne : 																*
* 		uint8_t Presence : 1 si capteur un ou plusieurs capteur sont 		*
* 						   branches sur la ligne 1-Wire						*
* 						   0 si aucun capteur branche						*
* 																			*
* Note : Les timings recommandes par Maxim Integrated ont ete utilises		*
****************************************************************************/
uint8_t Initialization(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	uint8_t Presence = 0;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
	usDelay(480);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
	usDelay(70);
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0) Presence = 1;
	else Presence = 0;
	usDelay(410);
	return Presence;	//Return 1 if devices are on the line
}


/****************************************************************************
* Fonction WriteHigh														*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Envoie un 1 logique au capteur										*
* 																			*
* Note : Les timings recommandes par Maxim Integrated ont ete utilises		*
****************************************************************************/
void WriteHigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
	usDelay(6);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
	usDelay(64);
}


/****************************************************************************
* Fonction WriteLow															*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Envoie un 0 logique au capteur										*
* 																			*
* Note : Les timings recommandes par Maxim Integrated ont ete utilises		*
****************************************************************************/
void WriteLow(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
	usDelay(60);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
	usDelay(10);
}

/****************************************************************************
* Fonction Read																*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Cree un time slot pour la lecture de donnee envoye par le capteur	*
* 																			*
* Retourne : 																*
* 		uint8_t bitValue : Valeur du bit envoyee par le capteur				*
* 																			*
* Note : Les timings recommandes par Maxim Integrated ont ete utilises		*
****************************************************************************/
uint8_t Read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	uint8_t bitValue = 0;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 0);
	usDelay(6);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, 1);
	usDelay(9);
	bitValue = 0x01 & (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin)); //00000001 mask
	usDelay(55);
	return bitValue;	//Returns read bit
}


/****************************************************************************
* Fonction CommandGenerator													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*		uint8_t command 	: Commande a envoyer au capteur en hex			*
*																			*
* Inclus :	"1WireDriver.h"													*
*																			*
* Description :																*
* 		Envoie la commande souhaitee au capteur								*
****************************************************************************/
void CommandGenerator(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t command){
	uint8_t LSB;
	for(i = 0; i < 8; i++){
		LSB = (command >> i) & 0x01;
		if(LSB == 1) WriteHigh(GPIOx, GPIO_Pin);
		else WriteLow(GPIOx, GPIO_Pin);
	}
}


/****************************************************************************
* Fonction ROMCodeGenerator													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx 	: Port du STM32								*
*		uint16_t GPIO_Pin		: Pin sur le port du STM32					*
*		uint64_t DeviceROMcode 	: Code ROM unique du capteur				*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Utilisee dans la fonction MatchROM pour generer un ROM Code unique  *
* 		pour selectionner un capteur specifique								*
****************************************************************************/
void ROMCodeGenerator(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint64_t DeviceROMcode){
	uint64_t LSB;
	for(i = 0; i < 64; i++){
		LSB = (DeviceROMcode >> i) & 0x01;
		if(LSB == 1) WriteHigh(GPIOx, GPIO_Pin);
		else WriteLow(GPIOx, GPIO_Pin);
	}
}

/****************************************************************************
* Fonction SearchROM														*
*																			*
*						FONCTION NON OPERATIONNELLE							*
****************************************************************************/
void SearchROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	//0xF0 command
	CommandGenerator(GPIOx, GPIO_Pin, 0xF0);
}


/****************************************************************************
* Fonction ReadROM															*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Lis le code ROM unique du capteur									*
* 																			*
* Retourne : 																*
* 		uint64_t ROMcode : Code ROM unique du capteur						*
****************************************************************************/
uint64_t ReadROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	uint64_t ROMcode = 0;
	//0x33 command
	CommandGenerator(GPIOx, GPIO_Pin, 0x33);
	for(i = 0; i < 64; i++){
		ROMcode = ROMcode | (Read(GPIOx, GPIO_Pin) << i);
	}
	return ROMcode;
}


/****************************************************************************
* Fonction MatchROM															*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx 	: Port du STM32								*
*		uint16_t GPIO_Pin		: Pin sur le port du STM32					*
*		uint64_t DeviceROMcode 	: Code ROM unique du capteur				*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Selectionne un capteuer specifique selon le ROM code en parametre	*
****************************************************************************/
void MatchROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint64_t DeviceROMcode){
	//0x55 command
	CommandGenerator(GPIOx, GPIO_Pin, 0x55);
	ROMCodeGenerator(GPIOx, GPIO_Pin, DeviceROMcode);
}


/****************************************************************************
* Fonction SkipROM															*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx 	: Port du STM32								*
*		uint16_t GPIO_Pin		: Pin sur le port du STM32					*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Selectionne tous les capteurs sur la ligne							*
****************************************************************************/
void SkipROM(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	//0xCC command
	CommandGenerator(GPIOx, GPIO_Pin, 0xCC);
}


/****************************************************************************
* Fonction AlarmSearch														*
*																			*
*						FONCTION NON OPERATIONNELLE							*
****************************************************************************/
void AlarmSearch(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	//0xEC command
	CommandGenerator(GPIOx, GPIO_Pin, 0xEC);
}


/****************************************************************************
* Fonction ConvertT															*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx 	: Port du STM32								*
*		uint16_t GPIO_Pin		: Pin sur le port du STM32					*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Demande aux capteurs d'ecrire la temperature mesuree dans leur 		*
* 		memoire (byte 0 et byte 1)								 								*
****************************************************************************/
void ConvertT(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	//0x44 command
	CommandGenerator(GPIOx, GPIO_Pin, 0x44);
	Read(GPIOx, GPIO_Pin);
	while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0); //Waits for conversion
}


/****************************************************************************
* Fonction WriteScratchpad													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*		uint8_t THreg 		: Valeur a ecrire dans le registre TH			*
*		uint8_t TLreg		: Valeur a ecrire dans le registre TL			*
*		uint8_t Configreg	: Valeur a ecrire dans le registre Config		*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Envoie 3 bytes de donnes dans les registres TH, TL et config		*
****************************************************************************/
void WriteScratchpad(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t THreg, uint8_t TLreg, uint8_t Configreg){
	//0x4E command
	CommandGenerator(GPIOx, GPIO_Pin, 0x4E);
	CommandGenerator(GPIOx, GPIO_Pin, THreg);
	CommandGenerator(GPIOx, GPIO_Pin, TLreg);
	CommandGenerator(GPIOx, GPIO_Pin, Configreg);
}


/****************************************************************************
* Fonction ReadScratchpad													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx   : Port du STM32								*
*		uint16_t GPIO_Pin  	  : Pin sur le port du STM32					*
*		uint8_t nBytes		  :	Nombre de bytes a lire						*
*		uint8_t MemoryArray[] :	Tableau ou stocker les valeurs 				*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Ecris dans le tableau entre en parametre les bytes de la memoire	*
* 		du capteur du byte 0 a nBytes										*
****************************************************************************/
void ReadScratchpad(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t nBytes, uint8_t MemoryArray[]){
	uint8_t byte = 0;
	//0xBE command
	CommandGenerator(GPIOx, GPIO_Pin, 0xBE);
	for(i = 0; i < nBytes; i++){ 	//Reads the number of bytes requested
		for(j = 0; j < 8; j++){		//Reads the 8 bits of the byte
			byte = byte | (Read(GPIOx, GPIO_Pin) << j);
		}
		MemoryArray[i] = byte; //Writes byte
		byte = 0;
	}
	Initialization(GPIOx, GPIO_Pin); //Stop the reading
}


/****************************************************************************
* Fonction CopyScratchpad													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx 	: Port du STM32								*
*		uint16_t GPIO_Pin		: Pin sur le port du STM32					*
*		uint64_t DeviceROMcode 	: Code ROM unique du capteur				*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Copie les valeurs du scratchpad vers l'EEPROM du capteur			*
****************************************************************************/
void CopyScratchpad(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	//0x48 command
	CommandGenerator(GPIOx, GPIO_Pin, 0x48);
}


/****************************************************************************
* Fonction ROMCodeGenerator													*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx 	: Port du STM32								*
*		uint16_t GPIO_Pin		: Pin sur le port du STM32					*
*		uint64_t DeviceROMcode 	: Code ROM unique du capteur				*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Copie les valeurs de l'EEPROM vers le scratchpad du capteur			*
****************************************************************************/
void RecallE2(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	//0xB8 command
	CommandGenerator(GPIOx, GPIO_Pin, 0xB8);
	Read(GPIOx, GPIO_Pin);
	while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == 0); //Waits for end of transmission
}


/****************************************************************************
* Fonction ReadPSU															*
*																			*
* Parametres : 																*
*		GPIO_TypeDef *GPIOx : Port du STM32									*
*		uint16_t GPIO_Pin	: Pin sur le port du STM32						*
*																			*
* Inclus : "1WireDriver.h"													*
*																			*
* Description :																*
* 		Demande au capteur si ils sont alimentes en mode parasite			*
* 																			*
* Retourne : 																*
* 		uint8_t ParasitePowerPresence : 1 si capteur(s) en parasite mode	*
* 		 								0 si aucun capteur en parasite mode *
****************************************************************************/
uint8_t ReadPSU(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	uint8_t ParasitePowerPresence = 0;
	//0xB4 command
	CommandGenerator(GPIOx, GPIO_Pin, 0xB4);
	if(Read(GPIOx, GPIO_Pin) == 0) ParasitePowerPresence = 1;
	else ParasitePowerPresence = 0;
	return ParasitePowerPresence;	//Returns 1 if devices on the line use parasite power
}
