/* ========================================================================== */
/*                                                                            */
/*   rules.c                                                                  */
/*   (c) 2009 icke2063                                                        */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

#include "rules.h"
#include "rules_config.h"

#if USE_RULE
uint8_t get_rule_count(void)
{
    uint8_t count;
    //aktuelle Anzahl auslesen
    eeprom_busy_wait ();	
		count = (uint8_t)eeprom_read_byte((unsigned char *)(RULES_EEPROM_STORE));
		//RULE_DEBUG("Rule count:%i\r\n\r\n",count);
		
		return count;
}

void init_rule(void)
{
    uint8_t count;
    
    RULE_DEBUG("\r\nInit Rules\r\n");
    
    //aktuelle Anzahl auslesen
		count = get_rule_count();
    
    if(count>RULES_MAX_COUNT) //fehlerhafte Anzahl berichtigen
    {
      //schreiben ins eeprom
			eeprom_busy_wait ();
			eeprom_write_byte((unsigned char *)(RULES_EEPROM_STORE),0);
			
			usart_write("Rules counter set back to 0\r\n");
    }		
}


uint8_t eeprom_add_rule (RULES_STRUCTUR* rule)
{
/*  add_rule
* - freie Adresse suchen und Daten schreiben
* - Daten verifizieren 
*   1 -> alles OK Speicherstelle mit dem Anfang erh�hen
*   0 -> ansonsten Fehlerausgabe
*   
*/  
    uint8_t count;
    uint16_t target_adress;
    
    RULE_DEBUG("\r\nAdd Rules\r\n");
    
    //aktuelle Anzahl auslesen
		count = get_rule_count();
    
    if(count<RULES_MAX_COUNT)
    {
   
      //Regel pruefen
      
      // Zieladresse berechnen
      target_adress= RULES_EEPROM_STORE+1+(count*sizeof(RULES_STRUCTUR));
      RULE_DEBUG("EEPROM write adress:%i\r\n",target_adress);

      //Addresse testen
      if(target_adress+1+sizeof(RULES_STRUCTUR) > EEPROM_SIZE)return 0;

      // Regel in den EEPROM schreiben
      eeprom_busy_wait ();
      eeprom_write_block((unsigned char *)rule,(unsigned char *)target_adress,sizeof(RULES_STRUCTUR));
      
  
      //bisher alles OK -> neuen Wert ins EEPROM schreiben
  		eeprom_busy_wait ();
  		eeprom_write_byte((unsigned char *)(RULES_EEPROM_STORE),++count);
  		
  		
  		RULE_DEBUG("Regel(%i) hinzu: If ( %c%i %c %c%i ) then %c%i = %i \r\n",count,(*rule).elem_A,(*rule).elem_A_ID,(*rule).comp,(*rule).elem_B,(*rule).elem_B_ID,(*rule).actor,(*rule).act_ID,(*rule).act_value);

    }
    else
      return 0;
    
    
		return 1;


}


uint8_t eeprom_get_rule (uint8_t pos, RULES_STRUCTUR *rule)
{
/*  get_rule
*   
*/

  uint16_t target_adress;
  uint8_t count;
  
  RULE_DEBUG("\r\nGet rule:%i\r\n",pos);
  
    //aktuelle Anzahl auslesen
		count = get_rule_count();
  
  //Innerhalb vorhandener Regeln ?
  if(pos>=0 && pos<=count)
    {
      // Zieladresse berechnen
      target_adress=RULES_EEPROM_STORE+1+(pos*sizeof(RULES_STRUCTUR));
      RULE_DEBUG("EEPROM Read adress:%i\r\n",target_adress);
    
      
      // Regel aus Speicher auslesen
      eeprom_busy_wait ();
      eeprom_read_block(rule,(unsigned char *)target_adress,sizeof(RULES_STRUCTUR));
    }
  else
    {
      RULE_DEBUG("Failure: Out of Range\r\n");
      return 0; //Fehler
    
    }
  return 1;
  
}


uint8_t eeprom_del_rule (uint8_t pos)
{
/*  del_rule
*   
*/  
  uint8_t count,i;
  uint16_t target_adress;
  RULES_STRUCTUR hilf;
  
  RULE_DEBUG("Del Rule:%i\r\n",pos);
  
  
  //aktuelle Anzahl auslesen
	count = get_rule_count();
  

  if(pos>=0 && pos<=count)
  {
    //folgende Regeln nachr�ckenen lassen
    for(i=pos;i<count;i++)
    {
      
      if(eeprom_get_rule(i+1,&hilf))
      {
          
        // Zieladresse berechnen
        target_adress=RULES_EEPROM_STORE+1+(i*sizeof(RULES_STRUCTUR));
        RULE_DEBUG("EEPROM write adress:%i\r\n",target_adress);
    
  
        // Regel in den EEPROM schreiben
        eeprom_busy_wait ();
        eeprom_write_block((unsigned char *)&hilf,(unsigned char *)target_adress,sizeof(RULES_STRUCTUR));
      }
      else
      {
        return 0;
      }
      
  
    }
  
    //bisher alles OK -> neuen Wert ins EEPROM schreiben
		eeprom_busy_wait ();
		eeprom_write_byte((unsigned char *)(RULES_EEPROM_STORE),--count);
  
    return 1;
  }
  else
  {
    RULE_DEBUG("Failure: Out of Range\r\n");
    return 0;
  }
  
return 0;
  
}


