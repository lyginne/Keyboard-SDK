/****************************************************************************

    test_led.c - ���� �������� ����������������� ������
                 ��� �������� ������ SDK-1.1

    (C) test_led.c, ������ �.�.  2007 �.

��� ��������� ���������; �� ������ �������� �������������� �� �/���
�������������� �� � ������������ � ������������� ������������
��������� GNU, �������������� ������ ���������� ��; ���� ������ 2,
���� (�� ������ ������) ����� ����� ������� ������.

��� ��������� ���������������� � �������, ��� ��� ����� ��������,
�� ��� �����-���� ��������; ���� ��� ��������������� ��������
������������ �������� ��� ����������� ��� ���������� ����.  ���
��������� ��������� �������� �������� ������������� ������������
�������� GNU.

�� ������ ���� �������� ����� ������������� ������������ ��������
GNU ������ � ���� ����������; ���� ���, �������� �� ������: Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA

----------------------------------------------------------------------------
������, �����-���������, ������� �������������� ������� ���������  
e-mail: kluchev@d1.ifmo.ru

****************************************************************************/
#include "aduc812.h"
#include "led.h"
#include "max.h"
#include "queue.h"
#include "sound.h"
#include "keyboard.h"
#include "uart.h"
#include "timer.h"
volatile queue readBuffer;
volatile queue writeBuffer;


	

void main (void) {
	unsigned char first, second, third;
	unsigned char hundredsDec, dozensDec, unitsDec;
	unsigned char result;
	unsigned char firstValue, secondValue;
	
	queueInit(&writeBuffer);
	queueInit(&readBuffer);
	InitSound();

	initUart(&writeBuffer);
	KB_Init(&readBuffer);

	EA = 1;
	
	
	for(;;){
		first = dequeue(&readBuffer);
		
		if (first==0)			
			continue;
		second = dequeue(&readBuffer);
		if(second != '#'){
			third = dequeue(&readBuffer);
			if(first>='A')
				firstValue=first-'A'+0xA;
			else
				firstValue=first-'0';
			if(second>='A')
				secondValue=second-'A'+0xA;
			else
				secondValue=second-'0';
			result = (firstValue<<4)+(secondValue);
		}
		else{
			if(first>='A')
				firstValue=first-'A'+0xA;
			else
				firstValue=first-'0';
			result = firstValue;
		}
		hundredsDec = result/100;
		if (result>=100)
			dozensDec = (result%100)/10;
		else
			dozensDec = result/10;
		unitsDec = result%10;
		blockUserTranslation();
		if(hundredsDec!=0)
			enqueue(&writeBuffer, hundredsDec+'0');
		if(dozensDec!=0||hundredsDec!=0)
			enqueue(&writeBuffer, dozensDec + '0');			
		enqueue(&writeBuffer, unitsDec +'0');
		enqueue(&writeBuffer, '\n');
		beginUserTranslation();
	}
}

