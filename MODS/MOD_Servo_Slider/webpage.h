/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        some changes by Michael Kleiber (slider on webpage)
 known Problems: none
 Version:        06.12.2008
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

//****************************************************************************
//Dateien und Webseiten am ende dieser Seite in Tabelle eintragen !!!!!!!
//****************************************************************************


//----------------------------------------------------------------------------
//Dazustellende Webseite
PROGMEM char Page0[] = {"401 Unauthorized%END"};

PROGMEM char Page1[] = 
{
  "<html><head>"
	"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />"
	"<title>AVR mini Webserver</title>"
	"<style type=\"text/css\">"
	
	"<!--"
	"BODY {	margin: 0 0 0 0; background-color: white; text-align:center; }"
	"P, DIV, H1, H2 {font-family: verdana, arial; font-size: 11px;}"
	"H1 {  background-color: #CCCCFF; padding-top:5px; padding-bottom:5px;"
	"font-size: 20px; color: #004060; font-weight:bold; letter-spacing: 8px;"
	"border-top: 2px solid #EEEEEE; border-left: 2px solid #EEEEEE;"
	"border-bottom: 2px solid #004060; border-right: 2px solid #004060;}"
	"H2 { font-size: 15px; padding-top: 20px; padding-left:50px; padding-right:50px;"
	"color:#004080; font-weight:bold; text-decoration:underline;"
	"border-top: 1px solid #004080;} P, H1, H2 {margin: 0px 0px 5px 0px;}"
	"-->"
	
	"</style></head>"
	"<blockquote>"
	"<body><div align=\"center\">"
	"<h1>Webserver</h1>"
	"</div>"
	
	"<div align=\"left\">" 

	"<form name=\"form1\" method=\"post\" action=\"\">"
	
	//Ports Schalten
  "<p>Outputs:</p>"
  "<table border=\"3\">"
  "<tr>"
  "<th><p>PC0</p></th>"
  "<th><p>PC1</p></th>"
  "<th><p>PC2</p></th>"
  "<th><p>PC3</p></th>"
  "<th><p>PC4</p></th>"
  "<th><p>PC5</p></th>"
  "<th><p>PC6</p></th>"
  "<th><p>PC7</p></th>"
  "</tr>"
  "<tr>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"A\" %PORTC0></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"B\" %PORTC1></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"C\" %PORTC2></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"D\" %PORTC3></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"E\" %PORTC4></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"F\" %PORTC5></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"G\" %PORTC6></td>"
  "<td><input type=\"checkbox\" name=\"OUT\" value=\"H\" %PORTC7></td>"
  "</tr>"
  "</table>"
	"<br>"
  //Eing�nge Abfragen %PINxy x = PORT / y = PIN
  "<p>Inputs:</p>"
  "<table border=\"3\">"
  "<tr>"
  "<th><p>PD2</p></th>"
  "<th><p>PD3</p></th>"
  "<th><p>PD4</p></th>"
  "<th><p>PD5</p></th>"
  "<th><p>PD6</p></th>"
  "<th><p>PD7</p></th>"
  "</tr>"
  "<tr>"
	"<td><img src=\"%PIND2\" width=\"17\" height=\"17\"></td>"
	"<td><img src=\"%PIND3\" width=\"17\" height=\"17\"></td>"
	"<td><img src=\"%PIND4\" width=\"17\" height=\"17\"></td>"
	"<td><img src=\"%PIND5\" width=\"17\" height=\"17\"></td>"
	"<td><img src=\"%PIND6\" width=\"17\" height=\"17\"></td>"
	"<td><img src=\"%PIND7\" width=\"17\" height=\"17\"></td>"
  "</tr>"
  "</table>"
	"<br>"
  //Rechner �ber WOL einschalten
  #if USE_WOL
  "<p>Wake on LAN:</p>"
  "<table border=\"3\">"
  "<tr>"
  "<th><p>PC1</p></th>"
  "<th><p>PC2</p></th>"
  "</tr>"
  "<tr>"
	"<td><img src=\"%WOL0\" width=\"17\" height=\"17\"></td>"
	"<td><img src=\"%WOL1\" width=\"17\" height=\"17\"></td>"
  "</tr>"
  "<tr>"
	"<td><input type=\"checkbox\" name=\"OUT\" value=\"W\"></td>"
	"<td><input type=\"checkbox\" name=\"OUT\" value=\"X\"></td>"
  "</tr>"
  "</table>"
	"<br>"
	#endif //USE_WOL

  #if USE_ADC
	"<br>"
	"<table border=\"3\">"
	"<tr>"
	"<td><p align=\"center\">AD-Kanal0</p></td>"
	"<td><p align=\"center\"></p><img src=\"balke.gif\" width=\"%VA@00\" height=\"10\"></td>"
	"<td><p align=\"center\">%VA@00</p></td>"
	"</tr>"
	"<tr>"
	"<td><p align=\"center\">AD-Kanal1</p></td>"
	"<td><p align=\"center\"></p><img src=\"balke.gif\" width=\"%VA@01\" height=\"10\"></td>"
	"<td><p align=\"center\">%VA@01</p></td>"
	"</tr>"
	"</table>"
	#endif //USE_ADC
  
	// Eingabe f�r Servo
  "<p>Servo:</p>"
  "<input type=\"text\" id=\"SliVal\" name=\"SLI\" VALUE=\"%VA@08\" SIZE=\"3\">"
  // Slider und Javascript
  "<div style=\"position:absolute; margin-top:5; margin-left:10;height:22; width:160\">"
  "<div style=\"position:absolute; top:-1px; left:-1px; height:1px;width:270px; background-color:#606060; overflow:hidden\"></div>"
  "<div style=\"position:absolute; top:-1px; left:-1px; height:17px;width:1px; background-color:#606060; overflow:hidden\"></div>"
  "<div style=\"position:absolute; top:-1px; left:268px; height:17px;width:1px; background-color:#606060; overflow:hidden\"></div>"
  "<div style=\"position:absolute; top:15px; left:-1px; height:1px;width:270px; background-color:#606060; overflow:hidden\"></div>"
  "<div id=\"slider\" style=\"position:absolute; top:-4px; left:%VA@08px;height:20px; width:20px; cursor:pointer\">"
  "<img src=\"arrow.gif\" width=12 height=20>"
  "</div></div>"

  "<script type=\"text/javascript\">"
  "var isDrag = false;"
  "var sliderButton;"
  "var xo = 0;"

  "slider.onmousedown = mouseDownSlider;"
  "document.onmousemove = mouseMoveSlider;"
  "slider.onmouseup = mouseUpSlider;"

  "function mouseUpSlider(){"
  "isDrag = false;"
  "return true;"
  "}"

  "function mouseDownSlider(e){"
  "isDrag=true;"
  "var tx=parseInt(slider.style.left);"
  "if(document.all){"
  "xo=event.clientX+document.body.scrollLeft-tx;"
  "}"
  "else{"
  "xo=e.pageX-tx;"
  "}"
  "return false;"
  "}"
  "function mouseMoveSlider(e)"
  "{"
  "if(isDrag)"
  "{"
  "if(document.all)"
  "{"
  "x=event.clientX+document.body.scrollLeft;"
  "}"
  "else"
  "{"
  "x=e.pageX;"
  "}"
  "x=Math.max(0,Math.min(x-xo,255));"
  "slider.style.left=x;"
  // *** \/ *** Slider-Wert in Eingabefeld �bertragen 
  "document.getElementById('SliVal').value=x;"
  // *** /\ ***
  "}"
  "return false;"
  "}"
  "</script>"
    
	"<br>"
	"<input type=\"submit\" name=\"SUB\" value=\"O.K.\">"
	//Statusseite
	"<p align=\"left\"> <h2></h2>"
	"<p><a href=\"Status.htm\">Status</a></p><BR>"
	"</div></body></html>"
	"%END"};	

