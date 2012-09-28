/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Webserver Applikation

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
#include "httpd.h"
#include "webpage.h"

#if USE_RULE
#include "rules/rules_webpage.h"
#endif


struct http_table http_entry[MAX_TCP_ENTRY];

//Hier wird das codierte Passwort aus config.h gespeichert.
unsigned char http_auth_passwort[20];

unsigned char post_in[5] = {'O','U','T','='};
unsigned char post_ready[5] = {'S','U','B','='};

unsigned char PORT_tmp_A = 0;
unsigned char PORT_tmp_C = 0;
unsigned char PORT_tmp_D = 0;

unsigned char TOGG_tmp_A = 0;
unsigned char TOGG_tmp_C = 0;
unsigned char TOGG_tmp_D = 0;

unsigned char post_eeprom[6] = {'A','D','D','='};
unsigned char post_val[6] = {'V','A','L','='};

unsigned short int eeprom_address = 0;


static unsigned short int position = EEPROM_SIZE;
char time_string_Buffer[10];

// unsigned char internal_message[10];

PROGMEM char http_header1[]={	"HTTP/1.0 200 Document follows\r\n"
								"Server: AVR_WEB_Switch\r\n"
								"Content-Type: text/html\r\n\r\n"};

PROGMEM char http_header2[]={	"HTTP/1.0 200 Document follows\r\n"
								"Server: AVR_WEB_Switch\r\n"
								"Content-Type: image/jpg\r\n\r\n"};

PROGMEM char http_header3[]={	"HTTP/1.0 401 Unauthorized\r\n"
								"Server: AVR_WEB_Switch\r\n"
								"WWW-Authenticate: Basic realm=\"NeedPassword\""
								"\r\nContent-Type: text/html\r\n\r\n"};

//----------------------------------------------------------------------------
//Kein Zugriff Seite bei keinem Passwort
PROGMEM char Page0[] = {"401 Unauthorized%END"};

unsigned char rx_header_end[5] = {"\r\n\r\n\0"};

//----------------------------------------------------------------------------
//Initialisierung des Httpd Testservers
void httpd_init (void)
{
	//HTTP_AUTH_STRING 
	decode_base64((unsigned char*)HTTP_AUTH_STRING,http_auth_passwort);

	//Serverport und Anwendung eintragen
	add_tcp_app (HTTPD_PORT, (void(*)(unsigned char))httpd);
}
   
//----------------------------------------------------------------------------
//http Testserver
void httpd (unsigned char index)
{
    //Verbindung wurde abgebaut!
    if (tcp_entry[index].status & FIN_FLAG)
    {
        return;
    }

	//Allererste Aufruf des Ports f�r diese Anwendung
	//HTTPD_Anwendungsstack l�schen
	if(tcp_entry[index].app_status==1)
	{
		httpd_stack_clear(index);
	}
	
	//HTTP wurde bei dieser Verbindung zum ersten mal aufgerufen oder
	//HTML Header Retransmission!
	if (tcp_entry[index].app_status <= 2)
	{	
		httpd_header_check (index);
		return;
	}
	
	//Der Header wurde gesendet und mit ACK best�tigt (tcp_entry[index].app_status+1)
	//war das HTML Packet fertig, oder m�ssen weitere Daten gesendet werden, oder Retransmission?
	if (tcp_entry[index].app_status > 2 && tcp_entry[index].app_status < 0xFFFE)
	{
		httpd_data_send (index);
		return;
	}
	
	//Verbindung kann geschlossen werden! Alle HTML Daten wurden gesendet TCP Port kann
	//geschlossen werden (tcp_entry[index].app_status >= 0xFFFE)!!
	if (tcp_entry[index].app_status >= 0xFFFE)
	{
		tcp_entry[index].app_status = 0xFFFE;
		tcp_Port_close(index);
		return;
	}
	return;
}

