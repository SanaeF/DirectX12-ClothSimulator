#pragma once
#include<wrl.h>
#include<memory>
#include<map>
#include<DirectXMath.h>
#include<d3dx12.h>
#include<DirectXTex.h>

namespace libGraph {
	class DxViewPort2D;
	class UploadTexture;
	class Dx2DMatrix;
	class DxGraph2DIndex;
	class Dx2DRootSignature;
	class DxGraphicsPipeline;
}
namespace lib {
	class DirectX12Manager;
	class DxGraphics2D {
	private:

		enum {
			TYPE_UMA,
			TYPE_DMA
		};
		struct GraphicData {
			DirectX::XMMATRIX* matrix;
			ID3D12DescriptorHeap* tex_desc_heap;
			ID3D12Resource* tex_buffer;
			D3D12_VERTEX_BUFFER_VIEW vb_view;
			D3D12_VIEWPORT view_port = {};
			std::vector<DirectX::XMMATRIX*> matrix_arry;
			std::vector<ID3D12DescriptorHeap*> tex_desc_heap_arry;
		};
		struct DrawGraphParam {
			float x;
			float y;
			double angle;
			double size;
		};
		std::vector<GraphicData> m_Graph_data;
		std::vector<GraphicData*> m_pGraph_data;
		std::shared_ptr<libGraph::UploadTexture> m_Texture;
		std::shared_ptr <libGraph::Dx2DMatrix> m_Matrix;
		std::shared_ptr<libGraph::DxGraph2DIndex> m_Index;
		std::shared_ptr<libGraph::Dx2DRootSignature> m_Root_signature;
		std::shared_ptr<DirectX12Manager> m_Dx12;

		D3D12_VIEWPORT m_View_port = {};
		D3D12_RECT m_Scissor_rect = {};

		static int m_Handle_count;
		int m_Copy_type;

	public:
		int load(const wchar_t* path, libGraph::DxGraphicsPipeline& pipeline);
		void setDrawArea(int top, int left, int right, int bottom);
		void draw(float x, float y, float size, double Angle, int Handle);
		void beingDraw(libGraph::DxGraphicsPipeline& pipeline);
		DxGraphics2D(std::shared_ptr<DirectX12Manager>& dx_12);
		~DxGraphics2D();
	private:
		void graphicPipeline_Prop();
		void createMatrix(int Handle, int num);
		void drawMatrix(DrawGraphParam Paramater, int InstancedCount, int Handle);
		void drawCommand(int InstancedCount, int Handle);
	};
}