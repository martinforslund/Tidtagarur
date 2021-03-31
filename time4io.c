#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void) {
	int sw = (PORTD >> 8) & 0x0F; //flyttar 8 bits framåt eftersom switcharna ligger mellan bits 8-11. 
	return sw;
}

int getbtns(void) {
	int btns = (PORTD >> 5) & 0x07;
	return btns;

}

int getbtn1(void){
	int btn1 = (PORTF >> 1) & 0x0001;
	return btn1;
}