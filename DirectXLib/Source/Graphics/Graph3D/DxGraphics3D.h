#pragma once
#include <memory>
#include <vector>
#include <d3dx12.h>
#include <wrl.h>
#include<DirectXMath.h>
#include<DirectXTex.h>

namespace libGraph {
	class DxUploadTex2D;
	class Dx2DMatrix;
	class Dx2DRootSignature;
	class Dx2DPipeline;
}
namespace lib {
	class DirectX12Manager;
	class Graph3D {
	public:
		static int mHandleCount;
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		struct ModelData {
			DirectX::XMMATRIX* mMatrix;
			ID3D12DescriptorHeap* mTexDescHeap;
			ID3D12Resource* mTextureBuffer;
			std::vector<DirectX::XMMATRIX*> MatrixArry;
			std::vector<ID3D12DescriptorHeap*> TexDescHeapArry;
			D3D12_VERTEX_BUFFER_VIEW VBView;
			D3D12_INDEX_BUFFER_VIEW IBView;
			ComPtr<ID3D12Resource> VB;
			ComPtr<ID3D12Resource> IB;
			UINT IndexCount;
			UINT VertexCount;
		};
		struct DrawGraphParam {
			float x;
			float y;
			double angle;
			double size;
		};
		std::vector<ModelData> mModelData;
		std::shared_ptr<DirectX12Manager> mDx12;
		std::shared_ptr <libGraph::Dx2DMatrix>mMatrix;
		std::shared_ptr<libGraph::DxUploadTex2D>mTexture;
		std::shared_ptr<libGraph::Dx2DRootSignature> mRootSignature;
	public:
		Graph3D(std::shared_ptr<DirectX12Manager>& dx12);
		int loadFbx(libGraph::Dx2DPipeline& pipeline);
		void Draw(float x, float y, float size, double Angle, int Handle);
		void beginDraw(libGraph::Dx2DPipeline& pipeline, int Handle);
	private:
		void mCreateMatrix(int Handle, int num);
		void mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle);
		void mDrawCommand(int InstancedCount, int Handle);
	};
};