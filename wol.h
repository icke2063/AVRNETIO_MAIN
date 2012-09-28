/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Thomas
 Remarks:        
 known Problems: none
 Version:        23.01.2008
 Description:    WOL

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
#include <config.h>

#if USE_WOL

    unsigned char wol_bcast_ip[4];
	unsigned char wol_mac[6];
    unsigned char wol_enable;

	#define WOL_DEBUG usart_write
	//#define WOL_DEBUG(...)

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include "stack.h"
	#include "usart.h"

	#define WOL_PORT				9
	
	void wol_init(void);
	void wol_request(void); 
	
#endif //USE_NTP
