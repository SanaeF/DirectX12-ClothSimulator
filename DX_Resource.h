#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<map>
#include<DirectXTex.h>
#include<wrl.h>
#include<string>
#include<functional>

class DX_Resource {
private:
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	std::vector<ID3D12Resource*> _backBuffers;//バックバッファ(2つ以上…スワップチェインが確保)

	ComPtr< ID3D12Device> _dev = nullptr;//デバイス
	ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;//レンダーターゲット用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;//深度バッファビュー用デスクリプタヒープ
	ComPtr < ID3D12GraphicsCommandList> _cmdList = nullptr;//コマンドリスト

	ComPtr<ID3D12Resource> mScPolyRes;
	ComPtr<ID3D12DescriptorHeap> mScPolyRTV_Heap;
	ComPtr<ID3D12DescriptorHeap> mScPolySRV_Heap;

	ComPtr<ID3D12Resource> mScPolyVB;

	D3D12_VERTEX_BUFFER_VIEW mScPolyVBV;

	void writte();

	void tex_view();

	void target();

	bool createScreenPolygon();

	void barrier(ID3D12Resource* p, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

public:

	void DrawScreen();

	bool ClearDrawScreen();

	void ScreenFlip();
};