#include "MassSpringModel.h"
#include "../../../VectorMath/VectorMath.h"

namespace phy {
	MassSpringModel::MassSpringModel(
		MODEL_FILE model_type, 
		std::vector<lib::ModelParam>& vertex,
		std::vector<UINT>& index,
		std::vector<std::vector<int>>& index_group
	) :
		m_File_type(model_type),
		m_Vertex(vertex),
		m_Index(index),
		m_Index_group(index_group)
	{
		m_Result.resize(8);
	}
	std::vector<int> MassSpringModel::create(int num) {
		for (int ite = 0; ite < 8; ite++)m_Result[ite] = -1;
		createMatrix3x3(num);
		createOutOfP4(num);
		return m_Result;
	}
	//3x3行列の質点モデルを生成
	void MassSpringModel::createMatrix3x3(int vertex_id) {
		//対応する全インデックスを取得
		auto all_index = loadAllIndex(vertex_id);
		//自身以外の周囲(3x3)のインデックスを取得
		auto related_index = loadRelatedIndex(vertex_id, all_index);
		//周囲(3x3)のインデックス数に応じてタイプを分ける
		m_Edge_type = pointType(all_index, related_index);
		//中心から十字方向のインデックスと、4つ角のインデックスを生成
		std::vector<int> nearest_index;
		std::vector<int> corners_index;
		if (m_Edge_type == EDGE_TYPE::CORNER) {//角の場合(推定6個のインデックス)
			nearest_index = loadNearestIndex(2, vertex_id, related_index);//2個の最寄質点を取得
			corners_index = loadCorners(1, related_index, nearest_index);
		}
		if (m_Edge_type == EDGE_TYPE::EDGE) {//端の場合(推定12個のインデックス)
			nearest_index = loadNearestIndex(3, vertex_id, related_index);//3個の最寄質点を取得
			corners_index = loadCorners(2, related_index, nearest_index);
		}
		if (m_Edge_type == EDGE_TYPE::NONE) {//端でない場合(推定24個のインデックス)
			nearest_index = loadNearestIndex(4, vertex_id, related_index);//4個の最寄質点を取得
			corners_index = loadCorners(4, related_index, nearest_index);
		}
		//出力用のベクター配列に格納
		for (int ite = 0; ite < nearest_index.size(); ite++) {
			m_Result[ite] = m_Index[nearest_index[ite]];
		}
		for (int ite = 0; ite < corners_index.size(); ite++) {
			m_Result[4 + ite] = m_Index[corners_index[ite]];
		}
	}
	//3x3行列外にある4つのの質点モデルを生成
	void MassSpringModel::createOutOfP4(int vertex_id) {
		//int point_num = 0;
		//if (m_Edge_type == EDGE_TYPE::CORNER)point_num = 2;
		//if (m_Edge_type == EDGE_TYPE::EDGE)point_num = 3;
		//if (m_Edge_type == EDGE_TYPE::NONE)point_num = 4;
		//for (int ite = 0; ite < point_num; ite++) {
		//	//対応する全インデックスを取得
		//	auto all_index = loadAllIndex(m_Result[ite]);
		//	//自身以外の周囲(3x3)のインデックスを取得
		//	auto related_index = loadRelatedIndex(m_Result[ite], all_index);
		//	//3x3のインデックスを除外
		//	auto data = exclusionMatrix3x3(vertex_id, related_index.constant);
		//	//中心から近いインデックス
		//	auto nearest_index = loadNearestIndex(data.count, vertex_id, data);
		//	if (nearest_index.size() != 0)m_Result[8 + ite] = m_Index[nearest_index[0]];
		//}
	}
	//端かどうかを調べる
	MassSpringModel::EDGE_TYPE
		MassSpringModel::pointType(IndexData& all_index, IndexData& related_index) {
		if (related_index.count == 3)return EDGE_TYPE::CORNER;//角でした。
		if (related_index.count == 5)return EDGE_TYPE::EDGE;//辺だよ。
		if (related_index.count == 8)return EDGE_TYPE::NONE;//結局、端じゃない
		if (m_File_type == MODEL_FILE::FBX) {
			if (related_index.count == 3)return EDGE_TYPE::CORNER;//角でした。
			if (related_index.count == 5)return EDGE_TYPE::EDGE;//辺だよ。
			if (related_index.count == 8)return EDGE_TYPE::NONE;//結局、端じゃない
		}
		if (m_File_type == MODEL_FILE::PMX) {
			if (related_index.count == 3)return EDGE_TYPE::CORNER;//角でした。
			if (related_index.count == 5)return EDGE_TYPE::EDGE;//辺だよ。
			if (related_index.count == 8)return EDGE_TYPE::NONE;//結局、端じゃない
		}
		assert(0 && "布シミュレーターに対応出来ないモデルを使用しています。ごめんね");
		return EDGE_TYPE::INDEX_ERROR;
	}
	//調べている頂点番号と一致するインデックス番号を全て取得
	MassSpringModel::IndexData
		MassSpringModel::loadAllIndex(int vertex_id) {
		IndexData data;
		data.reset(12);
		//仮想頂点用のインデックスを初期化(-1を代入)
		for (int ite = 0; ite < 6; ite++) {
			data.constant[ite] = -1;
		}
		//調べている頂点番号と一致するインデックス番号を確保
		for (int num = 0; num < m_Index_group[vertex_id].size(); num++) {
			data.constant[data.count] = m_Index_group[vertex_id][num];
			data.count++;
		}
		return data;
	}
	//関連のある頂点番号を全て取得
	MassSpringModel::IndexData
		MassSpringModel::loadRelatedIndex(int vertex_id, IndexData& all_index) {
		IndexData data;
		data.reset(48);
		if (m_File_type == MODEL_FILE::FBX) {
			//取得したインデックスに対応するポリゴンの全てのインデックスを取得
			for (int ite = 0; ite < all_index.count; ite++) {
				if (all_index.constant[ite] != -1) {
					int id = all_index.constant[ite] % 6;
					for (int ite2 = 0; ite2 < 6; ite2++) {
						auto id2 = all_index.constant[ite] - id + ite2;
						//同一のインデックスを除外
						bool is_input = true;
						for (int ite3 = 0; ite3 < data.count; ite3++) {
							if (m_Index[id2] == m_Index[data.constant[ite3]]) {
								is_input = false;
								break;
							}
						}
						if (is_input)for (int ite3 = 0; ite3 < all_index.count; ite3++) {
							if (m_Index[id2] == m_Index[all_index.constant[ite3]]) {
								is_input = false;
								break;
							}
						}
						if (is_input) {
							if (m_Index[id2] == vertex_id)is_input = false;
						}
						if (!is_input)continue;
						data.constant[data.count] = id2;
						data.count++;
					}
				}
			}
			return data;
		}
		if (m_File_type == MODEL_FILE::PMX) {
			//頂点が同一のインデックスに対応する三角形のインデックス番号をすべて取得する
			for (int ite = 0; ite < all_index.count; ite++) {

			}
		}
	}
	//十字方向の質点を取得
	std::vector<int>
		MassSpringModel::loadNearestIndex(int num, int target_index, IndexData& related_index) {
		IndexData result;
		result.reset(related_index.count);
		std::vector<float> distance(related_index.count);
		//頂点番号同士の座標が近い順で並び変える
		for (int ite = 0; ite < related_index.count; ite++) {
			auto dist = lib::VectorMath::distance(
				m_Vertex[target_index].position, m_Vertex[m_Index[related_index.constant[ite]]].position
			);
			//頂点が近い順に並び替え
			for (int ite2 = 0; ite2 < related_index.count; ite2++) {
				if (distance[ite2] == 0 || distance[ite2] > dist) {
					for (int ite3 = 0; ite3 < num; ite3++) {
						auto cid = num - ite3 - 2;
						if (cid < 0)continue;
						result.constant[cid + 1] = result.constant[cid];
					}
					result.constant[ite2] = related_index.constant[ite];
					break;
				}
			}
		}
		result.constant.resize(num);
		return result.constant;
	}
	//調べた十字方向の質点以外を取得
	std::vector<int>
		MassSpringModel::loadCorners(int num, IndexData& related_index, std::vector<int>& four_point) {
		IndexData data;
		data.reset(36);
		//取得した周囲のインデックスデータを調べる
		for (int ite = 0; ite < related_index.count; ite++) {
			bool is_input = true;
			//十字方向の質点を対象から除外する
			for (int ite2 = 0; ite2 < four_point.size(); ite2++) {
				if (related_index.constant[ite] == four_point[ite2]) {
					is_input = false;
					break;
				}
			}
			if (is_input) {
				data.constant[data.count] = related_index.constant[ite];
				data.count++;
			}
		}
		data.constant.resize(num);
		return data.constant;
	}
	MassSpringModel::IndexData 
		MassSpringModel::exclusionMatrix3x3(int vertex_id, std::vector<int>& out_of_matrix_index) {
		auto data = out_of_matrix_index;
		for (int ite = 0; ite < data.size(); ite++) {
			for (int ite2 = 0; ite2 < m_Result.size(); ite2++) {
				if (data[ite] == -1)continue;
				auto id = m_Index[data[ite]];
				if (id == m_Result[ite2] || id == vertex_id) {
					if (m_Result[ite2] != -1)data[ite] = -1;
				}
			}
		}
		IndexData result;
		result.reset(data.size());
		for (int ite = 0; ite < data.size(); ite++) {
			if (data[ite] != -1) {
				result.constant[result.count] = data[ite];
				result.count++;
			}
		}
		result.constant.resize(result.count);
		return result;
	}
	MassSpringModel::~MassSpringModel() {

	}
}