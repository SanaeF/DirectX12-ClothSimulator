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
		struct BufferData {
			DirectX::XMFLOAT3 vert_pos;
			DirectX::XMFLOAT3 color;
			int pre_id0;
			int pre_id1;
			int pre_id2;
			int pre_id3;
			int pre_id4;
			int pre_id5;
			int pre_id6;
			int pre_id7;
		};
		std::vector<BufferData> m_Input;
		BufferData* m_pInput;
		ID3DBlob* m_Shader;
		ID3D12RootSignature* m_RootSig;
		ID3D12PipelineState* m_Pipeline;
		ID3D12DescriptorHeap* m_Heap;
		ID3D12Resource* m_Input_buffer;
		ID3D12Resource* m_Output_buffer;
		ID3D12Resource* m_Resource;
		std::vector<SpringData> m_Output_spring;
		SpringData* m_pOutput_spring;
		void* data;
		bool m_Is_looped;
	public:
		ClothSpringShader(std::vector<SpringData> spring, ComPtr<ID3D12Device> device);
		void dataInput(lib::ModelData& model, std::vector<SpringData> spring, ComPtr<ID3D12Device> device);
		void createCompute(ComPtr<ID3D12Device> device);
		void execution(std::shared_ptr<lib::DirectX12Manager>& dx_12);
		std::vector<SpringData> getSpring();
	private:
		bool createBuffer(ComPtr<ID3D12Device> device);
		bool createRootSig(ComPtr<ID3D12Device> device);
		bool createPipeline(ComPtr<ID3D12Device> device);
		bool createHeap(ComPtr<ID3D12Device> device);
		bool createResource(ComPtr<ID3D12Device> device);
		bool createUVA(ComPtr<ID3D12Device> device);
		bool resourceMap();
		void output();
	};
}