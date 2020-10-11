#include <iostream>
#include "DXHandler.h"

int DXHandler::getHandle(const char* FilePath, void* data) {
	/*@test*/
	const int buf = 334;
	int* mem = new int[buf];
	int Handle = 0;
	for (int memNum = 0; memNum < buf; memNum++) {
		if (memNum == Handle) {
			mem[memNum] = sizeof(&FilePath);
			return Handle;
		}
	}
	delete[] mem;
	
	/*@test_end*/
	return -1;
}

