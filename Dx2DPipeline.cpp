#include "Dx2DPipeline.h"
#include "Dx12Wrapper.h"

#include "Dx2DRootSignature.h"

#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
{
	"POSITION",
	0,
	DXGI_FORMAT_R32G32B32_FLOAT,
	0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	0
},
{
	"TEXCOORD",
	0,
	DXGI_FORMAT_R32G32_FLOAT,
	0,
	D3D12_APPEND_ALIGNED_ELEMENT,
	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	0
},
};

void Dx2DPipeline::LoadShader() {
	LoadVertexShaderFile(L"Vertex2D.hlsl", "Vertex2D", "vs_5_0");
	LoadPixelShaderFile(L"Pixel2D.hlsl", "Pixel2D", "ps_5_0");

}

void Dx2DPipeline::ShaderState() {
	mGraph_Pipeline.pRootSignature = nullptr;

	mGraph_Pipeline.VS.pShaderBytecode = getVertShaderBlob()->GetBufferPointer();
	mGraph_Pipeline.VS.BytecodeLength = getVertShaderBlob()->GetBufferSize();
	mGraph_Pipeline.PS.pShaderBytecode = getPixelShaderBlob()->GetBufferPointer();
	mGraph_Pipeline.PS.BytecodeLength = getPixelShaderBlob()->GetBufferSize();
}

void Dx2DPipeline::SampleMaskState() {
	mGraph_Pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
}

void Dx2DPipeline::RasterizerState() {
	mGraph_Pipeline.RasterizerState.MultisampleEnable = false;
	mGraph_Pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	mGraph_Pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	mGraph_Pipeline.RasterizerState.DepthClipEnable = true;
}

void Dx2DPipeline::BlendState() {
	mGraph_Pipeline.BlendState.AlphaToCoverageEnable = true;
	mGraph_Pipeline.BlendState.IndependentBlendEnable = false;

	mRTB_Desc.BlendEnable = true;

	//アルファテスト設定
	mRTB_Desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	mRTB_Desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	mRTB_Desc.BlendOp = D3D12_BLEND_OP_ADD;
	mRTB_Desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	mRTB_Desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	mRTB_Desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

	mRTB_Desc.LogicOpEnable = false;
	mRTB_Desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	
	mGraph_Pipeline.BlendState.RenderTarget[0] = mRTB_Desc;
}

void Dx2DPipeline::Layout() {
	mGraph_Pipeline.InputLayout.pInputElementDescs = inputLayout;
	mGraph_Pipeline.InputLayout.NumElements = _countof(inputLayout);
	mGraph_Pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	mGraph_Pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}

void Dx2DPipeline::RenderTargets() {
	mGraph_Pipeline.NumRenderTargets = 1;
	mGraph_Pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
}

void Dx2DPipeline::AntiAiliasing() {
	mGraph_Pipeline.SampleDesc.Count = 1;
	mGraph_Pipeline.SampleDesc.Quality = 0;
}

void Dx2DPipeline::RootSignatureState(
	std::shared_ptr<Dx12Wrapper> DxWrap,
	Dx2DRootSignature& mRootSig,
	D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
) {
	mRootSig.CreateRootSignature(*DxWrap, RS_Desuc);
	mGraph_Pipeline.pRootSignature = mRootSig.getRootSignature();
}

void Dx2DPipeline::CreateGraphicsPipeline(std::shared_ptr<Dx12Wrapper> DxWrap) {
	auto result = DxWrap->Device()->CreateGraphicsPipelineState(
		&mGraph_Pipeline,
		IID_PPV_ARGS(&mPipeline_State)
	);
	if (!SUCCEEDED(result)) {
		assert(0 && "CreateGraphicsPipelineState Error!");
	}
}

void Dx2DPipeline::setPipelineState(ID3D12PipelineState* Pipeline_State) {
	this->mPipeline_State = Pipeline_State;
}

void Dx2DPipeline::setGraphPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline) {
	this->mGraph_Pipeline = gpipeline;
}

void Dx2DPipeline::setRTBDesc(D3D12_RENDER_TARGET_BLEND_DESC RTB_Desc) {
	this->mRTB_Desc = RTB_Desc;
}

ID3D12PipelineState* Dx2DPipeline::getPipelineState() {
	return mPipeline_State;
}