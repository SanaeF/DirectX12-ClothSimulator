#pragma once
#include <memory>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../DirectXLib/Source/Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
	class ComputeShader;
}
namespace phy {
	class ClothNewPosShader {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::shared_ptr<lib::DirectX12Manager> m_Dx12;
		std::shared_ptr<lib::ComputeShader> m_Shader;
		struct PositionFrame {
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
		};
		struct CNPSInfo {
			std::vector<SimulateParam>sim_param;
			std::vector<PositionFrame>frame;
			DirectX::XMINT3 thread;
			bool is_created;
			int compute_handle;
		};
		static std::vector<CNPSInfo> shaderHandler;
		int m_Model_id;
	public:
		ClothNewPosShader(int model_id, std::shared_ptr<lib::DirectX12Manager>& dx_12);
		~ClothNewPosShader();
		void execution(
			ClothForce& world_f,
			std::vector<lib::ModelVertex>& vert,
			std::vector<SpringData>& spring,
			std::vector<PolygonModel>& polygon_model,
			bool no_add_vel
		);
		PositionFrame getFrame(int model_id);
	private:
		void dataAssign(std::vector<lib::ModelVertex>& vert, std::vector<SpringData>& spring);
	};
}