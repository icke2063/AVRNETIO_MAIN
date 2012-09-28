/*------------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Commando Interpreter

 Erweitert von RoBue f�r 1-Wire, dig. u. analog. Ausgabe, Schalten der Ports 
 ab Version 1.4a

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

#include "cmd.h"

volatile unsigned int variable[MAX_VAR];
char cmd_buffer[40];

COMMAND_STRUCTUR COMMAND_TABELLE[] = // Befehls-Tabelle
{
	{"RESET",command_reset},
//	{"ARP",command_arp},
//	{"TCP",command_tcp},
	{"IP",command_ip},
	{"NET",command_net},
	{"ROUTER",command_router},
//	{"MAC",command_mac},
//	{"VER",command_ver},
	{"SV",command_setvar},
	{"TIME",command_time},
//	{"PING", command_ping},

	{"PORTA",command_porta},
	{"PORTAGET",command_getPORTA},

	{"PORTC",command_portc},
	{"PORTCGET",command_getPORTC},
	{"PORTCSET",command_setPORTC},
	{"PORTCTOG",command_togPORTC},
	

	#if HELPTEXT
	{"HELP",command_help},
	{"?",command_help},
	#endif //HELPTEXT
	{"eeprom_read",eeprom_read},
  {"eeprom_write",eeprom_write},
	
	
	{NULL,NULL} 
};

#if HELPTEXT
	PROGMEM char helptext[] = {
		"HELP     - print Helptext\r\n"
		"?        - print Helptext\r\n"
		"RESET    - reset the AVR - Controller\r\n"
//		"ARP      - list the ARP table\r\n"
		"TCP      - list the tcp table\r\n"
		"IP       - list/change ip\r\n"
		"NET      - list/change netmask\r\n"
		"ROUTER   - list/change router ip\r\n"
		"TIME     - get time\r\n"

		"PORTA    - list PORTA\r\n"
		"PORTAGET - get PORTA 0-7\r\n"

		"PORTC    - list PORTC\r\n"
		"PORTCGET - get PORTC 0-7\r\n"
		"PORTCSET - set PORTC 0-7 0/1\r\n"
		"PORTCTOG - toggle PORTC 0-7 xxxx(ms)\r\n"

//		"VER    - list version number\r\n"
		"SV     - set variable\r\n"
//		"PING   - send Ping\r\n"

	};
#endif

//------------------------------------------------------------------------------
//Commando auswerten
unsigned char extract_cmd (char *string_pointer)
{
	//Stringzeiger;
	char *string_pointer_tmp;
	char *string_pointer_rest;
	unsigned char cmd_index = 0;
 
    string_pointer_tmp = strsep(&string_pointer," "); 

	//Kommando in Tabelle suchen
	while(strcasecmp(COMMAND_TABELLE[cmd_index].cmd,string_pointer_tmp))
    {
        //Abruch Whileschleife und Unterprogramm verlassen 
        if (COMMAND_TABELLE[++cmd_index].cmd == 0) return(0);
    }
    
    //Variablen finden und auswerten
	for (unsigned char a = 0; a<MAX_VAR;a++)
	{ 
        string_pointer_tmp = strsep(&string_pointer,"., ");  
		variable[a] = strtol(string_pointer_tmp,string_pointer_rest,0);
		if(variable[a]==NULL)
		{
      //usart_write("Erkannt:%iRestl:%i\r\n",variable[a],strlen(string_pointer_rest));
      if(strlen(string_pointer_rest)>0)
      {
        //usart_write("Stabe:%c\r\n",string_pointer_rest[0]);
        //usart_write("%s",string_pointer_rest);
        //usart_write("\r\n");
        
        variable[a]=(unsigned int)string_pointer_tmp[0];
      }
    }
		
	}

    //Kommando ausf�hren
	COMMAND_TABELLE[cmd_index].fp();
	return(1); 
}

//------------------------------------------------------------------------------
//Reset ausf�hren
void command_reset (void)
{
	RESET();
}

//------------------------------------------------------------------------------
//print/edit own IP
void command_ip (void)
{
	write_eeprom_ip(IP_EEPROM_STORE);
	(*((unsigned long*)&myip[0])) = get_eeprom_value(IP_EEPROM_STORE,MYIP);
	usart_write("My IP: %1i.%1i.%1i.%1i\r\n",myip[0],myip[1],myip[2],myip[3]);
}

//------------------------------------------------------------------------------
//
void write_eeprom_ip (unsigned int eeprom_adresse)
{
	if (*((unsigned int*)&variable[0]) != 0x00000000)
	{	
		//value ins EEPROM schreiben
		for (unsigned char count = 0; count<4;count++)
		{
			eeprom_busy_wait ();
			eeprom_write_byte((unsigned char *)(eeprom_adresse + count),variable[count]);
		}
	}
}

//------------------------------------------------------------------------------
//print/edit Netmask
void command_net (void)
{
	write_eeprom_ip(NETMASK_EEPROM_STORE);
	(*((unsigned long*)&netmask[0])) = get_eeprom_value(NETMASK_EEPROM_STORE,NETMASK);
	usart_write("NETMASK: %1i.%1i.%1i.%1i\r\n",netmask[0],netmask[1],netmask[2],netmask[3]);
}

//------------------------------------------------------------------------------
//print/edit Router IP
void command_router (void)
{
	write_eeprom_ip(ROUTER_IP_EEPROM_STORE);
	(*((unsigned long*)&router_ip[0])) = get_eeprom_value(ROUTER_IP_EEPROM_STORE,ROUTER_IP);
	usart_write("Router IP: %1i.%1i.%1i.%1i\r\n",router_ip[0],router_ip[1],router_ip[2],router_ip[3]);
}


/*

//------------------------------------------------------------------------------
//print own mac
void command_mac (void)
{
	usart_write("My MAC: %2x:%2x:%2x:%2x:%2x:%2x\r\n",mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);
}

//------------------------------------------------------------------------------
//print enc28j60 chip version
void command_ver (void)
{
#if USE_ENC28J60
	usart_write("AVR WEB-Switch Version: %s\r\n", SoftVers);
	usart_write("ENC28J60 Version: %1x\r\n", enc28j60_revision);
#endif

#if USE_RTL8019
	usart_write("RTL8019 Ethernetcard\r\n");
#endif
}

//------------------------------------------------------------------------------
//print ARP table
void command_arp (void)
{
	for (unsigned char index = 0;index<MAX_ARP_ENTRY;index++)
	{
		usart_write("%2i  MAC:%2x",index,arp_entry[index].arp_t_mac[0]);
		usart_write(".%2x",arp_entry[index].arp_t_mac[1]);
		usart_write(".%2x",arp_entry[index].arp_t_mac[2]);
		usart_write(".%2x",arp_entry[index].arp_t_mac[3]);
		usart_write(".%2x",arp_entry[index].arp_t_mac[4]);
		usart_write(".%2x",arp_entry[index].arp_t_mac[5]);
		
		usart_write("  IP:%3i",(arp_entry[index].arp_t_ip&0x000000FF));
		usart_write(".%3i",((arp_entry[index].arp_t_ip&0x0000FF00)>>8));
		usart_write(".%3i",((arp_entry[index].arp_t_ip&0x00FF0000)>>16));
		usart_write(".%3i",((arp_entry[index].arp_t_ip&0xFF000000)>>24));
			
		usart_write("  Time:%4i\r\n",arp_entry[index].arp_t_time);
	}

}

//------------------------------------------------------------------------------
//print ARP table
void command_tcp (void)
{
	for (unsigned char index = 0;index<MAX_TCP_ENTRY;index++)
	{
		usart_write("%2i",index);
		usart_write("  IP:%3i",(tcp_entry[index].ip&0x000000FF));
		usart_write(".%3i",((tcp_entry[index].ip&0x0000FF00)>>8));
		usart_write(".%3i",((tcp_entry[index].ip&0x00FF0000)>>16));
		usart_write(".%3i",((tcp_entry[index].ip&0xFF000000)>>24));
		usart_write("  PORT:%4i",LBBL_ENDIAN_INT(tcp_entry[index].src_port));
		usart_write("  Time:%4i\r\n",tcp_entry[index].time);
	}

}
*/