//----------------------------------------------------------------------------
//Status Seite
PROGMEM char Page2[] = 
{
  "<html>"
	"<body><div align=\"center\">"
	"<h1>Status Page</h1>"
	"</div><BR>"
	"<div>Visitors: %VA@09</div>"
	"<div>Online since: %STR0</div>"
	"<BR><BR> <p align=\"left\"> <h2></h2><BR>"
	"<p><a href=\"index.htm\">Home</a></p><BR>"
	"</body></html>"
	"%END"
};

//----------------------------------------------------------------------------
//Das GIF Bild f�r LED on
PROGMEM char led_on[] = 
{
	0x47,0x49,0x46,0x38,0x39,0x61,0x11,0x00,0x11,0x00,0xA5,0x3F,0x00,0x4D,0x4D,0x4D,
	0x50,0x50,0x4F,0x54,0x54,0x54,0x41,0x6A,0x44,0x5C,0x5C,0x5C,0x3F,0x73,0x42,0x62,
	0x62,0x62,0x6A,0x6B,0x6B,0x16,0xA7,0x1E,0x6F,0x6F,0x6F,0x5F,0x7A,0x60,0x19,0xAD,
	0x21,0x1C,0xB2,0x25,0x56,0x94,0x59,0x7B,0x7B,0x7B,0x1A,0xBD,0x23,0x23,0xBA,0x2C,
	0x1B,0xC3,0x25,0x83,0x83,0x83,0x1C,0xCA,0x26,0x7C,0x8B,0x7D,0x1C,0xD3,0x26,0x8B,
	0x8B,0x8B,0x2D,0xCC,0x36,0x1C,0xD8,0x26,0x1D,0xDF,0x28,0x90,0x91,0x91,0x1D,0xE1,
	0x28,0x1E,0xE2,0x29,0x21,0xE2,0x2C,0x35,0xDB,0x3F,0x2E,0xE4,0x38,0x9C,0x9D,0x9D,
	0x3E,0xE6,0x47,0x40,0xE6,0x49,0xA5,0xA5,0xA5,0x47,0xE7,0x50,0x50,0xE4,0x58,0xA9,
	0xA9,0xA9,0x4E,0xE8,0x56,0xAE,0xAE,0xAD,0xAF,0xB0,0xAF,0xB0,0xB0,0xB0,0x64,0xE6,
	0x6B,0xB4,0xB4,0xB5,0xB8,0xB8,0xB9,0x6C,0xEC,0x73,0x77,0xED,0x7D,0xBF,0xC0,0xBF,
	0xC0,0xC0,0xC0,0xC2,0xC2,0xC2,0xA6,0xDA,0xA9,0xC9,0xC8,0xC9,0x93,0xF1,0x98,0xCC,
	0xCC,0xCC,0xD1,0xD1,0xD1,0xA5,0xF3,0xA9,0xD5,0xD5,0xD5,0xDE,0xDE,0xDE,0xE0,0xE0,
	0xE0,0xC5,0xF7,0xC8,0xE7,0xE7,0xE7,0xEF,0xEF,0xEF,0xFF,0xFF,0xFF,0x21,0xF9,0x04,
	0x01,0x0A,0x00,0x3F,0x00,0x2C,0x00,0x00,0x00,0x00,0x11,0x00,0x11,0x00,0x00,0x06,
	0xED,0xC0,0x9F,0x50,0xD8,0xCB,0xC9,0x54,0xA9,0x96,0x6D,0xC8,0xFC,0xF9,0x74,0x37,
	0x1A,0x8C,0x65,0x02,0x69,0x40,0xB4,0xA6,0x6F,0xA7,0x2B,0x2D,0x10,0xE0,0xC5,0x45,
	0xE2,0x68,0x0D,0x7D,0xBB,0x19,0x23,0xE2,0x09,0x7D,0x32,0x95,0xC7,0xA2,0x91,0x80,
	0x09,0x75,0x3A,0xC6,0xC5,0xF5,0x72,0x9D,0x3E,0x1B,0x15,0x13,0x0B,0x09,0x12,0x3F,
	0x3D,0x37,0x1E,0x11,0x2B,0x35,0x38,0x35,0x2E,0x22,0x1D,0x1B,0x18,0x0F,0x10,0x06,
	0x2C,0x39,0x34,0x0B,0x1E,0x2F,0x38,0x3C,0x38,0x2F,0x24,0x1D,0x1B,0x1B,0x15,0x0B,
	0x06,0x16,0x32,0x30,0x08,0x21,0x2E,0x35,0x38,0x35,0x2E,0x22,0x1C,0xA4,0x18,0x08,
	0x04,0x07,0x2A,0x2C,0x08,0x1F,0x27,0x2E,0x2F,0x2E,0x27,0x1F,0xA4,0x1B,0x19,0x08,
	0x04,0x02,0x29,0x26,0x08,0x19,0x1F,0x22,0x24,0x22,0x1F,0xC4,0x1B,0x19,0x08,0x02,
	0x06,0x2D,0x20,0x0B,0x18,0x1B,0x1D,0xDE,0xD3,0x1B,0x15,0x0B,0x02,0x12,0x36,0x1A,
	0x10,0x0F,0xE0,0xD3,0x11,0x10,0x00,0x28,0x3F,0x20,0x14,0x0B,0x13,0xEA,0x1B,0x13,
	0x0B,0x03,0x09,0x42,0x34,0x0E,0x0D,0x0B,0x0F,0x18,0x32,0x6C,0xC8,0x80,0xE1,0xC1,
	0x82,0x02,0x00,0x58,0x0C,0x69,0x91,0x40,0x01,0x84,0x05,0x60,0x10,0x2C,0x60,0x10,
	0x00,0xC0,0x08,0x26,0x3F,0x62,0x48,0x30,0x60,0x80,0x00,0x01,0x01,0x02,0x00,0x24,
	0x60,0x81,0x71,0x08,0x0B,0x0B,0x07,0x04,0x18,0x90,0x80,0x02,0x63,0x10,0x00,0x3B,
	'%','E','N','D'};
	
