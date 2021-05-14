#pragma once
#include "../../ShaderManager/ShaderManager.h"

namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class Dx2DRootSignature;
	class Dx2DPipeline :public lib::ShaderManager {
	public:
		enum { Shader2D, Shader3D };
	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC mGraph_Pipeline = {};
		D3D12_RENDER_TARGET_BLEND_DESC mRTB_Desc = {};
		ID3D12PipelineState* mPipeline_State = nullptr;
		int mType;
	public:
		Dx2DPipeline(int type);
		~Dx2DPipeline();
		void CreateGraphicsPipeline(
			std::shared_ptr<lib::DirectX12Manager>& DxWrap,
			Dx2DRootSignature& mRootSig,
			D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
		);
		void LoadShader()override;
		void RootSignatureState(
			std::shared_ptr<lib::DirectX12Manager>& DxWrap,
			D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline,
			Dx2DRootSignature& mRootSig,
			D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
		);
		void ShaderState();
		void SampleMaskState();
		void RasterizerState();
		void BlendState();
		void Layout();
		void Layout2D();
		void Layout3D();
		void RenderTargets();
		void AntiAiliasing();
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