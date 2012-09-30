/*
 * webpage.c
 *
 *  Created on: 06.09.2012
 *      Author: icke
 */

#include "base_webpage.h"
#include "httpd.h"

#if USE_ADC

#include "analog.h"

#endif

static unsigned char base_position = 0;

unsigned char base_create_httpd_data(unsigned char** new_page_pointer, char *var_conversion_buffer,unsigned char *eth_buffer,unsigned int *pos){

	unsigned int a=*pos;
	unsigned char str_len;


	unsigned short pin_value;
	unsigned char pin_type;

	unsigned char in;
	unsigned char out;
	unsigned char sw;
	unsigned char tog;
	unsigned char adc;


//	eth_buffer[TCP_DATA_START + a++] = 'n';


	//INPUT
	if (strncasecmp_P("IO",*new_page_pointer,2)==0)
	{
		*new_page_pointer=*new_page_pointer-1;
		//erster Aufruf -> Tabellen Header anfuegen
		if(base_position == 0){

			//write
			strcpy_P(var_conversion_buffer, PSTR("<table style=\"width: 300px;\">\r\n"));
			str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
			memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
			a = a + (str_len);


			//write
			strcpy_P(var_conversion_buffer, PSTR("<tr><td>P</td><td>Name</td><td>Value</td><td>Switch</td><td>Toggle</td></tr>\r\n"));
			str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
			memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
			a = a + (str_len);
			base_position++;
			*pos = --a;
			return 1;
		}

		//4 Ports a 8Pins
		while((base_position < 33) && (a<(MTU_SIZE-(TCP_DATA_START) - (10 + 1 +15 + MAX_NAME_LENGTH +34 + 7 + 9)))){

			unsigned short port = (base_position-1)/8;
			unsigned short pin = (base_position-1)%8;

			pin_value = 0;
			pin_type=0;

			in = 0;
			out = 0;
			sw = 0;
			tog = 0;
			adc = 0;

			switch( port ){
			case 0:	//PORTA

				in = DIG_IN_PA;
				out = DIG_OUT_PA;
				sw = DIG_SW_PA;
				tog = DIG_IMP_PA;
				adc = ADC_PA;

					pin_value = PINA;
					pin_value = (pin_value & (1<<pin));

					break;

			case 1:	//PORTB
					base_position = 16;
					break;

			case 2:	//PORTC
				in = DIG_IN_PC;
				out = DIG_OUT_PC;
				sw = DIG_SW_PC;
				tog = DIG_IMP_PC;

				if((in |out) && (PINC & (1<<pin))){
					pin_value = 1;
				}
				break;
			case 3:	//PORTD
				in = DIG_IN_PD;
				out = DIG_OUT_PD;
				sw = DIG_SW_PD;
				tog = DIG_IMP_PD;

				if((in |out) && (PIND & (1<<pin))){
					pin_value = 1;
				}
				break;
			default:
					break;
			}

			if(out & (1<<pin)){	//aktueller Pin als Ausgang markiert
				if(pin_value)
					pin_value = 0;
				else
					pin_value = 1;

				if(sw & (1<<pin)){
					pin_type = 'S';	//Schaltanweisung markieren
				}

				if(tog & (1<<pin)){
					if(pin_type){
						pin_type = 'F';	//Fehler markieren
					}
					else{
						pin_type ='T';	//Taster markieren
					}
				}

			}

			if(in & (1<<pin)){	//aktueller Pin als Eingang markiert

				if(pin_type){
					pin_type = 'F';	//Fehler markieren
				}else{
					pin_type = 'I';	//Input markieren
				}
			}

			if(adc & (1<<pin)){	//aktueller Pin als Eingang markiert

				if(pin_type){
					pin_type = 'F';	//Fehler markieren
				}else{
					pin_type = 'A';	//Input markieren
				}
			}




			//DDR - PORTA
			if(pin_type){
				strcpy_P(var_conversion_buffer, PSTR("<tr><td>P"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);

				//write portnumber
				eth_buffer[TCP_DATA_START + a++] = 0x41 + port;

				//write pinnumber		1
				eth_buffer[TCP_DATA_START + a++] = 0x30 + pin;

				//write "</td><td>N</td>"		15+MAX_NAME_LENGTH
				strcpy_P(var_conversion_buffer, PSTR("</td><td>"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);

				//write Name powered by eeprom
				uint16_t mark=0;
				int address= NAME_EEPROM_STORE+((((port>1)?port-1:port)*8)+pin)*MAX_NAME_LENGTH;
				while((mark < MAX_NAME_LENGTH) && (eth_buffer[TCP_DATA_START+(a-1)] != '\0')) {
					eth_buffer[TCP_DATA_START+a++]= eeprom_read_byte(address+mark++);
				}


				//
				strcpy_P(var_conversion_buffer, PSTR("</td>"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);


				//Value
				if(pin_type != 'A'){
					//write "<td style=\"background-color: rgb("	34
					strcpy_P(var_conversion_buffer, PSTR("<td style=\"background-color: rgb("));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					//color		7
					if(pin_value)
					{
						strcpy_P(var_conversion_buffer, PSTR("153,0,0")); // rot
					}
					else
					{
						strcpy_P(var_conversion_buffer, PSTR("0,153,0")); // gr�n
					}
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a += str_len;

					//write ");\"></td>"	9
					strcpy_P(var_conversion_buffer, PSTR(");\"></td>"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);
					}

#if USE_ADC
				//WRITE SWITCH CHECKBOX
				if(pin_type == 'A'){

					strcpy_P(var_conversion_buffer, PSTR("<td>"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);


					itoa (var_adc[pin],var_conversion_buffer,10);
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len-1);

					strcpy_P(var_conversion_buffer, PSTR("</td>"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

				}
#endif
				//SWITCH
				strcpy_P(var_conversion_buffer, PSTR("<td>"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);


				//WRITE SWITCH CHECKBOX
				if(pin_type == 'F'){
					eth_buffer[TCP_DATA_START+a++]='F';
				}


				//WRITE SWITCH CHECKBOX
				if(pin_type == 'S'){

					//write "checkbox"
					strcpy_P(var_conversion_buffer, PSTR("<input type=\"checkbox\" "));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					if(!pin_value){
						//write "checked"
						strcpy_P(var_conversion_buffer, PSTR("checked"));
						str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
						memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
						a = a + (str_len);
					}

					//write "value"
					strcpy_P(var_conversion_buffer, PSTR(" name=\"OUT\" value=\"S"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					//write portnumber
					eth_buffer[TCP_DATA_START + a++] = 0x41+port;

					//write pinnumber
					eth_buffer[TCP_DATA_START + a++] = 0x30+pin;

					//write "end tag"
					strcpy_P(var_conversion_buffer, PSTR("\">"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);
				}

				//write ");\"></td>"	9
				strcpy_P(var_conversion_buffer, PSTR("</td>"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);



				//TOGGLE
				strcpy_P(var_conversion_buffer, PSTR("<td>"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);

				//WRITE SWITCH CHECKBOX
				if(pin_type == 'T'){

					//write "checkbox"
					strcpy_P(var_conversion_buffer, PSTR("<input type=\"checkbox\" "));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					if(!pin_value){
						//write "checked"
						strcpy_P(var_conversion_buffer, PSTR("checked"));
						str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
						memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
						a = a + (str_len);
					}

					//write "value"
					strcpy_P(var_conversion_buffer, PSTR(" name=\"OUT\" value=\"T"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					//write portnumber
					eth_buffer[TCP_DATA_START + a++] = 0x41+port;

					//write pinnumber
					eth_buffer[TCP_DATA_START + a++] = 0x30+pin;

					//write "end tag"
					strcpy_P(var_conversion_buffer, PSTR("\">"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);
				}

				//write ");\"></td>"	9
				strcpy_P(var_conversion_buffer, PSTR("</td>"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);

				//write ");\"></td>"	9
				strcpy_P(var_conversion_buffer, PSTR("</tr>\r\n"));
				str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
				memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
				a = a + (str_len);



			}
			base_position++;
			*pos = --a;
			return 1;
		}



		//erster Aufruf -> Tabellen Footer anfuegen
		if(base_position >= 33){

			base_position = 0;
			//write "<table>\r\n"	9
			strcpy_P(var_conversion_buffer, PSTR("</table>\r\n"));
			str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
			memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
			a = a + (str_len);


			*new_page_pointer=*new_page_pointer + 3;
		}
		*pos = --a;
		return 1;
	}
return 0;
}
