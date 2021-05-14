#pragma once
#include<wrl.h>
#include<memory>
#include<map>
#include<DirectXMath.h>
#include<d3dx12.h>
#include<DirectXTex.h>

namespace libGraph {
	class DxViewPort2D;
	class DxUploadTex2D;
	class Dx2DMatrix;
	class DxIndex2D;
	class Dx2DRootSignature;
	class Dx2DPipeline;
}
namespace lib {
	class DirectX12Manager;
	class Dx2DGraph {
	private:

		enum {
			eTYPE_UMA,
			eTYPE_DMA
		};
		struct GraphicData {
			DirectX::XMMATRIX* mMatrix;
			ID3D12DescriptorHeap* mTexDescHeap;
			ID3D12Resource* mTextureBuffer;
			D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
			D3D12_VIEWPORT mViewPort = {};
			std::vector<DirectX::XMMATRIX*> MatrixArry;
			std::vector<ID3D12DescriptorHeap*> TexDescHeapArry;
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

		std::shared_ptr<libGraph::DxUploadTex2D>mTexture;
		std::shared_ptr <libGraph::Dx2DMatrix>mMatrix;
		std::shared_ptr<libGraph::DxIndex2D> mIndex;
		std::shared_ptr<libGraph::Dx2DRootSignature> mRootSignature;
		//std::shared_ptr<libGraph::Dx2DPipeline> mPipeline;

		//std::shared_ptr <libGraph::DxViewPort2D> mViewPort;

		std::shared_ptr<DirectX12Manager> mDxWrap;

		D3D12_VIEWPORT mViewport = {};
		D3D12_RECT mScissorrect = {};

		static int mHandleCount;
		int CopyType;

	public:

		int Load(const wchar_t* path, libGraph::Dx2DPipeline& pipeline);

		void SetDrawArea(int top, int left, int right, int bottom);


		void Draw(float x, float y, float size, double Angle, int Handle);

		void BeingDraw(libGraph::Dx2DPipeline& pipeline);

		Dx2DGraph(std::shared_ptr<DirectX12Manager>& dx12);
		~Dx2DGraph();

	private:

		void GraphicPipeline_Prop();

		void mCreateMatrix(int Handle, int num);

		void mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle);

		void mDrawCommand(int InstancedCount, int Handle);

	};
}