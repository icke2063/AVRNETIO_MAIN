/*
 * eeprom_mapping.h
 *
 *  Created on: 08.09.2012
 *      Author: icke
 */

#ifndef EEPROM_MAPPING_H_
#define EEPROM_MAPPING_H_

	#define EEPROM_SIZE	1024

	#define IP_EEPROM_STORE 30			//4 Byte
	#define NETMASK_EEPROM_STORE 34		//4 Byte
	#define ROUTER_IP_EEPROM_STORE 38	//4 Byte

	#define NTP_IP_EEPROM_STORE 	50	//4 Byte

//#if USE_WOL
//	#define WOL_MAC_EEPROM_STORE	60 //6 Byte
//	#define WOL_BCAST_EEPROM_STORE	66 //4 Byte
//#endif


	#define NAME_EEPROM_STORE	70	//EEPROM ADDRESS
	#define	MAX_NAME_LENGTH		16	//MAX STRING LENGTH
	#define	NAME_COUNT			24	//NUMBER OF USEABLE PINS
	#define NAME_EEPROM_END		NAME_EEPROM_STORE + (MAX_NAME_LENGTH*NAME_COUNT)+1

//#if USE_S0
//	#define S0_EEPROM_STORE	(NAME_EEPROM_END +1)
//#endif

//#if USE_RULE
//	#define RULES_EEPROM_STORE	(S0_EEPROM_STORE +1)
//	#define	RULES_MAX_COUNT		100
//#endif

#endif /* EEPROM_MAPPING_H_ */
