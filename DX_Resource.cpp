#include <iostream>
#include "DX_Resource.h"
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

bool DX_Resource::CreatePeraResourcesAndView(
	ComPtr<ID3D12Device> _dev,
	ComPtr<ID3D12DescriptorHeap> _rtvHeaps,
	ComPtr<ID3D12DescriptorHeap> _dsvHeap,
	ComPtr <ID3D12GraphicsCommandList> _cmdList,
	std::vector<ID3D12Resource*> _backBuffers
	) {
	if (!writte(_dev,_rtvHeaps, _backBuffers))return false;
	if (!tex_view(_dev))return false;
	if (!target(_dsvHeap, _cmdList))return false;
	return true;

}

bool DX_Resource::writte(
	ComPtr< ID3D12Device> _dev,
	ComPtr<ID3D12DescriptorHeap> _rtvHeaps,
	std::vector<ID3D12Resource*> _backBuffers
) {

	auto heapDesc = _rtvHeaps->GetDesc();
	auto& bbuff = _backBuffers[0];
	auto resDesc = bbuff->GetDesc();
	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	float clsClr[4] = { 0.5,0.5,0.5,1.0 };
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_B8G8R8A8_UNORM, clsClr);

	auto result = _dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(mScPolyRes.ReleaseAndGetAddressOf())
	);
	//if (FAILED(result)) {
	//	assert(0);
	//	return false;
	//}

	return true;

}

bool DX_Resource::tex_view(ComPtr< ID3D12Device> _dev) {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;

	if (FAILED(_dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(mScPolyRTV_Heap.ReleaseAndGetAddressOf())))) {
		assert(0);
		return false;
	}

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	_dev->CreateRenderTargetView(
		mScPolyRes.Get(),
		&rtvDesc,
		mScPolyRTV_Heap->GetCPUDescriptorHandleForHeapStart()
	);
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	if (FAILED(_dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(mScPolySRV_Heap.ReleaseAndGetAddressOf())))){
		assert(0);
		return false;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	_dev->CreateShaderResourceView(
		mScPolyRes.Get(),
		&srvDesc,
		mScPolySRV_Heap->GetCPUDescriptorHandleForHeapStart()
		);

	return true;
}

bool DX_Resource::target(
	ComPtr<ID3D12DescriptorHeap> _dsvHeap,
	ComPtr < ID3D12GraphicsCommandList> _cmdList
) {
	auto rtvHeapPointer = mScPolyRTV_Heap->GetCPUDescriptorHandleForHeapStart();

	_cmdList->OMSetRenderTargets(
		1,
		&rtvHeapPointer,
		false,
		&_dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
	return true;
}

bool DX_Resource::descRange() {
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//b
	range[0].BaseShaderRegister = 0;//0
	range[0].NumDescriptors = 1;

	range[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//t
	range[1].BaseShaderRegister = 0;//0
	range[1].NumDescriptors = 1;

	range[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//t
	range[2].BaseShaderRegister = 1;//1
	range[2].NumDescriptors = 1;

	setState(eROOT_PARAMETER);
	return true;
}

bool DX_Resource::rootParam() {
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[0].DescriptorTable.pDescriptorRanges = &range[0];
	rp[0].DescriptorTable.NumDescriptorRanges = 1;

	rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//
	rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[1].DescriptorTable.pDescriptorRanges = &range[1];
	rp[1].DescriptorTable.NumDescriptorRanges = 1;

	rp[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//
	rp[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[2].DescriptorTable.pDescriptorRanges = &range[2];
	rp[2].DescriptorTable.NumDescriptorRanges = 1;

	setState(eROOT_SIGNATURE_DESC);
	return true;
}

bool DX_Resource::rootSignature(ComPtr< ID3D12Device> _dev) {
	rsDesc.NumParameters = 0;
	rsDesc.pParameters = 0;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	auto result = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, rsBlob.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	//if (!CheckResult(result, errBlob.Get())) {
	//	assert(0);
	//	return false;
	//}
	result = _dev->CreateRootSignature(0, rsBlob->GetBufferPointer(), rsBlob->GetBufferSize(), IID_PPV_ARGS(mScPolyRS.ReleaseAndGetAddressOf()));
	//if (!CheckResult(result)) {
	//	assert(0);
	//	return false;
	//}

	result = D3DCompileFromFile(L"VertexPolygon.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"vertexShader", "vs_5_0", 0, 0, vs.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	//if (!CheckResult(result, errBlob.Get())) {
	//	assert(0);
	//	return false;
	//}
	result = D3DCompileFromFile(L"PixelPolygon.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"ps", "ps_5_0", 0, 0, ps.ReleaseAndGetAddressOf(), errBlob.ReleaseAndGetAddressOf());
	//if (!CheckResult(result, errBlob.Get())) {
	//	assert(0);
	//	return false;
	//}

	setState(eGRAPHICS_PIPELINE_DESC);
	return true;
}

bool DX_Resource::gpipeline(ComPtr< ID3D12Device> _dev) {

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

	auto result = _dev->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(mScPolyPipeline.ReleaseAndGetAddressOf()));
	//if (!CheckResult(result)) {
	//	assert(0);
	//	return false;
	//}

	return true;
}

bool DX_Resource::pipeline(ComPtr< ID3D12Device> _dev) {
	if (!descRange())return false;
	if (!rootParam())return false;
	if (!rootSignature(_dev))return false;
	if (!gpipeline(_dev))return false;

	return true;
}

void DX_Resource::barrier(
	ID3D12Resource* p,
	D3D12_RESOURCE_STATES before,
	D3D12_RESOURCE_STATES after,
	ComPtr < ID3D12GraphicsCommandList> _cmdList
) {

	_cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			p,
			before,
			after,
			0
		)
	);

}

bool DX_Resource::createScreenPolygon(ComPtr< ID3D12Device> _dev) {

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
	//‚Ç‚Á‚¿‚©‹t
	ScPolyVertex* mappedScPoly = nullptr;
	mScPolyVB->Map(0, nullptr, (void**)&mappedScPoly);
	copy(begin(sPol), end(sPol), mappedScPoly);
	mScPolyVB->Unmap(0, nullptr);

	return true;
}

bool DX_Resource::ClearDrawScreen() {

	return true;
}

void DX_Resource::DrawScreen(ComPtr < ID3D12GraphicsCommandList> _cmdList) {
	_cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(mScPolyRes.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	_cmdList->SetGraphicsRootSignature(mScPolyRS.Get());
	_cmdList->SetPipelineState(mScPolyPipeline.Get());
	_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_cmdList->IASetVertexBuffers(0, 1, &mScPolyVBV);
	_cmdList->DrawInstanced(4, 1, 0, 0);
}

void DX_Resource::ScreenFlip() {

}

void inline DX_Resource::setState(int num) {
	//mPipeline_State = num;
}