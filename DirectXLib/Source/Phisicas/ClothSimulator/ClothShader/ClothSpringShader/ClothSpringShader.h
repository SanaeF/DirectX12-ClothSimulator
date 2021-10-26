#pragma once
#include <memory>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../DirectXLib/Source/Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
	class ComputeShader;
}
namespace phy {
	class ClothSpringShader {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		//ID3DBlob* m_Shader;
		ID3DBlob* m_Root_blob;

		std::shared_ptr<lib::DirectX12Manager>& m_Dx12;
		std::shared_ptr<lib::ComputeShader> m_Shader;

		ComPtr<ID3D12Device> m_Device;
		struct MaxPosition {
			bool is_simulated;
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
		};
		struct CSSInfo {
			std::vector<lib::ModelVertex>out_model;
			std::vector<SpringData> out_spring;
			std::vector<MaxPosition> out_param;
			std::vector<SimulateParam> sim_param;
			DirectX::XMINT3 thread;
			bool is_created;
			int compute_handle;
		};
		static std::vector<CSSInfo> shaderHandler;
		bool m_Is_simulated;
		bool m_Is_looped;
		int m_Model_id;
	public:
		ClothSpringShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12);
		~ClothSpringShader();
		void create(int step, lib::ModelData& model, std::vector<MassModel>& mass_model, std::vector<SpringData>& spring);
		void execution(bool is_simulated, lib::ModelData& model, std::vector<SpringData>& spring);
		void dataChange(int model_id, lib::ModelData& model, std::vector<SpringData>& spring);
		static MaxPosition getMaxMinPos(int id);
		bool isSimulated();
	private:
		void createOutput(
			lib::ModelData& model,
			std::vector<SpringData>& spring,
			std::vector<std::vector<int>>& mass_spring_id
		);
		void createOutParam();
		bool createInput();
		//シェーダーに送信するためにVectorを使わないデータ構造の初期生成
		void createMassSpringforGPU(
			lib::ModelData& model,
			std::vector<SpringData>& spring,
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
		void dataAssign(bool is_simulated, lib::ModelData& model, std::vector<SpringData>& spring);
	};
}