//----------------------------------------------------------------------------
//HTTPD_STACK l�schen
void httpd_stack_clear (unsigned char index)
{
	http_entry[index].http_header_type =0;
	http_entry[index].first_switch = 0;
	http_entry[index].http_auth = HTTP_AUTH_DEFAULT;
	http_entry[index].new_page_pointer = 0;
	http_entry[index].old_page_pointer = 0;
	http_entry[index].post = 0;
	http_entry[index].auth_ptr = http_auth_passwort;
	http_entry[index].post_ptr = post_in;
	http_entry[index].post_ready_ptr = post_ready;
	http_entry[index].hdr_end_pointer = rx_header_end;
	HTTP_DEBUG("\r\n**** NEUE HTTP ANFORDERUNG ****\r\n\r\n");	
	return;
}

//----------------------------------------------------------------------------
//Eintreffenden Header vom Client checken
void httpd_header_check (unsigned char index)
{
	unsigned int a = 0;
	
	if(strcasestr_P((char*)&eth_buffer[TCP_DATA_START_VAR],PSTR("POST"))!=0)
		{
		http_entry[index].post = 1;
		}
	
	//finden der Authorization und das Ende im Header auch �ber mehrere Packete hinweg!!	
	if(*http_entry[index].hdr_end_pointer != 0)
	{		
		for(a=TCP_DATA_START_VAR;a<(TCP_DATA_END_VAR);a++)
		{	
			HTTP_DEBUG("%c",eth_buffer[a]);
			
			if(!http_entry[index].http_auth) 
			{
				if (eth_buffer[a] != *http_entry[index].auth_ptr++)
				{
					http_entry[index].auth_ptr = http_auth_passwort;
				}
				if(*http_entry[index].auth_ptr == 0) 
				{
					http_entry[index].http_auth = 1;
					HTTP_DEBUG("  <---LOGIN OK!--->\r\n");
				}
			}
			
			if (eth_buffer[a] != *http_entry[index].hdr_end_pointer++)
			{
				http_entry[index].hdr_end_pointer = rx_header_end;
			}
			
			//Das Headerende wird mit (CR+LF+CR+LF) angezeigt!
			if(*http_entry[index].hdr_end_pointer == 0) 
			{
				HTTP_DEBUG("<---HEADER ENDE ERREICHT!--->\r\n");
				break;
			}
		}
	}
	
	//Einzelne Postpacket (z.B. bei firefox)
	if(http_entry[index].http_auth && http_entry[index].post == 1)
	{
		for(a = TCP_DATA_START_VAR;a<(TCP_DATA_END_VAR);a++)
		{	
			// Schaltanweisung finden!
			if (eth_buffer[a] != *http_entry[index].post_ptr++)
			{
				http_entry[index].post_ptr = post_in;
			}
			if(*http_entry[index].post_ptr == 0) 
			{

				// icke2063:
				// Schaltanweisung [S,T][A..D][0..7]
				unsigned char port = eth_buffer[a+2];
				unsigned char pin = eth_buffer[a+3]-0x30;

				switch (eth_buffer[a+1])
				{
				case ('S')://Schalten
					switch(port){
					    case ('A')://PORTA
							if(DIG_OUT_PA & DIG_SW_PA & ~DIG_IMP_PA & (1<<pin)){
								PORT_tmp_A |= (1<<pin);
							}
							break;
					    case ('C')://PORTC
							if(DIG_OUT_PC & DIG_SW_PC & ~DIG_IMP_PC & (1<<pin)){
								PORT_tmp_C |= (1<<pin);
							}
							break;
					    case ('D')://PORTD
							if(DIG_OUT_PD & DIG_SW_PD & ~DIG_IMP_PD & (1<<pin)){
								PORT_tmp_D |= (1<<pin);
							}
							break;
					    }
					break;

				case ('T')://Tasten
					switch(port){
					    case ('A')://PORTA
							if(DIG_OUT_PA & ~DIG_SW_PA & DIG_IMP_PA & (1<<pin)){
								TOGG_tmp_A |= (1<<pin);
							}
							break;
					    case ('C')://PORTC
							if(DIG_OUT_PC & ~DIG_SW_PC & DIG_IMP_PC & (1<<pin)){
								TOGG_tmp_C |= (1<<pin);
							}
							break;
					    case ('D')://PORTD
							if(DIG_OUT_PD & ~DIG_SW_PD & DIG_IMP_PD & (1<<pin)){
								TOGG_tmp_D |= (1<<pin);
							}
							break;
					    }
					break;

					default:;;
				}
				http_entry[index].post_ptr = post_in;
				//Schaltanweisung wurde gefunden
			}
		

				// EEPROM Adresse finden!
				if (eth_buffer[a] != post_eeprom[post_eeprom[5]++])
				{
					post_eeprom[5] = 0;
				}
				if(post_eeprom[post_eeprom[5]] == 0)
				{

					//Ende finden
					int ende = a;
					while((ende < TCP_DATA_END_VAR) && (eth_buffer[ende] !=0x26))
					{
					    ende++;
					}

					//Hexadezimal ?
					if(((eth_buffer[a+1] != 0x26) && (eth_buffer[a+1] == 0x30)) && ((eth_buffer[a+2] != 0x26) && (eth_buffer[a+2] == 0x78)))
					{
					    eeprom_address = strtol(&eth_buffer[a+1],&eth_buffer[ende],16);
					}else{
					    eeprom_address = strtol(&eth_buffer[a+1],&eth_buffer[ende],10);
					}
					

					if(eeprom_address > EEPROM_SIZE)
					{
					    eeprom_address = 0;
					}
					    post_eeprom[5] = 0;
				}



				// EEPROM Wert finden!
				if (eth_buffer[a] != post_val[post_val[5]++])
				{
					post_val[5] = 0;
				}
				if(post_val[post_val[5]] == 0)
				{
					//Ende finden
					int ende = a;
					while((ende < TCP_DATA_END_VAR) && (eth_buffer[ende] !=0x26))
					{
					    ende++;
					}

					//Hexadezimal ?
					if(   ((eth_buffer[a+1] != 0x26) && (eth_buffer[a+1] == 0x30)) && ((eth_buffer[a+2] != 0x26) && (eth_buffer[a+2] == 0x78)))
					{
					    eeprom_write_byte(eeprom_address,(char)strtol(&eth_buffer[a+1],&eth_buffer[ende],16));
					}else{
					    int pos = a+1;
					    while((pos<ende) && (eeprom_address < EEPROM_SIZE))
					    {
					    	eeprom_write_byte(eeprom_address++,eth_buffer[pos]);
					    	pos++;
					    }
					}
					post_val[5] = 0;
				}


			//Submit schlie�t die suche ab!
			if (eth_buffer[a] != *http_entry[index].post_ready_ptr++)
			{
				http_entry[index].post_ready_ptr = post_ready;
			}
			if(*http_entry[index].post_ready_ptr == 0)
			{
				http_entry[index].post = 0;
				// icke2063
				
				eeprom_address = EEPROM_SIZE;
								
				// Schaltanweisung ausf�hren = PORTA setzen
				PORTA = PORT_tmp_A & DIG_OUT_PA;
				PORT_tmp_A = 0;

				// Schaltanweisung ausf�hren = PORTC setzen
				PORTC = PORT_tmp_C & DIG_OUT_PC;
				PORT_tmp_C = 0;

				// Schaltanweisung ausf�hren = PORTD setzen
				PORTD = PORT_tmp_D & DIG_OUT_PD;
				PORT_tmp_D = 0;

				//Toggle PINS
				toggle_PA(TOGG_tmp_A);
				TOGG_tmp_A = 0;
				toggle_PC(TOGG_tmp_C);
				TOGG_tmp_C = 0;
				toggle_PD(TOGG_tmp_D);
				TOGG_tmp_D = 0;





				break;
				//Submit gefunden
			}
		}
	}

	
	
	//Welche datei wird angefordert? Wird diese in der Flashspeichertabelle gefunden?
	unsigned char page_index = 0;
	
	if (!http_entry[index].new_page_pointer)
	{
		for(a = TCP_DATA_START_VAR+5;a<(TCP_DATA_END_VAR);a++)
		{
			if (eth_buffer[a] == '\r')
			{
				eth_buffer[a] = '\0';
				break;
			}
		}
	
		while(WEBPAGE_TABLE[page_index].filename)
		{
			if (strcasestr((char*)&eth_buffer[TCP_DATA_START_VAR],WEBPAGE_TABLE[page_index].filename)!=0) 
				{
					http_entry[index].http_header_type = 1;
					HTTP_DEBUG("\r\n\r\nDatei gefunden: ");
					HTTP_DEBUG("%s",(char*)WEBPAGE_TABLE[page_index].filename);
					HTTP_DEBUG("<----------------\r\n\r\n");	
					if (strcasestr(WEBPAGE_TABLE[page_index].filename,".jpg")!=0)
					{
						http_entry[index].http_header_type = 1;
					}
					if (strcasestr(WEBPAGE_TABLE[page_index].filename,".gif")!=0)
					{
						http_entry[index].http_header_type = 1;
					}	
					if (strcasestr(WEBPAGE_TABLE[page_index].filename,".htm")!=0)
					{
						http_entry[index].http_header_type = 0;	
					}	
					http_entry[index].new_page_pointer = WEBPAGE_TABLE[page_index].page_pointer;
					break;
				}
			page_index++;
		}
	}

	//Wurde das Ende vom Header nicht erreicht
	//kommen noch weitere St�cke vom Header!
	if ((*http_entry[index].hdr_end_pointer != 0) || (http_entry[index].post == 1))
	{
		//Der Empfang wird Quitiert und es wird auf weiteres Headerst�ck gewartet
		tcp_entry[index].status =  ACK_FLAG;
		create_new_tcp_packet(0,index);
		//Warten auf weitere Headerpackete
		tcp_entry[index].app_status = 1;
		return;
	}	
	
	//Wurde das Passwort in den ganzen Headerpacketen gefunden?
	//Wenn nicht dann ausf�hren und Passwort anfordern!
	if((!http_entry[index].http_auth) && tcp_entry[index].status&PSH_FLAG)
	{	
		//HTTP_AUTH_Header senden!
		http_entry[index].new_page_pointer = Page0;
		memcpy_P((char*)&eth_buffer[TCP_DATA_START_VAR],http_header3,(sizeof(http_header3)-1));
		tcp_entry[index].status =  ACK_FLAG | PSH_FLAG;
		create_new_tcp_packet((sizeof(http_header3)-1),index);
		tcp_entry[index].app_status = 2;
		return;
	}
	
	//Standart INDEX.HTM Seite wenn keine andere gefunden wurde
	if (!http_entry[index].new_page_pointer)
	{
		//Besucher Counter
		var_array[MAX_VAR_ARRAY-1]++;
			// RoBue:
			// Counter zuruecksetzen
			if (var_array[MAX_VAR_ARRAY-1] == 10000)
				var_array[MAX_VAR_ARRAY-1] = 0;
		http_entry[index].new_page_pointer = Page1;
		http_entry[index].http_header_type = 0;
	}	
	
	tcp_entry[index].app_status = 2;
	//Seiten Header wird gesendet
	if(http_entry[index].http_header_type == 1)
	{
		memcpy_P((char*)&eth_buffer[TCP_DATA_START_VAR],http_header2,(sizeof(http_header2)-1));
        tcp_entry[index].status =  ACK_FLAG | PSH_FLAG;
        create_new_tcp_packet((sizeof(http_header2)-1),index);
        return;
	}
     
	if(http_entry[index].http_header_type == 0)
	{
		memcpy_P((char*)&eth_buffer[TCP_DATA_START_VAR],http_header1,(sizeof(http_header1)-1));
        tcp_entry[index].status =  ACK_FLAG | PSH_FLAG;
        create_new_tcp_packet((sizeof(http_header1)-1),index);
        return;
	}
    return;
}

