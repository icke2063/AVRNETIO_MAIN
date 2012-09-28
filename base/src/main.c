/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Webserver uvm.

 Modifiziert f�r AVR-NET-IO:	RoBue
 Version:			1.5 TEST-3
 Datum:				14.02.2009

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
----------------------------------------------------------------------------*/

#include <avr/io.h>
#include <config.h>

#if USE_ADC
#include "analog.h"
#endif


#if USE_RULE
    #include "rules/rules_config.h"
    #include "rules/rules.h"
#endif

#include "usart.h"
#include "enc28j60.h"
//#include "rtl8019.h"
#include "stack.h"
#include "timer.h"

#include "httpd.h"
#include "cmd.h"
#include "telnetd.h"

#include "base64.h"
#include "http_get.h"




#if USE_I2C
#include "TWI/TWI_Master.h"
#endif


#if USE_NTP
  #include "ntp.h"
#endif


#if USE_MAIL
  #include "sendmail.h"
#endif //USE_MAIL


// RoBue:
// Variablen-Array 
// zum Abspeichern verschiedener Werte
// und zum Einf�gen in die Webseite %VA@00 bis %VA@19
// VA0-7	-> Analogwert von PORTA0-7	(aktive Kanäle und 1-wire beachten)
// VA8		-> Position Servo fuer Webcam
// VA9		-> Manueller Betrieb ein/aus
// VA10-17	-> Schaltwerte Temperaturen
// VA18,19	-> Schaltwerte analog
// VA20-23	-> Schaltzeiten ein/aus hh,mm,hh,mm
// VA24-28	-> Reserve
// VA-Ende	-> Counter
unsigned int var_array[MAX_VAR_ARRAY] = {10,50,30,0,0,0,0,0,25,0,15,15,15,15,15,15,15,15,750,750,12,0,12,0,0,0,0,0,0,0};
// RoBue:
// Die Uhrzeit wird zentral in main.c berechnet (s.u.)
// Die anderen Programmteile lesen nur noch hh,mm,ss aus.
// -> httpd.h/c, cmd.h/c'


#if USE_ADC
unsigned int var_adc[ADC_COUNT];
#endif


unsigned char hh;
unsigned char mm;
unsigned char ss;

// RoBue:	
// Das periodische Auslesen der 1-Wire-Sensoren erfolgt in main.c (s.u.)
// Die anderen Programmteile lesen nur noch "ow_array" aus.
// -> httpd.h/c, cmd.h/c 

#if USE_OW
	#include "1-wire/ds18x20.h"
	#include "1-wire/onewire.h"
	// Variable
	// Anmerkung RoBue:
	// Anzahl der Sensoren (MAXSENSORS) und ID -> config.h
	// Speicherplatz f�r 1-wire Sensorwerte (mit Minuswerten)
	// und zus�tzlich f�r die Tages-Min/Max-Werte
	// (min -> ow_array[MAXSENSORS] bis ow_array[MAXSENSORS*2-1])
	// (max -> ow_array[MAXSENSORS*2] bis ow_array[MAXSENSORS*3-1])
	int16_t ow_array[MAXSENSORS*3];
	// Speicherplatz f�r die ID der Sensoren
	// Position 0-4 eig. ID, Pos. 5+6 ist die Temperatur, Pos. 7 CRC-Byte
	PROGMEM	uint8_t		DS18B20IDs[MAXSENSORS+1][OW_ROMCODE_SIZE] = {
							OW_ID_T01,	// 1. DS18B20
							OW_ID_T02,
							OW_ID_T03,
							OW_ID_T04,
							OW_ID_T05,
							OW_ID_T06,
							OW_ID_T07,
							OW_ID_T08,
							OW_ID_Last };	// Endmarker

	uint8_t auslesen	= 0;	// 0 -> Sensoren nicht auslesen
	uint8_t messen		= 1;	// 1 -> Sensoren zum Messen auffordern
	uint8_t minmax		= 1;	// 1 -> Zur�cksetzen der Min/Max-Werte 1x/Tag
	
#endif



#if USE_S0
	#include "S0/S0.h"
#endif


//----------------------------------------------------------------------------

// Hier startet das Hauptprogramm
// ******************************

