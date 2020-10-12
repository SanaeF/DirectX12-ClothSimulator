#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include<unordered_map>
#include<wrl.h>
#include<memory>

class PMDRenderer;

class DXPMDMaterial {
private:
	PMDRenderer& _renderer;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//PMD用共通テクスチャ(白、黒、グレイスケールグラデーション)
	ComPtr<ID3D12Resource> _whiteTex = nullptr;
	ComPtr<ID3D12Resource> _blackTex = nullptr;
	ComPtr<ID3D12Resource> _gradTex = nullptr;

	ID3D12Resource* CreateDefaultTexture(ComPtr< ID3D12Device> _dev,size_t width, size_t height);
	ID3D12Resource* CreateWhiteTexture(ComPtr< ID3D12Device> _dev);//白テクスチャの生成
	ID3D12Resource* CreateBlackTexture(ComPtr< ID3D12Device> _dev);//黒テクスチャの生成
	ID3D12Resource* CreateGrayGradationTexture(ComPtr< ID3D12Device> _dev);//グレーテクスチャの生成

public:

	HRESULT CreateMaterialAndTextureView(ComPtr< ID3D12Device> _dev, PMDRenderer& _renderer);

	DXPMDMaterial(PMDRenderer& _renderer);

};