/* ========================================================================== */
/*                                                                            */
/*   rules.h                                                                  */
/*   (c) 2009 icke2063                                                        */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */


#ifndef _RULES_H_
	#define _RULES_H_	
	
	/*Speicherstruktur der Regeln
	*  
	*  	[A..D,L,S,(T)];[0..255];[<,>,=,!,~];[A..D,L,S,(T)];[0..255];[A..D,S];[0..255][0,1]
	*  
	*  
	*
	*  1.Byte:
	*  Startadresse (am Ende des EEPROMS):Anzahl der gespeicherten Regeln    	
	*/
  
  #include "usart.h"

  //#define RULE_DEBUG usart_write
	#define RULE_DEBUG(...)
  
  typedef struct
	{
    unsigned char elem_A;
    uint8_t elem_A_ID;
    unsigned char comp;
    unsigned char elem_B;
    uint8_t elem_B_ID;
    unsigned char actor;
    uint8_t act_ID;
    uint8_t act_value;
	} RULES_STRUCTUR;
	

	uint8_t get_rule_count(void);
	
	void init_rule(void);                              //Initialisierung
	uint8_t eeprom_add_rule (RULES_STRUCTUR *tmp);     //Regel hinzufuegen
	uint8_t eeprom_del_rule (uint8_t);                 //Regel entfernen
	uint8_t eeprom_get_rule (uint8_t pos, RULES_STRUCTUR *rule);      //Regel aus EEPROM auslesen
  uint8_t eeprom_check_rule(uint8_t pos);
  
  uint8_t eeprom_run_rule(uint8_t pos);
  
  
  
	//void run_event (uint8_t);
	//void check_rules (void)  //Regeln testen -> Aktion starten
	
	
#endif //_RULES_H_
