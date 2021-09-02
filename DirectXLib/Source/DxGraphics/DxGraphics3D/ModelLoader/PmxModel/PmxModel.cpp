#include "PmxModel.h"
#include <fstream>
#include <array>
namespace model {
	constexpr int NO_DATA_FLAG = -1;
	bool PmxModel::loadFile(const std::wstring& file_path) {
		if (file_path.empty())return false;
		// モデルファイルのパスからモデルフォルダのパスを抽出
		//std::wstring folderPath{ file_path.begin(), file_path.begin() + file_path.rfind(L'\\') + 1 };
		// ファイルオープン
		std::ifstream pmxFile{ file_path, (std::ios::binary | std::ios::in) };
		if (pmxFile.fail()) {
			pmxFile.close();
			return false;
		}
		// ヘッダー -------------------------------
		std::array<byte, 4> pmxHeader{};
		constexpr std::array<byte, 4> PMX_MAGIC_NUMBER{ 0x50, 0x4d, 0x58, 0x20 };
		enum HeaderDataIndex{
			ENCODING_FORMAT,
			NUMBER_OF_ADD_UV,
			VERTEX_INDEX_SIZE,
			TEXTURE_INDEX_SIZE,
			MATERIAL_INDEX_SIZE,
			BONE_INDEX_SIZE,
			RIGID_BODY_INDEX_SIZE
		};

		for (int ite = 0; ite < 4; ite++){
			pmxHeader[ite] = pmxFile.get();
		}
		if (pmxHeader != PMX_MAGIC_NUMBER){
			pmxFile.close();
			return false;
		}

		// ver2.0以外は非対応
		float version{};
		pmxFile.read(reinterpret_cast<char*>(&version), 4);
		if (!DirectX::XMScalarNearEqual(version, 2.0f, DirectX::g_XMEpsilon.f[0])){
			pmxFile.close();
			return false;
		}

		byte hederDataLength = pmxFile.get();
		if (hederDataLength != 8){
			pmxFile.close();
			return false;
		}
		std::array<byte, 8> hederData{};
		for (int ite = 0; ite < hederDataLength; ite++){
			hederData[ite] = pmxFile.get();
		}
		//UTF-8は非対応
		if (hederData[0] != 0){
			pmxFile.close();
			return false;
		}

		unsigned arrayLength{};
		for (int ite = 0; ite < 4; ite++){
			pmxFile.read(reinterpret_cast<char*>(&arrayLength), 4);
			for (unsigned j = 0; j < arrayLength; j++)
			{
				pmxFile.get();
			}
		}
		auto& data = lib::ModelData::Object;
		// 頂点
		int numberOfVertex{};
		pmxFile.read(reinterpret_cast<char*>(&numberOfVertex), 4);
		data.vertex.resize(numberOfVertex);
		data.index_group.resize(numberOfVertex);
		for (int ite = 0; ite < numberOfVertex; ite++) {
			pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].position), 12);
			pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].normal), 12);
			pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].tex), 8);
			if (hederData[NUMBER_OF_ADD_UV] != 0)
			{
				for (int j = 0; j < hederData[NUMBER_OF_ADD_UV]; ++j)
				{
					pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].addUV[j]), 16);
				}
			}
			const byte weightMethod = pmxFile.get();
			switch (weightMethod)
			{
			case lib::ModelParam::Weight::BDEF1:
				data.vertex[ite].weight.type = lib::ModelParam::Weight::BDEF1;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born1), hederData[BONE_INDEX_SIZE]);
				data.vertex[ite].weight.born2 = NO_DATA_FLAG;
				data.vertex[ite].weight.born3 = NO_DATA_FLAG;
				data.vertex[ite].weight.born4 = NO_DATA_FLAG;
				data.vertex[ite].weight.weight1 = 1.0f;
				break;
			case lib::ModelParam::Weight::BDEF2:
				data.vertex[ite].weight.type = lib::ModelParam::Weight::BDEF2;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born1), hederData[BONE_INDEX_SIZE]);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born2), hederData[BONE_INDEX_SIZE]);
				data.vertex[ite].weight.born3 = NO_DATA_FLAG;
				data.vertex[ite].weight.born4 = NO_DATA_FLAG;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.weight1), 4);
				data.vertex[ite].weight.weight2 = 1.0f - data.vertex[ite].weight.weight1;
				break;
			case lib::ModelParam::Weight::BDEF4:
				data.vertex[ite].weight.type = lib::ModelParam::Weight::BDEF4;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born1), hederData[BONE_INDEX_SIZE]);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born2), hederData[BONE_INDEX_SIZE]);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born3), hederData[BONE_INDEX_SIZE]);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born4), hederData[BONE_INDEX_SIZE]);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.weight1), 4);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.weight2), 4);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.weight3), 4);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.weight4), 4);
				break;
			case lib::ModelParam::Weight::SDEF:
				data.vertex[ite].weight.type = lib::ModelParam::Weight::SDEF;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born1), hederData[BONE_INDEX_SIZE]);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.born2), hederData[BONE_INDEX_SIZE]);
				data.vertex[ite].weight.born3 = NO_DATA_FLAG;
				data.vertex[ite].weight.born4 = NO_DATA_FLAG;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.weight1), 4);
				data.vertex[ite].weight.weight2 = 1.0f - data.vertex[ite].weight.weight1;
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.c), 12);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.r0), 12);
				pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].weight.r1), 12);
				break;
			default:
				pmxFile.close();
				return false;
			}
			pmxFile.read(reinterpret_cast<char*>(&data.vertex[ite].edgeMagnif), 4);
			if (data.vertex[ite].weight.born1 == NO_DATA_FLAG){
				pmxFile.close();
				return false;
			}
		}
		// 面
		int numOfSurface{};
		pmxFile.read(reinterpret_cast<char*>(&numOfSurface), 4);
		data.index.resize(numOfSurface);
		for (int i = 0; i < numOfSurface; i++){
			pmxFile.read(reinterpret_cast<char*>(&data.index[i]), hederData[VERTEX_INDEX_SIZE]);
			data.index_group[data.index[i]].push_back(i);
			if (data.index[i] == NO_DATA_FLAG){
				pmxFile.close();
				return false;
			}
		}
	}
}