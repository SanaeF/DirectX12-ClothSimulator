#pragma once
#include "../Vertex/Vertex.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>
#include <vector>
namespace physlib {
	class Physics;
}
namespace lib {
	class ClothSimulator{
	private:
		std::shared_ptr<physlib::Physics>mPhys;
		std::vector<DirectX::XMFLOAT3> perVertex;
		std::vector<UINT> perIndex;
		std::vector<int> mEquivalentIndex;
	public:
		ClothSimulator();
		~ClothSimulator();
		void inputParamater();
		void calculate(int step, std::vector<Vertex>& vertex, std::vector<UINT>& index);
	private:
		DirectX::XMFLOAT3 middlePosition(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);
		void createVirtualPointData(int vertexNum, std::vector<Vertex> vertex, std::vector<UINT> index);
		void outputVertex(int vertexNum, std::vector<Vertex>& vertex);
		bool between(float param, float deltaP1, float deltaP2);
	};
};