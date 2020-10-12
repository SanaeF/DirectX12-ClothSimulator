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
class DXPMDMaterial;

class DXVMDMotion;

class DXPMDModel{
private:
	//PMDRenderer& _renderer;
	//std::shared_ptr<PMDRenderer> _renderer;
	std::shared_ptr<Dx12Wrapper> _dx12;
	std::shared_ptr <DXPMDMaterial> mMaterial;

	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//ComPtr<ID3D12Resource> _vb = nullptr;
	//ComPtr<ID3D12Resource> _ib = nullptr;
	//ComPtr<ID3D12Resource> _transformMat = nullptr;//座標変換行列(今はワールドのみ)
	//ComPtr<ID3D12Resource> _materialBuff = nullptr;
	//ComPtr<ID3D12DescriptorHeap> _materialHeap = nullptr;//マテリアルヒープ(5個ぶん)


	std::shared_ptr<DXVMDMotion>Motion;


	DirectX::XMFLOAT3 _rotator;
	DirectX::XMFLOAT3 _pos;
	DWORD _startTime;

	float _angle;//テスト用Y軸回転

public:

	DXPMDModel(std::shared_ptr<Dx12Wrapper> dx, const char* path);
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