#pragma once
#include <memory>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../DirectXLib/Source/Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
}
namespace phy {
	class ClothCollisionShader {
	private:
		struct ColliderData {
			ClothData cloth;
			int area_id;
		};
		std::vector<int> split_area;
		struct HandleData {
			D3D12_CPU_DESCRIPTOR_HANDLE desc_handle;
			ID3D12RootSignature* root_sig;
			ID3D12PipelineState* pipeline;
			ID3D12DescriptorHeap* desc_heap;
			ID3D12DescriptorHeap* input_heap;
			ID3D12Resource* output_res;
			ID3D12Resource* input_res;
			std::vector<ColliderData> output;
			std::vector<ColliderData> input;
			bool is_created;
			void* data;
		};
		static std::vector<HandleData> m_Data_handle;
		std::shared_ptr<lib::DirectX12Manager>& m_Dx12;

		ID3DBlob* m_Shader;
		ID3DBlob* m_Root_blob;
		DirectX::XMFLOAT3 m_High_pos;//モデル空間p1
		DirectX::XMFLOAT3 m_Low_pos;//モデル空間p2
		bool m_Is_looped;
		int m_Model_id;
	public:
		ClothCollisionShader(
			int id,
			DirectX::XMFLOAT3 max, DirectX::XMFLOAT3 min,
			std::shared_ptr<lib::DirectX12Manager>& dx12
		);
		void create(
			lib::ModelData& model,
			std::vector<SpringData>& spring,
			std::vector<std::vector<int>>& mass_spring_id
		);
		void execution();
		void dataChange(int model_id, lib::ModelData& model, std::vector<SpringData>& spring);
	private:
		void createDataInOutter(
			lib::ModelData& model,
			std::vector<std::vector<int>>& mass_spring_id
		);
		bool createDataInputter();
		//シェーダーに送信するためにVectorを使わないデータ構造の初期生成
		void createMassSpringforGPU(
			lib::ModelData& model,
			std::vector<std::vector<int>>& mass_spring_id
		);
		bool loadShader();
		bool createPipeline();
		bool createHeap();
		bool createOutputResource();
		bool createUAV();
		bool outputMap();
		bool createInputUAV();
		bool createInputResource();
		bool inputMap();
		void dataAssign();
	};
}