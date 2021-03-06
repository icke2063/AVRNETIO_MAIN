/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        09.11.2007
 Description:    Html Seiten

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
#ifndef _WEBPAGE_H
	#define _WEBPAGE_H


#include "config.h"
#include "base_webpage.h"
#if USE_RULES
	#include "rules_webpage.h"
#endif

//****************************************************************************
//Dateien und Webseiten am Ende dieser Seite in Tabelle eintragen !!!!!!!
//****************************************************************************

//----------------------------------------------------------------------------
//Dazustellende Webseite
	PROGMEM char Page1[] = {
	"<html><head><meta http-equiv=\"refresh\" content=\"30\" text/html; charset=iso-8859-1\">\r\n"

	"<title>AVR NET IO</title>\r\n"

	"<style type=\"text/css\">\r\n"
  	".fett {font-weight: bold; margin-top:15px;}\r\n"
  	"td { cellpadding: 2px; }\r\n"
  	"body { font-family: arial;  font-size: 1.0 em; color: #000000; background-color: rgb(255, 255, 204)}\r\n"
  	"h1 {text-align: center; font-weight: bold; color: white; font-size: 1.2 em}\r\n"
	"</style>\r\n"

	"</head><body><center>\r\n"

	"<table style=\"width: 600px; text-align: left; background-color: rgb(153, 153, 255);\" cellpadding=\"5\">\r\n"
			"<tr><td></td></tr>\r\n"
	"<tr><td><h1>AVR-NET-IO</h1>\r\n"
	"<table style=\"width: 100%; font-weight: bold;\">\r\n"
        "<tr><td>Serverzeit: %TI@</td><td style=\"text-align: right;\">Pagecounter: %VA@29</td></tr></table>\r\n"
	"</td></tr></table>\r\n"

	"<br>"

	"<form name=\"form1\" method=\"post\" action=\" \">\r\n"
	"<table style=\"width: 600px; text-align: center; align=center; background-color: rgb(255, 255, 153);\" border=\"0\">\r\n"
	"<tr><td></td></tr>\r\n"
	"<tr><td></td></tr>\r\n"
	"<tr><td><p align=\"center\">INPUT/OUTPUT</p><hr></td></tr>\r\n"
	"<tr><td>%IO</td></tr>\r\n"
	"<tr><td><hr><p align=\"center\"><input type=\"submit\" name=\"SUB\" value=\"Senden\"></p></form></td></tr>\r\n"

	"</table>\r\n"


// => Schalten mit PORTC
// checkbox-value-Werte A-H -> httpd

	"</td></tr></table>\r\n"
// Schaltanweisungen --------------------------------------------------------
	"<br><hr>\r\n"
	"<div>PORTB -> ENC28J60</div>\r\n"
	"<div>Achtung: Kein LCD an PORTC anschlie�en! Besch�digung m�glich.</div>\r\n"
	"<br>\r\n"

	"<div>Webserver von Ulrich Radig " SoftVers "</div>\r\n"
	"<div>Compiliert am "__DATE__" um "__TIME__" mit GCC Version "__VERSION__"</div>\r\n"
	"<div>"RoBueVers" f�r AVR-NET-IO</div>\r\n"
		
	"</span></body></html>\r\n"
	"%END"};

// RoBue:
// Daten-Seite: 1-Wire-Werte, AD-Werte  
PROGMEM char Page2[] = {

	// AD-Werte
	"%VA@04\r\n"
	"%VA@05\r\n"
	"%VA@06\r\n"
	"%END"};

PROGMEM char EEProm[] = {
		"<form name=\"form2\" method=\"post\" action=\" \">"
		"ADDR:<input name=\"ADD\"><br>"
		"<input type=\"hidden\"name=\"gnaa\">"
		"VALUE:<input name=\"VAL\">"
		"<input type=\"submit\" name=\"SUB\" value=\"Senden\">"
		"</form>\r\n"
	"%EEPROM<br>\r\n"
	"%END"};
#if USE_RULES
PROGMEM char Rules[] = {
	"%RULES\r\n"
	"%END"};
#endif

//----------------------------------------------------------------------------
//Das GIF Bild f�r den Balken
//PROGMEM char bild_balken[] = {
//	0x47,0x49,0x46,0x38,0x39,0x61,0x02,0x00,0x0A,0x00,0xA2,0x00,0x00,0xA2,0xA5,0xED,
//	0x84,0x8F,0xE5,0x66,0x7C,0xDD,0x49,0x60,0xD6,0x21,0x44,0xDA,0xFE,0x01,0x02,0x00,
//	0x00,0x00,0x00,0x00,0x00,0x21,0xF9,0x04,0x05,0x14,0x00,0x05,0x00,0x2C,0x00,0x00,
//	0x00,0x00,0x02,0x00,0x0A,0x00,0x00,0x03,0x0A,0x48,0x34,0x23,0x12,0x01,0x48,0x00,
//	0x1D,0x53,0x09,0x00,0x3B,'%','E','N','D'};
//-----------------------------------------------------------------------------
//Nur Dateiname und Pointer eintragen	
	WEBPAGE_ITEM WEBPAGE_TABLE[] = // Befehls-Tabelle
	{
	{"index.htm",Page1},
	{"daten.html",Page2},
	{"eeprom.html",EEProm},
#if	USE_RULES
	{"rules.html",Rules},
#endif
//	{"balke.gif",bild_balken},
//	{"ledon.gif",led_on},
//	{"ledoff.gif",led_off},
	{NULL,NULL} 
	};

#endif //_WEBPAGE_H
