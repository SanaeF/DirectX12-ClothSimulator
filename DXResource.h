#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<map>
#include<DirectXTex.h>
#include<wrl.h>
#include<string>
#include<functional>

class DXResource {
private:

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> mScPolyRes;

	ComPtr<ID3D12DescriptorHeap> mScPolyRTV_Heap;
	ComPtr<ID3D12DescriptorHeap> mScPolySRV_Heap;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

public:

	bool CreateScreenResAndView(
		ComPtr< ID3D12Device> _dev,
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps,
		ComPtr<ID3D12DescriptorHeap> _dsvHeap,
		ComPtr < ID3D12GraphicsCommandList> _cmdList,
		std::vector<ID3D12Resource*> _backBuffers
	);

	ComPtr<ID3D12Resource> getResource();

	ComPtr<ID3D12DescriptorHeap> getRTV_Heap();

	DXResource(){}
private:
	bool mWritter(
		ComPtr< ID3D12Device> device,
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps,
		std::vector<ID3D12Resource*> _backBuffers
	);

	bool mTarget(
		ComPtr<ID3D12DescriptorHeap> _dsvHeap,
		ComPtr < ID3D12GraphicsCommandList> _cmdList
	);

	bool mViewer(ComPtr< ID3D12Device> device);

	bool mHeapRTV(ComPtr< ID3D12Device> device);
	bool mHeapSRV(ComPtr< ID3D12Device> device);
	bool mDescRTV(ComPtr< ID3D12Device> device);
	bool mDescSRV(ComPtr< ID3D12Device> device);

};

//	（　・∀・）　　|　|　ｶﾞｯ!
//	⊂　　　　）　  |　|
//	  Ｙ　 / ノ　　　人
//		  / ）　 　 < 　>__Λ  ∩
//	  ＿ /し'  ／／. Ｖ｀Д´）/ ←>>1
//	（＿フ彡　　　　　 /
