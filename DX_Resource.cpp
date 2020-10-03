#include <iostream>
#include "DX_Resource.h"
#include<cassert>
#include<d3dx12.h>

using namespace DirectX;
using namespace std;

void DX_Resource::writte() {
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
}

void DX_Resource::tex_view() {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;

	auto result = _dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(mScPolyRTV_Heap.ReleaseAndGetAddressOf()));

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

	result = _dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(mScPolySRV_Heap.ReleaseAndGetAddressOf()));

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

}

void DX_Resource::target() {
	auto rtvHeapPointer = mScPolyRTV_Heap->GetCPUDescriptorHandleForHeapStart();

	_cmdList->OMSetRenderTargets(
		1,
		&rtvHeapPointer,
		false,
		&_dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void DX_Resource::barrier(
	ID3D12Resource* p,
	D3D12_RESOURCE_STATES before,
	D3D12_RESOURCE_STATES after) {

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

bool DX_Resource::createScreenPolygon() {

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

void DX_Resource::DrawScreen() {
	_cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(mScPolyRes.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void DX_Resource::ScreenFlip() {

}