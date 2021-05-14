#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<map>
#include<unordered_map>
#include<DirectXTex.h>
#include<wrl.h>
#include<string>
#include<functional>

class DXResource;
class DX_MultRend;
class PMDRenderer;

namespace lib {
	class DirectX12Manager {

		std::shared_ptr<DX_MultRend>Render;
		std::shared_ptr<DXResource>Resource;

		HWND _hwnd;
		UINT64 _fenceValue;
		SIZE _winSize, _graphicSize;
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//DXGIまわり
		ComPtr < IDXGIFactory4> _dxgiFactory = nullptr;//DXGIインターフェイス
		ComPtr < IDXGISwapChain4> _swapchain = nullptr;//スワップチェイン

		//DirectX12まわり
		ComPtr< ID3D12Device> _dev = nullptr;//デバイス
		ComPtr < ID3D12CommandAllocator> _cmdAllocator = nullptr;//コマンドアロケータ
		ComPtr < ID3D12GraphicsCommandList> _cmdList = nullptr;//コマンドリスト
		ComPtr < ID3D12CommandQueue> _cmdQueue = nullptr;//コマンドキュー

		//表示に関わるバッファ周り
		ComPtr<ID3D12Resource> _depthBuffer = nullptr;//深度バッファ
		std::vector<ID3D12Resource*> _backBuffers;//バックバッファ(2つ以上…スワップチェインが確保)
		ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;//レンダーターゲット用デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;//深度バッファビュー用デスクリプタヒープ
		std::unique_ptr<D3D12_VIEWPORT> _viewport;//ビューポート
		std::unique_ptr<D3D12_RECT> _scissorrect;//シザー矩形
		//シーンを構成するバッファまわり
		ComPtr<ID3D12Resource> _sceneConstBuff = nullptr;

		struct SceneData {
			DirectX::XMMATRIX view;//ビュー行列
			DirectX::XMMATRIX proj;//プロジェクション行列
			DirectX::XMFLOAT3 eye;//視点座標
		};
		SceneData* _mappedSceneData;
		ComPtr<ID3D12DescriptorHeap> _sceneDescHeap = nullptr;

		//フェンス
		ComPtr<ID3D12Fence> _fence = nullptr;
		UINT64 _fenceVal = 0;

		//ロード用テーブル
		using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
		std::map < std::string, LoadLambda_t> _loadLambdaTable;
		//テクスチャテーブル
		std::unordered_map<std::string, ComPtr<ID3D12Resource>> _textureTable;

		std::vector<int> mFuncCount;

		std::vector<int> mFuncCount3D;

	public:
		DirectX12Manager(HWND hwnd);
		~DirectX12Manager();

		bool Init(SIZE pix);

		void Draw(std::shared_ptr<PMDRenderer> renderer);
		void Update();
		void ClearScreen();
		void CommandClear();

		void SetNextCommand();

		///テクスチャパスから必要なテクスチャバッファへのポインタを返す
		///@param texpath テクスチャファイルパス
		ComPtr<ID3D12Resource> GetTextureByPath(const char* texpath);

		ComPtr< ID3D12Device> Device();//デバイス
		ComPtr < ID3D12GraphicsCommandList> CmdList();//コマンドリスト
		ComPtr < IDXGISwapChain4> Swapchain();//スワップチェイン

		D3D12_VIEWPORT* getViewPort();//ビューポート
		D3D12_RECT* getScissorrect();//シザー矩形

		void setCount(int Handle, int num);
		int getCount(int Handle);
		void setCount3D(int Handle, int num);
		int getCount3D(int Handle);

		void ClearCount();

		void SetScene();

		void ScreenFlip();

		void setPixelSize(SIZE pix);

		int GetCallFunctionCount();

		SIZE getWinSize();

		SIZE getPixelSize();

	private:

		//最終的なレンダーターゲットの生成
		HRESULT	CreateFinalRenderTargets();
		//デプスステンシルビューの生成
		HRESULT CreateDepthStencilView();

		//スワップチェインの生成
		HRESULT CreateSwapChain(const HWND& hwnd, SIZE pix);

		//DXGIまわり初期化
		HRESULT InitializeDXGIDevice();



		//コマンドまわり初期化
		HRESULT InitializeCommand();

		//ビュープロジェクション用ビューの生成
		HRESULT CreateSceneView();

		//テクスチャローダテーブルの作成
		void CreateTextureLoaderTable();
		//テクスチャ名からテクスチャバッファ作成、中身をコピー
		ID3D12Resource* CreateTextureFromFile(const char* texpath);
	};
}