#pragma once
#include<memory>

class Dx12Wrapper;

class Dx3DGraph {
private:

	std::shared_ptr<Dx12Wrapper> mDxWrap;

public:

	int Load();

	Dx3DGraph(std::shared_ptr<Dx12Wrapper> DxWrap);
	~Dx3DGraph();

};