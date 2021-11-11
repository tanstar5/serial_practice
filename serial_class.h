#pragma once
#include <Windows.h>

/*Ref: https://aticleworld.com/serial-port-programming-using-win32-api/ ;
https://titanwolf.org/Network/Articles/Article?AID=604e75b1-64df-4c72-af89-39cb44766d68 */

class serial_class
{
public:
	HANDLE hComm;
	DWORD Baud;
	BYTE ByteSize = 8;
	char serial_buffer[64];

	const DWORD fparitySet = TRUE;
	const BYTE stopbitSet = ONESTOPBIT;
	const BYTE paritySet = 0;

	COMMTIMEOUTS timeouts;
	DWORD dwEventMask;
	DCB dcb;

	bool create_serial(const char* com_port = "\\\\.\\COM13",DWORD BaudRateSet = CBR_9600, BYTE ByteSizeSet = 8) {
		static bool is_succesful = true;
		
		dcb.BaudRate = BaudRateSet;
		dcb.fParity = fparitySet;
		dcb.StopBits = stopbitSet;
		dcb.ByteSize = ByteSizeSet;
		dcb.Parity = paritySet;
		dcb.DCBlength = sizeof(dcb);
		hComm = CreateFileA(com_port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		//Status = SetCommState(hComm, &dcb);
		static bool status_comm_state = SetCommState(hComm, &dcb);
		if (status_comm_state==false)
		{
			printf("Error to Setting DCB Structure\n");
			is_succesful &= false;
		}
		else {
			printf("DCB Structure set\n");
			is_succesful &= true;
		}
		if (hComm == INVALID_HANDLE_VALUE) {
			is_succesful &= false;
			printf("serial object not created in the specified COM port <INVALID_HANDLE_VALUE>\n");
		}			
		else{
			is_succesful &= true;
			Baud = BaudRateSet;
			ByteSize = ByteSizeSet;
			printf("serial object created\n");
			timeouts.ReadIntervalTimeout = 50;
			timeouts.ReadTotalTimeoutConstant = 50;
			timeouts.ReadTotalTimeoutMultiplier = 10;
			timeouts.WriteTotalTimeoutConstant = 50;
			timeouts.WriteTotalTimeoutMultiplier = 10;
			if (SetCommTimeouts(hComm, &timeouts) == FALSE)
			{
				printf("\nError Setting Time outs");	
				is_succesful &= false;
			}
			else {
				printf("Time outs set\n");
				is_succesful &= true;
			}
		}
		if (is_succesful == false) close_serial();
		return is_succesful;
	}

	bool write_serial() {
		static DWORD BytesWritten = 0;
		static bool is_succesful = WriteFile(hComm,serial_buffer,sizeof(serial_buffer),&BytesWritten,NULL);	
		if (is_succesful == true) { 
			printf("%d bytes writen to serial succesful\n", BytesWritten);
		};
		return is_succesful;
	}

	bool read_serial() {
		//Setting Receive Mask		
		static bool is_succesful = SetCommMask(hComm, EV_RXCHAR);
		is_succesful &= WaitCommEvent(hComm, &dwEventMask, NULL);
		
		if (is_succesful == false) {
			printf("Error reading while setting waitcommevent or setcommmask \n");
		};
		static DWORD NoBytesRead = 1;
		static char read_data;
		static char loop = 0;
		is_succesful &= true;
		//memset(serial_buffer,0,64);
		while (NoBytesRead>0) {
			is_succesful &= ReadFile(hComm, &read_data, sizeof(read_data), &NoBytesRead, NULL);
			serial_buffer[loop] = read_data;
			if (is_succesful == false) {
				printf("Error reading maybe interrupted \n");
			};
			++loop;
		}
		printf("read succesful and the message is: ");
		for (int index = 0; index < loop-1; ++index)
		{
			printf("%c", serial_buffer[index]);
		}
		printf("\n");
		return is_succesful;
	}

	

	void close_serial() {
		printf("Closing Serial handle object\n");
		CloseHandle(hComm);
	}

}s_obj;

