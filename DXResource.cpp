#include <iostream>
#include<cassert>
#include<d3dx12.h>
#include<d3dcompiler.h>
#include "DXResource.h"

#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std; 

bool DXResource::mHeapRTV(ComPtr< ID3D12Device> device) {

	heapDesc.NumDescriptors = 1;

	auto result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(mScPolyRTV_Heap.ReleaseAndGetAddressOf())
	);

	if (FAILED(result)) {
		assert(
			"おめでとう!				" &&
			"【mScPolyRTV_Heap】がスッカスカ～♪ ->>" && 0
		);
		return false;
		//	（　・∀・）　　|　|　ｶﾞｯ!
		//	⊂　　　　）　  |　|
		//	  Ｙ　 / ノ　　　人
		//		  / ）　 　 < 　>__Λ  ∩
		//	  ＿ /し'  ／／. Ｖ｀Д´）/ ←>>1
		//	（＿フ彡　　　　　 /
	}

	return true;
}
bool DXResource::mDescRTV(ComPtr< ID3D12Device> device) {

	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	auto handle = mScPolyRTV_Heap->GetCPUDescriptorHandleForHeapStart();
	device->CreateRenderTargetView(
		mScPolyRes.Get(),
		&rtvDesc,
		handle
	);
	//例外！！

	return true;
}
bool DXResource::mHeapSRV(ComPtr< ID3D12Device> device) {

	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto result = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(mScPolySRV_Heap.ReleaseAndGetAddressOf())
	);

	if (FAILED(result)) {
		assert(
			"おめでとう!				" &&
			"【mScPolySRV_Heap】がスッカスカ～♪ ->>" && 0
		);
		return false;
		//	（　・∀・）　　|　|　ｶﾞｯ!
		//	⊂　　　　）　  |　|
		//	  Ｙ　 / ノ　　　人
		//		  / ）　 　 < 　>__Λ  ∩
		//	  ＿ /し'  ／／. Ｖ｀Д´）/ ←>>1
		//	（＿フ彡　　　　　 /
	}

	return true;
}
bool DXResource::mDescSRV(ComPtr< ID3D12Device> device) {

	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	device->CreateShaderResourceView(
		mScPolyRes.Get(),
		&srvDesc,
		mScPolySRV_Heap->GetCPUDescriptorHandleForHeapStart()

	);

	return true;
}


bool DXResource::mWritter(
	ComPtr< ID3D12Device> device,
	ComPtr<ID3D12DescriptorHeap> rtvHeaps,
	std::vector<ID3D12Resource*> backBuffers
) {

	float clsClr[4] = { 1.5,0.5,1.5,1.0 };
	heapDesc = rtvHeaps->GetDesc();
	auto& bbuff = backBuffers[0];
	auto resDesc = bbuff->GetDesc();

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clsClr);//DXGI_FORMAT_B8G8R8A8_UNORM
	auto result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(mScPolyRes.ReleaseAndGetAddressOf())//ヌルポ！
	);
	if (FAILED(result)) {
		assert(
			"おめでとう!				" &&
			"【CreateCommittedResource】がバグったよ～♪ ->>" && 0
		);
		return false;
	}
	return true;

}

bool DXResource::mViewer(ComPtr< ID3D12Device> device) {
	//RTV
	if (!mHeapRTV(device)) {
		assert(
			"おめでとう!				" &&
			"error～♪ ->>" && 0
		);
		return false;
	}
	if (!mDescRTV(device)) {
		assert(
			"おめでとう!				" &&
			"error～♪ ->>" && 0
		);
		return false;
	}
	//SRV
	if (!mHeapSRV(device)) {
		assert(
			"おめでとう!				" &&
			"error～♪ ->>" && 0
		);
		return false;
	}
	if (!mDescSRV(device)) {
		assert(
			"おめでとう!				" &&
			"error～♪ ->>" && 0
		);
		return false;
	}
	return true;
}


bool DXResource::mTarget(
	ComPtr<ID3D12DescriptorHeap> dsvHeap,
	ComPtr < ID3D12GraphicsCommandList> cmdList
) {

	auto rtvHeapPointer = mScPolyRTV_Heap->GetCPUDescriptorHandleForHeapStart();

	auto descriptHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(
		1,
		&rtvHeapPointer,
		false,
		&descriptHandle
	);
	auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		mScPolyRes.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	cmdList->ResourceBarrier(
		1,
		&resBarrier
	);
	return true;
}


bool DXResource::CreateScreenResAndView(
	ComPtr<ID3D12Device> device,
	ComPtr<ID3D12DescriptorHeap> rtvHeaps,
	ComPtr<ID3D12DescriptorHeap> dsvHeap,
	ComPtr<ID3D12GraphicsCommandList> cmdList,
	std::vector<ID3D12Resource*> backBuffers
) {

	if (!mWritter(device, rtvHeaps, backBuffers))return false;
	if (!mViewer(device))return false;
	if (!mTarget(dsvHeap, cmdList))return false;
	return true;
}

ComPtr<ID3D12Resource> DXResource::getResource() {
	return mScPolyRes;
}

ComPtr<ID3D12DescriptorHeap> DXResource::getRTV_Heap() {
	return mScPolyRTV_Heap;
}