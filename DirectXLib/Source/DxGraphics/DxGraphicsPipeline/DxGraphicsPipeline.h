#pragma once
#include "../../ShaderManager/ShaderManager.h"

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class Dx2DRootSignature;
	class DxGraphicsPipeline :public lib::ShaderManager {
	public:
		enum { SHADER_2D, SHADER_3D };
	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_Graph_pipeline = {};
		D3D12_RENDER_TARGET_BLEND_DESC m_RTB_desc = {};
		ID3D12PipelineState* m_Pipeline_state = nullptr;
		int m_Type;
	public:
		DxGraphicsPipeline(int type);
		~DxGraphicsPipeline();
		void createGraphicsPipeline(
			std::shared_ptr<lib::DirectX12Manager>& DxWrap,
			Dx2DRootSignature& mRootSig,
			D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
		);
		void loadShader()override;
		void rootSignatureState(
			std::shared_ptr<lib::DirectX12Manager>& DxWrap,
			D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline,
			Dx2DRootSignature& mRootSig,
			D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
		);
		void shaderState();
		void sampleMaskState();
		void rasterizerState();
		void blendState();
		void layout();
		void layout2D();
		void layout3D();
		void renderTargets();
		void antiAiliasing();
		void setPipelineState(
			ID3D12PipelineState* Pipeline_State
		);
		void setGraphPipeline(
			D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline
		);
		void setRTBDesc(
			D3D12_RENDER_TARGET_BLEND_DESC RTB_Desc
		);
		D3D12_GRAPHICS_PIPELINE_STATE_DESC getPipeline();
		ID3D12PipelineState* getPipelineState();
	};
}