#pragma once
#include<memory>

class Dx12Wrapper;

class Dx2DGraph;

class DXGraph{
private:

	std::shared_ptr<Dx2DGraph>mDraw2D;

public:


	void Init();

	void DrawPrototype2D(int Handle);

	int Load2D(const wchar_t* path);


	DXGraph(std::shared_ptr<Dx12Wrapper> DxWrap);
	~DXGraph();
};