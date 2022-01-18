#pragma once
#include "ModelData/ModelData.h"
#include <memory>
#include <vector>
#include <d3dx12.h>
#include <wrl.h>
#include <string>
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
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		struct ModelHandleData {
			DirectX::XMMATRIX* matrix;
			ID3D12DescriptorHeap* tex_desc_heap;
			ID3D12Resource* texture_buffer;
			std::vector<DirectX::XMMATRIX*> matrix_arry;
			std::vector<ID3D12DescriptorHeap*> tex_desc_heap_arry;
			ModelData model;
		};
		struct DrawGraphParam {
			float x;
			float y;
			float z;
			double angle;
			double size;
		};
		static std::vector<ModelHandleData> m_Model_data;
		std::shared_ptr<DirectX12Manager> m_Dx12;
		std::shared_ptr <libGraph::Dx2DMatrix>m_Matrix;
		std::shared_ptr<libGraph::UploadTexture>m_Texture;
		std::shared_ptr<libGraph::Dx2DRootSignature> m_Root_signature;
	public:
		DxGraphics3D(std::shared_ptr<DirectX12Manager>& dx_12);
		int loadFbx(const char* path, libGraph::DxGraphicsPipeline& pipeline);
		int loadPmx(const std::wstring& file_path, libGraph::DxGraphicsPipeline& pipeline);
		void setupClothPhis(int step, ClothForce world_f, int handle);
		void worldForce(ClothForce world_f, int handle);
		void clothUseGPU(int handle, bool flag = true);
		void draw(float x, float y, float z, float size, double angle, int handle);
		void beginDraw(libGraph::DxGraphicsPipeline& pipeline, int handle);
		void updateClothPhis(int handle);
		void resetClothPhis(int handle);
	private:
		void createMatrix(int Handle, int num);
		void drawMatrix(DrawGraphParam paramater, int instancedCount, int handle);
		void drawCommand(int instancedCount, int handle);
	};
}