#pragma once
#include<memory>
#include "../Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace libGraph {
	class DxViewPort2D;
	class DxGraphicsPipeline;
}
namespace lib {
	class DirectX12Manager;
	class DxGraphics2D;
	class DxGraphics3D;
	class DxGraphics {
	private:
		std::shared_ptr<DxGraphics2D> m_Graphics2D;
		std::shared_ptr<DxGraphics3D> m_Graphics3D;
		std::shared_ptr<DirectX12Manager> m_Dx12;
		std::shared_ptr<libGraph::DxGraphicsPipeline> m_Pipeline2D;
		std::shared_ptr<libGraph::DxGraphicsPipeline> m_Pipeline3D;
		std::shared_ptr <libGraph::DxViewPort2D> m_View_port;
	public:
		void setArea(int x1, int y1, int x2, int y2);
		void init();
		int load2D(const wchar_t* path);//2D画像ロード(戻り値ハンドル,-1の場合エラー)
		void drawPrototype2D(float x, float y, float size, float Angle, int Handle);//描画
		int load3D(const wchar_t* path);
		void draw3D(float x, float y, float z, float size, float Angle);
		void setupClothSimulator(int step, ClothForce world_f, int Handle);
		void clothSimProc(int Handle);
		void clothReset(int Handle);
		void clearDraw();
		void screenFlip();
		DxGraphics(std::shared_ptr<DirectX12Manager>& dx_12);
		~DxGraphics();
	};
}