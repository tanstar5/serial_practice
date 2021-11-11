#include<stdio.h>
#include "serial_classv2.h"


int main() {

	serial_classv2 s_obj1;
	char info[64];
	
	
	bool status = s_obj1.create_serial();
	//s_obj1.serial_buffer[0] = 'A';
	
	//s_obj1.write_serial();

	//s_obj1.read_serial();
	char v = 'b';
	
	for (int n = 0; n<500; n++) {
		n;
		v += 1;
		
		s_obj1.serial_buffer[0] = 1;		
		s_obj1.write_serial();
		s_obj1.read_serial();
		
		printf("loop no %d and the value %d \n", n, (unsigned char)s_obj1.data_val[0]);
		
		Sleep(300);
		
	}
	
	return 0;
}