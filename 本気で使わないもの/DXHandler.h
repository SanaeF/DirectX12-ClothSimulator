#pragma once
#include<d3d12.h>
#include<map>
#include<wrl.h>
#include<memory>

#include "DxPMDManager.h"
 
class DXHandler {
private:

	//static std::map<int, void*>::iterator it;
	//static std::map<std::string,int>::iterator it2;
	static std::map<int, void*>mHandler;

	static int next;
	

public:

	int CreateHandle(void* data,int* HandleName);

	//ハンドルを登録した番号を返す
	int CreateHandler(void* data);

	std::map<int, void*>getHandleData();

	DXHandler(){

	}

private:


	//アドレスにIDが存在するかを確認する
	bool CheckHandleID(int* pHandleName);
};