#include "aduc812.h"
#include "queue.h"
#include "interrupt.h"
#include "led.h"
#include "keyboard.h"
#include "uart.h"
#include "sound.h"

static queue * _readBuffer;
static char * _delays;
static char _savedKeyChar;
static char tempForExpression[3];
static char expressionByteNumber = 0;

static xdata queue interruptWriteBuffer;

static char transmitting=0;
static char expressionReceiving=1;

void verifyAndSave(void)
{	
	if(tempForExpression[0]=='*'
		||tempForExpression[0]=='#'
		||tempForExpression[1]=='*'
		||tempForExpression[2]=='*'){
		expressionReceiving = 0;
		expressionByteNumber = 0;	
		return;
		
	}
	if(tempForExpression[1]=='#'){
		enqueue(_readBuffer,tempForExpression[0]);
		enqueue(_readBuffer,tempForExpression[1]);
		expressionByteNumber = 0;
		return;
	}
	if(tempForExpression[2]=='#'){
		enqueue(_readBuffer,tempForExpression[0]);
		enqueue(_readBuffer,tempForExpression[1]);
		enqueue(_readBuffer,tempForExpression[2]);
		expressionByteNumber = 0;
		return;
	}
	expressionByteNumber = 0;
	
}

void DelayExpired(void) __interrupt (1){
	*_delays = 0;
	
	if( kb_read_button_code() == _savedKeyChar){
		//leds(0xff);
		if(_savedKeyChar=='*'){
			enqueue(&interruptWriteBuffer,'\n');
			makeASound();
			beginTranslation();
			expressionByteNumber=0;
			expressionReceiving = 1;
			return;
		}
		enqueue(&interruptWriteBuffer,_savedKeyChar);
		beginTranslation();
		makeASound();
		if(_savedKeyChar == '#' || expressionByteNumber==2){			
			tempForExpression[expressionByteNumber]=_savedKeyChar;
			expressionReceiving = 0;
			if(_savedKeyChar != '#'){
				enqueue(&interruptWriteBuffer,'\n');
				makeAnErrorSound();
			}
			beginTranslation();
			verifyAndSave();
		}
		else{
			tempForExpression[expressionByteNumber]=_savedKeyChar;
			expressionByteNumber++;
		}
		
	}
		
}

void TimerInit(char * delays, queue * readBuffer){
	_readBuffer=readBuffer;
	_delays = delays;
	TMOD |= 0x01;
	TCON |= 0x10;
}

void SetDelayTimer(int value, char savedKeyChar){
	_savedKeyChar=savedKeyChar;
	SetVector(0x200B, (void*)DelayExpired);
	SetInterruptBuffer(&interruptWriteBuffer);
	TH0 = (value>>4);
	TL0 = value;
	ET0 = 1;
}
	

