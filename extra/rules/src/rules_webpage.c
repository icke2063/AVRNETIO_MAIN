/*
 * rules_webpage.c
 *
 *  Created on: 05.09.2012
 *      Author: icke
 */

// RULES
#include "rules_webpage.h"
#include "httpd.h"
#include "rules.h"
#include "eeprom_mapping.h"

#if USE_RULES
static unsigned char rule = 0;

unsigned char rules_create_httpd_data(unsigned char** new_page_pointer, char *var_conversion_buffer,unsigned char *eth_buffer,unsigned int *pos){

	unsigned int a=*pos;
	unsigned char str_len;

//	eth_buffer[TCP_DATA_START + a++] = 'n';

			if (strncasecmp_P("RULES",*new_page_pointer,5)==0)
			{

				int count=get_rule_count();

				if(rule == 0){
					//Rule Count
					strcpy_P(var_conversion_buffer, PSTR("Rule Count:"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);


					itoa(count,var_conversion_buffer,10);
					str_len = strnlen(var_conversion_buffer,10);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);


					strcpy_P(var_conversion_buffer, PSTR("<br>"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					eth_buffer[TCP_DATA_START + a++] = '\r';
					eth_buffer[TCP_DATA_START + a++] = '\n';
				}


				RULES_STRUCTUR cur_rul;

				while( (rule<count) && (a<(MTU_SIZE-(TCP_DATA_START)-10)) ){

					//Rule
					itoa(rule,var_conversion_buffer,10);
					str_len = strnlen(var_conversion_buffer,10);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					eth_buffer[TCP_DATA_START + a++] = '[';

					itoa(RULES_EEPROM_STORE+1+(rule*sizeof(RULES_STRUCTUR)),var_conversion_buffer,10);
					str_len = strnlen(var_conversion_buffer,10);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					eth_buffer[TCP_DATA_START + a++] = ']';

					eth_buffer[TCP_DATA_START + a++] = ':';
					eth_buffer[TCP_DATA_START + a++] = ' ';

					if(eeprom_get_rule(rule,&cur_rul)){


						strcpy_P(var_conversion_buffer, PSTR("if("));
						str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
						memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
						a = a + (str_len);


						eth_buffer[TCP_DATA_START + a++] = cur_rul.elem_A;

						itoa(cur_rul.elem_A_ID,var_conversion_buffer,10);
						str_len = strnlen(var_conversion_buffer,10);
						memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
						a = a + (str_len);

						eth_buffer[TCP_DATA_START + a++] = cur_rul.actor;

						eth_buffer[TCP_DATA_START + a++] = cur_rul.elem_B;



						itoa(cur_rul.elem_B_ID,var_conversion_buffer,10);
						str_len = strnlen(var_conversion_buffer,10);
						memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
						a = a + (str_len);

						strcpy_P(var_conversion_buffer, PSTR(") then"));
						str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
						memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
						a = a + (str_len);

					}
					rule++;

					strcpy_P(var_conversion_buffer, PSTR("<br>"));
					str_len = strnlen(var_conversion_buffer,CONVERSION_BUFFER_LEN);
					memmove(&eth_buffer[TCP_DATA_START+a],var_conversion_buffer,str_len);
					a = a + (str_len);

					eth_buffer[TCP_DATA_START + a++] = '\r';
					eth_buffer[TCP_DATA_START + a++] = '\n';

					*new_page_pointer = *new_page_pointer-1;
					a--;
					*pos = a;
					return 1;


				}

				if(rule>count || count == 0)
				{
					//end of operation
					*new_page_pointer = *new_page_pointer + 5;
					a--;
					*pos = a;
					rule=0;
					return 1;

				}
			}
return 0;
}
#endif
