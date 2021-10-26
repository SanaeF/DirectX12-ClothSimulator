#pragma once
#include <vector>
#include <wrl.h>
#include "../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
namespace phy {
	enum MODEL_FILE {
		FBX,PMX
	};
	//pmxモデルのみに対応した質点の生成(モデル形式によって、インデックスの関係が変わってきてしまう。)
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
		int m_Mid_point[4];//中心から十字方向の4点
		int m_Mid_points[4][4];
		int m_Cor_point[4];//3x3の十字方向以外4点
		int m_Cor_points[4][4];//3x3の十字方向以外4点
		int m_More_point[4];//3x3さらに外の十字方向4点
		int m_Polygon_point[6];//3x3さらに外の十字方向4点
		int m_Polygon_points[4][6];
		float m_Semi_high_p4;//十字方向との点の最小距離(折りたたまれたポリゴンを3x3から除外するため)
		float m_Semi_high_p4s[4];
		bool m_Is_bad_num;
		std::vector<int> m_CheckIGP;
		std::vector<std::vector<int>>m_Index_group;
		std::vector<lib::ModelVertex>m_Vertex;
		std::vector<UINT>m_Index;
		std::vector<int> m_Result;
		MODEL_FILE m_File_type;
		EDGE_TYPE m_Edge_type;
		EDGE_TYPE m_Edge_types[4];
	public:
		MassSpringModel(
			MODEL_FILE model_type,
			std::vector<lib::ModelVertex>& vertex,
			std::vector<UINT>& index,
			std::vector<std::vector<int>>& index_group
		);
		std::vector<int> create(int num);
		~MassSpringModel();
	private:
		//3x3行列の質点モデルを生成
		void createMatrix3x3(int vertex_id);

		//十字質点生成
		void createCross(int vertex_id, int* data);

		//3x3で十字方向以外の4点を取得
		void createCorner(int vertex_id, int mid_points[4][4], int* data);

		//3x3からさらに外の質点を取得
		void createMoreP(int vertex_id);

		//質点の種類(角、辺、中心)を判定する
		EDGE_TYPE pointType(IndexData& related_index);

		//調べている頂点番号と一致するインデックス番号を全て取得
		IndexData loadAllIndex(int vertex_id);

		//関連のある頂点番号を全て取得
		IndexData loadRelatedIndex(int vertex_id, IndexData& all_index);


		IndexData sortIndex(int vertex_id, int sort_max, IndexData& data);
		IndexData sortIndexUsingTextureMap(int vertex_id, int sort_max, IndexData& data);
		bool isThrowVertex(lib::ModelVertex& vert);
		IndexData changeMatrixData(int num, int* data);
		IndexData deleteNullIndex(IndexData& data);
		//重なった頂点を削除
		IndexData deleteOverIndex(IndexData& data);
		std::vector<int> checkIndexID(IndexData& data);
	};
}