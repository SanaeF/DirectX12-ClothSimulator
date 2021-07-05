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
		ID3D12Resource* mConstBuffer = nullptr;

		//ID3D12Resource* mInstBuffer = nullptr;
		struct MatricesData {
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX viewproj;
		};
		MatricesData* mapMatrix;
		DirectX::XMMATRIX* mConstMatrix;
		DirectX::XMMATRIX mMatrix;
		DirectX::XMMATRIX* mMapMatrix;

		DirectX::XMMATRIX mWorlMat;

		DirectX::XMMATRIX mLookAt;

		DirectX::XMMATRIX mPerspectiveFov;

		D3D12_SHADER_RESOURCE_VIEW_DESC mInstSRV_Desc = {};

	public:
		void createBuffer(lib::DirectX12Manager& DxWrap);
		void createBuffer3D(lib::DirectX12Manager& DxWrap);
		void ChangeMatrix(
			DirectX::XMMATRIX* pMatrix,
			float x,
			float y,
			float size,
			float RotaZ
		);
		void ChangeMatrix3D(
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

		void RotaInitialize(lib::DirectX12Manager& DxWrap);

	};
}