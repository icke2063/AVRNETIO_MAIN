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


	#define NAME_EEPROM_STORE	70	//EEPROM ADDRESS
	#define	MAX_NAME_LENGTH		16	//MAX STRING LENGTH
	#define	NAME_COUNT			24	//NUMBER OF USEABLE PINS
	#define NAME_EEPROM_END		NAME_EEPROM_STORE + (MAX_NAME_LENGTH*NAME_COUNT)+1


#if USE_RULES
	#define RULES_EEPROM_STORE	500
	#define	RULES_MAX_COUNT		100
#endif


#endif /* EEPROM_MAPPING_H_ */
