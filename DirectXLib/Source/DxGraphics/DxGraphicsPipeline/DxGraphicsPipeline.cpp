#include "DxGraphicsPipeline.h"
#include "../../DirectX12Manager/DirectX12Manager.h"

#include "../DxRootSignature/DxRootSignature.h"

#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace libGraph {
	D3D12_INPUT_ELEMENT_DESC layout_2d[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"SV_InstanceID",0,DXGI_FORMAT_R32_UINT,1,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA,1},
	};
	D3D12_INPUT_ELEMENT_DESC layout_3d[] = {
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "BONE_NO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		//{ "EDGE_FLG",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};
	DxGraphicsPipeline::DxGraphicsPipeline(int type) {
		m_Type = type;
		loadShader();
		shaderState();
		sampleMaskState();
		rasterizerState();
		blendState();
		layout();
		renderTargets();
		antiAiliasing();
	}
	DxGraphicsPipeline::~DxGraphicsPipeline() {

	}
	void DxGraphicsPipeline::loadShader() {
		if (m_Type == SHADER_2D) {
			loadVertexShaderFile(L"./DirectXLib/Shader/Shader2D/Vertex2D.hlsl", "Vertex2D", "vs_5_0");
			loadPixelShaderFile(L"./DirectXLib/Shader/Shader2D/Pixel2D.hlsl", "Pixel2D", "ps_5_0");
		}
		if (m_Type == SHADER_3D) {
			loadVertexShaderFile(L"./DirectXLib/Shader/Shader3D/Vertex3D.hlsl", "Vertex3D", "vs_5_0");
			loadPixelShaderFile(L"./DirectXLib/Shader/Shader3D/Pixel3D.hlsl", "Pixel3D", "ps_5_0");
		}

	}

	void DxGraphicsPipeline::shaderState() {
		m_Graph_pipeline.pRootSignature = nullptr;

		m_Graph_pipeline.VS.pShaderBytecode = getVertShaderBlob()->GetBufferPointer();
		m_Graph_pipeline.VS.BytecodeLength = getVertShaderBlob()->GetBufferSize();
		m_Graph_pipeline.PS.pShaderBytecode = getPixelShaderBlob()->GetBufferPointer();
		m_Graph_pipeline.PS.BytecodeLength = getPixelShaderBlob()->GetBufferSize();
	}

	void DxGraphicsPipeline::sampleMaskState() {
		m_Graph_pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	}

	void DxGraphicsPipeline::rasterizerState() {
		if (m_Type == SHADER_2D) {
			m_Graph_pipeline.RasterizerState.MultisampleEnable = false;
			m_Graph_pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			m_Graph_pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			m_Graph_pipeline.RasterizerState.DepthClipEnable = true;
		}
		if (m_Type == SHADER_3D) {
			m_Graph_pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			m_Graph_pipeline.RasterizerState.FrontCounterClockwise = false;
			m_Graph_pipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			m_Graph_pipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			m_Graph_pipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			m_Graph_pipeline.RasterizerState.DepthClipEnable = true;
			m_Graph_pipeline.RasterizerState.MultisampleEnable = false;
			m_Graph_pipeline.RasterizerState.AntialiasedLineEnable = false;
			m_Graph_pipeline.RasterizerState.ForcedSampleCount = 0;
			m_Graph_pipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			m_Graph_pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
			m_Graph_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//三角形
		}
	}

	void DxGraphicsPipeline::blendState() {
		m_Graph_pipeline.BlendState.AlphaToCoverageEnable = true;
		m_Graph_pipeline.BlendState.IndependentBlendEnable = false;

		m_RTB_desc.BlendEnable = true;

		//アルファテスト設定
		m_RTB_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		m_RTB_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		m_RTB_desc.BlendOp = D3D12_BLEND_OP_ADD;
		m_RTB_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
		m_RTB_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
		m_RTB_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

		m_RTB_desc.LogicOpEnable = false;
		m_RTB_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		m_Graph_pipeline.BlendState.RenderTarget[0] = m_RTB_desc;
	}

	void DxGraphicsPipeline::layout() {
		if (m_Type == SHADER_2D)layout2D();
		if (m_Type == SHADER_3D)layout3D();
	}
	void DxGraphicsPipeline::layout2D() {
		m_Graph_pipeline.InputLayout.pInputElementDescs = layout_2d;
		m_Graph_pipeline.InputLayout.NumElements = _countof(layout_2d);
		m_Graph_pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		m_Graph_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	}
	void DxGraphicsPipeline::layout3D() {
		m_Graph_pipeline.InputLayout.pInputElementDescs = layout_3d;
		m_Graph_pipeline.InputLayout.NumElements = _countof(layout_3d);
		m_Graph_pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		m_Graph_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		m_Graph_pipeline.DepthStencilState.DepthEnable = true;
		m_Graph_pipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		m_Graph_pipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_Graph_pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}
	void DxGraphicsPipeline::renderTargets() {
		m_Graph_pipeline.NumRenderTargets = 1;
		m_Graph_pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	void DxGraphicsPipeline::antiAiliasing() {
		m_Graph_pipeline.SampleDesc.Count = 1;
		m_Graph_pipeline.SampleDesc.Quality = 0;
	}

	void DxGraphicsPipeline::rootSignatureState(
		std::shared_ptr<lib::DirectX12Manager>& DxWrap,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline,
		Dx2DRootSignature& mRootSig,
		D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
	) {
		gpipeline.pRootSignature = mRootSig.createRootSignature(*DxWrap, RS_Desuc);
	}

	void DxGraphicsPipeline::createGraphicsPipeline(
		std::shared_ptr<lib::DirectX12Manager>& DxWrap,
		Dx2DRootSignature& mRootSig,
		D3D12_ROOT_SIGNATURE_DESC* RS_Desuc
	) {
		m_Graph_pipeline.pRootSignature = mRootSig.createRootSignature(*DxWrap, RS_Desuc);
		auto result = DxWrap->device()->CreateGraphicsPipelineState(
			&m_Graph_pipeline,
			IID_PPV_ARGS(&m_Pipeline_state)
		);

		if (!SUCCEEDED(result)) {
			assert(0 && "CreateGraphicsPipelineState Error!");
		}
	}

	void DxGraphicsPipeline::setPipelineState(ID3D12PipelineState* Pipeline_State) {
		this->m_Pipeline_state = Pipeline_State;
	}

	void DxGraphicsPipeline::setGraphPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline) {
		this->m_Graph_pipeline = gpipeline;
	}

	void DxGraphicsPipeline::setRTBDesc(D3D12_RENDER_TARGET_BLEND_DESC RTB_Desc) {
		this->m_RTB_desc = RTB_Desc;
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC
		DxGraphicsPipeline::getPipeline() {
		return m_Graph_pipeline;
	}

	ID3D12PipelineState*
		DxGraphicsPipeline::getPipelineState() {
		return m_Pipeline_state;
	}
}