//----------------------------------------------------------------------------
//Daten Packete an Client schicken
void httpd_data_send (unsigned char index)
{	
	unsigned int a;
	unsigned char str_len;
	unsigned short mark;
	
	char var_conversion_buffer[CONVERSION_BUFFER_LEN];
	
	//Passwort wurde im Header nicht gefunden
	if(!http_entry[index].http_auth)
	{
		http_entry[index].new_page_pointer = Page0;
	}

	//kein Packet empfangen Retransmission des alten Packetes
	if (tcp_entry[index].status == 0) 
	{
		http_entry[index].new_page_pointer = http_entry[index].old_page_pointer;
	}
	http_entry[index].old_page_pointer = http_entry[index].new_page_pointer;

	for (a = 0;a<(MTU_SIZE-(TCP_DATA_START)-150);a++)
	{
		unsigned char b;
		b = pgm_read_byte(http_entry[index].new_page_pointer++);
		eth_buffer[TCP_DATA_START + a] = b;
		
		//M�ssen Variablen ins Packet eingesetzt werden? ===> %VA@00 bis %VA@19
		if (b == '%')
		{


			if(base_create_httpd_data(&http_entry[index].new_page_pointer, var_conversion_buffer,eth_buffer,&a)>0)continue;


			// Variable
			if (strncasecmp_P("VA@",http_entry[index].new_page_pointer,3)==0)
			{	
				b = (pgm_read_byte(http_entry[index].new_page_pointer+3)-48)*10;
				b +=(pgm_read_byte(http_entry[index].new_page_pointer+4)-48);	
				itoa (var_array[b],var_conversion_buffer,10);
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len-1);
				http_entry[index].new_page_pointer=http_entry[index].new_page_pointer+5;
			}
			
			// EEPROM
			if (strncasecmp_P("EEPROM",http_entry[index].new_page_pointer,6)==0)
			{
				http_entry[index].new_page_pointer=http_entry[index].new_page_pointer-1;

				while( (position < 1024) && (a<(MTU_SIZE-(TCP_DATA_START)-10)) ){
					eeprom_busy_wait ();

					unsigned char	high = eeprom_read_byte(position);
					unsigned char	low = high & 0x0F;

					high = (high >> 4);


					if(high<10)
						high += 0x30;
					else
						high += 0x37;

					if(low<10)
						low += 0x30;
					else
						low += 0x37;


					switch(position%16){
					case 0:	//begin

							//write position 1
							itoa (position,var_conversion_buffer,10);
							str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
							memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
							a = a + (str_len);
							eth_buffer[TCP_DATA_START + a++] = ':';
							eth_buffer[TCP_DATA_START + a++] = ' ';

							eth_buffer[TCP_DATA_START + a++] = high;
							eth_buffer[TCP_DATA_START + a++] = low;
							eth_buffer[TCP_DATA_START + a++] = ' ';
							break;
					case 4:	//middle
							eth_buffer[TCP_DATA_START + a++] = ' ';
							eth_buffer[TCP_DATA_START + a++] = high;
							eth_buffer[TCP_DATA_START + a++] = low;
							eth_buffer[TCP_DATA_START + a++] = ' ';
							break;
					case 8:
							eth_buffer[TCP_DATA_START + a++] = ' ';
							eth_buffer[TCP_DATA_START + a++] = ' ';
							eth_buffer[TCP_DATA_START + a++] = high;
							eth_buffer[TCP_DATA_START + a++] = low;
							eth_buffer[TCP_DATA_START + a++] = ' ';
							break;
					case 12://middle
							eth_buffer[TCP_DATA_START + a++] = ' ';
							eth_buffer[TCP_DATA_START + a++] = high;
							eth_buffer[TCP_DATA_START + a++] = low;
							eth_buffer[TCP_DATA_START + a++] = ' ';
							break;
					case 15:
							eth_buffer[TCP_DATA_START + a++] = high;
							eth_buffer[TCP_DATA_START + a++] = low;

							strcpy_P(var_conversion_buffer, PSTR("<br>"));
							str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
							memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
							a = a + (str_len);

							eth_buffer[TCP_DATA_START + a++] = '\r';
							eth_buffer[TCP_DATA_START + a++] = '\n';

							position++;
							a--;
							continue;
							break;


					default:
							eth_buffer[TCP_DATA_START + a++] = high;
							eth_buffer[TCP_DATA_START + a++] = low;
							eth_buffer[TCP_DATA_START + a++] = ' ';
						break;
					}
					position++;

				}

				if(position >= 1023){
					http_entry[index].new_page_pointer=http_entry[index].new_page_pointer+7;
					position = 0;
				}
				a--;
			}

			// Zeitstring
			if (strncasecmp_P("TI@",http_entry[index].new_page_pointer,3)==0)
			{
				//unsigned char hh = (time/3600)%24;
				itoa (hh,var_conversion_buffer,10);
 	      			strcpy(time_string_Buffer,var_conversion_buffer);
        			strcat (time_string_Buffer,":");
				if ( mm < 10 ) {
					strcat (time_string_Buffer,"0");
						}
        			itoa (mm,var_conversion_buffer,10);
        			strcat(time_string_Buffer,var_conversion_buffer);
               			str_len = strnlen(time_string_Buffer,10);
        			memmove(&eth_buffer[TCP_DATA_START+a],time_string_Buffer,str_len);
	       			a = a + (str_len-1);
       			http_entry[index].new_page_pointer=http_entry[index].new_page_pointer+3;
			}


			//Einsetzen des Port Status %PORTxy durch "checked" wenn Portx.Piny = 1
			//x: A..G  y: 0..7 
			if (strncasecmp_P("PORT",http_entry[index].new_page_pointer,4)==0)
			{
				unsigned char pin  = (pgm_read_byte(http_entry[index].new_page_pointer+5)-48);	
				b = 0;
				switch(pgm_read_byte(http_entry[index].new_page_pointer+4))
				{
					case 'A':
						b = (PORTA & (1<<pin));
						break;
					case 'B':
						b = (PORTB & (1<<pin));
						break;
					case 'C':
						b = (PORTC & (1<<pin));
						break;
					case 'D':
						b = (PORTD & (1<<pin));
						break;
					// RoBue:
					// Checkbox Automatik
					case 'X':
						b = var_array[9];
						break; 
				}
				
				if(b)
				{
					strcpy_P(var_conversion_buffer, PSTR("checked"));
				}
				else
				{
					strcpy_P(var_conversion_buffer, PSTR("\0"));
				}
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a += str_len-1;
				http_entry[index].new_page_pointer = http_entry[index].new_page_pointer+6;
			}
						
			// RoBue:
			// Einsetzen des Pin Status %PI@xy bis %PI@xy 
			// durch gr�ne oder graue Hintergrundarbe der Tabellenzelle
      //x: 0..3  y: 0..7 

			//x = 0 : PINA / x = 1 : PINB / x = 2 : PINC / x = 3 : PIND

			if (strncasecmp_P("PIN",http_entry[index].new_page_pointer,3)==0)
			{
				unsigned char pin  = (pgm_read_byte(http_entry[index].new_page_pointer+4)-48);
        HTTP_DEBUG("Pin:%i\r\n",pgm_read_byte(http_entry[index].new_page_pointer+4)-48);	
				b = 0;
				switch(pgm_read_byte(http_entry[index].new_page_pointer+3))
				{
					case 'A':
						b = (PINA & (1<<pin));
						break;
					case 'B':
						b = (PINB & (1<<pin));
						break;
					case 'C':
						b = (PINC & (1<<pin));
						break;
					case 'D':
						b = (PIND & (1<<pin));
						break;    
				}
				
				if(b)
				{
					strcpy_P(var_conversion_buffer, PSTR("153, 0, 0")); // rot
				}
				else
				{
					strcpy_P(var_conversion_buffer, PSTR("0, 153, 0")); // gr�n
				}
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a += str_len-1;
				http_entry[index].new_page_pointer = http_entry[index].new_page_pointer+5;
			}
			//wurde das Ende des Packetes erreicht?
			//Verbindung TCP Port kann beim n�chsten ACK geschlossen werden
			//Schleife wird abgebrochen keine Daten mehr!!
			if (strncasecmp_P("END",http_entry[index].new_page_pointer,3)==0)
			{	
				tcp_entry[index].app_status = 0xFFFD;
				break;
			}
		}
	}
	//Erzeugte Packet kann nun gesendet werden!
	tcp_entry[index].status =  ACK_FLAG | PSH_FLAG;
	create_new_tcp_packet(a,index);
	return;
}


