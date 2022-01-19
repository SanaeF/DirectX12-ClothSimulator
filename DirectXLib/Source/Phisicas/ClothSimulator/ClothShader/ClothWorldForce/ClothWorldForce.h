#pragma once
#include <memory>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../DirectXLib/Source/Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
	class ComputeShader;
}
namespace phy {
	class ClothWorldForce {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::shared_ptr<lib::DirectX12Manager>& m_Dx12;
		std::shared_ptr<lib::ComputeShader> m_Shader;
		ComPtr<ID3D12Device> m_Device;
		struct PositionFrame {
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
		};
		struct CSSInfo {
			std::vector<SimulateParam>sim_param;
			std::vector<PositionFrame>frame;
			DirectX::XMINT3 thread;
			bool is_created;
			int compute_handle;
		};
		static std::vector<CSSInfo> m_ShaderHandler;
		int m_Model_id;
	public:
		ClothWorldForce(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12);
		void execution(
			int time,
			ClothForce& world_f, 
			std::vector<lib::ModelVertex>& vert, 
			std::vector<SpringData>& spring
		);
		PositionFrame getFrame(int model_id);
	private:
		void dataAssign(std::vector<lib::ModelVertex>& vert, std::vector<SpringData>& spring);
	};
}