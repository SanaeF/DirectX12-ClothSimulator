//#ifdef _DEBUG
//#include<iostream>
//
//#endif
//
//#include "GiDirect.h"
//
//
//#ifdef _DEBUG
//
//
////int main() {
////#else
////#include<Windows.h>
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//#endif
//	//Debug.OutputFormatString("Show window test.");
//
//	Win.set(1280, 960, "DX12�e�X�g");//�E�B���h�E�T�C�Y�w��
//
//#ifdef _DEBUG
//	//�f�o�b�O���C���[���I����
//	Debug.EnableLayer();
//#endif
//	////DirectX12�܂�菉����
//	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_dxgiFactory)))) {
//		if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&_dxgiFactory)))) {
//			return -1;
//		}
//	}
//
//
//	GiDirect.Init();//������
//
//
//	Win.setResol(1920, 1440);//�E�B���h�E��ʂ̉𑜓x
//
//
//	GiDirect.heap();//�q�[�v�ݒ�
//
//	DXGI_SWAP_CHAIN_DESC swcDesc = {};
//	result = _swapchain->GetDesc(&swcDesc);
//	std::vector<ID3D12Resource*> _backBuffers(swcDesc.BufferCount);
//	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
//
//
//	//SRGB�����_�[�^�[�Q�b�g�r���[�ݒ�
//	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
//	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//
//
//	//�摜���Â��Ȃ�ꍇ
//	//for (int i = 0; i < swcDesc.BufferCount; ++i) {
//	//	result = _swapchain->GetBuffer(i, IID_PPV_ARGS(&_backBuffers[i]));
//	//	_dev->CreateRenderTargetView(_backBuffers[i], &rtvDesc, handle);
//	//	handle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//	//}
//
//	//����ɕ\�����ꂿ������ꍇ
//	for (int i = 0; i < swcDesc.BufferCount; ++i) {
//		result = _swapchain->GetBuffer(i, IID_PPV_ARGS(&_backBuffers[i]));
//		_dev->CreateRenderTargetView(_backBuffers[i], nullptr, handle);
//		handle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//	}
//
//
//	ID3D12Fence* _fence = nullptr;
//	UINT64 _fenceVal = 0;
//	result = _dev->CreateFence(_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
//
//	ShowWindow(hwnd, SW_SHOW);//�E�B���h�E�\��
//
//
//	struct PMDHeader {
//		float version; //��F00 00 80 3F == 1.00
//		char model_name[20];//���f����
//		char comment[256];//���f���R�����g
//	};
//
//	FILE*fp;
//
//	char signature[3];
//	PMDHeader pmdheader = {};
//	fopen_s(&fp, "model/���V����~�N.pmd", "rb");
//	fread(signature, sizeof(signature), 1, fp);
//	fread(&pmdheader, sizeof(pmdheader), 1, fp);
//
//	constexpr unsigned int pmdvertex_size = 38;
//
//	unsigned int vertNum;
//
//	fread(&vertNum, sizeof(vertNum), 1, fp);
//	std::vector<unsigned char>vertices(vertNum* pmdvertex_size);
//
//	fread(vertices.data(), vertices.size(), 1, fp);
//
//	ID3D12Resource* vertBuff = nullptr;
//
//	result = _dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer(vertices.size()),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&vertBuff));
//
//
//	fclose(fp);
//
//
//	///////////////2D�`�揉����
//
//
//
//	//struct Vertex {
//	//	XMFLOAT3 pos;//XYZ���W
//	//	XMFLOAT2 uv;//UV���W
//	//};
//
//	//Vertex vertices[] = {
//	//	{{-1.0f,-1.0f,0.0f},{0.0f,1.0f} },//����
//	//	{{-1.0f,1.0f,0.0f} ,{0.0f,0.0f}},//����
//	//	{{1.0f,-1.0f,0.0f} ,{1.0f,1.0f}},//�E��
//	//	{{1.0f,1.0f,0.0f} ,{1.0f,0.0f}},//�E��
//	//};
//
//	D3D12_HEAP_PROPERTIES heapprop = {};
//	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
//	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//
//	D3D12_RESOURCE_DESC resdesc = {};
//	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resdesc.Width = sizeof(vertices);
//	resdesc.Height = 1;
//	resdesc.DepthOrArraySize = 1;
//	resdesc.MipLevels = 1;
//	resdesc.Format = DXGI_FORMAT_UNKNOWN;
//	resdesc.SampleDesc.Count = 1;
//	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
//	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//
//	////UPLOAD(�m�ۂ͉\)
//	//ID3D12Resource* vertBuff = nullptr;
//	//result = _dev->CreateCommittedResource(
//	//	&heapprop,
//	//	D3D12_HEAP_FLAG_NONE,
//	//	&resdesc,
//	//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	//	nullptr,
//	//	IID_PPV_ARGS(&vertBuff));
//
//
//	//Vertex* vertMap = nullptr;
//	//result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//
//	//std::copy(std::begin(vertices), std::end(vertices), vertMap);
//
//	//vertBuff->Unmap(0, nullptr);
//
//	D3D12_VERTEX_BUFFER_VIEW vbView = {};
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();//�o�b�t�@�̉��z�A�h���X
//	vbView.SizeInBytes = vertices.size();//�S�o�C�g��
//	vbView.StrideInBytes = pmdvertex_size;//1���_������̃o�C�g��
//
//
//	unsigned char* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//
//	std::copy(std::begin(vertices), std::end(vertices), vertMap);
//
//	unsigned short indices[] = { 0,1,2, 2,1,3 };
//
//	ID3D12Resource* idxBuff = nullptr;
//	//�ݒ�́A�o�b�t�@�̃T�C�Y�ȊO���_�o�b�t�@�̐ݒ���g���܂킵��
//	//OK���Ǝv���܂��B
//	resdesc.Width = sizeof(indices);
//	result = _dev->CreateCommittedResource(
//		&heapprop,
//		D3D12_HEAP_FLAG_NONE,
//		&resdesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&idxBuff));
//
//	//������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
//	unsigned short* mappedIdx = nullptr;
//	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
//	std::copy(std::begin(indices), std::end(indices), mappedIdx);
//	idxBuff->Unmap(0, nullptr);
//
//	//�C���f�b�N�X�o�b�t�@�r���[���쐬
//	D3D12_INDEX_BUFFER_VIEW ibView = {};
//	ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
//	ibView.Format = DXGI_FORMAT_R16_UINT;
//	ibView.SizeInBytes = sizeof(indices);
//
//
//
//	ID3DBlob* _vsBlob = nullptr;
//	ID3DBlob* _psBlob = nullptr;
//
//	ID3DBlob* errorBlob = nullptr;
//	result = D3DCompileFromFile(L"BasicVertexShader.hlsl",
//		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
//		"BasicVS", "vs_5_0",
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//		0, &_vsBlob, &errorBlob);
//	if (FAILED(result)) {
//		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
//			::OutputDebugStringA("�t�@�C������������܂���");
//		}
//		else {
//			std::string errstr;
//			errstr.resize(errorBlob->GetBufferSize());
//			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
//			errstr += "\n";
//			OutputDebugStringA(errstr.c_str());
//		}
//		exit(1);//�s�V�������ȁc
//	}
//	result = D3DCompileFromFile(L"BasicPixelShader.hlsl",
//		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
//		"BasicPS", "ps_5_0",
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//		0, &_psBlob, &errorBlob);
//	if (FAILED(result)) {
//		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
//			::OutputDebugStringA("�t�@�C������������܂���");
//		}
//		else {
//			std::string errstr;
//			errstr.resize(errorBlob->GetBufferSize());
//			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
//			errstr += "\n";
//			OutputDebugStringA(errstr.c_str());
//		}
//		exit(1);//�s�V�������ȁc
//	}
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
//			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//			{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//			{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//			{ "BONE_NO",0,DXGI_FORMAT_R16G16_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//			{ "WEIGHT",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//			//{ "EDGE_FLG",0,DXGI_FORMAT_R8_UINT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//	};
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
//	gpipeline.pRootSignature = nullptr;
//	gpipeline.VS.pShaderBytecode = _vsBlob->GetBufferPointer();
//	gpipeline.VS.BytecodeLength = _vsBlob->GetBufferSize();
//	gpipeline.PS.pShaderBytecode = _psBlob->GetBufferPointer();
//	gpipeline.PS.BytecodeLength = _psBlob->GetBufferSize();
//
//	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//���g��0xffffffff
//
//	//
//	gpipeline.BlendState.AlphaToCoverageEnable = true;//�A���t�@�l���v�Z���邩?
//	gpipeline.BlendState.IndependentBlendEnable = false;
//
//	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
//
//	//�ЂƂ܂����Z���Z�⃿�u�����f�B���O�͎g�p���Ȃ�
//	renderTargetBlendDesc.BlendEnable = false;
//	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	//�ЂƂ܂��_�����Z�͎g�p���Ȃ�
//	renderTargetBlendDesc.LogicOpEnable = false;
//
//	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;
//
//	gpipeline.RasterizerState.MultisampleEnable = false;//�܂��A���`�F���͎g��Ȃ�
//	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
//	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//���g��h��Ԃ�
//	gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�����̃N���b�s���O�͗L����
//
//	//�c��
//	gpipeline.RasterizerState.FrontCounterClockwise = false;
//	gpipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
//	gpipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
//	gpipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
//	gpipeline.RasterizerState.AntialiasedLineEnable = false;
//	gpipeline.RasterizerState.ForcedSampleCount = 0;
//	gpipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
//
//
//	gpipeline.DepthStencilState.DepthEnable = false;
//	gpipeline.DepthStencilState.StencilEnable = false;
//
//	gpipeline.InputLayout.pInputElementDescs = inputLayout;//���C�A�E�g�擪�A�h���X
//	gpipeline.InputLayout.NumElements = _countof(inputLayout);//���C�A�E�g�z��
//
//	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//�X�g���b�v���̃J�b�g�Ȃ�
//	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//�O�p�`�ō\��
//
//	gpipeline.NumRenderTargets = 1;//���͂P�̂�
//	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`1�ɐ��K�����ꂽRGBA
//
//	gpipeline.SampleDesc.Count = 1;//�T���v�����O��1�s�N�Z���ɂ��P
//	gpipeline.SampleDesc.Quality = 0;//�N�I���e�B�͍Œ�
//
//	ID3D12RootSignature* rootsignature = nullptr;
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
//	descTblRange[0].NumDescriptors = 1;//�e�N�X�`���ЂƂ�
//	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//��ʂ̓e�N�X�`��
//	descTblRange[0].BaseShaderRegister = 0;//0�ԃX���b�g����
//	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//	descTblRange[1].NumDescriptors = 1;//�e�N�X�`���ЂƂ�
//	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//��ʂ̓e�N�X�`��
//	descTblRange[1].BaseShaderRegister = 0;//0�ԃX���b�g����
//	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//
//	D3D12_ROOT_PARAMETER rootparam = {};
//	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	rootparam.DescriptorTable.pDescriptorRanges = descTblRange;//�f�X�N���v�^�����W�̃A�h���X
//	rootparam.DescriptorTable.NumDescriptorRanges = 2;//�f�X�N���v�^�����W��
//	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����
//
//
//
//	rootSignatureDesc.pParameters = &rootparam;//���[�g�p�����[�^�̐擪�A�h���X
//	rootSignatureDesc.NumParameters = 1;//���[�g�p�����[�^��
//
//	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
//	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�
//	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�
//	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�
//	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
//	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//��Ԃ��Ȃ�(�j�A���X�g�l�C�o�[)
//	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
//	samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//�I�[�o�[�T���v�����O�̍ۃ��T���v�����O���Ȃ��H
//	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂݉�
//
//	rootSignatureDesc.pStaticSamplers = &samplerDesc;
//	rootSignatureDesc.NumStaticSamplers = 1;
//
//	ID3DBlob* rootSigBlob = nullptr;
//	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
//	result = _dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
//	rootSigBlob->Release();
//
//
//	gpipeline.pRootSignature = rootsignature;
//	ID3D12PipelineState* _pipelinestate = nullptr;
//	result = _dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&_pipelinestate));
//
//
//	int img_width2 = 2048, img_height2 = 2048;
//
//	int window_width = 2048, window_height = 2048;
//
//	D3D12_VIEWPORT viewport = {};
//	viewport.Width = img_width2;//�o�͐�̕�(�s�N�Z����)
//	viewport.Height = img_height2;//�o�͐�̍���(�s�N�Z����)
//	viewport.TopLeftX = 0;//�o�͐�̍�����WX
//	viewport.TopLeftY = 0;//�o�͐�̍�����WY
//	viewport.MaxDepth = 1.0f;//�[�x�ő�l
//	viewport.MinDepth = 0.0f;//�[�x�ŏ��l
//
//
//	D3D12_RECT scissorrect = {};
//	scissorrect.top = 0;//�؂蔲������W
//	scissorrect.left = 0;//�؂蔲�������W
//	scissorrect.right = scissorrect.left + window_width;//�؂蔲���E���W
//	scissorrect.bottom = scissorrect.top + window_height;//�؂蔲�������W
//
//
//	TexMetadata metadata = {};
//	ScratchImage scratchImg = {};
//	//result = LoadFromWICFile(L"dat/boad/board.png", WIC_FLAGS_NONE, &metadata, scratchImg);
//	result = LoadFromWICFile(L"dat/boad/titlechar.png", WIC_FLAGS_NONE, &metadata, scratchImg);
//	auto img = scratchImg.GetImage(0, 0, 0);//���f�[�^���o
//
//	D3D12_HEAP_PROPERTIES uploadHeapProp = {};
//	uploadHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//Upload�p
//	uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//	uploadHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
//	uploadHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0
//
//	D3D12_RESOURCE_DESC resDesc = {};
//	resDesc.Format = DXGI_FORMAT_UNKNOWN;
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;//�P�Ȃ�o�b�t�@�Ƃ���
//	auto pixelsize = scratchImg.GetPixelsSize();
//	resDesc.Width = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT)*img->height;//�f�[�^�T�C�Y
//
//	resDesc.Height = 1;//
//	resDesc.DepthOrArraySize = 1;//
//	resDesc.MipLevels = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;//�A�������f�[�^�ł���
//	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//�Ƃ��Ƀt���O�Ȃ�
//	resDesc.SampleDesc.Count = 1;//�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
//	resDesc.SampleDesc.Quality = 0;//
//
//
//	//���ԃo�b�t�@�쐬
//	ID3D12Resource* uploadbuff = nullptr;
//	result = _dev->CreateCommittedResource(
//		&uploadHeapProp,
//		D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
//		&resDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,//CPU���珑�����݉\
//		nullptr,
//		IID_PPV_ARGS(&uploadbuff)
//	);
//
//	//���Ƀe�N�X�`���̂��߂̃q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES texHeapProp = {};
//	texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//�e�N�X�`���p
//	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//	texHeapProp.CreationNodeMask = 0;//�P��A�_�v�^�̂���0
//	texHeapProp.VisibleNodeMask = 0;//�P��A�_�v�^�̂���0
//
//	//���\�[�X�ݒ�(�ϐ��͎g���܂킵)
//	resDesc.Format = metadata.format;
//	resDesc.Width = metadata.width;//��
//	resDesc.Height = metadata.height;//����
//	resDesc.DepthOrArraySize = metadata.arraySize;//2D�Ŕz��ł��Ȃ��̂łP
//	resDesc.MipLevels = metadata.mipLevels;//�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
//	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);//2D�e�N�X�`���p
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
//
//
//
//
//
//	//�e�N�X�`���o�b�t�@�쐬
//	ID3D12Resource* texbuff = nullptr;
//	result = _dev->CreateCommittedResource(
//		&texHeapProp,
//		D3D12_HEAP_FLAG_NONE,//���Ɏw��Ȃ�
//		&resDesc,
//		D3D12_RESOURCE_STATE_COPY_DEST,//�R�s�[��
//		nullptr,
//		IID_PPV_ARGS(&texbuff)
//	);
//	uint8_t* mapforImg = nullptr;//image->pixels�Ɠ����^�ɂ���
//	result = uploadbuff->Map(0, nullptr, (void**)&mapforImg);//�}�b�v
//	auto srcAddress = img->pixels;
//	auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
//	for (int y = 0; y < img->height; ++y) {
//		std::copy_n(srcAddress,
//			rowPitch,
//			mapforImg);//�R�s�[
//		//1�s���Ƃ̒�������킹�Ă��
//		srcAddress += img->rowPitch;
//		mapforImg += rowPitch;
//	}
//	uploadbuff->Unmap(0, nullptr);//�A���}�b�v
//
//	D3D12_TEXTURE_COPY_LOCATION src = {}, dst = {};
//	dst.pResource = texbuff;
//	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
//	dst.SubresourceIndex = 0;
//
//	src.pResource = uploadbuff;//���ԃo�b�t�@
//	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;//�t�b�g�v�����g�w��
//	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint = {};
//	UINT nrow;
//	UINT64 rowsize, size;
//	auto desc = texbuff->GetDesc();
//	_dev->GetCopyableFootprints(&desc, 0, 1, 0, &footprint, &nrow, &rowsize, &size);
//	src.PlacedFootprint = footprint;
//	src.PlacedFootprint.Offset = 0;
//	src.PlacedFootprint.Footprint.Width = metadata.width;
//	src.PlacedFootprint.Footprint.Height = metadata.height;
//	src.PlacedFootprint.Footprint.Depth = metadata.depth;
//	src.PlacedFootprint.Footprint.RowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
//	src.PlacedFootprint.Footprint.Format = img->format;
//
//
//
//
//	{
//		//_cmdAllocator->Reset();//�L���[���N���A
//		//_cmdList->Reset(_cmdAllocator, nullptr);
//
//		_cmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
//
//		D3D12_RESOURCE_BARRIER BarrierDesc = {};
//		BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//		BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//		BarrierDesc.Transition.pResource = texbuff;
//		BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//		BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
//		BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
//
//		_cmdList->ResourceBarrier(1, &BarrierDesc);
//		_cmdList->Close();
//		//�R�}���h���X�g�̎��s
//		ID3D12CommandList* cmdlists[] = { _cmdList };
//		_cmdQueue->ExecuteCommandLists(1, cmdlists);
//		////�҂�
//		_cmdQueue->Signal(_fence, ++_fenceVal);
//
//		if (_fence->GetCompletedValue() != _fenceVal) {
//			auto event = CreateEvent(nullptr, false, false, nullptr);
//			_fence->SetEventOnCompletion(_fenceVal, event);
//			WaitForSingleObject(event, INFINITE);
//			CloseHandle(event);
//		}
//		_cmdAllocator->Reset();//�L���[���N���A
//		_cmdList->Reset(_cmdAllocator, nullptr);
//
//
//
//	}
//
//
//
//	//�萔�o�b�t�@�쐬
//	auto worldMat = XMMatrixRotationY(XM_PIDIV4);
//	XMFLOAT3 eye(0, 10, -15);//<< eye(0,0,-5);
//	XMFLOAT3 target(0, 10, 0);
//	XMFLOAT3 up(0, 1, 0);
//	auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
//	auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2,//��p��90��
//		static_cast<float>(window_width) / static_cast<float>(window_height),//�A�X��
//		1.0f,//�߂���  << 1.0f
//		100.0f//������
//	);
//	ID3D12Resource* constBuff = nullptr;
//	result = _dev->CreateCommittedResource(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//		D3D12_HEAP_FLAG_NONE,
//		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(XMMATRIX) + 0xff)&~0xff),
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&constBuff)
//	);
//
//	XMMATRIX* mapMatrix;//�}�b�v��������|�C���^
//	result = constBuff->Map(0, nullptr, (void**)&mapMatrix);//�}�b�v
//	*mapMatrix = worldMat * viewMat*projMat;
//
//	ID3D12DescriptorHeap* basicDescHeap = nullptr;
//	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
//	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
//	descHeapDesc.NodeMask = 0;//�}�X�N��0
//	descHeapDesc.NumDescriptors = 2;//SRV1��CBV1��
//	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//�f�X�N���v�^�q�[�v���
//	result = _dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));//����
//
//	//�ʏ�e�N�X�`���r���[�쐬
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = metadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA(0.0f�`1.0f�ɐ��K��)
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//��q
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
//	srvDesc.Texture2D.MipLevels = 1;//�~�b�v�}�b�v�͎g�p���Ȃ��̂�1
//
//	//�f�X�N���v�^�̐擪�n���h�����擾���Ă���
//	auto basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
//	//�V�F�[�_���\�[�X�r���[�̍쐬
//	_dev->CreateShaderResourceView(texbuff, //�r���[�Ɗ֘A�t����o�b�t�@
//		&srvDesc, //��قǐݒ肵���e�N�X�`���ݒ���
//		basicHeapHandle
//	);
//	basicHeapHandle.ptr +=
//		_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
//	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
//	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
//	//�萔�o�b�t�@�r���[�̍쐬
//	_dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
//
//
//	MSG msg = {};
//	float angle = 0.0f;
//	while (true) {
//
//		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		//�����A�v���P�[�V�������I�����Ď���message��WM_QUIT�ɂȂ�
//		if (msg.message == WM_QUIT) {
//			break;
//		}
//
//		angle -= 0.05f;
//		worldMat = XMMatrixRotationY(angle);
//		*mapMatrix = worldMat * viewMat * projMat;
//
//
//		//DirectX����
//		//�o�b�N�o�b�t�@�̃C���f�b�N�X���擾
//		auto bbIdx = _swapchain->GetCurrentBackBufferIndex();
//
//		//D3D12_RESOURCE_BARRIER BarrierDesc = {};
//		//BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
//		//BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
//		//BarrierDesc.Transition.pResource = _backBuffers[bbIdx];
//		//BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
//		//BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
//		//BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
//
//		_cmdList->ResourceBarrier(1,
//			&CD3DX12_RESOURCE_BARRIER::Transition(_backBuffers[bbIdx],
//				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
//
//		_cmdList->SetPipelineState(_pipelinestate);
//
//
//		//�����_�[�^�[�Q�b�g���w��
//		auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
//		rtvH.ptr += bbIdx * _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//		_cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
//
//		//��ʃN���A
//
//
//		float clearColor[] = { 1.0f,1.0f,1.0f,1.0f };//���F
//		_cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
//
//
//		_cmdList->RSSetViewports(1, &viewport);
//		_cmdList->RSSetScissorRects(1, &scissorrect);
//		_cmdList->SetGraphicsRootSignature(rootsignature);
//
//		_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
//		_cmdList->IASetVertexBuffers(0, 1, &vbView);
//		_cmdList->IASetIndexBuffer(&ibView);
//
//		_cmdList->SetGraphicsRootSignature(rootsignature);
//		_cmdList->SetDescriptorHeaps(1, &basicDescHeap);
//		_cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());
//
//		//auto heapHandle=basicDescHeap->GetGPUDescriptorHandleForHeapStart();
//		//heapHandle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//		//_cmdList->SetGraphicsRootDescriptorTable(1, heapHandle);
//
//		//_cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
//		_cmdList->DrawInstanced(vertNum, 1, 0, 0);
//
//		_cmdList->ResourceBarrier(1,
//			&CD3DX12_RESOURCE_BARRIER::Transition(_backBuffers[bbIdx],
//				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
//
//		//���߂̃N���[�Y
//		_cmdList->Close();
//
//
//
//		//�R�}���h���X�g�̎��s
//		ID3D12CommandList* cmdlists[] = { _cmdList };
//		_cmdQueue->ExecuteCommandLists(1, cmdlists);
//		////�҂�
//		_cmdQueue->Signal(_fence, ++_fenceVal);
//
//		if (_fence->GetCompletedValue() != _fenceVal) {
//			auto event = CreateEvent(nullptr, false, false, nullptr);
//			_fence->SetEventOnCompletion(_fenceVal, event);
//			WaitForSingleObject(event, INFINITE);
//			CloseHandle(event);
//		}
//		_cmdAllocator->Reset();//�L���[���N���A
//		_cmdList->Reset(_cmdAllocator, _pipelinestate);//�ĂуR�}���h���X�g�����߂鏀��
//
//
//		//�t���b�v
//		_swapchain->Present(1, 0);
//
//	}
//	//�����N���X�g��񂩂�o�^�������Ă�
//	UnregisterClass(w.lpszClassName, w.hInstance);
//	return 0;
//}