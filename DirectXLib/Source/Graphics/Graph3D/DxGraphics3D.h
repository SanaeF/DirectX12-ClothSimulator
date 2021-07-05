#pragma once
#include "../Graph3D/Vertex/Vertex.h"
#include <memory>
#include <vector>
#include <d3dx12.h>
#include <wrl.h>
#include<DirectXMath.h>
#include<DirectXTex.h>
#include "../../Phisicas/ClothSimulator/ClothData/SpringData.h"

namespace libGraph {
	class DxUploadTex2D;
	class Dx2DMatrix;
	class Dx2DRootSignature;
	class Dx2DPipeline;
}
namespace phy {	class ClothSimulator;}
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
			std::vector<Vertex> vertex;
			std::vector<UINT> index;
			std::vector<std::vector<int>>index_group;
			std::vector<Vertex> pre_vertex;
			std::vector<std::vector<int>> pre_index;
			std::vector<SpringData>spring_data;
		};
		struct DrawGraphParam {
			float x;
			float y;
			float z;
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
		void setupClothSimulator(int Handle);
		void Draw(float x, float y, float z, float size, double Angle, int Handle);
		void beginDraw(libGraph::Dx2DPipeline& pipeline, int Handle);
		void ClothSimProc(int Handle);
		void clothReset(int Handle);
	private:
		void mCreateMatrix(int Handle, int num);
		void mDrawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle);
		void mDrawCommand(int InstancedCount, int Handle);
	};
}