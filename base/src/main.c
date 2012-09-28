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
	

	//Ethernetcard Interrupt enable
	ETH_INT_ENABLE;
	
	//Globale Interrupts einschalten
	sei(); 
	
	



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


	//USART Daten f�r Telnetanwendung?
	telnetd_send_data();
    }

return(0);
}
