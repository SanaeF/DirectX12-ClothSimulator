#pragma once
#include "ModelData/ModelData.h"
#include <memory>
#include <vector>
#include <d3dx12.h>
#include <wrl.h>
#include<DirectXMath.h>
#include<DirectXTex.h>
#include "../../Phisicas/ClothSimulator/ClothData/SpringData.h"

namespace libGraph {
	class UploadTexture;
	class Dx2DMatrix;
	class Dx2DRootSignature;
	class DxGraphicsPipeline;
}
namespace phy {	class ClothSimulator;}
namespace lib {
	class DirectX12Manager;
	class DxGraphics3D {
	public:
		static int m_Handle_count;
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		struct ModelHandleData {
			DirectX::XMMATRIX* matrix;
			ID3D12DescriptorHeap* tex_desc_heap;
			ID3D12Resource* texture_buffer;
			std::vector<DirectX::XMMATRIX*> matrix_arry;
			std::vector<ID3D12DescriptorHeap*> tex_desc_heap_arry;
			D3D12_VERTEX_BUFFER_VIEW vb_view;
			D3D12_INDEX_BUFFER_VIEW ib_view;
			ComPtr<ID3D12Resource> vb;
			ComPtr<ID3D12Resource> ib;
			std::vector<ModelData> vertex;
			std::vector<UINT> index;
			std::vector<std::vector<int>>index_group;
			std::vector<ModelData> pre_vertex;
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
		std::vector<ModelHandleData> m_Model_data;
		std::shared_ptr<DirectX12Manager> m_Dx12;
		std::shared_ptr <libGraph::Dx2DMatrix>m_Matrix;
		std::shared_ptr<libGraph::UploadTexture>m_Texture;
		std::shared_ptr<libGraph::Dx2DRootSignature> m_Root_signature;
	public:
		DxGraphics3D(std::shared_ptr<DirectX12Manager>& dx_12);
		int loadFbx(const char* path, libGraph::DxGraphicsPipeline& pipeline);
		void setupClothPhis(int Handle);
		void draw(float x, float y, float z, float size, double Angle, int Handle);
		void beginDraw(libGraph::DxGraphicsPipeline& pipeline, int Handle);
		void updateClothPhis(int Handle);
		void resetClothPhis(int Handle);
	private:
		void createMatrix(int Handle, int num);
		void drawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle);
		void drawCommand(int InstancedCount, int Handle);
	};
}