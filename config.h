/*----------------------------------------------------------------------------
Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
Author:         Radig Ulrich
Remarks:        
known Problems: none
Version:        03.11.2007
Description:    Webserver Config-File

-----------------------------------------------------------------------------
 
Modifiziert f�r AVR-NET-IO von Pollin durch RoBue u.a.

**************************************************
*Achtung: Bitte alles hier sorgfaeltig durchlesen*
**************************************************

Grundidee: Konfiguration �ber EINE Datei (= config.h)
-----------------------------------------------------

- Taktfrequenz
- PORTS
- LCD
- Bezeichnungen f�r die Schaltausg�nge (-> webpage.h)
- Netzwerkeinstellungen (MAC, IP. Subnet, Routing, NTP, WOL)
- 1-Wire-Sensoren (Anzahl, ID)
- ...
Rest l�sst sich �ber UART/Telnet und Webpage konfigurieren

 
Konfiguration (vgl. main.c, httpd.c, webpage.h):
-----------------------------------------------
 
PORTA -> Eingabe (A0-3 digital, A4-6 analog, A7 1-Wire)
PORTB -> ENC-Netwerkcontroller (PB0 -> LCD)
PORTC -> Schaltausgaenge (z.B. Pollin Relaiskarte)
PORTD -> D0,1 -> UART, RS232, WebCam
          D2-7 -> LCD (4-Bit Mode), Schaltausgaenge, Servo

PORTD ist fuer zusaetzliche "Spielereien" gedacht.
Dabei ist jedoch nicht alles zugleich m�glich!
 

Standard-Beispiel-Konfiguration von config.h fuer V1.5:
------------------------------------------------------

UART/RS232 (PORTD0,1)
LCD im 4-Bit Mode an PORTD2-7, PORTB0 (= EXT-Port) 

Temperaturmessung mit 5 Sensoren (DS1820)
	- Raum_1
	- Raum_2
	- Aussen
	- Zirkulationspumpe (R�cklauf, Vorlauf)
(Die IDs der Sensoren muessen in config.h. manuell eingegeben werden.
Es gibt bisher noch keine sinnvolle automatische Suchfunktion.
Platz ist fuer max. 8 Sensoren reserviert.)
 

Schaltfunktionen (vgl. main.c, httpd.c, webpage.h):
	- PORTC0: Raum_1 abhaengig von Sensor 0
	- PORTC1: Raum_2 abhaengig von Sensor 1 und Fenster offen/zu (PORTA1)
	- PORTC4: Zirkulationspumpe mit Sensor 3,4
	- PORTC6: Licht ein/aus abhaengig von LDR an PORTA6 (AD-Wert)
	- PORTC7: Zeitschaltuhr


Erweiterungsmoeglichkeiten in V1.5 ueber config.h:
-------------------------------------------------

- minimal (nur Schaltfunktionen)
	#define OUTC 			0xFF
	#define OUTD 			0xFC
	#define USE_PORTD_SCHALT	1
	#define USE_SER_LCD		0
	#define USE_OW			0
	#define USE_AUTOMATIK		0

- WebCam
	#define F_CPU 14745600UL
	#define USE_CAM			1
	#define CAM_RESELUTION		0
(UART/RS232 faellt automatisch weg)

- Servo (evtl. in Zusammenarbeit mit WebCam)
	#define USE_SERVO		1
	#define OUTD 			0xFC
(PORTD7 wird benoetigt, d.h. kein LCD mehr moeglich)
	#define USE_SER_LCD		0 (LCD im seriellen Mode geht)
	#define USE_LCD_4Bit		0 (LCD im 4-Bit Mode geht auf keinen Fall!)
(Schaltfunktion fuer PORTD abschalten)
	#define USE_PORTD_SCHALT	0
	
- PORTD2-7 fuer zusaetzliche Schaltfunktionen
	#define USE_PORTD_SCHALT	1	
	#define OUTD 			0xFC
(Kein LCD, kein Servo moeglich)
	#define USE_SERVO		0
	#define USE_SER_LCD		0

- SHT71-Sensor
	#define USE_SHT			1
(In makefile bitte dann aendern:
	# cni:
	# Add LibSHT
	SRC += sht/libsht.c
Braucht viel Platz!)

Nat�rlich sind auch andere Kombinationen m�glich,
dies erfordert aber den Eingriff in andere Programmteile (Sourcen)
-> main.c, httpd.c, webpage.h.
Solche �nderungen sollten �ber sog. MOD-Files erfolgen,
die die vorhandenen Source-File durch entsprechend angepasste ersetzen.

Achtung:
Falls bestimmte Routinen gar nicht gebraucht werden (z.B. LCD, 1-Wire),
spart es Platz, diese auch in "makefile" zu deaktivieren ("#" davorsetzen).

Viel Spa� und Erfolg, RoBue
	
-----------------------------------------------------------------------------

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

#ifndef _CONFIG_H_
	#define _CONFIG_H_	
	
	//Version
	#define SoftVers "V1.0.99"
	#define RoBueVers "Version 1.5 RoBue vom 18.02.2009"
	#define Version "1.5"
	

// Konfiguration des Systems
// -------------------------

	//Taktfrequenz (bei AVR-NET-IO i.d.R. 16MHz
	#define F_CPU 16000000UL	
	//#define F_CPU 14745600UL
	//#define F_CPU 11059200UL
	
	//Timertakt intern oder extern
	#define EXTCLOCK 0 //0=Intern 1=Externer Uhrenquarz

	//Baudrate der seriellen Schnittstelle
	#define BAUDRATE 9600
	// define BAUDRATE 19200 // -> Handy Siemens C35i
	


#define USE_ADC			1

#define USE_OW			0	//PINA 7 reserviert
#define USE_I2C			0
#define USE_S0          0
#define	USE_WATCHDOG	0
#define	USE_WOL			0
#define USE_NTP			0 // 1= NTP Client on

#define USE_RULE  		0



#include "eeprom_mapping.h"

// PORTS (HEX) (1=OUTPUT / 0=INPUT)
// --------------------------------
// PORTA
// (PORTA4-7 ist mit blauen Schraubklemmen verbunden)
#if USE_ADC
	#include "analog_config.h"
#else
	#define ADC_COUNT 0
	#define	ADC_PA	0x00
#endif

#if	USE_OW
	#define	OW_PA		0x80
#else
	#define	OW_PA		0x00
#endif

// PORTA: restlichen digitalen Ports: Eingang/Ausgang

//0-3 -> dig. input, 4-6 -> analog. input, 7 - 1-Wire
#define DIG_IN_PA		0x0F & ~OW_PA & ~ADC_PA	//als Eingang markieren
#define DIG_OUT_PA		0x00 & ~OW_PA & ~ADC_PA	//als Ausgang markieren

#define DIG_IMP_PA		0x00 & ~OW_PA & ~ADC_PA	//als IMPULSE markieren
#define DIG_SW_PA		0x00 & ~OW_PA & ~ADC_PA	//als SCHALTER markieren

#define OUTA 			((0x00 | DIG_OUT_PA) & ~DIG_IN_PA)


//PORTB -> ENC-Netzwerkkontroller
//nur �ndern wenn man wei� was man macht!

//PORTC -> 0-1 -> I2C;2-7 -> dig. I/O

#if	USE_I2C
#define	I2C_PC		0x03
#else
#define	I2C_PC		0x00
#endif

#define DIG_IN_PC		0x00 & ~I2C_PC	//als Eingang markieren
#define DIG_OUT_PC		0xFF & ~I2C_PC	//als Ausgang markieren

#define DIG_IMP_PC		0xFF & ~I2C_PC	//als IMPULSE markieren
#define DIG_SW_PC		0x00 & ~I2C_PC	//als SCHALTER markieren

#define OUTC 			((0x00 | DIG_OUT_PC) & ~DIG_IN_PC)


//PORTD

#if	USE_S0
#define	S0_PD		0x0C
#else
#define	S0_PD		0x00
#endif


#define DIG_IN_PD		0xF0 & ~S0_PD & ~0x03	//als Eingang markieren
#define DIG_OUT_PD		0x00 & ~S0_PD & ~0x03	//als Ausgang markieren

#define DIG_IMP_PD		0x00 & ~S0_PD & ~0x03	//als IMPULSE markieren
#define DIG_SW_PD		0x00 & ~S0_PD & ~0x03	//als SCHALTER markieren


//PORTD -> 0-1 -> UART; 2-3 -> S0; 4-7 -> dig. I/O
#define OUTD 			(((0x00 | DIG_OUT_PD) & ~DIG_IN_PD))


// Namen f�r die dig. I/O -> webpage.h
// ---------------------------------------------------

// Netzwerkeinstellungen
// ---------------------

	//ETH_M32_EX (www.ulrichradig.de)
	#define USE_ENC28J60		1
	//Holger Buss (www.mikrocontroller.com) Mega32-Board
	#define USE_RTL8019		0

	//IP des Webservers
	#define MYIP		IP(10,0,0,91)

	//Netzwerkmaske
	#define NETMASK		IP(255,255,255,0)

	//IP des Routers
	#define ROUTER_IP	IP(10,0,0,1)

	//IP des NTP-Servers z.B. Server 1.de.pool.ntp.org
	#define NTP_IP		IP(192,168,0,1)

	
	//MAC Adresse des Webservers	
	#define MYMAC1	0x00
	#define MYMAC2	0x22
	#define MYMAC3	0xF9
	#define MYMAC4	0x01
	#define MYMAC5	0x00
	#define MYMAC6	0x01

	//Umrechnung von IP zu unsigned long
	#define IP(a,b,c,d) ((unsigned long)(d)<<24)+((unsigned long)(c)<<16)+((unsigned long)(b)<<8)+a

	//Webserver mit Passwort? (0 == mit Passwort)
	#define HTTP_AUTH_DEFAULT	1
	
	//AUTH String "USERNAME:PASSWORT" max 14Zeichen 
	//f�r Username:Passwort
	#define HTTP_AUTH_STRING "admin:uli1"

#if	USE_OW
// 1-Wire-Einstellungen
// --------------------

	// 1-Wire ja/nein (0=nein, 1=ja)

	// max. Anzahl der Sensoren
	#define MAXSENSORS	20
	// z.Z. nur EIN 1-Wire-Bus
	#define OW_ONE_BUS	1

	// Port fuer 1-Wire
	#define OW_PIN  PA7
	#define OW_IN   PINA
	#define OW_OUT  PORTA
	#define OW_DDR  DDRA

	// ROM-IDs der DS1820 m�ssen "per Hand" eingetragen werden!
	// Damit kann man am leichtesten einem Sensor eine Aufgabe zuordnen.
	// Die IDs kann man mit dem Flash-File "sensoren.hex" ermitteln.
	#define OW_ID_T01	{0x28,0x65,0x3E,0x15,0x02,0x00,0x00,0xE6}
	#define OW_ID_T02	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_T03	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_T04	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_T05	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_T06	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_T07	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_T08	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} // leer
	#define OW_ID_Last	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

	// Startwerte in ow_array
	#define OW_START	150
	#define OW_MINMAX	0

	// Entsprechende Ortszuweisung f�r die Sensoren fuer webpage.h:
	#define T00	"Raum_1"
	#define T01	"Raum_2"
	#define T02	"Aussen"
	#define T03	"Pumpe VL"
	#define T04	"Pumpe RL"
	#define T05	"Reserve"
	#define T06	"Reserve"
	#define T07	"Reserve"


#endif	//USE_OW

// Rest
// ----

	// Schaltautomatik installieren (-> main.c, httpd.c, webpage.h)
	#define USE_AUTOMATIK	0
  

	// Feuchtesensor HIH4000 (experimentell)
	//#define USE_HIH4000	0
	//#define VA_IN_HIH4000	4
	//#define VA_OUT_HIH4000	24
	
	//Emailversand benutzen? Konfiguration des
	//Emailclient in der Sendmail.h
	#define USE_MAIL        0
    
	//Commandos und Ausgaben erfolgen �ber Telnet
	//UART/RS232 geht dann nicht mehr!
	#define CMD_TELNET      0

	#define MAX_VAR_ARRAY	30	//    urspr�nglich 10
	// RoBue:
	// Variablen-Array 
	// zum Abspeichern verschiedener Werte
	// und zum Einf�gen in die Webseite %VA@00 bis %VA@29
	// VA0-3	-> ??? / SHT71
	// VA4-7	-> Analogwert von PORTA4-7
	// VA8		-> Reserve (Servo-Position)
	// VA9		-> Schaltautomatik ein/aus
	// VA10-17	-> Schalttemperaturen
	// VA18,19	-> Schaltwert analog
	// VA20-23	-> Schaltzeiten ein/aus hh,mm,hh,mm
	// VA24-28	-> Reserve / HIH4000
	// VA-Ende	-> Counter
	
    
#endif //_CONFIG_H
