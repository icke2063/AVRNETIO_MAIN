/*------------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Commando Interpreter

 Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation ver�ffentlicht, 
 weitergeben und/oder modifizieren, entweder gem�� Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder sp�teren Version. 

 Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, 
 da� es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/

#ifndef _CMD_H_
	#define _CMD_H_	
		
	#include <avr/io.h>
	#include <string.h>
	#include <stdlib.h>
	#include <avr/eeprom.h>
	#include "usart.h"
	#include "stack.h"
	#include "httpd.h"
	#include "ntp.h"
	#include "sendmail.h"
    #include <util/delay.h>
    #include "S0/S0.h"
	
	#if USE_WOL
    #include "wol.h"
  #endif
  
  #if USE_RULE
	 #include "rules/rules.h"
	#endif
	
	typedef struct
	{
		char* cmd; 		// Stringzeiger auf Befehlsnamen
		void(*fp)(void);  	// Zeiger auf auszuf�hrende Funktion
	} COMMAND_STRUCTUR;	
	
	#define MAX_VAR	10
	#define HELPTEXT	1

	// -> main.c
	extern unsigned char hh;
	extern unsigned char mm;
	extern unsigned char ss;

	// -> main.c
	extern unsigned int var_array[MAX_VAR_ARRAY];
	
	extern unsigned char extract_cmd (char *);
	extern void write_eeprom_ip (unsigned int);
	
	//reset the unit
	extern void command_reset		(void);
	//Ausgabe der ARP Tabelle
//	extern void command_arp 		(void);
	//Ausgabe der TCP Tabelle
//	extern void command_tcp 		(void);
	
	//�ndern/ausgabe der IP, NETMASK, ROUTER_IP, NTP_IP
	extern void command_ip 			(void);
	extern void command_net 		(void);
	extern void command_router		(void);
	extern void command_ntp 		(void);
//	extern void command_mac			(void);
//	extern void command_ver			(void);
	extern void command_setvar		(void);
	extern void command_time		(void);
	extern void command_ntp_refresh		(void);

	extern void command_porta		(void);
	extern void command_getPORTA		(void);

	extern void command_portc		(void);
	extern void command_getPORTC		(void);
	extern void command_setPORTC		(void);
	extern void command_togPORTC		(void);

	#if USE_PORTD_SCHALT
	extern void command_portd		(void);
	extern void command_setPORTD		(void);
	#endif
	
	#if USE_PORTD_EING
	extern void command_portd		(void);
  extern void command_getPORTD		(void);
	#endif
	
	#if USE_RULE
	extern void add_rule (void);
	extern void del_rule (void);
	extern void show_rule (void);
	extern void list_rules (void);
	extern void check_rule (void);
	extern void run_rule (void);
  #endif
	
	
	#if USE_S0
	extern void command_S0		(void);
	#endif

	#if USE_WOL
	extern void command_wol 		(void);
	#endif //USE_WOL
	
	#if USE_MAIL
	extern void command_mail 		(void);
	#endif //USE_MAIL

	#if USE_OW
	extern void command_ow			(void);
	#endif //USE_OW

  void eeprom_read (void);
  void eeprom_write (void);

//	extern void command_ping 		(void);
	extern void command_help 		(void);
	
	#define RESET() {asm("ldi r30,0"); asm("ldi r31,0"); asm("ijmp");}
	
#endif //_CMD_H_
