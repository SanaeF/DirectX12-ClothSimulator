#include "ClothSimulator.h"
#include <cassert>
#include "../../DirectX12/DirectXLib/Source/DirectX12/DirectX12Manager.h"
#include "../Physics/Physics.h"
#include "../Vector3D/Vector3D.h"
#include "ClothShader/ClothShader.h"
#include <thread>

namespace lib {
	ClothSimulator::ClothSimulator(ComPtr<ID3D12Device> device, std::vector<Vertex> vertex, std::vector<UINT> index) {
		mPhys.reset(new physlib::Physics());
		mShader.reset(new compute::ClothShader(device, vertex, index));
	}
	void ClothSimulator::inputParamater() {
		mPhys->param.mass = 1.f;
		mPhys->param.gravity = 9.8f/6000;
		mPhys->param.resistance = 0.1f;
		mPhys->param.acc = 5.f;
		mPhys->param.spring.constant = 1.f;
		mPhys->param.spring.shrink = 0.5f;
		mPhys->param.spring.stretch = 0.15f;
		//mPhys->param.force3.x = 1;
		//mPhys->param.force3.y = 1;
		//mPhys->param.force3.z = 1;
	}
	void ClothSimulator::calculate(
		int step, std::shared_ptr<DirectX12Manager> dx12, std::vector<Vertex>& vertex, std::vector<UINT>& index
	) {//コイツはシェーダー
	/*	mShader->ShaderCalculater(dx12);
		vertex = mShader->getSimulatedData().vertex;
		index = mShader->getSimulatedData().index;
		return;*/


		lib::Vector3D vec;
		mEquivalentIndex.resize(vertex.size());
		perVertex = vertex;

		mPhys->param.acc++;
		int beltNum = 128;
		for (int vNum = 0; vNum < (vertex.size() - beltNum); vNum++) {
			createVirtualPointData(vNum, vertex, index);
			float Natulength = vec.distance(
				perVertex[perIndex[0]].position,
				perVertex[perIndex[3]].position
			);

			mPhys->param.gravity *= mPhys->param.mass;
			mPhys->param.wind *= mPhys->param.mass;
			mPhys->param.spring.constant *= mPhys->param.mass;
			//積分
			mPhys->param.force3.y += mPhys->param.gravity;
			//mPhys->param.force3.x += mPhys->param.wind * (sin(mPhys->param.acc) * 0.5 + 0.5);
			//mPhys->param.force3 = vec.scale(mPhys->param.force3, step * step * 0.5 / mPhys->param.mass);//いらない
			mPhys->param.resistance = 1.f - mPhys->param.resistance * step;

			DirectX::XMFLOAT3 dtX = vec.subtract(virVertex[0], perVertex[vNum].position);
			dtX = vec.add(dtX, mPhys->param.force3);
			dtX = vec.scale(dtX, mPhys->param.resistance);
			virVertex[0] = vec.add(virVertex[0], dtX);

			for (int ite = 0; ite < step; ite++) {
				auto d = vec.distance(virVertex[0], perVertex[3].position);
				float f = (d - Natulength) * mPhys->param.spring.constant;
				f >= 0 ? f *= mPhys->param.spring.shrink : f *= mPhys->param.spring.stretch;
				auto dtX = vec.subtract(perVertex[3].position, perVertex[0].position);
				dtX = vec.normalize(dtX);
				dtX = vec.scale(dtX, f);
				dtX = vec.scale(dtX, step * step * 0.5 / mPhys->param.mass);

				auto dx_p1 = vec.scale(dtX, 1.f);
				virVertex[0] = vec.add(virVertex[0], dx_p1);
				//auto dx_p2 = vec.scale(dtX, 1.f);
				//perVertex[2] = vec.add(perVertex[2], dx_p2);
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
		virVertex.resize(5);
		perIndex.resize(4);
		std::vector<int> data;
		for (int num = 0; num < index.size(); num++) {
			if (index[num] == vertexNum)data.push_back(num);
			if (data.size() > 6)break;
		}
		mEquivalentIndex[vertexNum] = data.size() + 1;
		for (int num = 0; num < data.size(); num++) {
			if (data[num] % 6 == 0) {//P3
				auto id = data[num] + 5;
				if(between(id, 0, index.size()))perIndex[0] = index[id];
				else perIndex[0] = 0;
			}
			if (data[num] % 6 == 1) {//P4
				auto id = data[num] - 1;
				if (between(id, 0, index.size()))perIndex[1] = index[id];
				else perIndex[1] = 0;
			}
			if (data[num] % 6 == 2) {//P1
				auto id = data[num] - 1;
				if (between(id, 0, index.size()))perIndex[2] = index[id];
				else perIndex[2] = 0;
			}
			if (data[num] % 6 == 3) {//P2
				auto id = data[num] - 1;
				if (between(id, 0, index.size()))perIndex[3] = index[id];
				else perIndex[3] = 0;
			}
		}
		virVertex[0] = vertex[vertexNum].position;
		if (between(perIndex[2], 0, vertex.size()))virVertex[1] = vertex[perIndex[2]].position;
		if (between(perIndex[3], 0, vertex.size()))virVertex[2] = vertex[perIndex[3]].position;
		if (between(perIndex[0], 0, vertex.size()))virVertex[3] = vertex[perIndex[0]].position;
		if (between(perIndex[1], 0, vertex.size()))virVertex[4] = vertex[perIndex[1]].position;
	}
	void ClothSimulator::outputVertex(int vertexNum, std::vector<Vertex>& vertex) {
		vertex[vertexNum].position = virVertex[0];
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