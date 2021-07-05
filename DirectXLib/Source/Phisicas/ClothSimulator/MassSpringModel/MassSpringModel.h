#pragma once
#include <vector>
#include <wrl.h>
#include "../../../Graphics/Graph3D/Vertex/Vertex.h"
namespace phy {
	class MassSpringModel {
	private:
		enum EDGE_TYPE {
			CORNER,//角の場合
			EDGE,//辺の場合
			NONE,//端じゃない場合
			INDEX_ERROR
		};
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
		std::vector<std::vector<int>>m_Index_group;
		std::vector<lib::Vertex>m_Vertex;
		std::vector<UINT>m_Index;
		std::vector<int> m_Result;
		EDGE_TYPE m_Edge_type;
		int m_Vert_num;
	public:
		MassSpringModel(std::vector<lib::Vertex> vertex, std::vector<UINT> index, std::vector<std::vector<int>>index_group);
		std::vector<int> create(int num);
		~MassSpringModel();
	private:
		//端かどうかを調べる
		EDGE_TYPE pointType(IndexData all_index, IndexData related_index);
		//調べている頂点番号と一致するインデックス番号を全て取得
		IndexData loadAllIndex();
		//関連のある頂点番号を全て取得
		IndexData loadRelatedIndex(IndexData all_index);
		//十字方向の質点を取得
		std::vector<int> loadNearestIndex(int num, IndexData related_index);
		//調べた十字方向の質点以外を取得
		std::vector<int> loadCorners(int num, IndexData related_index, std::vector<int> four_point);
		void loadAddNearest(std::vector<int> nearest_index);
		IndexData loadNearestSearch(int num, std::vector<int> nearest_index);
		IndexData loadAddNearestIndex(int num, IndexData searched_nearest);
	};
}