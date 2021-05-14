#include "DxGraphicsPipeline.h"
#include "../../DirectX12/DirectX12Manager.h"

#include "RootSignature/DxRootSignature.h"

#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace libGraph {
	D3D12_INPUT_ELEMENT_DESC layout2D[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"SV_InstanceID",0,DXGI_FORMAT_R32_UINT,1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1},
	};
	D3D12_INPUT_ELEMENT_DESC layout3D[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "BONE_NO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		//{ "EDGE_FLG",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};
	Dx2DPipeline::Dx2DPipeline(int type) {
		mType = type;
		LoadShader();
		ShaderState();
		SampleMaskState();
		RasterizerState();
		BlendState();
		Layout();
		RenderTargets();
		AntiAiliasing();
	}
	Dx2DPipeline::~Dx2DPipeline() {

	}
	void Dx2DPipeline::LoadShader() {
		if (mType == Shader2D) {
			LoadVertexShaderFile(L"Vertex2D.hlsl", "Vertex2D", "vs_5_0");
			LoadPixelShaderFile(L"Pixel2D.hlsl", "Pixel2D", "ps_5_0");
		}
		if (mType == Shader3D) {
			LoadVertexShaderFile(L"./DirectXLib/Shader/Shader3D/Vertex3D.hlsl", "Vertex3D", "vs_5_0");
			LoadPixelShaderFile(L"./DirectXLib/Shader/Shader3D/Pixel3D.hlsl", "Pixel3D", "ps_5_0");
		}

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
		if (mType == Shader2D) {
			mGraph_Pipeline.RasterizerState.MultisampleEnable = false;
			mGraph_Pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			mGraph_Pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			mGraph_Pipeline.RasterizerState.DepthClipEnable = true;
		}
		if (mType == Shader3D) {
			mGraph_Pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			mGraph_Pipeline.RasterizerState.FrontCounterClockwise = false;
			mGraph_Pipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			mGraph_Pipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			mGraph_Pipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			mGraph_Pipeline.RasterizerState.DepthClipEnable = true;
			mGraph_Pipeline.RasterizerState.MultisampleEnable = false;
			mGraph_Pipeline.RasterizerState.AntialiasedLineEnable = false;
			mGraph_Pipeline.RasterizerState.ForcedSampleCount = 0;
			mGraph_Pipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			mGraph_Pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
			mGraph_Pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//三角形
		}
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
		if (mType == Shader2D)Layout2D();
		if (mType == Shader3D)Layout3D();
	}
	void Dx2DPipeline::Layout2D() {
		mGraph_Pipeline.InputLayout.pInputElementDescs = layout2D;
		mGraph_Pipeline.InputLayout.NumElements = _countof(layout2D);
		mGraph_Pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		mGraph_Pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}
	void Dx2DPipeline::Layout3D() {
		mGraph_Pipeline.InputLayout.pInputElementDescs = layout3D;
		mGraph_Pipeline.InputLayout.NumElements = _countof(layout3D);
		mGraph_Pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		mGraph_Pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		mGraph_Pipeline.DepthStencilState.DepthEnable = true;
		mGraph_Pipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		mGraph_Pipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		mGraph_Pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
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
		std::shared_ptr<lib::DirectX12Manager>& DxWrap,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline,
		Dx2DRootSignature& mRootSig,
		D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
	) {
		gpipeline.pRootSignature = mRootSig.CreateRootSignature(*DxWrap, RS_Desuc);
	}

	void Dx2DPipeline::CreateGraphicsPipeline(
		std::shared_ptr<lib::DirectX12Manager>& DxWrap,
		Dx2DRootSignature& mRootSig,
		D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
	) {
		mGraph_Pipeline.pRootSignature = mRootSig.CreateRootSignature(*DxWrap, RS_Desuc);
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

	D3D12_GRAPHICS_PIPELINE_STATE_DESC
		Dx2DPipeline::getPipeline() {
		return mGraph_Pipeline;
	}

	ID3D12PipelineState*
		Dx2DPipeline::getPipelineState() {
		return mPipeline_State;
	}
}