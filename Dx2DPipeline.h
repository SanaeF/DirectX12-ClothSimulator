#pragma once
#include "ShaderManager.h"

class Dx12Wrapper;
class Dx2DRootSignature;

class Dx2DPipeline :ShaderManager {
private:

	D3D12_GRAPHICS_PIPELINE_STATE_DESC mGraph_Pipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC mRTB_Desc = {};

	ID3D12PipelineState* mPipeline_State = nullptr;


public:

	void CreateGraphicsPipeline(
		std::shared_ptr<Dx12Wrapper> DxWrap,
		Dx2DRootSignature& mRootSig,
		D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
	);

	void LoadShader()override;

	void RootSignatureState(
		std::shared_ptr<Dx12Wrapper> DxWrap,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline,
		Dx2DRootSignature& mRootSig,
		D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
	);

	void ShaderState();

	void SampleMaskState();

	void RasterizerState();

	void BlendState();

	void Layout();

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