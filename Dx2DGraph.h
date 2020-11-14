#pragma once
#include<wrl.h>
#include<memory>
#include<map>
#include<DirectXMath.h>
#include<d3dx12.h>
#include<DirectXTex.h>
class Dx12Wrapper;

class DxViewPort2D;

class DxUploadTex2D;
class Dx2DMatrix;
//class DxIndex2D;
class Dx2DRootSignature;
class Dx2DPipeline;

class Dx2DGraph {
private:

	enum {
		eTYPE_UMA,
		eTYPE_DMA
	};

	struct GraphicData {
		DirectX::XMMATRIX* mMatrix;
		ID3D12DescriptorHeap* mTexDescHeap;
		ID3D12PipelineState* mPipelineState;
		ID3D12RootSignature* mRootSignature;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
		D3D12_INDEX_BUFFER_VIEW mVertexIndexView;
		D3D12_VIEWPORT mViewPort = {};
	};

	struct DrawGraphParam {
		float x;
		float y;
		double angle;
		double size;
	};

	/*std::map<int, GraphicData>  mGraphData;
	std::map<int, GraphicData*> pGraphData;*/

	std::vector<GraphicData>  mGraphData;
	std::vector<GraphicData*> pGraphData;

	std::shared_ptr<DxUploadTex2D>mTexture;
	std::shared_ptr <Dx2DMatrix>mMatrix;
	//std::shared_ptr<DxIndex2D> mIndex;
	std::shared_ptr<Dx2DRootSignature> mRootSignature;
	std::shared_ptr<Dx2DPipeline> mPipeline;

	std::shared_ptr <DxViewPort2D> mViewPort;

	std::shared_ptr<Dx12Wrapper> mDxWrap;

	D3D12_VIEWPORT mViewport = {};
	D3D12_RECT mScissorrect = {};

	static int mHandleCount;
	int CopyType;

public:

	int Load(const wchar_t* path);

	void SetDrawArea(int top, int left, int right, int bottom);

	void Draw(float x, float y, float size, double Angle, int Handle);

	Dx2DGraph(std::shared_ptr<Dx12Wrapper> DxWrap);
	~Dx2DGraph();

private:

	void GraphicPipeline();

	void CopyTex(int num, int Handle);

	void mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle);

	void mDrawCommand(int InstancedCount, int Handle);

};