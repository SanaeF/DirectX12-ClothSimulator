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

class DXVMDMotion;

class DXPMDModel{
private:
	//PMDRenderer& _renderer;
	std::shared_ptr<PMDRenderer> _renderer;
	std::shared_ptr<Dx12Wrapper> _dx12;

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

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
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

	std::shared_ptr<DXVMDMotion>Motion;

	D3D12_VERTEX_BUFFER_VIEW _vbView = {};
	D3D12_INDEX_BUFFER_VIEW _ibView = {};
	DirectX::XMFLOAT3 _rotator;
	DirectX::XMFLOAT3 _pos;
	DWORD _startTime;

	float _angle;//テスト用Y軸回転

public:

	DXPMDModel(std::shared_ptr<Dx12Wrapper> dx, std::shared_ptr<PMDRenderer> renderer, const char* path);
	~DXPMDModel();

	///クローンは頂点およびマテリアルは共通のバッファを見るようにする
	DXPMDModel* Clone();

	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void LoadVMDFile(const char* filepath, const char* name);
	void PlayAnimation();
	void Update();
	void Draw();

	const DirectX::XMFLOAT3& GetPosition()const;
	const DirectX::XMFLOAT3& GetRotate()const;

private:
	//PMDファイルのロード
	void LoadPMDFile(const char* path);

	void MotionUpdate();

	void LoadMaterial(unsigned int vertNum, std::string strModelPath, FILE* fp);

	friend PMDRenderer;

	//読み込んだマテリアルをもとにマテリアルバッファを作成
	HRESULT CreateMaterialData();
	//マテリアル＆テクスチャのビューを作成
	HRESULT CreateMaterialAndTextureView();

};