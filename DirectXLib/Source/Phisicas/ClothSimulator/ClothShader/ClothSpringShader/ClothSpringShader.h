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

		struct CSSInfo {
			std::vector<SimulateParam> sim_param;
			DirectX::XMINT3 thread;
			bool is_created;
			int compute_handle;
		};
		static std::vector<CSSInfo> shaderHandler;
		bool m_Is_looped;
		int m_Model_id;
	public:
		ClothSpringShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12);
		~ClothSpringShader();
		void create(
			ClothForce& world_f,
			lib::ModelData& model,
			std::vector<MassModel>& mass_model,
			std::vector<SpringData>& spring,
			std::vector<lib::ModelVertex>& pre_vert
		);
		void execution(std::vector<SpringData>& spring);
	private:
		void dataAssign(std::vector<SpringData>& spring);
	};
}