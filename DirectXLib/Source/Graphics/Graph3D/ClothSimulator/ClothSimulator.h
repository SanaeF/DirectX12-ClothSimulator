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
	class ClothSimulator {
	private:
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;
		std::vector<Vertex> perVertex;
		std::shared_ptr<physlib::Physics>mPhys;
		std::vector<DirectX::XMFLOAT3> virVertex;
		std::vector<UINT> perIndex;
		std::vector<int> mEquivalentIndex;
		std::shared_ptr<compute::ClothShader> mShader;
	public:
		ClothSimulator(ComPtr<ID3D12Device> device, std::vector<Vertex> vertex, std::vector<UINT> index);
		~ClothSimulator();
		void inputParamater();
		void calculate(int step, std::shared_ptr<DirectX12Manager> dx12, std::vector<Vertex>& vertex, std::vector<UINT>& index);
	private:
		DirectX::XMFLOAT3 middlePosition(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		void createVirtualPointData(int vertexNum, std::vector<Vertex> vertex, std::vector<UINT> index);
		void outputVertex(int vertexNum, std::vector<Vertex>& vertex);
		bool between(float param, float deltaP1, float deltaP2);
	};
};