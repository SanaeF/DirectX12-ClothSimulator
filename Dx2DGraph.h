#pragma once
#include<wrl.h>
#include<memory>
#include<map>
#include<DirectXMath.h>
#include<d3dx12.h>

class Dx12Wrapper;

class DxTexture2D;

class DxIndex2D;
class Dx2DRootSignature;
class Dx2DPipeline;

class Dx2DGraph {
private:

	struct GraphicData {
		ID3D12DescriptorHeap* mTexDescHeap;
		ID3D12PipelineState* mPipelineState;
		ID3D12RootSignature* mRootSignature;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
		D3D12_INDEX_BUFFER_VIEW mVertexIndexView;
	};
	std::map<int, GraphicData>  mGraphData;
	std::map<int, GraphicData*> pGraphData;

	std::shared_ptr<DxTexture2D>mTexture;

	std::shared_ptr<DxIndex2D> mIndex;
	std::shared_ptr<Dx2DRootSignature> mRootSignature;
	std::shared_ptr<Dx2DPipeline> mPipeline;

	std::shared_ptr<Dx12Wrapper> mDxWrap;

	D3D12_VIEWPORT mViewport = {};
	D3D12_RECT mScissorrect = {};

	static int mHandleCount;

public:

	int Load(const wchar_t* path);

	void Draw(int Handle);

	Dx2DGraph(std::shared_ptr<Dx12Wrapper> DxWrap);
	~Dx2DGraph();

private:

	void GraphicPipeline();

};