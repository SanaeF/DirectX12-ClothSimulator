#pragma once
#include<d3dx12.h>
#include<DirectXMath.h>
#include<DirectXTex.h>
#include<map>
namespace lib {
	class DirectX12Manager;
}
namespace libGraph {
	class Dx2DMatrix {
	private:
		ID3D12Resource* m_pConst_buffer = nullptr;
		//ID3D12Resource* mInstBuffer = nullptr;
		struct MatricesData {
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX viewproj;
		};
		MatricesData* m_pMatrix_data;
		DirectX::XMMATRIX* m_pConst_matrix;
		DirectX::XMMATRIX m_matrix;
		DirectX::XMMATRIX* m_pMap_matrix;
		DirectX::XMMATRIX m_Worl_mat;
		DirectX::XMMATRIX m_Look_at;
		DirectX::XMMATRIX m_Perspective_fov;
		D3D12_SHADER_RESOURCE_VIEW_DESC m_Inst_srv_desc = {};
	public:
		void createBuffer(lib::DirectX12Manager& dx_12);
		void createBuffer3D(lib::DirectX12Manager& dx_12);
		void changeMatrix(
			DirectX::XMMATRIX* pMatrix,
			float x,
			float y,
			float size,
			float RotaZ
		);
		void changeMatrix3D(
			DirectX::XMMATRIX* pMatrix,
			float x,
			float y,
			float z,
			float size,
			float RotaZ
		);
		ID3D12Resource* getConstBuffer();
		DirectX::XMMATRIX* getMatData();
	private:
		void rotaInitialize(lib::DirectX12Manager& dx_12);
	};
}