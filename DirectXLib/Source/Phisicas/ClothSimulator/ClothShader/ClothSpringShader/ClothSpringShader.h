#pragma once
#include <memory>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../DirectXLib/Source/Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
}
namespace phy {
	class ClothSpringShader {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		ID3DBlob* m_Shader;
		ID3DBlob* m_Root_blob;
		std::shared_ptr<lib::DirectX12Manager>& m_Dx12;
		ComPtr<ID3D12Device> m_Device;
		struct ModelParamater {
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
		};
		struct ShaderInfo {
			D3D12_CPU_DESCRIPTOR_HANDLE desc_handle;
			ID3D12RootSignature* root_sig;
			ID3D12PipelineState* pipeline;
			ID3D12DescriptorHeap* desc_heap;
			ID3D12DescriptorHeap* input_heap;
			ID3D12Resource* out_spring_res;
			ID3D12Resource* out_param_res;
			ID3D12Resource* in_spring_res;
			std::vector<ClothData> out_spring;
			std::vector<ClothData> in_spring;
			std::vector<ModelParamater> out_param;
			DirectX::XMINT3 thread;
			bool is_created;
			void* spring_data;
			void* param_data;
		};
		static std::vector<ShaderInfo> shaderHandler;
		//	void* data;
		bool m_Is_looped;
		int m_Model_id;
	public:
		ClothSpringShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12);
		~ClothSpringShader();
		void create(lib::ModelData& model, std::vector<SpringData>& spring, std::vector<std::vector<int>>& mass_spring_id);
		void execution(int steps);
		void dataChange(int model_id, lib::ModelData& model, std::vector<SpringData>& spring);
		static ModelParamater getMaxMinPos(int id);
	private:
		void createOutput(
			lib::ModelData& model,
			std::vector<std::vector<int>>& mass_spring_id
		);
		void createOutParam();
		bool createInput();
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
		bool createInputResource();
		bool createInputUAV();
		bool inputMap();
		bool createParamOutResource();
		bool createParamOutUAV();
		bool outParamMap();
		void dataAssign();
	};
}