#pragma once
#include "../../Vertex/Vertex.h"
#include <vector>
#include <wrl.h>
namespace model {
	class VirtualPoint {
	private:
		struct IndexData {
			std::vector<int> constant;
			int count;
			void reset(int size) {
				constant.resize(size);
				count = 0;
				for (int ite = 0; ite < constant.size(); ite++) {
					constant[ite] = -1;
				}
			}
		};
		std::vector<lib::Vertex>m_Vertex;
		std::vector<UINT>m_Index;
		int m_Vert_num;
	public:
		VirtualPoint(int vert_num, std::vector<lib::Vertex> vertex, std::vector<UINT> index);
		std::vector<int> create();
	private:
		std::vector<int> createIndex6(IndexData all_index);
		//調べている頂点番号と一致するインデックス番号を全て取得
		IndexData loadAllIndex();
		//関連のある頂点番号を全て取得
		IndexData loadRelatedIndex(IndexData all_index);
		//関連のインデックスの中から、上の列を取得
		std::vector<int> loadHighIndex(IndexData related_index);
		//関連のインデックスの中から、下の列を取得
		std::vector<int> loadLowIndex(IndexData related_index);
		//関連のインデックスの中から、同じ列のインデックスを2個取得
		std::vector<int> loadWidthIndex(std::vector<int> high_index, std::vector<int> low_index, IndexData related_index);
		//調べた十字方向の質点以外を取得
		std::vector<int> loadCorners(IndexData related_index, std::vector<int> four_point);
		//調べている頂点に対して近くの頂点を探す(loadHighIndexもしくはloadLowIndexで生成したインデックス)
		int nearestIndexID(std::vector<int> indeces);
		//頂点カラーが白かどうかを取得
		bool isWhiteVertex(lib::Vertex vert);
		//IDとデータがインデックス的に同値かどうかを調べる
		bool isEquivalent(int id, IndexData data);
	};
}