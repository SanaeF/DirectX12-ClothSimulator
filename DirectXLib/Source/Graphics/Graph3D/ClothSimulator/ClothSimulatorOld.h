#pragma once
#include "../Vertex/Vertex.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <d3d12.h>
namespace compute { class ClothShader; };
namespace physlib { class Physics; };
namespace lib {
	class DirectX12Manager;
	class ClothSimulatorOld {
	private:
		enum SPRING_TYPE {
			STRUCTURAL,
			SHEAR,
			BENDING
		};
		struct ClothParamater {
			float damping;
		};
		ClothParamater m_Param;
		SPRING_TYPE m_Spring_type[4];
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		//std::vector<Vertex> m_Per_vertex;
		std::shared_ptr<physlib::Physics>m_Phys;
		std::vector<float> weigth;
		std::vector<DirectX::XMFLOAT3> velocity;
		std::vector<UINT>* perIndex;
		std::vector<int> m_EquivalentIndex;
		std::shared_ptr<compute::ClothShader> m_Shader;
	public:
		ClothSimulatorOld(
			ComPtr<ID3D12Device> device, 
			std::vector<Vertex> vertex, 
			std::vector<UINT> index
		);
		~ClothSimulatorOld();
		void inputParamater();
		void calculate(
			int step, 
			std::shared_ptr<DirectX12Manager> dx12, 
			std::vector<Vertex>& vertex, 
			std::vector<Vertex> pre_vertex,
			std::vector<Vertex>* vir_vertex,
			std::vector<UINT>& index,
			std::vector<int>* pre_index
		);
	private:
		void multiCalculate(
			int th_num, int id, int step, 
			std::vector<Vertex>& vertex, 
			std::vector<Vertex> pre_vertex,
			std::vector<Vertex>* vir_vertex,
			std::vector<UINT>& index, 
			std::vector<int>* per_index
		);
		DirectX::XMFLOAT3 spring(
			int id,
			int step,
			int vertex_num,
			Vertex vert1,
			Vertex vert2,
			float length,
			float shrink,
			float stretch
		);
		DirectX::XMFLOAT3 getSpringForce(
			Vertex vert1,
			Vertex vert2,
			float length,
			float constant
		);
		std::vector<DirectX::XMFLOAT3> createVirtualPointData(
			int vertexNum,
			std::vector<Vertex> vertex,
			std::vector<UINT> index,
			std::vector<int>* per_index
		);
		bool between(float param, float deltaP1, float deltaP2);
	};
};