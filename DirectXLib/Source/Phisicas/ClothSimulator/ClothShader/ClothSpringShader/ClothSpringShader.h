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
		std::shared_ptr<lib::DirectX12Manager>& m_Dx12;
		std::shared_ptr<lib::ComputeShader> m_Shader;
		ComPtr<ID3D12Device> m_Device;

		struct ResultParam {
			bool is_simulated;
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
		};
		struct CSSInfo {
			std::vector<ResultParam> out_param;
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
		void create(lib::ModelData& model, std::vector<MassModel>& mass_model, std::vector<SpringData>& spring);
		void execution(bool is_simulated, lib::ModelData& model, std::vector<SpringData>& spring);
		static ResultParam getResultParam(int id);
		bool isSimulated();
	private:
		void dataAssign(bool is_simulated, lib::ModelData& model, std::vector<SpringData>& spring);
	};
}