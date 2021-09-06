#include "PmxModel.h"
#include <fstream>
#include <array>
#include "../../DirectXLib/Source/VectorMath/VectorMath.h"
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
		is_same.resize(numberOfVertex);
		is_input_same.resize(numberOfVertex);
		is_standard.resize(numberOfVertex);
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
			float w;
			is_same[ite] = false;
			is_input_same[ite] = false;
			is_standard[ite] = false;
			switch (weightMethod){
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
				if (data.vertex[ite].weight.born2 == 1)fixed_index.push_back(ite);
				if (data.vertex[ite].weight.born2 == 2) {
					same_position.push_back(ite);
					is_same[ite] = true;
				}
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
				fixed_index.push_back(ite);
				same_position.push_back(ite);
				is_same[ite] = true;
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
			if (is_same[ite]) {
				setStandardIndex(ite, data);
			}
		}

		// 面
		int numOfSurface{};
		pmxFile.read(reinterpret_cast<char*>(&numOfSurface), 4);
		data.index.resize(numOfSurface);
		for (int ite = 0; ite < numOfSurface; ite++){
			pmxFile.read(reinterpret_cast<char*>(&data.index[ite]), hederData[VERTEX_INDEX_SIZE]);
			if (data.index[ite] == NO_DATA_FLAG){
				pmxFile.close();
				return false;
			}
		}
		//固定頂点
		fixedIndex(data);
		//同一頂点まとめ
		sameIndex(data);
		//インデックスをグループ化する
		groupIndex(data);
	}
	void PmxModel::setStandardIndex(int id, lib::ModelData& data) {
		for (int ite2 = 0; ite2 < same_position.size(); ite2++) {
			auto id2 = same_position[ite2];
			if (id == id2)continue;//自身と比較しているかどうか
			if (is_standard[id2])continue;//まだ格納されていない頂点基準かどうか
			auto dist = lib::VectorMath::distance(
				data.vertex[id].position,
				data.vertex[id2].position
			);
			if (dist == 0)is_standard[id] = true;//同一頂点の基準である
		}
	}
	//固定頂点のカラー割り当て
	void PmxModel::fixedIndex(lib::ModelData& data) {
		for (int ite = 0; ite < fixed_index.size(); ite++) {
			auto id = fixed_index[ite];
			data.vertex[id].color.x = 1.0;
			data.vertex[id].color.y = 0.0;
			data.vertex[id].color.z = 0.0;
		}
	}
	//同一頂点のインデックスをまとめる
	void PmxModel::sameIndex(lib::ModelData& data) {
		for (int ite = 0; ite < data.index.size(); ite++) {
			UINT id1 = data.index[ite];
			if (!is_same[id1])continue;
			//頂点が同一のインデックス番号を取得
			for (int ite2 = 0; ite2 < same_position.size(); ite2++) {
				auto id2 = same_position[ite2];
				if (id1 <= id2)continue;
				if (is_standard[id2])continue;
				auto dist = lib::VectorMath::distance(
					data.vertex[id1].position,
					data.vertex[id2].position
				);
				//インデックスに同一頂点のIDを格納
				if (dist == 0)data.index[ite] = id2;
			}
		}
	}
	//インデックスをグループ化する
	void PmxModel::groupIndex(lib::ModelData& data) {
		for (int ite = 0; ite < data.index.size(); ite++) {
			data.index_group[data.index[ite]].push_back(ite);
		}
	}
	////同一頂点を取得
	//int PmxModel::sameIndex(
	//	int num,
	//	std::vector<bool>same_position,
	//	lib::ModelData data
	//) {
	//	for (int ite = 0; ite < same_position.size(); ite++) {
	//		if (!data.same_position[ite])continue;
	//		if (ite == num)continue;
	//		auto dist = lib::VectorMath::distance(
	//			data.vertex[ite].position, 
	//			data.vertex[num].position
	//		);
	//		if (dist == 0)return ite;
	//	}
	//	return -1;
	//}
}