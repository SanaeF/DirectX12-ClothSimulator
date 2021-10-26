#include "DxGraphicsMatrix.h"

#include "../../DirectX12Manager/DirectX12Manager.h"


using namespace DirectX;

namespace libGraph {
	void Dx2DMatrix::createBuffer(lib::DirectX12Manager& dx_12) {
		rotaInitialize(dx_12);
		int instNum = 30;
		m_matrix = XMMatrixIdentity();
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper構造体
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(m_matrix) + 0xff) & ~0xff);//Helper構造体
		dx_12.device()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_pConst_buffer)
		);
		auto result = m_pConst_buffer->Map(0, nullptr, (void**)&m_pMap_matrix);
		*m_pMap_matrix = m_matrix * m_Look_at * m_Perspective_fov;
		m_pConst_buffer->Unmap(0, nullptr);
	}
	void Dx2DMatrix::createBuffer3D(lib::DirectX12Manager& DxWrap) {
		rotaInitialize(DxWrap);
		int instNum = 30;
		DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(XM_PI);
		m_matrix = XMMatrixIdentity();
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);//Helper構造体
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(MatricesData) + 0xff) & ~0xff);//Helper構造体
		DxWrap.device()->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_pConst_buffer)
		);
		auto result = m_pConst_buffer->Map(0, nullptr, (void**)&m_pMatrix_data);
		m_pMatrix_data->world = m_matrix;
		m_pMatrix_data->eye = m_Eye;
		m_pMatrix_data->view = m_Look_at;
		m_pMatrix_data->proj = m_Perspective_fov;
		//mConstBuffer->Unmap(0, nullptr);
	}
	void Dx2DMatrix::changeMatrix(
		DirectX::XMMATRIX* pMatrix,
		float x,
		float y,
		float size,
		float RotaZ
	) {
		m_Worl_mat = DirectX::XMMatrixTransformation2D(
			XMVectorSet(0, 0, 0, 0),
			0,
			XMVectorSet(size, size, 0, 0),
			XMVectorSet(0, 0, 0, 0),
			RotaZ,
			XMVectorSet(x, y, 0, 0)
		);
		*pMatrix = m_Worl_mat * m_Look_at * m_Perspective_fov;
	}

	void Dx2DMatrix::changeMatrix3D(
		DirectX::XMMATRIX* pMatrix,
		float x,
		float y,
		float z,
		float size,
		float RotaZ
	) {
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(x, y, z);
		//DirectX::XMMATRIX rotate_x = DirectX::XMMatrixRotationX(XM_PI/6);
		//DirectX::XMMATRIX rotate_y = DirectX::XMMatrixRotationY(XM_PI / 2);
		//DirectX::XMMATRIX rotate_z = DirectX::XMMatrixRotationZ(XM_PI);
		DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(size, size, size);
		m_Worl_mat = translate;
		m_pMatrix_data->world = m_Worl_mat;
	}


	void Dx2DMatrix::rotaInitialize(lib::DirectX12Manager& dx_12) {
		XMFLOAT3 eye(0, 0, -1);
		XMFLOAT3 target(0, 0, 0);
		XMFLOAT3 up(0, 1, 0);
		float pixW = static_cast<float>(dx_12.getPixelSize().cx);
		float pixH = static_cast<float>(dx_12.getPixelSize().cy);
		m_Look_at = XMMatrixLookAtLH(
			XMLoadFloat3(&eye),
			XMLoadFloat3(&target),
			XMLoadFloat3(&up)
		);
		m_Perspective_fov = XMMatrixPerspectiveFovLH(
			XM_PIDIV2,
			pixW / pixH,
			1.f,
			10.f
		);
		m_Eye = eye;
	}

	ID3D12Resource*
		Dx2DMatrix::getConstBuffer() {
		return m_pConst_buffer;
	}

	DirectX::XMMATRIX*
		Dx2DMatrix::getMatData() {

		return m_pMap_matrix;
	}
}