void toggle_PA (uint8_t porta)
{


	for(int i=0;i<8;i++){
		if(OUTA & porta & (1<<i)){
		      HTTP_DEBUG("Toggle PA(%i)\r\n",i);
		      HTTP_DEBUG("PA(%i):%i\r\n",i,PINA&(1<<i));

			//Toggle PIN
		    //1.Wechsel
			if(PINA&(1<<i))	//PIN == 1
			{
				HTTP_DEBUG("1.Eins->Null(%i)\r\n",i);
			PORTA = PORTA &= ~(1<<i);
			}
			else				//PIN == 0
			{
				HTTP_DEBUG("1.Null->Eins(%i)\r\n",i);
			PORTA = PORTA |= (1<<i) ;
			}

	  		//Warten
	  		_delay_ms(300);

	  		//2.Wechsel
			if(PINA & (1<<i))	//PIN == 1
			{
				HTTP_DEBUG("2.Eins->Null(%i)\r\n",i);
				PORTA = PORTA &= ~(1<<i);
			}
			else				//PIN == 0
			{
				HTTP_DEBUG("2.Null->Eins(%i)\r\n",i);
				PORTA = PORTA |= (1<<i) ;
			}
		}
	}
}

void toggle_PC (uint8_t portc)
{
	for(int i=0;i<8;i++){
		if(OUTC & portc & (1<<i)){
		      HTTP_DEBUG("Toggle PC(%i)\r\n",i);
		      HTTP_DEBUG("PC(%i):%i\r\n",i,PINC&(1<<i));

			//Toggle PIN
		    //1.Wechsel
			if(PINC&(1<<i))	//PIN == 1
			{
				HTTP_DEBUG("1.Eins->Null(%i)\r\n",i);
			PORTC = PORTC &= ~(1<<i);
			}
			else				//PIN == 0
			{
				HTTP_DEBUG("1.Null->Eins(%i)\r\n",i);
			PORTC = PORTC |= (1<<i) ;
			}

	  		//Warten
	  		_delay_ms(300);

	  		//2.Wechsel
			if(PINC & (1<<i))	//PIN == 1
			{
				HTTP_DEBUG("2.Eins->Null(%i)\r\n",i);
				PORTC = PORTC &= ~(1<<i);
			}
			else				//PIN == 0
			{
				HTTP_DEBUG("2.Null->Eins(%i)\r\n",i);
				PORTC = PORTC |= (1<<i) ;
			}
		}
	}
}

void toggle_PD (uint8_t portd)
{
	for(int i=0;i<8;i++){
		if(OUTD & portd & (1<<i)){
		      HTTP_DEBUG("Toggle PD(%i)\r\n",i);
		      HTTP_DEBUG("PD(%i):%i\r\n",i,PIND&(1<<i));

			//Toggle PIN
		    //1.Wechsel
			if(PIND&(1<<i))	//PIN == 1
			{
				HTTP_DEBUG("1.Eins->Null(%i)\r\n",i);
			PORTD = PORTD &= ~(1<<i);
			}
			else				//PIN == 0
			{
				HTTP_DEBUG("1.Null->Eins(%i)\r\n",i);
			PORTD = PORTD |= (1<<i) ;
			}

	  		//Warten
	  		_delay_ms(300);

	  		//2.Wechsel
			if(PIND & (1<<i))	//PIN == 1
			{
				HTTP_DEBUG("2.Eins->Null(%i)\r\n",i);
				PORTD = PORTD &= ~(1<<i);
			}
			else				//PIN == 0
			{
				HTTP_DEBUG("2.Null->Eins(%i)\r\n",i);
				PORTD = PORTD |= (1<<i) ;
			}
		}
	}
}
