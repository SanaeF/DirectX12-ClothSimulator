#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<map>
#include<unordered_map>
#include<DirectXTex.h>
#include<wrl.h>
#include<string>
#include<functional>
namespace lib {
	class DirectX12Manager {
		HWND m_Hwnd;
		UINT64 m_Fence_value;
		SIZE m_Win_size, m_Graphic_size;
		template<typename T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//DXGIまわり
		ComPtr<IDXGIFactory4> m_DxGi_factory = nullptr;//DXGIインターフェイス
		ComPtr<IDXGISwapChain4> m_Swapchain = nullptr;//スワップチェイン

		//DirectX12まわり
		ComPtr<ID3D12Device> m_Device = nullptr;//デバイス
		ComPtr<ID3D12CommandAllocator> m_Cmd_allocator = nullptr;//コマンドアロケータ
		ComPtr<ID3D12GraphicsCommandList> m_Cmd_list = nullptr;//コマンドリスト
		ComPtr<ID3D12CommandQueue> m_Cmd_queue = nullptr;//コマンドキュー

		//表示に関わるバッファ周り
		ComPtr<ID3D12Resource> m_Depth_buffer = nullptr;//深度バッファ
		std::vector<ID3D12Resource*> m_pBack_buffers;//バックバッファ(2つ以上…スワップチェインが確保)
		ComPtr<ID3D12DescriptorHeap> m_RTV_heaps = nullptr;//レンダーターゲット用デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> m_DSV_heap = nullptr;//深度バッファビュー用デスクリプタヒープ
		std::unique_ptr<D3D12_VIEWPORT> m_View_port;//ビューポート
		std::unique_ptr<D3D12_RECT> m_Scissor_rect;//シザー矩形
		//シーンを構成するバッファまわり
		ComPtr<ID3D12Resource> m_Scene_const_buff = nullptr;

		struct SceneData {
			DirectX::XMMATRIX view;//ビュー行列
			DirectX::XMMATRIX proj;//プロジェクション行列
			DirectX::XMFLOAT3 eye;//視点座標
		};
		SceneData* m_pMapped_scene_data;
		ComPtr<ID3D12DescriptorHeap> m_Scene_desc_heap = nullptr;

		//フェンス
		ComPtr<ID3D12Fence> m_Fence = nullptr;
		UINT64 m_Fence_val = 0;

		//ロード用テーブル
		using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
		std::map<std::string, LoadLambda_t> m_Load_lambda_table;
		//テクスチャテーブル
		std::unordered_map<std::string, ComPtr<ID3D12Resource>> m_Texture_table;
		std::vector<int> m_Func_count;
		std::vector<int> m_Func_count_3d;
	public:
		DirectX12Manager(HWND hwnd);
		~DirectX12Manager();
		bool init(SIZE pix);
		//void draw(std::shared_ptr<PMDRenderer> renderer);
		//void update();
		void clearScreen();
		void commandClear();
		void setNextCommand();
		///テクスチャパスから必要なテクスチャバッファへのポインタを返す
		///@param texpath テクスチャファイルパス
		ComPtr<ID3D12Resource> getTextureByPath(const char* texpath);
		ComPtr<ID3D12Device> device();//デバイス
		ComPtr<ID3D12GraphicsCommandList> cmdList();//コマンドリスト
		ComPtr<IDXGISwapChain4> swapchain();//スワップチェイン
		D3D12_VIEWPORT* getViewPort();//ビューポート
		D3D12_RECT* getScissorrect();//シザー矩形

		void clearCount();
		void screenFlip();
		void setScene();
		void setCount(int Handle, int num);
		void setCount3D(int Handle, int num);
		int getCount(int Handle);
		void setPixelSize(SIZE pix);
		int getCount3D(int Handle);
		SIZE getWinSize();
		SIZE getPixelSize();

	private:
		//最終的なレンダーターゲットの生成
		HRESULT	createFinalRenderTargets();
		//デプスステンシルビューの生成
		HRESULT createDepthStencilView();
		//スワップチェインの生成
		HRESULT createSwapChain(const HWND& hwnd, SIZE pix);
		//DXGIまわり初期化
		HRESULT initializeDXGIDevice();
		//コマンドまわり初期化
		HRESULT initializeCommand();
		//ビュープロジェクション用ビューの生成
		//HRESULT CreateSceneView();
		//テクスチャローダテーブルの作成
		//void CreateTextureLoaderTable();
		//テクスチャ名からテクスチャバッファ作成、中身をコピー
		ID3D12Resource* createTextureFromFile(const char* texpath);
	};
}