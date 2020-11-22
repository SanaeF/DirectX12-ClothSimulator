#pragma once
#include<memory>

class Dx12Wrapper;

class Dx2DGraph;

class DxGraph {
private:

	std::shared_ptr<Dx2DGraph>mDraw2D;

public:

	void SetArea(int x1, int y1, int x2, int y2);

	void Init();

	void DrawPrototype2D(float x, float y, float size, float Angle, int Handle);

	int Load2D(const wchar_t* path);

	void setMatrix();

	void BasicDraw2D();

	DxGraph(std::shared_ptr<Dx12Wrapper> DxWrap);
	~DxGraph();

};