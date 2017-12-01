/*
Name:		Relay.h
Author:	Kel
Editor:	http://www.visualmicro.com
*/

#ifndef _Relay_h
#define _Relay_h

#include "Arduino.h"


class Relay {
public:
	Relay(int pin);
	void open();
	void close();
private:
	int _pin;
};



#endif