uint8_t eeprom_check_rule (uint8_t pos)
{
    
  /*  check_rule
  *   
  */
  
  uint8_t count;
  RULES_STRUCTUR cur_rul;
  uint8_t elem_A,elem_B;
  
  unsigned char elem_X;
  uint8_t out_x,value_X,i;
  
  #if USE_OW
  extern volatile int16_t ow_array[MAXSENSORS];	
  #endif
  
  RULE_DEBUG("check rule:%i\r\n",pos);
  
  //aktuelle Anzahl auslesen
	count = get_rule_count();
  

  if(pos>0 && pos<=count)
  {

    if(eeprom_get_rule(pos,&cur_rul))
    {
        
        
        for(i=0;i<2;i++)
        {
          if(i==0)
          {
            elem_X=cur_rul.elem_A;
            value_X=cur_rul.elem_A_ID;
          }
          else
          {
            elem_X=cur_rul.elem_B;
            value_X=cur_rul.elem_B_ID;
          }
        
          switch(elem_X){
            case 'A': //PortA
                      out_x=PINA&(1<<(int)value_X);
                      break;
            case 'B': //PortB
                      out_x=PINB&(1<<(int)value_X);
                      break;
            case 'C': //PortC
                      out_x=PINC&(1<<(int)value_X);
                      break;
            case 'D': //PortD
                      out_x=PIND&(1<<(int)value_X);
                      break;
            case 'S': //static value
                      out_x=value_X;
                      break;
            #if USE_OW
            case 'T': //Temperatursensor
                      out_x=(uint8_t)(ow_array[value_X]/10);
                      break;
            #endif
            case 'L': //vorhandene Logik abfragen
                      out_x=eeprom_check_rule(value_X);
                      break;
            default:;
            }
            
          if(i==0)
          {
            elem_A=out_x;
          }
          else
          {
            elem_B=out_x;
          }
          
        }
        
        RULE_DEBUG("Element A: %i\r\n",elem_A);
        RULE_DEBUG("Element B: %i\r\n",elem_B);
        RULE_DEBUG("Compare: %c\r\n",cur_rul.comp);

            
        switch(cur_rul.comp){
        case '<':if(elem_A < elem_B)return 1;else return 0;break;
        case '>':if(elem_A > elem_B)return 1;else return 0;break;
        case '=':if(elem_A == elem_B)return 1;else return 0;break;
        case '!':if(elem_A != elem_B)return 1;else return 0;break;
        case '&':if(elem_A & elem_B)return 1;else return 0;break;
        case '|':if(elem_A | elem_B)return 1;else return 0;break;
        
        default:;
        }
    }
    
  }
    else
  {
    RULE_DEBUG("Failure: Out of Range\r\n");
    return 0;
  }
    
    return 0;   
}



uint8_t eeprom_run_rule(uint8_t pos)
{
RULES_STRUCTUR cur_rul;

  if(eeprom_check_rule(pos))
  {
    if(eeprom_get_rule(pos,&cur_rul))//set actor
    {
      switch(cur_rul.actor)
      {
          case 'A': //PortA
                    if (cur_rul.act_value == 1)
                      PORTA = PORTA |= (1<<cur_rul.act_ID); 	//  setzt Bit  in PORT und setzt damit Pin auf high 
                    else if (cur_rul.act_value == 0)
                      PORTA = PORTA &= ~(1<<cur_rul.act_ID);
                    return 1;
                    break;
          case 'B': //PortB
                    if (cur_rul.act_value == 1)
                      PORTB = PORTB |= (1<<cur_rul.act_ID); 	//  setzt Bit  in PORT und setzt damit Pin auf high 
                    else if (cur_rul.act_value == 0)
                      PORTB = PORTB &= ~(1<<cur_rul.act_ID);
                    return 1;
                    break;
          case 'C': //PortC
                    if (cur_rul.act_value == 1)
                      PORTC = PORTC |= (1<<cur_rul.act_ID); 	//  setzt Bit  in PORT und setzt damit Pin auf high 
                    else if (cur_rul.act_value == 0)
                      PORTC = PORTC &= ~(1<<cur_rul.act_ID);
                    return 1;
                    break;
          case 'D': //PortD
                    if (cur_rul.act_value == 1)
                      PORTD = PORTD |= (1<<cur_rul.act_ID); 	//  setzt Bit  in PORT und setzt damit Pin auf high 
                    else if (cur_rul.act_value == 0)
                      PORTD = PORTD &= ~(1<<cur_rul.act_ID);
                    return 1;
                    break;
          default:;
      }
    }
  }
  
return 0;

}
#endif
