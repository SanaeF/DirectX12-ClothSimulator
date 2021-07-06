#include "MassSpringModel.h"
#include "../../../VectorMath/VectorMath.h"

namespace phy {
	MassSpringModel::MassSpringModel(std::vector<lib::ModelData> vertex, std::vector<UINT> index, std::vector<std::vector<int>>index_group) :
		m_Vert_num(0),
		m_Vertex(vertex),
		m_Index(index),
		m_Index_group(index_group) {
		m_Result.resize(12);
	}
	std::vector<int> MassSpringModel::create(int num) {
		for (int ite = 0; ite < 12; ite++)m_Result[ite] = -1;
		m_Vert_num = num;
		auto all_index = loadAllIndex();
		auto related_index = loadRelatedIndex(all_index);
		m_Edge_type = pointType(all_index, related_index);
		std::vector<int> nearest_index;
		if (m_Edge_type == EDGE_TYPE::CORNER) {//角の場合(推定6個のインデックス)
			nearest_index = loadNearestIndex(2, related_index);//2個の最寄質点を取得
			loadCorners(1, related_index, nearest_index);
		}
		if (m_Edge_type == EDGE_TYPE::EDGE) {//端の場合(推定12個のインデックス)
			nearest_index = loadNearestIndex(3, related_index);//3個の最寄質点を取得
			loadCorners(2, related_index, nearest_index);
		}
		if (m_Edge_type == EDGE_TYPE::NONE) {//端でない場合(推定24個のインデックス)
			nearest_index = loadNearestIndex(4, related_index);//4個の最寄質点を取得
			loadCorners(4, related_index, nearest_index);
		}
		loadAddNearest(nearest_index);
		return m_Result;
	}
	//端かどうかを調べる
	MassSpringModel::EDGE_TYPE
		MassSpringModel::pointType(IndexData all_index, IndexData related_index) {
		if (related_index.count == 3)return EDGE_TYPE::CORNER;//角でした。
		if (related_index.count == 5)return EDGE_TYPE::EDGE;//辺だよ。
		if (related_index.count == 8)return EDGE_TYPE::NONE;//結局、端じゃない
		assert("布シミュレーターに対応出来ないモデルを使用しています。ヒント:ポリゴンは四角形で生成する。");
		return EDGE_TYPE::INDEX_ERROR;
	}
	//調べている頂点番号と一致するインデックス番号を全て取得
	MassSpringModel::IndexData
		MassSpringModel::loadAllIndex() {
		IndexData data;
		data.reset(12);
		//仮想頂点用のインデックスを初期化(-1を代入)
		for (int ite = 0; ite < 6; ite++) {
			data.constant[ite] = -1;
		}
		//調べている頂点番号と一致するインデックス番号を確保
		for (int num = 0; num < m_Index_group[m_Vert_num].size(); num++) {
			data.constant[data.count] = m_Index_group[m_Vert_num][num];
			data.count++;
		}
		return data;
	}
	//関連のある頂点番号を全て取得
	MassSpringModel::IndexData
		MassSpringModel::loadRelatedIndex(IndexData all_index) {
		IndexData data;
		data.reset(48);
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
						if (id2 == all_index.constant[ite3]) {
							is_input = false;
							break;
						}
					}
					if (!is_input)continue;
					data.constant[data.count] = id2;
					data.count++;
				}
			}
		}
		return data;
	}
	//十字方向の質点を取得
	std::vector<int>
		MassSpringModel::loadNearestIndex(int num, IndexData related_index) {
		IndexData result;
		result.reset(related_index.count);
		std::vector<float> distance(related_index.count);
		//頂点番号同士の座標が近い順で並び変える
		for (int ite = 0; ite < related_index.count; ite++) {
			auto dist = lib::VectorMath::distance(
				m_Vertex[m_Vert_num].position, m_Vertex[m_Index[related_index.constant[ite]]].position
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
		for (int ite = 0; ite < num; ite++) {
			m_Result[ite] = m_Index[result.constant[ite]];
		}
		result.constant.resize(num);
		return result.constant;
	}
	//調べた十字方向の質点以外を取得
	std::vector<int>
		MassSpringModel::loadCorners(int num, IndexData related_index, std::vector<int> four_point) {
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
		//出力用の配列に結果を代入
		std::vector<int> result(num);
		for (int ite = 0; ite < num; ite++) {
			result[ite] = data.constant[ite];
			m_Result[4 + ite] = m_Index[result[ite]];
		}
		data.constant.reserve(num);
		return data.constant;
	}
	void MassSpringModel::loadAddNearest(std::vector<int> nearest_index) {
		for (int ite = 0; ite < 4; ite++) {
			auto searched_nearest = loadNearestSearch(ite, nearest_index);
			loadAddNearestIndex(ite, searched_nearest);
		}
	}
	MassSpringModel::IndexData
		MassSpringModel::loadNearestSearch(int num, std::vector<int> nearest_index) {
		IndexData result;
		if (m_Result[num] == -1)return result;
		result.reset(12);
		//調べている頂点番号と一致するインデックス番号を確保
		for (int ite = 0; ite < m_Index_group[m_Result[num]].size(); ite++) {
			result.constant[result.count] = m_Index_group[m_Result[num]][ite];
			result.count++;
		}
		result = loadRelatedIndex(result);
		for (int ite = 0; ite < result.constant.size(); ite++) {
			if (result.constant[ite] == -1)continue;
			//3x3のインデックスIDを除外
			for (int ite2 = 0; ite2 < 8; ite2++) {
				if (m_Index[result.constant[ite]] == m_Result[ite2] || m_Index[result.constant[ite]] == m_Vert_num) {
					result.constant[ite] = -1;
					break;
				}
			}
		}
		return result;
	}
	MassSpringModel::IndexData
		MassSpringModel::loadAddNearestIndex(int num, IndexData searched_nearest) {
		IndexData result;
		if (searched_nearest.constant.size() == 0)return result;
		result.reset(searched_nearest.count);
		//頂点番号同士の座標が近い順で並び変える
		std::vector<float> distance(searched_nearest.count);
		for (int ite2 = 0; ite2 < searched_nearest.count; ite2++) {
			if (searched_nearest.constant[ite2] == -1)continue;
			auto dist = lib::VectorMath::distance(
				m_Vertex[m_Result[num]].position, m_Vertex[m_Index[searched_nearest.constant[ite2]]].position
			);
			//頂点が近い順に並び替え
			for (int ite3 = 0; ite3 < searched_nearest.count; ite3++) {
				if (distance[ite3] == 0 || distance[ite3] > dist) {
					for (int ite4 = 0; ite4 < searched_nearest.count; ite4++) {
						auto cid = searched_nearest.count - ite4 - 2;
						if (cid < 0)continue;
						result.constant[cid + 1] = result.constant[cid];
					}
					result.constant[ite3] = searched_nearest.constant[ite2];
					break;
				}
			}
			m_Result[8 + num] = m_Index[result.constant[0]];
		}
		return result;
	}
	MassSpringModel::~MassSpringModel() {

	}
}