//----------------------------------------------------------------------------
//Das GIF Bild f�r den LED off
PROGMEM char led_off[] =
{
	0x47,0x49,0x46,0x38,0x39,0x61,0x11,0x00,0x11,0x00,0xA5,0x3F,0x00,0x4D,0x4D,0x4D,
	0x50,0x50,0x50,0x52,0x52,0x52,0x54,0x54,0x54,0x57,0x56,0x57,0x5A,0x5A,0x59,0x5D,
	0x5D,0x5D,0x5E,0x5D,0x5D,0x5E,0x5E,0x5E,0x60,0x60,0x60,0x62,0x62,0x62,0x63,0x63,
	0x63,0x64,0x64,0x64,0x65,0x65,0x65,0x69,0x69,0x69,0x6E,0x6E,0x6E,0x70,0x70,0x70,
	0x77,0x77,0x77,0x78,0x78,0x78,0x7A,0x7A,0x7A,0x7B,0x7B,0x7B,0x7E,0x7E,0x7E,0x7F,
	0x7F,0x7F,0x80,0x80,0x80,0x81,0x81,0x81,0x83,0x83,0x83,0x87,0x87,0x87,0x89,0x89,
	0x89,0x8B,0x8B,0x8B,0x8E,0x8E,0x8E,0x90,0x91,0x91,0x91,0x91,0x92,0x93,0x93,0x93,
	0x97,0x97,0x97,0x9A,0x9A,0x9A,0x9C,0x9C,0x9C,0xA5,0xA5,0xA5,0xA9,0xA9,0xA9,0xAB,
	0xAB,0xAB,0xAC,0xAC,0xAC,0xAE,0xAE,0xAD,0xAF,0xAF,0xAF,0xAF,0xB0,0xAF,0xB0,0xB0,
	0xB0,0xB2,0xB2,0xB2,0xB4,0xB4,0xB4,0xB4,0xB4,0xB5,0xB5,0xB5,0xB5,0xB8,0xB8,0xB9,
	0xBF,0xC0,0xBF,0xBF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC2,0xC2,0xC2,0xC8,0xC8,0xC8,0xC9,
	0xC9,0xC9,0xCC,0xCC,0xCC,0xD1,0xD1,0xD1,0xD5,0xD5,0xD5,0xDD,0xDD,0xDD,0xDE,0xDF,
	0xDE,0xE0,0xE0,0xE0,0xE7,0xE7,0xE7,0xEF,0xEF,0xEF,0xFF,0xFF,0xFF,0x21,0xF9,0x04,
	0x01,0x0A,0x00,0x3F,0x00,0x2C,0x00,0x00,0x00,0x00,0x11,0x00,0x11,0x00,0x00,0x06,
	0xEC,0xC0,0x9F,0x50,0xD8,0xCB,0xD1,0x56,0x2A,0xD8,0x6D,0xC8,0xFC,0xF9,0x76,0xB8,
	0x5A,0xEC,0x55,0x1A,0x79,0x46,0xB5,0xA6,0x8F,0xA7,0x13,0x35,0x10,0x06,0xC4,0x42,
	0xA3,0xA1,0xC0,0x86,0x3E,0xDE,0xCC,0x31,0xE9,0x7C,0x36,0x95,0x09,0xA4,0x11,0x81,
	0xC8,0x84,0x3B,0x9D,0xA3,0x73,0x62,0x9D,0x46,0x1B,0x16,0x16,0x12,0x0C,0x0F,0x18,
	0x3F,0x3D,0x38,0x1F,0x13,0x27,0x34,0x37,0x34,0x27,0x20,0x18,0x82,0x10,0x12,0x0A,
	0x2E,0x39,0x36,0x0D,0x1D,0x2C,0x37,0x3B,0x37,0x2C,0x21,0x19,0x82,0x13,0x0A,0x09,
	0x1C,0x34,0x31,0x08,0x1F,0x26,0x34,0x37,0x34,0x27,0x20,0x17,0x82,0x15,0x08,0x07,
	0x0E,0x2B,0x2F,0x06,0x1B,0x23,0x27,0x2C,0x27,0x23,0x1B,0x82,0x82,0x06,0x05,0x03,
	0x29,0x25,0x08,0x15,0x1B,0x20,0x21,0x20,0x1B,0xC3,0x16,0x15,0x08,0x04,0x0C,0x30,
	0x23,0x0B,0x13,0x16,0x18,0x19,0x18,0xD1,0x16,0x13,0x0A,0x02,0x19,0x37,0x1E,0x17,
	0x10,0xDF,0xD1,0x10,0x15,0x00,0x28,0x3F,0x23,0x1A,0x0D,0x12,0xE9,0x16,0x12,0x0A,
	0x01,0x0F,0x42,0x35,0x14,0x11,0x0C,0x10,0x13,0x15,0x2C,0x54,0x98,0x00,0x41,0x41,
	0x01,0x00,0x2D,0x86,0xC0,0x80,0xF0,0x40,0x82,0x02,0x04,0x06,0x10,0x28,0x78,0x10,
	0x00,0x00,0x89,0x26,0x33,0x30,0x28,0x48,0x60,0xA0,0x00,0x01,0x01,0x00,0x1E,0xB4,
	0x68,0x32,0xC4,0x05,0x07,0x07,0x03,0x18,0x64,0x40,0xD1,0x24,0x08,0x00,0x3B,
	'%','E','N','D'
};
	
