#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<map>
#include<DirectXTex.h>
#include<wrl.h>
#include<string>
#include<functional>

class DX_MultRend {
private:
	enum {
		eDESCRIPTOR_RANGE,
		eROOT_PARAMETER,
		eROOT_SIGNATURE_DESC,
		eGRAPHICS_PIPELINE_DESC
	};

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> mScPolyRes;
	ComPtr<ID3D12Resource> mScPolyVB;

	ComPtr<ID3D12DescriptorHeap> mScPolyRTV_Heap;
	ComPtr<ID3D12DescriptorHeap> mScPolySRV_Heap;
	ComPtr<ID3D12PipelineState> mScPolyPipeline;
	ComPtr<ID3D12RootSignature> mScPolyRS;

	ComPtr<ID3DBlob> rsBlob;
	ComPtr<ID3DBlob> errBlob;
	ComPtr<ID3DBlob> vs;
	ComPtr<ID3DBlob> ps;

	D3D12_INPUT_ELEMENT_DESC layout[2] = {
	{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};

	D3D12_DESCRIPTOR_RANGE range[3] = {};
	D3D12_ROOT_PARAMETER rp[3] = {};
	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	D3D12_VERTEX_BUFFER_VIEW mScPolyVBV;

public:

	bool CreateScreenResAndView(
		ComPtr< ID3D12Device> _dev,
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps,
		ComPtr<ID3D12DescriptorHeap> _dsvHeap,
		ComPtr < ID3D12GraphicsCommandList> _cmdList,
		std::vector<ID3D12Resource*> _backBuffers
	);

	bool createScreenPolygon(ComPtr< ID3D12Device> _dev);

	void DrawScreen(ComPtr < ID3D12GraphicsCommandList> _cmdList);

	bool ClearDrawScreen();

	bool pipeline(ComPtr< ID3D12Device> _dev);

	void ScreenFlip();

	DX_MultRend()
	{
	}
private:


	bool writte(
		ComPtr< ID3D12Device> _dev,
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps,
		std::vector<ID3D12Resource*> _backBuffers
	);
	bool tex_view(ComPtr< ID3D12Device> _dev);
	bool target(
		ComPtr<ID3D12DescriptorHeap> _dsvHeap,
		ComPtr < ID3D12GraphicsCommandList> _cmdList
	);

	bool descRange();
	bool rootParam();
	bool rootSignature(ComPtr< ID3D12Device> _dev);
	bool gpipeline(ComPtr< ID3D12Device> _dev);

	void barrier(
		ID3D12Resource* p,
		D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after,
		ComPtr < ID3D12GraphicsCommandList> _cmdList
	);

};