int main(void)
{  

    int pin;

	//Konfiguration der Ausg�nge bzw. Eing�nge
	//Definition erfolgt in der config.h
	DDRA = OUTA;
//	PORTA = ~OUTA;

	DDRC = OUTC;
//	PORTC = ~OUTC;

	DDRD = OUTD;
//	PORTD = ~OUTD;

	// icke2063:
	// Pullups einschalten
	for(pin=0;pin<8;pin++){
		//DDR - PORTA
//		if(DIG_IN_PA & (1<<pin)){
//			PORTA |= (1 << pin);
//		}

		//DDR - PORTC
//		if(DIG_IN_PC & (1<<pin)){
//			PORTC |= (1 << pin);
//		}
//
//		//DDR - PORTD
//		if(DIG_IN_PD & (1<<pin)){
//			PORTD |= (1 << pin);
//		}
	}


#if USE_I2C
	/*
	** Initiate TWI Master Interface with bitrate of 100000 Hz
	*/
/*		if (!TWIM_Init (100000))
			{
			printf ("Error in initiating TWI interface\n");
			while (1);
			}*/
#endif



  #if USE_S0
    // Pullups einschalten
    PORTD |= (1 << PORTD2) | (1 << PORTD3);
	#endif


    unsigned long a;

    usart_init(BAUDRATE); // setup the UART

	#if USE_ADC
		ADC_Init();
	#endif
	
	usart_write("\n\rSystem Ready\n\r");
    usart_write("Compiliert am "__DATE__" um "__TIME__"\r\n");
    usart_write("Compiliert mit GCC Version "__VERSION__"\r\n");

	for(a=0;a<1000000;a++){asm("nop");};

	//Applikationen starten
	stack_init();
	httpd_init();
	telnetd_init();
	
	#if USE_S0
	   S0_Init();
  #endif
	
	#if USE_RULE
	   init_rule();
  #endif
	
	//Ethernetcard Interrupt enable
	ETH_INT_ENABLE;
	
	//Globale Interrupts einschalten
	sei(); 
	
	
	#if USE_NTP
	        ntp_init();
	        ntp_request();
	#endif //USE_NTP
	  
	#if USE_MAIL
	        mail_client_init();
	#endif //USE_MAIL  


	// Startwerte f�r ow_array setzen
#if USE_OW
		uint8_t i = 0;
		for (i=0;i<MAXSENSORS;i++){
			ow_array[i]=OW_START;
		}
		for (i=MAXSENSORS;i<MAXSENSORS*3;i++){
			ow_array[i]=OW_MINMAX;
		}
		DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );
		for(a=0;a<1000000;a++){asm("nop");};
		auslesen = 0;
		minmax = 1;
#endif



