#include "ClothSimulator.h"
#include <cassert>
#include "../Physics/Physics.h"
#include "../Vector3D/Vector3D.h"
#include <thread>

namespace lib {
	ClothSimulator::ClothSimulator() {
		mPhys.reset(new physlib::Physics());
	}
	void ClothSimulator::inputParamater() {
		mPhys->param.mass = 1.f;
		mPhys->param.gravity = 9.8f;
		mPhys->param.resistance = 0.1f;
		mPhys->param.acc = 5.f;
		//mPhys->param.force3.x = 1;
		//mPhys->param.force3.y = 1;
		//mPhys->param.force3.z = 1;
	}
	void ClothSimulator::calculate(
		int step, std::vector<Vertex>& vertex, std::vector<UINT>& index
	) {//コイツはシェーダー
		lib::Vector3D vec;
		mEquivalentIndex.resize(vertex.size());
		mPhys->param.acc++;
		for (int vNum = 0; vNum < vertex.size() - 128; vNum++) {
			createVirtualPointData(vNum, vertex, index);
			auto perPoint = perVertex;
			mPhys->param.gravity *= mPhys->param.mass;
			mPhys->param.wind *= mPhys->param.mass;
			mPhys->param.constantSpring *= mPhys->param.mass;
			//積分
			mPhys->param.force3.y += mPhys->param.gravity;
			mPhys->param.force3.x += mPhys->param.wind * (sin(mPhys->param.acc) * 0.5 + 0.5);
			mPhys->param.force3 = vec.scale(mPhys->param.force3, step * step * 0.5 / mPhys->param.mass);
			mPhys->param.resistance = 1.f - mPhys->param.resistance * step;

			for (int ite = 0; ite < 5; ite++) {
				DirectX::XMFLOAT3 dtX = vec.subtract(perPoint[ite], perVertex[ite]);
				perPoint[ite] = perVertex[ite];
				dtX = vec.add(dtX, mPhys->param.force3);
				dtX = vec.scale(dtX, mPhys->param.resistance);

				perVertex[ite] = vec.add(perVertex[ite], dtX);
			}
			outputVertex(vNum, vertex);
		}
	}

	DirectX::XMFLOAT3 ClothSimulator::middlePosition(
		DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2
	) {
		DirectX::XMFLOAT3 result;
		result.x = (p1.x + p2.x) / 2;
		result.y = (p1.y + p2.y) / 2;
		result.z = (p1.z + p2.z) / 2;
		return result;
	}
	void ClothSimulator::createVirtualPointData(
		int vertexNum, std::vector<Vertex> vertex, std::vector<UINT> index
	) {
		perVertex.resize(5);
		perIndex.resize(4);
		std::vector<int> data;
		for (int num = 0; num < index.size(); num++) {
			if (index[num] == vertexNum)data.push_back(num);
			if (data.size() > 6)break;
		}
		mEquivalentIndex[vertexNum] = data.size() + 1;
		for (int num = 0; num < data.size(); num++) {
			if (data[num] % 6 == 0)perIndex[0] = data[num] + 5;//P3
			if (data[num] % 6 == 1)perIndex[1] = data[num] - 1;//P4
			if (data[num] % 6 == 2)perIndex[2] = data[num] - 1;//P1
			if (data[num] % 6 == 3)perIndex[3] = data[num] + 2;//P2
		}
		perVertex[0] = vertex[vertexNum].position;
		if (between(perIndex[2], 0, vertex.size()))perVertex[1] = vertex[perIndex[2]].position;
		if (between(perIndex[3], 0, vertex.size()))perVertex[2] = vertex[perIndex[3]].position;
		if (between(perIndex[0], 0, vertex.size()))perVertex[3] = vertex[perIndex[0]].position;
		if (between(perIndex[1], 0, vertex.size()))perVertex[4] = vertex[perIndex[1]].position;
	}
	void ClothSimulator::outputVertex(int vertexNum, std::vector<Vertex>& vertex) {
		vertex[vertexNum].position = perVertex[0];
		//vertex[perIndex[0]].position = perVertex[3];
		//vertex[perIndex[1]].position = perVertex[4];
		//vertex[perIndex[2]].position = perVertex[1];
		//vertex[perIndex[3]].position = perVertex[2];
	}
	bool ClothSimulator::between(float param, float deltaP1, float deltaP2) {
		if (deltaP1 <= param && param < deltaP2)return true;
		return false;
	}
	ClothSimulator::~ClothSimulator() {
	}
}