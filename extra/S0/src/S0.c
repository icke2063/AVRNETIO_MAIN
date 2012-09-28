/*----------------------------------------------------------------------------
 Copyright:      
 Author:         
 Remarks:        
 known Problems: 
 Version:        
 Description:    

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
#include "S0.h"
#if USE_S0


ISR(INT0_vect)
{
CountInt[0]++;
}

ISR(INT1_vect)
{
CountInt[1]++;
}


//----------------------------------------------------------------------------
//Initialisierung der S0 Interrupteingaengen und PortD Ausgaenge
void S0_Init (void)
{
  S0_DEBUG("S0 Initialisierung\r\n");
  
  GICR |= (1<<INT0)|(1<<INT1);   // External Interrupt 0 Enable & Interrupt 0 Enable
  
  MCUCR |= (1<<ISC01)|(1<<ISC00)|(1<<ISC11)|(1<<ISC10);   // Int0/Int1 auf steigende Flanke

  read_eeprom();
   
}
  
//----------------------------------------------------------------------------
//Auslesen der im eeprom gespeicherten Z�hlerst�nde
void read_eeprom (void)
{
  
  uint32_t hilf;
  
  
  // Zieladresse berechnen
  S0_adress=S0_EEPROM_STORE;
  S0_DEBUG("EEPROM Read adress:%i\r\n",S0_adress);

  
  // Counter aus Speicher auslesen
  eeprom_busy_wait ();
  eeprom_read_block(&hilf,(unsigned char *)S0_adress,sizeof(uint32_t));
  
  CountInt[0]=hilf;
  
  if(CountInt[0]==0xFFFFFFFF)CountInt[0]=0;
  
  // Zieladresse berechnen
  S0_adress=S0_EEPROM_STORE+sizeof(uint32_t);
  
  // Counter aus Speicher auslesen
  eeprom_busy_wait ();
  eeprom_read_block(&hilf,(unsigned char *)S0_adress,sizeof(uint32_t));
  
  CountInt[1]=hilf;
  
  if(CountInt[1]==0xFFFFFFFF)CountInt[1]=0;
  
}

void save_eeprom (void)
{
  
  // Zieladresse berechnen
  S0_adress=S0_EEPROM_STORE;
  S0_DEBUG("EEPROM write adress:%i\r\n",S0_adress);
  
  // Counter in den EEPROM schreiben
  eeprom_busy_wait ();
  eeprom_write_block((unsigned char *)CountInt[0],(unsigned char *)S0_adress,sizeof(uint32_t));
  
  // Zieladresse berechnen
  S0_adress=S0_EEPROM_STORE+sizeof(uint32_t);
  
  // Counter in den EEPROM schreiben
  eeprom_busy_wait ();
  eeprom_write_block((unsigned char *)CountInt[1],(unsigned char *)S0_adress,sizeof(uint32_t));
  
}


//----------------------------------------------------------------------------
uint8_t S0_data (uint8_t p)
{
    uint8_t sreg_tmp;
    uint32_t hilf;
    sreg_tmp = SREG;    /* Sichern */
    cli();
    
    if(p>=0 && p<=1)
    {
      usart_write ("%i\r\n",CountInt[p]);
      hilf=CountInt[p];
      SREG = sreg_tmp;    /* Wiederherstellen */
      return hilf;
    }
    
    SREG = sreg_tmp;    /* Wiederherstellen */
    
}
#endif
//----------------------------------------------------------------------------
