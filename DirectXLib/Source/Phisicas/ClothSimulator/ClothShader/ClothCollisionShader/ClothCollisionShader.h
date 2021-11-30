#pragma once
#include <memory>
#include "../../DirectXLib/Source/DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../DirectXLib/Source/Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
	class DirectX12Manager;
	class ComputeShader;
}
namespace phy {
	class ClothCollisionShader {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::shared_ptr<lib::DirectX12Manager>& m_Dx12;
		std::shared_ptr<lib::ComputeShader> m_Shader;
		ComPtr<ID3D12Device> m_Device;

		struct SpaceData {
			int space_ount;
			int spaceID[XYZ_ALL];
		};
		struct SendParam {
			int vertex_size;
			int split_num;
			DirectX::XMFLOAT3 max_pos;
			DirectX::XMFLOAT3 min_pos;
			DirectX::XMFLOAT3 zero_pos;
		};
		struct CollisionParam {
			int vertex_size;
		};

		struct ResultParam {
			bool is_simulated;
		};
		struct CSSInfo {
			std::vector<SendParam> send_param;
			std::vector<CollisionParam> collision_param;
			DirectX::XMINT3 thread;
			bool is_created;
			int compute_handle;
			int sort_handle;
		};
		static std::vector<CSSInfo> shaderHandler;

		std::vector<int> split_area;
		std::vector<SpaceData> space;

		int m_Model_id;
	public:
		ClothCollisionShader(
			int id,
			std::shared_ptr<lib::DirectX12Manager>& dx12
		);
		void create(
			DirectX::XMFLOAT3 max, DirectX::XMFLOAT3 min,
			lib::ModelData& model,
			std::vector<MassModel>& mass_model,
			std::vector<SpringData>& spring_data,
			std::vector<lib::ModelVertex>& pre_vert,
			std::vector<lib::ModelVertex>& last_vertex
		);
		void execution(lib::ModelData& model, std::vector<SpringData>& spring_data);
	private:
		void executeSortShader(bool is_input, lib::ModelData& model, DirectX::XMFLOAT3 max_pos, DirectX::XMFLOAT3 min_pos);
		void dataAssign(lib::ModelData& model, std::vector<SpringData>& spring_data);
	};
}