//------------------------------------------------------------------------------
//�ndern einer Variable
void command_setvar (void)
{
	var_array[variable[0]] = variable[1];
	usart_write("Inhalt der Variable[%2i] = %2i\r\n",variable[0],var_array[variable[0]]);
}

//------------------------------------------------------------------------------
//print Time
void command_time (void)
{
//	unsigned char hh = (time/3600)%24;
//	unsigned char mm = (time/60)%60;
//	unsigned char ss = time %60;
	usart_write ("\n\rTIME: %2i:%2i:%2i\r\n",hh,mm,ss);
}

/*
//------------------------------------------------------------------------------
// Sende "Ping" an angegebene Adresse
void command_ping (void)
{
	if (*((unsigned int*)&variable[0]) != 0x00000000)
	{
		unsigned long dest_ip = ((variable[0])+((unsigned long)(variable[1])<<8)+((unsigned long)(variable[2])<<16)+((unsigned long)(variable[3])<<24));
		
		//ARP Request senden
		arp_request (dest_ip);
		
		//ICMP-Nachricht Type=8 Code=0: Echo-Anfrage
		//TODO: Sequenznummer, Identifier 
		icmp_send(dest_ip,0x08,0x00,1,1);
	}

}
*/


//------------------------------------------------------------------------------
//print helptext
void command_help (void)
{
#if HELPTEXT
	unsigned char data;
	PGM_P helptest_pointer = helptext;
	
	do
	{
		data = pgm_read_byte(helptest_pointer++);
		usart_write("%c",data);
	}while(data != 0);
#endif //HELPTEXT
}


