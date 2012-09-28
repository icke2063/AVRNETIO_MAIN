#ifndef _ANALOG_CONFIG_H_
	#define _ANALOG_CONFIG_H_

// AD-Wandler benutzen?
	#define MINCHANNEL 4   //CHANNEL: 0..7 Beispiel MINCHANNEL:4 -> Pin 0-3 digitale Eingaenge OW:1-> Pin 7 reserviert >> ADC bei Pin 4,5,6
	#define	ADC_PA		0x70
	#define ADC_COUNT 8-MINCHANNEL

#endif //_ANALOG_CONFIG_H
