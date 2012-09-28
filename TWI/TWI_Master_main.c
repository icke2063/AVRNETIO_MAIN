/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					Main.c
 Description:    			Main routine for testing
 							TWI_Master_main.c
 ********************************************************/
#include <stdio.h>
#include <avr/interrupt.h>

#include "General.h"
#include "RS232.h"
#include "Delay.h"
#include "TWI_Master.h"
/*
** This main programm demonstrates how to use the 
** implemented TWI master functions. These are:
**	TWIM_Init
**	TWIM_ReadAck
**	TWIM_ReadNack
**	TWIM_Write
**	TWIM_Stop
**
** For testing this program, use the program
** TWI_Slave_main.c in the slave uC and connect the
** two TWI lines properly (don't forget to also
** connect GND between Master and Slave!)
**
** Used uC for Master is ATMega32
*/
int main (void)
	{
	uint8_t		i;
	uint8_t		j=0;
	uint8_t		Data[8];
	uint8_t		SlaveAddress = 15;
/*
** Clear any interrupt
*/
	cli ();
/*
** Wait 1 second for POR
*/
	Delay_ms (1000);
/*
** Initiate RS232
*/
	RS232_Init ();
	printf ("Hello world...\n");
/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init (100000))
		{
		printf ("Error in initiating TWI interface\n");
		while (1);
		}
/*
** Endless loop
*/
	while (1)
		{
/*
** Read byte(s) from the slave.
** It is implicitely assumed, that the slave will send
** 8 bytes
*/
    	if (!TWIM_Start (SlaveAddress, TWIM_READ))
			{
			TWIM_Stop ();
			printf ("Could not start TWI Bus for READ\n");
	    	}
		else
			{
			for (i=0;i<7;i++)
				{
	        	Data[i] = TWIM_ReadAck ();
				printf ("Reading Byte %d: %d\n", i, Data[i]);
				}
        	Data[7] = TWIM_ReadNack ();
			printf ("Reading Byte %d: %d\n", i, Data[7]);
			TWIM_Stop ();
			Delay_ms (1000);
			}
/*
** Write byte(s) to the slave.
** It is implicitely assumed, that the slave will
** accepts 8 bytes
*/
    	if (!TWIM_Start (SlaveAddress, TWIM_WRITE))
			{
			TWIM_Stop ();
			printf ("Could not start TWI Bus for WRITE\n");
	    	}
		else
			{
			for (i=0;i<8;i++)
				{
	        	TWIM_Write (j++);
				printf ("Byte %d sent: %d\n", i, j);
				}
			TWIM_Stop ();
			Delay_ms (1000);
			}
/*
** Do something else
*/
		i++;
		}

	return 0;
	}