//Hauptschlfeife
// *************
		
	while(1)
	{

	#if USE_ADC
		ANALOG_ON;
	#endif

	eth_get_data();
		
        //Terminalcommandos auswerten
	if (usart_status.usart_ready){
	usart_write("\r\n");
		if(extract_cmd(&usart_rx_buffer[0]))
		{
			//usart_write("Ready\r\n\r\n");
		}
		else
		{
			usart_write("ERROR\r\n\r\n");
		}
		usart_status.usart_ready =0;
	
	}
	
	// RoBue:
	// Uhrzeit bestimmen
	hh = (time/3600)%24;
	mm = (time/60)%60;
	ss = time%60;


	#if USE_OW
	// RoBue:
	// Zur�cksetzen der Min/Max-Werte um 00:00 Uhr einschalten
	if (( hh == 00 )&&( mm == 00 )) {
		minmax = 1;
	}
	#endif

	// ******************************************************************
	// RoBue:
	// 1-Wire-Temperatursensoren (DS18B20) abfragen
	// ******************************************************************

#if USE_OW

		uint8_t i = 0;
		uint8_t subzero, cel, cel_frac_bits;
		uint8_t tempID[OW_ROMCODE_SIZE];

		// Messen bei ss=5,15,25,35,45,55		
		if ( ss%10 == 5 ) {

		// Messen?
		if ( messen == 1 ) {

		// RoBue Anmerkung:
		// Hiermit werden ALLE Sensoren zum Messen aufgefordert.
		// Aufforderung nur bestimmter Sensoren:
		// "NULL" durch "tempID" ersetzen
		
		// RoBue Testausgabe UART:
		// usart_write("Starte Messvorgang ...\r\n");		 

		DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );
		
		// Kein Messen mehr bis ss=5,15,25,35,45,55
		messen = 0;

		// Jetzt kann ausgelesen werden
		auslesen = 1;

		}	// -> if messen
		
		}	// -> if ss

		// Auslesen bei ss=8,18,28,38,48,58
		if ( ss%10 == 8 ) {

		// Auslesen?
		if ( auslesen == 1 ) {

		// (erste) ID ins RAM holen
		memcpy_P(tempID,DS18B20IDs[0],OW_ROMCODE_SIZE);	
				
		while ( tempID[0] != 0 ) {
		//while ( tempID[0] == 0x10 ) {
				
			// RoBue Anmerkung:
			// Hiermit wird jeweils ein einzelner Sensor ausgelesen
			// und die Temperatur in ow_array abgelegt.
			// Achtung:
			// Pro Sekunde k�nnen max. ca. 10 Sensoren ausgelesen werden!
			if ( DS18X20_read_meas( tempID, &subzero,&cel, &cel_frac_bits) == DS18X20_OK ) {

				ow_array[i] = DS18X20_temp_to_decicel(subzero, cel, cel_frac_bits);

				// Minuswerte:
					if ( subzero )
						ow_array[i] *= (-1);
				
				// min/max:
				if ( minmax == 1 ) {
					// Zur�cksetzen der Min/Max_Werte 1x/Tag
					// auf die gerade aktuellen Temperaturen 
					ow_array[i+MAXSENSORS] = ow_array[i];
					ow_array[i+MAXSENSORS*2] = ow_array[i];
				}
				else {
					// Abgleich der Temp. mit den gespeicherten Min/Max-Werten
					if (ow_array[i]  < ow_array[i+MAXSENSORS])
        					ow_array[i+MAXSENSORS] = ow_array[i];
               				if (ow_array[i]  > ow_array[i+MAXSENSORS*2])
						ow_array[i+MAXSENSORS*2] = ow_array[i];
        			}

				//TWert = DS18X20_temp_to_decicel(subzero, cel, cel_frac_bits);
				//ow_array[i] = TWert;
				
			}	// -> if
			else {
				usart_write("\r\nCRC Error (lost connection?) ");
				DS18X20_show_id_uart( tempID, OW_ROMCODE_SIZE );


			}	// -> else
		
		// n�chste ID ins RAM holen
		memcpy_P(tempID,DS18B20IDs[++i],OW_ROMCODE_SIZE);

		}	// -> while
		
		}	// -> if auslesen

		auslesen = 0;	// Auslesen vorl�ufig abschalten
		messen = 1;	// Messen wieder erm�glichen
		minmax = 0;	// Min/Max-Werte vergleichen

		}	// -> if ss
		
#endif
	

  //Empfang von Zeitinformationen
	#if USE_NTP
		if(!ntp_timer){
		ntp_timer = NTP_REFRESH;
		ntp_request();
		}
	#endif //USE_NTP
		
  //Versand von E-Mails
  #if USE_MAIL
    if (mail_enable == 1)
    {
        mail_enable = 0;
        mail_send();
    }
  #endif //USE_MAIL
        
  //Rechner im Netzwerk aufwecken
  #if USE_WOL
    if (wol_enable == 1)
    {
        wol_enable = 0;
        wol_request();
    }
  #endif //USE_WOL
    
  #if USE_RULE
  // rulen bei ss=9,19,29,39,49,59		
	if ( ss%10 == 9 )
  {
    uint8_t count,pos;
 
    count=get_rule_count();
    for(pos=0;pos<count;pos++)
    {
      eeprom_run_rule(pos);
    }
  }
  
  #endif //USE_RULE
  
  #if USE_S0
  
  if ( mm == 00 )
  {
  
    save_eeprom();
  }
  
  #endif //USE_S0
     
	//USART Daten f�r Telnetanwendung?
	telnetd_send_data();
    }

return(0);
}
