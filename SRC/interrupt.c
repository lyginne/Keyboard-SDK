void SetVector(unsigned char xdata * Address, void * Vector) 
{ 
	unsigned char xdata * TmpVector; // ��������� ����������
	// ������ ������ �� ���������� ������ ������������ 
	// ��� ������� �������� ���������� ljmp, ������ 02h
	*Address = 0x02; 
	// ����� ������������ ����� �������� Vector
	TmpVector = (unsigned char xdata *) (Address + 1); 
	*TmpVector = (unsigned char) ((unsigned short)Vector >> 8); 
	++TmpVector; 
	*TmpVector = (unsigned char) Vector;
// ����� �������, �� ������ Address ������ 
// ������������� ���������� ljmp Vector
}