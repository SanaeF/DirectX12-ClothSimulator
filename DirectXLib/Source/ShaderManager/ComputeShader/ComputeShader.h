#pragma once
#include <utility>
#include <vector>
#include<d3dx12.h>
#include <wrl.h>
#include "../../DxGraphics/DxGraphics3D/ModelData/ModelData.h"
#include "../../Phisicas/ClothSimulator/ClothData/SpringData.h"
namespace lib {
    class DirectX12Manager;
	class ComputeShader {
	private:
        struct Element{
            int size_of;
            int num;
        };
        struct ComputeShaderInfo {
            ID3DBlob* shader;
            ID3D12RootSignature* root_sig;
            ID3D12PipelineState* pipeline_state;
            ID3D12DescriptorHeap* desc_heap;
            std::vector<ID3D12Resource*> resource;
            std::vector<Element> element;
            std::vector<void*> data;
        };
        std::shared_ptr<lib::DirectX12Manager> m_Dx12;
        static std::vector<ComputeShaderInfo> m_Shader_handle;
        static int m_Handle_count;
        int m_Handle_id;
    public:
        //シェーダーに送るデータ構造の個数を指定
        ComputeShader(const char* file_path, const char* func_name, int slot_max, int& handle, std::shared_ptr<lib::DirectX12Manager> dx12);
        //生成済みのシェーダー用の初期化
        ComputeShader(int handle, std::shared_ptr<lib::DirectX12Manager> dx12);

        //送るデータのサイズをスロット指定で配置
        void inputBufferSize(int slot_id, int size, int type);

        //アライメントするかどうか
        void createResource(bool is_alingnment, int slot_id);

        //UAVが作られます
        void createUnorderdAccessView();

        //送信用データのマップ
        void mapInput(int slot_id, const void* data);

        //取得用データのマップ
        void mapOutput(int slot_id);

        //シェーダー実行
        void execution(int x, int y, int z);

        void* getData(int slot_id);
    private:
        //コンパイルするシェーダーファイルの読み込み
        void loadShaderFile(const char* file_path, const char* func_name);

        //パイプラインステイトの作成
        void createPipline();

        //ヒープの作成
        void createHeap(int slot_max);
	};
}