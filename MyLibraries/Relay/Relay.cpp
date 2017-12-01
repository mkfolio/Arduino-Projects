/*
Name:	Relay.cpp
Author:	Kel
Editor:	http://www.visualmicro.com
*/

#include "Arduino.h"
#include "Relay.h"

Relay::Relay(int pin) {
	pinMode(pin, OUTPUT);
	_pin = pin;
}

void Relay::close() {
	digitalWrite(_pin, LOW);
}

void Relay::open() {
	digitalWrite(_pin, HIGH);
}






