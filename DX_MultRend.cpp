#include <iostream>
#include "DX_MultRend.h"
#include<cassert>
#include<d3dx12.h>
#include<d3dcompiler.h>

#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

bool DX_MultRend::descRange() {
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//b
	range.BaseShaderRegister = 0;//0
	range.NumDescriptors = 1;

	return true;
}

bool DX_MultRend::rootParam() {
	rp.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//
	rp.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp.DescriptorTable.pDescriptorRanges = &range;
	rp.DescriptorTable.NumDescriptorRanges = 1;


	return true;
}

bool DX_MultRend::rootSignature(ComPtr< ID3D12Device> device) {
	D3D12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0);
	rsDesc.NumParameters = 1;
	rsDesc.pParameters = &rp;
	rsDesc.NumStaticSamplers = 1;
	rsDesc.pStaticSamplers = &sampler;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	auto result = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, rsBlob.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		assert(0 && "D3D12SerializeRootSignature");
		return false;
	}
	result = device->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(mScPolyRS.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		assert(0 && "CreateRootSignature");
		return false;
	}

	result = D3DCompileFromFile(L"VertexPolygon.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"vertexShader", "vs_5_0", 0, 0, vs.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		//エラー処理
		return result;
	}
	result = D3DCompileFromFile(L"PixelPolygon.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"ps", "ps_5_0", 0, 0, ps.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		assert(0 && "CreateGraphicsPipelineState");
		return false;
	}


	return true;
}

bool DX_MultRend::gpipeline(ComPtr< ID3D12Device> device) {
	D3D12_INPUT_ELEMENT_DESC layout[2] = {
   {   "POSITION",
	   0,
	   DXGI_FORMAT_R32G32B32_FLOAT,
	   0,
	   D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	   0},
   {   "TEXCOORD",
	   0,
	   DXGI_FORMAT_R32G32_FLOAT,
	   0,
	   D3D12_APPEND_ALIGNED_ELEMENT,
	   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
	   0},
	};

	gpsDesc.InputLayout.NumElements = _countof(layout);
	gpsDesc.InputLayout.pInputElementDescs = layout;

	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vs.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(ps.Get());

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	gpsDesc.pRootSignature = mScPolyRS.Get();

	auto result = device->CreateGraphicsPipelineState(
		&gpsDesc,
		IID_PPV_ARGS(mScPolyPipeline.ReleaseAndGetAddressOf())
	);
	if (FAILED(result)) {
		assert(0&&"CreateGraphicsPipelineState");
		return false;
	}

	return true;
}

bool DX_MultRend::gpipelineTest(ComPtr< ID3D12Device> device) {
	D3D12_INPUT_ELEMENT_DESC layout[2] = {
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
	gpsDesc.InputLayout.NumElements = _countof(layout);
	gpsDesc.InputLayout.pInputElementDescs = layout;

	auto result = D3DCompileFromFile(L"VertexPolygon.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"vertexShader", "vs_5_0", 0, 0, vs.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		//エラー処理
		return result;
	}
	result = D3DCompileFromFile(L"PixelPolygon.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"ps", "ps_5_0", 0, 0, ps.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	if (FAILED(result)) {
		//エラー処理
		return result;
	}

	gpsDesc.VS = CD3DX12_SHADER_BYTECODE(vs.Get());
	gpsDesc.PS = CD3DX12_SHADER_BYTECODE(ps.Get());

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	rsDesc.NumParameters = 0;
	rsDesc.pParameters = 0;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		rsBlob.ReleaseAndGetAddressOf(),
		errBlob.ReleaseAndGetAddressOf()
	);

	if (FAILED(result)) {
		//エラー処理
		return result;
	}
	result = device->CreateRootSignature(
		0,
		rsBlob->GetBufferPointer(),
		rsBlob->GetBufferSize(),
		IID_PPV_ARGS(mScPolyRS.ReleaseAndGetAddressOf())
	);

	if (FAILED(result)) {
		//エラー処理
		return result;
	}

	gpsDesc.pRootSignature = mScPolyRS.Get();

	result = device->CreateGraphicsPipelineState(
		&gpsDesc,
		IID_PPV_ARGS(mScPolyPipeline.ReleaseAndGetAddressOf())
	);
}


bool DX_MultRend::pipeline(ComPtr< ID3D12Device> device) {
	if (!descRange())return false;
	if (!rootParam())return false;
	if (!rootSignature(device))return false;
	if (!gpipeline(device))return false;

	return true;
}

void DX_MultRend::barrier(
	ID3D12Resource* p,
	D3D12_RESOURCE_STATES before,
	D3D12_RESOURCE_STATES after,
	ComPtr < ID3D12GraphicsCommandList> cmdList
) {

	cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			p,
			before,
			after,
			0
		)
	);

}

bool DX_MultRend::createScreenPolygon(ComPtr< ID3D12Device> _dev) {

	struct ScPolyVertex {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	ScPolyVertex sPol[4] = {
		{{-1,-1,0.1},{0,1}},
		{{-1, 1,0.1},{0,0}},
		{{ 1,-1,0.1},{1,1}},
		{{ 1, 1,0.1},{1,0}}
	};

	auto result = _dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(sPol)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mScPolyVB.ReleaseAndGetAddressOf())
	);

	mScPolyVBV.BufferLocation = mScPolyVB->GetGPUVirtualAddress();
	mScPolyVBV.SizeInBytes = sizeof(sPol);
	mScPolyVBV.StrideInBytes = sizeof(ScPolyVertex);
	//どっちか逆
	ScPolyVertex* mappedScPoly = nullptr;
	mScPolyVB->Map(0, nullptr, (void**)&mappedScPoly);
	copy(begin(sPol), end(sPol), mappedScPoly);
	mScPolyVB->Unmap(0, nullptr);

	return true;
}

bool DX_MultRend::ClearDrawScreen() {

	return true;
}

void DX_MultRend::DrawScreen(ComPtr<ID3D12Resource> ScPolyRes, ComPtr < ID3D12GraphicsCommandList> _cmdList) {
	_cmdList->SetGraphicsRootSignature(mScPolyRS.Get());
	_cmdList->SetPipelineState(mScPolyPipeline.Get());
	_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_cmdList->IASetVertexBuffers(0, 1, &mScPolyVBV);
	_cmdList->DrawInstanced(4, 1, 0, 0);
}

void DX_MultRend::ScreenFlip() {

}

ID3D12PipelineState*DX_MultRend::getPipeline() {
	return mScPolyPipeline.Get();
}