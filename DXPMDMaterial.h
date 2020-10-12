#pragma once
#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include<map>
#include<unordered_map>
#include<wrl.h>
#include<memory>

class Dx12Wrapper;
class PMDRenderer;

class DXPMDMaterial {
private:

	//シェーダ側に投げられるマテリアルデータ
	struct MaterialForHlsl {
		DirectX::XMFLOAT3 diffuse; //ディフューズ色
		float alpha; // ディフューズα
		DirectX::XMFLOAT3 specular; //スペキュラ色
		float specularity;//スペキュラの強さ(乗算値)
		DirectX::XMFLOAT3 ambient; //アンビエント色
	};
	//それ以外のマテリアルデータ
	struct AdditionalMaterial {
		std::string texPath;//テクスチャファイルパス
		int toonIdx; //トゥーン番号
		bool edgeFlg;//マテリアル毎の輪郭線フラグ
	};
	struct Material {
		unsigned int indicesNum;//インデックス数
		MaterialForHlsl material;
		AdditionalMaterial additional;
	};

	Dx12Wrapper& mDX;
	//PMDRenderer& _renderer;
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//PMD用共通テクスチャ(白、黒、グレイスケールグラデーション)
	ComPtr<ID3D12Resource> _whiteTex = nullptr;
	ComPtr<ID3D12Resource> _blackTex = nullptr;
	ComPtr<ID3D12Resource> _gradTex = nullptr;
	ComPtr<ID3D12Resource> _vb = nullptr;
	ComPtr<ID3D12Resource> _ib = nullptr;
	ComPtr<ID3D12Resource> _transformMat = nullptr;//座標変換行列(今はワールドのみ)
	ComPtr<ID3D12Resource> _materialBuff = nullptr;
	ComPtr<ID3D12DescriptorHeap> _materialHeap = nullptr;//マテリアルヒープ(5個ぶん)
	std::vector<Material> _materials;
	std::vector<ComPtr<ID3D12Resource>> _textureResources;
	std::vector<ComPtr<ID3D12Resource>> _sphResources;
	std::vector<ComPtr<ID3D12Resource>> _spaResources;
	std::vector<ComPtr<ID3D12Resource>> _toonResources;

	D3D12_VERTEX_BUFFER_VIEW _vbView = {};
	D3D12_INDEX_BUFFER_VIEW _ibView = {};

	ID3D12Resource* CreateDefaultTexture(ComPtr< ID3D12Device> _dev,size_t width, size_t height);
	ID3D12Resource* CreateWhiteTexture(ComPtr< ID3D12Device> _dev);//白テクスチャの生成
	ID3D12Resource* CreateBlackTexture(ComPtr< ID3D12Device> _dev);//黒テクスチャの生成
	ID3D12Resource* CreateGrayGradationTexture(ComPtr< ID3D12Device> _dev);//グレーテクスチャの生成

public:

	void LoadMaterial(Dx12Wrapper& DX12, unsigned int vertNum, std::string strModelPath, FILE* fp);

	HRESULT CreateMaterialData(ComPtr< ID3D12Device> _dev);

	HRESULT CreateMaterialAndTextureView(ComPtr< ID3D12Device> _dev);

	DXPMDMaterial(Dx12Wrapper& DX12);

};