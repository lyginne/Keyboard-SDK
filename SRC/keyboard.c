#include "max.h"

static char symbolTable[]={'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
//� 4-� ������� ����� ������ ��������� �����
unsigned char kb_read_row(void){
	return (read_max(KB)>>4);
}

//���������� �������� �������
void kb_wtite_col(unsigned char column){
	write_max(KB, column);
}

//������� �������� �������
void kb_select_col(unsigned char columnNumber){
	char columnValue=1;
	columnValue<<=columnNumber;
	kb_wtite_col(~columnValue);
}
//��������� ����� ������� �������
//-2 - ������  �� ������
//-1 - ������ ������ ����� �������
char kb_read_row_number(){

	unsigned char read_row_result;
	
	read_row_result = kb_read_row();
	switch (read_row_result){
		case 0x0E:
			return 0;
		case 0x0D:
			return 1;
		case 0x0B:
			return 2;
		case 0x07:
			return 3;
		case 0x0F:
			return -2;
	}
	return -1;
}

//��������� ��� ������� �������
//-1 - ������ �� ������ ��� ������
char kb_read_button_code(void){
	char pressed = 0;
	char btnNumber=-1;
	char rowNumber=-2;
	char i;
	char tempi=0;
	for(i=0;i<4;i++){
		kb_select_col(i);
		rowNumber=kb_read_row_number();
		if(rowNumber==-2)
			continue;
		if((rowNumber==-1)||(btnNumber!=-1)){			
			return -1;
		}
		btnNumber=(rowNumber<<2)|i;
		tempi=i;
		
	}
	if(btnNumber==-1)
		return -2;
	return symbolTable[btnNumber];
}
	