//----------------------------------------------------------------------------
//Das GIF Bild f�r den Balken
PROGMEM char bild_balken[] = 
{
	0x47,0x49,0x46,0x38,0x39,0x61,0x02,0x00,0x0A,0x00,0xA2,0x00,0x00,0xA2,0xA5,0xED,
	0x84,0x8F,0xE5,0x66,0x7C,0xDD,0x49,0x60,0xD6,0x21,0x44,0xDA,0xFE,0x01,0x02,0x00,
	0x00,0x00,0x00,0x00,0x00,0x21,0xF9,0x04,0x05,0x14,0x00,0x05,0x00,0x2C,0x00,0x00,
	0x00,0x00,0x02,0x00,0x0A,0x00,0x00,0x03,0x0A,0x48,0x34,0x23,0x12,0x01,0x48,0x00,
	0x1D,0x53,0x09,0x00,0x3B,
	'%','E','N','D'
};

PROGMEM char bild_arrw[] = 
{
	0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x0C, 0x00, 0x14, 0x00, 0xF1, 0x00, 
	0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xC8, 0x60, 0x60, 0x60, 0x00, 0x00, 
	0x00, 0x21, 0xF9, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 
	0x00, 0x00, 0x0C, 0x00, 0x14, 0x00, 0x00, 0x08, 0x45, 0x00, 0x01, 0x08, 
	0x04, 0x10, 0xA0, 0x60, 0x80, 0x81, 0x08, 0x09, 0x1A, 0x4C, 0x38, 0xD0, 
	0xE0, 0xC1, 0x81, 0x02, 0x22, 0x0A, 0x70, 0x28, 0x51, 0x00, 0x42, 0x87, 
	0x0C, 0x05, 0x62, 0xCC, 0xB8, 0x91, 0x61, 0xC7, 0x84, 0x1F, 0x2F, 0x2E, 
	0xE4, 0x38, 0xD2, 0x63, 0x49, 0x90, 0x27, 0x45, 0x16, 0x54, 0xE9, 0xB0, 
	0xA5, 0xC2, 0x96, 0x25, 0x61, 0xAE, 0x64, 0x39, 0x13, 0x65, 0x4D, 0x00, 
	0x15, 0x27, 0x06, 0xC8, 0x19, 0x10, 0x00, 0x3B,
	'%','E','N','D'
};	
//----------------------------------------------------------------------------
//Nur Dateiname und Pointer eintragen	
WEBPAGE_ITEM WEBPAGE_TABLE[] = // Befehls-Tabelle
{
	{"index.htm",Page1},
	{"Status.htm",Page2},
	{"ledon.gif",led_on},
	{"ledoff.gif",led_off},
  {"arrow.gif",bild_arrw},
  {"balke.gif",bild_balken},
	{NULL,NULL} 
};

#endif //_WEBPAGE_H








