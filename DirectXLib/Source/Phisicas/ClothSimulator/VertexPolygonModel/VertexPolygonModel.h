#pragma once
#include <vector>
#include <wrl.h>
#include "../ClothData/SpringData.h"
#include "../../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
namespace phy {
	class VertexPolygonModel {
	private:
		std::vector<PolygonModel>m_Polygon_model;
	public:
		VertexPolygonModel(lib::ModelData& model);
		//頂点ごとに生成した複数ある三角形の頂点データを取得する
		std::vector<PolygonModel> getData();
	private:
		//id番目のポリゴンモデルに複数の「三角形の頂点ID」と三角形の数を取得する
		void createPolygonID(int id, std::vector<UINT>& index, std::vector<std::vector<int>>& index_group);
		//三角形の頂点をインデックス番号int[3]で返す
		std::vector<int> createIndex(int vert_id, int group_id, std::vector<std::vector<int>>& index_group);
		//index番号を頂点IDに変換
		int getVertex(int id, std::vector<UINT>& index);
	};
}