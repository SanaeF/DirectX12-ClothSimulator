#pragma once
#include<memory>

namespace libGraph {
	class DxViewPort2D;
	class Dx2DPipeline;
}
namespace lib {
	class DirectX12Manager;
	class Dx2DGraph;
	class Graph3D;
	class DxGraph {
	private:
		std::shared_ptr<Dx2DGraph>mDraw2D;
		std::shared_ptr<Graph3D>mDraw3D;
		std::shared_ptr<DirectX12Manager> mDxWrap;
		std::shared_ptr<libGraph::Dx2DPipeline> mPipeline2D;
		std::shared_ptr<libGraph::Dx2DPipeline> mPipeline3D;
		std::shared_ptr <libGraph::DxViewPort2D> mViewPort;
	public:
		void SetArea(int x1, int y1, int x2, int y2);
		void Init();
		int Load2D(const wchar_t* path);//2D画像ロード(戻り値ハンドル,-1の場合エラー)
		void DrawPrototype2D(float x, float y, float size, float Angle, int Handle);//描画
		int Load3D();
		void Draw3D(float x, float y, float size, float Angle);
		void ClearDraw();
		void ScreenFlip();
		DxGraph(std::shared_ptr<DirectX12Manager>& DxWrap);
		~DxGraph();
	};
}