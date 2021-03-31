/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

void *stdin,*stdout,*stderr;

char textstring[30];
int mm = 0; //minuter
int ss = 0;	//sekunder
int ms = 0;	//millisekunder
//int tmm = 0; 
//int tss = 0; // - || -
//int tms = 0; // - || -
int bool = 0x0; //ifall timern är på eller inte
int storedLaps[] = {0,0,0,0,0,0,0,0}; //värden på varven som har lagrats.
char showTheLaps[8][30]; //chararray med alla varv.
int cL = 0; //current LAP
int btns = 0; //värdet på knappen.
int pos = 0; //position i skrollen
int toggled = 0x0; //togglar ifall knappen har blivit nedtryckt eller inte.


volatile int *portE = (volatile int*) 0xbf886110; //initatear portE som visar vilket varv man är på.

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	TRISDSET = (0xFF << 5);
	volatile int *trisE = (volatile int *) 0xbf886100;
	*trisE &= 0xFF00; //cleara första 0-8 bits
	T2CONSET = 0x070; //Prescaler 1:256 då det är en 16-bitars timer vilket gör att vi måste scalea ner.
	TMR2 = 0x0; //sätta timern till 0.
	PR2 = 0xC35; // 80Mhz/256 / 100 = 1 ms med två decimaler.
	
	
  
}

void printTime() //uppdaterar värdet som är högst upp på skärmen.
{
	sprintf(textstring, "%02d:%02d,%02d",mm,ss,ms);
}


void display_strings() //kallar på den här när programmet ska uppdatera strängarna.
{
		display_string( 0, textstring );
		display_string(1,showTheLaps[pos]);
		display_string(2,showTheLaps[pos+1]);
		display_string(3,showTheLaps[pos+2]);
		display_update();
}


void storeLap() { //lagrar varv
		int tms = (storedLaps[cL] % 100); //temporärt när värdet ska sparas.
		int tss = ((storedLaps[cL]%10000)/100);
		int tmm = (storedLaps[cL]/10000); 
		sprintf(showTheLaps[cL],"LAP %d: %02d:%02d,%02d",cL+1,tmm,tss,tms);
}


void newLap() { //nytt varv
	if (cL < 7) {
	T2CONCLR = 0x8000;
	storedLaps[cL] = (mm*10000 + ss*100 + ms);
	storeLap();
	
	mm= 0;
	ss = 0;
	ms = 0;
	cL++;
	printTime();
	if (bool == 1){
		T2CONSET = 0x8000;
	}
	*portE = (1 << cL);
	display_strings();
	}
}

void scrollDown() { //Skrollar ner
	if(pos+3 < cL )
	{
		pos++;
		display_strings();
	}
}

void scrollUp() { //Skrollar upp ett steg
	if (pos-1 != -1)
	{
		pos--;
		display_strings();
	}
}



void clickCheck() { //alla möjliga funktioner
	
	if (btns == 4) //finns två möjliga knappalternativ här. "paus" knappen.
	{
		
		bool = (~(bool) & 0x01); //inverterar värdet då det ska vara möjligt att ha två funktioner på samma knapp.
		if (bool == 1)
		{
			T2CONSET = 0x8000;
		}
		if (bool == 0)
		{
			T2CONCLR = 0x8000;
			ms -=1; //eftersom man pausar så kommer den att ha gått en ms framåt då det är en delay.
		}
	}
	if (btns == 2 ) { //nytt varv
		int zerocheck = mm+ss+ms;
		if (zerocheck != 0){ //motverka att funktionen blir 0.
			newLap();
		}
	}
	if (btns == 1) {
		scrollDown();	
	}
	if (btns == 8) {
		scrollUp();
	}
}

void tickfunction () {
	ms += 1;
	if (ms == 100) {
		ms = 0;
		ss += 1;
	}
	if (ss == 60){
		ss = 0;
		mm += 1;
	}
	if (mm == 60){ //kommer alltid vara 60:00,00 då det blir tidtagarurens gräns.
		ms = 0;
		ss = 0;
	}
	
}

void buttonCheck (int btns){ //kollar vilken knapp som är intryckt och utför den operation som knappen har.
	if (btns == 0 && toggled == 1) //om btns värdet blir 0 medans toggled är 1 kommer toggeled att gå tillbaka till 0.
	{
		toggle();
	}
	else if (btns != 0 && toggled == 0) { 
	if (btns == 4 || btns == 2){ //delay endast för nytt varv eller paus.
	delay(96); //liten delay för att motverka bouncing.
	}
	toggle(); //flagga för att visa att knappen har blivit nedtryckt.
	clickCheck();
	}
}

void toggle() {
	toggled = ~(toggled) & 0x1; //inverterar värdet för att växla mellan 1 och 0.
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	btns = (getbtns() | (getbtn1() << 3)); //plockar ut värdet.
	buttonCheck(btns);
	if (IFS(0) & 0x100) { 
	IFSCLR(0) = 0x100;
	tickfunction();
	printTime();
	display_strings();
	}
}