//------------------------------------------------------------------------------
// print PORTA
void command_porta (void)
{
	// Digitalwerte (0/1) -> PORTA0-3
	usart_write ("A0: %i\r\nA1: %i\r\nA2: %i\r\nA3: %i\r\n",(PINA&0b00000001),(PINA&0b00000010)>>1,(PINA&0b00000100)>>2,(PINA&0b00001000)>>3);	

	// Analogwerte (0-1023) -> PORTA4-6
	uint16_t analog;
	uint8_t i;
	for(i=4;i<7;i++)
		{
		analog = (var_array[i]);
		usart_write ("A%i: %i\r\n",i,analog);
		}
}

void command_getPORTA (void)
{
	// Digitalwerte (0/1) -> PORTA0-3
	if(variable[0]>=0 && variable[0]<=3)
		{
		usart_write ("%i\r\n",PINA&(1<<(int)variable[0]));
		}

	if(variable[0]>=4 && variable[0]<=7)
		{
		uint16_t analog;
		analog = (var_array[variable[0]]);
		usart_write ("%i",analog);
		}

}

// print Ausgabewert von PORTC0-7
void command_portc (void)

{
	usart_write ("C0: %i\r\nC1: %i\r\n",(PINC&0b00000001)>>0,(PINC&0b00000010)>>1);	
	usart_write ("C2: %i\r\nC3: %i\r\nC4: %i\r\n",(PINC&0b00000100)>>2,(PINC&0b00001000)>>3,(PINC&0b00010000)>>4);	
	usart_write ("C5: %i\r\nC6: %i\r\nC7: %i\r\n",(PINC&0b00100000)>>5,(PINC&0b01000000)>>6,(PINC&0b10000000)>>7);	
}

//------------------------------------------------------------------------------

void command_getPORTC (void)
{
	if(variable[0]>=0 && variable[0]<=7)
		{
		//usart_write (PINC&(1<<variable[0]));
		usart_write ("%i\r\n",PINC&(1<<(int)variable[0]));
		}
}

//PORTC setzen
void command_setPORTC (void)
{
	usart_write("setportC %1i.%1i\r\n",variable[0],variable[1]);    
	if (variable[1] == 1)
		PORTC = PORTC |= (1<<variable[0]) ; 
		//  setzt Bit  in PORT und setzt damit Pin auf high 
	else if (variable[1] == 0)
		PORTC = PORTC &= ~(1<<variable[0]);
		//l�scht Bit  in PORT und setzt damit Pin auf low    
	else
		usart_write("ERROR PARAMETER  [Pin.Status]\r\n");
}

void command_togPORTC (void)
{
	usart_write("togglePortD %1i->%1i ms\r\n",variable[0],variable[1]);    
	
	if(variable[0]>=0 && variable[0]<=7)
	{
		//1.Wechsel
		if(PINC&(1<<variable[0]))	//PIN == 1
		{
			PORTC = PORTC &= ~(1<<variable[0]);
		}
		else				//PIN == 0
		{
			PORTC = PORTC |= (1<<variable[0]) ;
		}

		//Warten
		uint8_t i;
		uint16_t delay;
		delay = variable[1];
		
		if(delay>=0 && delay<=9999)
		{
			for(i=0;i<(uint16_t)(delay/10);i++)
			{
				_delay_ms(10);
			}
		}
		else
		{
			_delay_ms(100);
		}		

		//2.Wechsel
		if(PINC&(1<<variable[0]))	//PIN == 1
		{
			PORTC = PORTC &= ~(1<<variable[0]);
		}
		else				//PIN == 0
		{
			PORTC = PORTC |= (1<<variable[0]) ;
		}
	}
}



void eeprom_read (void)
{
  unsigned char value[1];

		eeprom_busy_wait ();	
		value[0] = eeprom_read_byte((unsigned char *)(variable[0]));
    usart_write("EEPROM Write:(%i):%c\r\n",variable[0],value[0]);
}

void eeprom_write (void)
{
	
  //schreiben ins eeprom
      uint16_t addi;
      addi=variable[0];
      
      usart_write("EEPROM Write:(%i):%i\r\n",addi,variable[1]);
      
			eeprom_busy_wait ();
			eeprom_write_byte((unsigned char *)(addi),variable[1]);
		
}
