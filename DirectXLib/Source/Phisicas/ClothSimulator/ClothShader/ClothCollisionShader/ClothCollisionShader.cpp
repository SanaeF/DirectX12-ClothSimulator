#include "ClothCollisionShader.h"
#include "../../DirectXLib/Source/DirectX12Manager/DirectX12Manager.h"
#include "CollisionSort/CollisionSort.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
namespace phy {
	std::vector<ClothCollisionShader::HandleData> ClothCollisionShader::m_Data_handle;

	ClothCollisionShader::ClothCollisionShader(
		int id,
		DirectX::XMFLOAT3 max, DirectX::XMFLOAT3 min,
		std::shared_ptr<lib::DirectX12Manager>& dx12):
		m_Model_id(id),
		m_High_pos(max),
		m_Low_pos(min),
		m_Dx12(dx12)
	{
		if (m_Data_handle.size() <= m_Model_id)m_Data_handle.resize(m_Model_id + 1);
	}
	void ClothCollisionShader::create(
		lib::ModelData& model,
		std::vector<SpringData>& spring,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		if (65535 < model.vertex.size()) {
			assert(0 && "<このモデルは上限を超えています!>DirectX12の使用により、クロスシミュレーターに使用できる頂点数は65535個が限界です。");
		}
		m_Data_handle[m_Model_id].output.resize(model.vertex.size());
		m_Data_handle[m_Model_id].input_model.resize(model.vertex.size());
		m_Data_handle[m_Model_id].input_space.resize(XYZ_ALL);
		CollisionSort sort(model.vertex.size(), m_High_pos, m_Low_pos);
		for (int ite = 0; ite < XYZ_ALL; ite++)m_Data_handle[m_Model_id].input_space[ite].space_ount = 0;
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			m_Data_handle[m_Model_id].input_model[ite].cloth.pos = model.vertex[ite].position;
			m_Data_handle[m_Model_id].input_model[ite].cloth.spring = spring[ite];
			m_Data_handle[m_Model_id].input_model[ite].area_id = sort.spaceInput(ite, m_Data_handle[m_Model_id].input_model[ite].cloth.pos);
			int area_id = m_Data_handle[m_Model_id].input_model[ite].area_id;
			if (m_Data_handle[m_Model_id].input_space[area_id].space_ount>=47) {
				for (int ite2 = 0; ite2 < XYZ_ALL; ite2++) {
					if (m_Data_handle[m_Model_id].input_space[ite2].space_ount < 47) {
						area_id = ite2;
						break;
					}
				}
			}
			m_Data_handle[m_Model_id].input_space[m_Data_handle[m_Model_id].input_space[area_id].space_ount].spaceID[area_id] = ite;
			m_Data_handle[m_Model_id].input_space[area_id].space_ount++;
		}
		createDataInOutter(model, mass_spring_id);
		if (!createDataInputter()) {
			m_Data_handle[m_Model_id].is_created = false;
			return;
		}
		m_Data_handle[m_Model_id].is_created = true;
	}
	void ClothCollisionShader::execution() {
		m_Dx12->cmdList()->SetComputeRootSignature(m_Data_handle[m_Model_id].root_sig);
		m_Dx12->cmdList()->SetPipelineState(m_Data_handle[m_Model_id].pipeline);
		m_Dx12->cmdList()->SetDescriptorHeaps(1, &m_Data_handle[m_Model_id].desc_heap);
		auto handle = m_Data_handle[m_Model_id].desc_heap->GetGPUDescriptorHandleForHeapStart();
		m_Dx12->cmdList()->SetComputeRootDescriptorTable(0, handle);
		//コンピュートシェーダーの実行
		m_Dx12->cmdList()->Dispatch(m_Data_handle[m_Model_id].thread.x, m_Data_handle[m_Model_id].thread.y, 1);
		dataAssign();
	}
	void ClothCollisionShader::createDataInOutter(
		lib::ModelData& model,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		if (m_Data_handle[m_Model_id].is_created)return;
		int size = sqrt(m_Data_handle[m_Model_id].input_model.size());
		int size_out = m_Data_handle[m_Model_id].input_model.size() - (size * size);
		auto& thread = m_Data_handle[m_Model_id].thread;
		thread.x = size + size_out;
		thread.y = size;
		createMassSpringforGPU(model, mass_spring_id);
		if (!loadShader())return;
		if (!createPipeline())return;
		if (!createHeap())return;
		if (!createOutputResource())return;
		if (!createUAV())return;
		if (!outputMap())return;
		m_Is_looped = false;
	}
	bool ClothCollisionShader::createDataInputter() {
		if (!m_Data_handle[m_Model_id].is_created) {
			if (!createInputResource())return false;
			if (!createInputUAV())return false;
			if (!createInput2Resource())return false;
			if (!createInput2UAV())return false;
		}
		if (!inputMap())return false;
		if (!input2Map())return false;
		return true;
	}
	void ClothCollisionShader::createMassSpringforGPU(
		lib::ModelData& model,
		std::vector<std::vector<int>>& mass_spring_id
	) {
		auto& input = m_Data_handle[m_Model_id].input_model;
		input[0].cloth.vertex_size = model.vertex.size();
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			/*input[ite].cloth.pre_pos = model.pre_vert[ite].position;
			input[ite].cloth.color = model.pre_vert[ite].color;*/
			input[ite].cloth.id0 = mass_spring_id[ite][0];
			input[ite].cloth.id1 = mass_spring_id[ite][1];
			input[ite].cloth.id2 = mass_spring_id[ite][2];
			input[ite].cloth.id3 = mass_spring_id[ite][3];
			input[ite].cloth.id4 = mass_spring_id[ite][4];
			input[ite].cloth.id5 = mass_spring_id[ite][5];
			input[ite].cloth.id6 = mass_spring_id[ite][6];
			input[ite].cloth.id7 = mass_spring_id[ite][7];
			input[ite].cloth.id8 = mass_spring_id[ite][8];
			input[ite].cloth.id9 = mass_spring_id[ite][9];
			input[ite].cloth.id10 = mass_spring_id[ite][10];
			input[ite].cloth.id11 = mass_spring_id[ite][11];
		}
	}
	bool ClothCollisionShader::loadShader() {
		auto result = D3DCompileFromFile(
			L"./DirectXLib/Shader/Shader3D/Cloth/ClothCollider.hlsl", nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, "ClothCollider", "cs_5_1",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &m_Shader, nullptr
		);
		ID3DBlob* sig = nullptr;
		result = D3DGetBlobPart(m_Shader->GetBufferPointer(), m_Shader->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		//ルートシグネチャの生成
		result = m_Dx12->device()->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(), IID_PPV_ARGS(&m_Data_handle[m_Model_id].root_sig));
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothCollisionShader::createPipeline() {
		D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
		desc.CS.pShaderBytecode = m_Shader->GetBufferPointer();
		desc.CS.BytecodeLength = m_Shader->GetBufferSize();
		desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.pRootSignature = m_Data_handle[m_Model_id].root_sig;
		auto hr = m_Dx12->device()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&m_Data_handle[m_Model_id].pipeline));

		return true;
	}
	bool ClothCollisionShader::createHeap() {
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = 0;
		desc.NumDescriptors = 3;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		auto result = m_Dx12->device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Data_handle[m_Model_id].desc_heap));
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothCollisionShader::createOutputResource() {
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;
		//サイズは定数バッファと同じように指定
		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Height = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.MipLevels = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Width = (sizeof(ColliderData) * m_Data_handle[m_Model_id].output.size() + 0xff) & ~0xff;
		auto result = m_Dx12->device()->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&m_Data_handle[m_Model_id].output_res));
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothCollisionShader::createUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = m_Data_handle[m_Model_id].output.size();
		desc.Buffer.StructureByteStride = sizeof(ColliderData);

		m_Data_handle[m_Model_id].desc_handle = m_Data_handle[m_Model_id].desc_heap->GetCPUDescriptorHandleForHeapStart();
		m_Dx12->device()->CreateUnorderedAccessView(m_Data_handle[m_Model_id].output_res, nullptr, &desc, m_Data_handle[m_Model_id].desc_handle);
		m_Data_handle[m_Model_id].desc_handle.ptr += m_Dx12->device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		return true;
	}
	bool ClothCollisionShader::outputMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pOutputDataBegin;
		auto result = m_Data_handle[m_Model_id].output_res->Map(0, &range, &m_Data_handle[m_Model_id].data);
		return true;
	}
	bool ClothCollisionShader::createInputResource() {
		const UINT buffer_size = static_cast<UINT>(m_Data_handle[m_Model_id].input_model.size()) * sizeof(ColliderData);
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Height = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.MipLevels = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Width = buffer_size;
		auto result = m_Dx12->device()->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_Data_handle[m_Model_id].in_model_res)
		);
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothCollisionShader::createInputUAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = m_Data_handle[m_Model_id].input_model.size();
		desc.Buffer.StructureByteStride = sizeof(ColliderData);
		m_Dx12->device()->CreateUnorderedAccessView(m_Data_handle[m_Model_id].in_model_res, nullptr, &desc, m_Data_handle[m_Model_id].desc_handle);
		m_Data_handle[m_Model_id].desc_handle.ptr += m_Dx12->device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		return true;
	}
	bool ClothCollisionShader::inputMap() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pInputDataBegin;
		auto& info = m_Data_handle[m_Model_id];
		auto result = info.in_model_res->Map(0, &range, reinterpret_cast<void**>(&pInputDataBegin));
		memcpy(
			pInputDataBegin,
			info.input_model.data(),
			static_cast<UINT>(info.input_model.size()) * sizeof(ColliderData)
		);
		info.in_model_res->Unmap(0, nullptr);
		if (!SUCCEEDED(result))return false;
		return true;
	}

	bool ClothCollisionShader::createInput2Resource() {
		const UINT buffer_size = static_cast<UINT>(m_Data_handle[m_Model_id].input_space.size()) * sizeof(SpaceData);
		D3D12_HEAP_PROPERTIES prop{};
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		prop.CreationNodeMask = 1;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
		prop.Type = D3D12_HEAP_TYPE_CUSTOM;
		prop.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC desc{};
		desc.Alignment = 0;
		desc.DepthOrArraySize = 1;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Height = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.MipLevels = 1;
		desc.SampleDesc = { 1, 0 };
		desc.Width = buffer_size;
		auto result = m_Dx12->device()->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&m_Data_handle[m_Model_id].in_space_res)
		);
		if (!SUCCEEDED(result))return false;
		return true;
	}
	bool ClothCollisionShader::createInput2UAV() {
		D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Buffer.NumElements = m_Data_handle[m_Model_id].input_space.size();
		desc.Buffer.StructureByteStride = sizeof(SpaceData);
		m_Dx12->device()->CreateUnorderedAccessView(m_Data_handle[m_Model_id].in_space_res, nullptr, &desc, m_Data_handle[m_Model_id].desc_handle);
		m_Data_handle[m_Model_id].desc_handle.ptr += m_Dx12->device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		return true;
	}
	bool ClothCollisionShader::input2Map() {
		D3D12_RANGE range{ 0, 0 };
		UINT8* pInputDataBegin;
		auto& info = m_Data_handle[m_Model_id];
		auto result = info.in_space_res->Map(0, &range, reinterpret_cast<void**>(&pInputDataBegin));
		memcpy(
			pInputDataBegin,
			info.input_space.data(),
			static_cast<UINT>(info.input_space.size()) * sizeof(SpaceData)
		);
		info.in_space_res->Unmap(0, nullptr);
		if (!SUCCEEDED(result))return false;
		return true;
	}

	void ClothCollisionShader::dataAssign() {
		auto& hd = m_Data_handle[m_Model_id];
		hd.output.assign(
			(ColliderData*)hd.data,
			(ColliderData*)hd.data + hd.output.size());
	}
	void ClothCollisionShader::dataChange(
		int model_id, lib::ModelData& model, std::vector<SpringData>& spring
	) {
		auto& output = m_Data_handle[m_Model_id].output;
		if (!output[0].cloth.simulate)return;
		for (int ite = 0; ite < model.vertex.size(); ite++) {
			model.vertex[ite].position = output[ite].cloth.pos;
			//spring[ite].force = output[ite].cloth.spring.force;
		}
	}
}