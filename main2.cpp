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
//	Win.set(1280, 960, "DX12テスト");//ウィンドウサイズ指定
//
//#ifdef _DEBUG
//	//デバッグレイヤーをオンに
//	Debug.EnableLayer();
//#endif
//	////DirectX12まわり初期化
//	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_dxgiFactory)))) {
//		if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&_dxgiFactory)))) {
//			return -1;
//		}
//	}
//
//
//	GiDirect.Init();//初期化
//
//
//	Win.setResol(1920, 1440);//ウィンドウ画面の解像度
//
//
//	GiDirect.heap();//ヒープ設定
//
//	DXGI_SWAP_CHAIN_DESC swcDesc = {};
//	result = _swapchain->GetDesc(&swcDesc);
//	std::vector<ID3D12Resource*> _backBuffers(swcDesc.BufferCount);
//	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
//
//
//	//SRGBレンダーターゲットビュー設定
//	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
//	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//
//
//	//画像が暗くなる場合
//	//for (int i = 0; i < swcDesc.BufferCount; ++i) {
//	//	result = _swapchain->GetBuffer(i, IID_PPV_ARGS(&_backBuffers[i]));
//	//	_dev->CreateRenderTargetView(_backBuffers[i], &rtvDesc, handle);
//	//	handle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//	//}
//
//	//正常に表示されちゃった場合
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
//	ShowWindow(hwnd, SW_SHOW);//ウィンドウ表示
//
//
//	struct PMDHeader {
//		float version; //例：00 00 80 3F == 1.00
//		char model_name[20];//モデル名
//		char comment[256];//モデルコメント
//	};
//
//	FILE*fp;
//
//	char signature[3];
//	PMDHeader pmdheader = {};
//	fopen_s(&fp, "model/鷺澤美咲ミク.pmd", "rb");
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
//	///////////////2D描画初期化
//
//
//
//	//struct Vertex {
//	//	XMFLOAT3 pos;//XYZ座標
//	//	XMFLOAT2 uv;//UV座標
//	//};
//
//	//Vertex vertices[] = {
//	//	{{-1.0f,-1.0f,0.0f},{0.0f,1.0f} },//左下
//	//	{{-1.0f,1.0f,0.0f} ,{0.0f,0.0f}},//左上
//	//	{{1.0f,-1.0f,0.0f} ,{1.0f,1.0f}},//右下
//	//	{{1.0f,1.0f,0.0f} ,{1.0f,0.0f}},//右上
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
//	////UPLOAD(確保は可能)
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
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();//バッファの仮想アドレス
//	vbView.SizeInBytes = vertices.size();//全バイト数
//	vbView.StrideInBytes = pmdvertex_size;//1頂点あたりのバイト数
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
//	//設定は、バッファのサイズ以外頂点バッファの設定を使いまわして
//	//OKだと思います。
//	resdesc.Width = sizeof(indices);
//	result = _dev->CreateCommittedResource(
//		&heapprop,
//		D3D12_HEAP_FLAG_NONE,
//		&resdesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&idxBuff));
//
//	//作ったバッファにインデックスデータをコピー
//	unsigned short* mappedIdx = nullptr;
//	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
//	std::copy(std::begin(indices), std::end(indices), mappedIdx);
//	idxBuff->Unmap(0, nullptr);
//
//	//インデックスバッファビューを作成
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
//			::OutputDebugStringA("ファイルが見当たりません");
//		}
//		else {
//			std::string errstr;
//			errstr.resize(errorBlob->GetBufferSize());
//			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
//			errstr += "\n";
//			OutputDebugStringA(errstr.c_str());
//		}
//		exit(1);//行儀悪いかな…
//	}
//	result = D3DCompileFromFile(L"BasicPixelShader.hlsl",
//		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
//		"BasicPS", "ps_5_0",
//		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
//		0, &_psBlob, &errorBlob);
//	if (FAILED(result)) {
//		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
//			::OutputDebugStringA("ファイルが見当たりません");
//		}
//		else {
//			std::string errstr;
//			errstr.resize(errorBlob->GetBufferSize());
//			std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
//			errstr += "\n";
//			OutputDebugStringA(errstr.c_str());
//		}
//		exit(1);//行儀悪いかな…
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
//	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//中身は0xffffffff
//
//	//
//	gpipeline.BlendState.AlphaToCoverageEnable = true;//アルファ値を計算するか?
//	gpipeline.BlendState.IndependentBlendEnable = false;
//
//	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
//
//	//ひとまず加算や乗算やαブレンディングは使用しない
//	renderTargetBlendDesc.BlendEnable = false;
//	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//
//	//ひとまず論理演算は使用しない
//	renderTargetBlendDesc.LogicOpEnable = false;
//
//	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;
//
//	gpipeline.RasterizerState.MultisampleEnable = false;//まだアンチェリは使わない
//	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
//	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//中身を塗りつぶす
//	gpipeline.RasterizerState.DepthClipEnable = true;//深度方向のクリッピングは有効に
//
//	//残り
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
//	gpipeline.InputLayout.pInputElementDescs = inputLayout;//レイアウト先頭アドレス
//	gpipeline.InputLayout.NumElements = _countof(inputLayout);//レイアウト配列数
//
//	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//ストリップ時のカットなし
//	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;//三角形で構成
//
//	gpipeline.NumRenderTargets = 1;//今は１つのみ
//	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0～1に正規化されたRGBA
//
//	gpipeline.SampleDesc.Count = 1;//サンプリングは1ピクセルにつき１
//	gpipeline.SampleDesc.Quality = 0;//クオリティは最低
//
//	ID3D12RootSignature* rootsignature = nullptr;
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
//	descTblRange[0].NumDescriptors = 1;//テクスチャひとつ
//	descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//種別はテクスチャ
//	descTblRange[0].BaseShaderRegister = 0;//0番スロットから
//	descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//	descTblRange[1].NumDescriptors = 1;//テクスチャひとつ
//	descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;//種別はテクスチャ
//	descTblRange[1].BaseShaderRegister = 0;//0番スロットから
//	descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//
//	D3D12_ROOT_PARAMETER rootparam = {};
//	rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	rootparam.DescriptorTable.pDescriptorRanges = descTblRange;//デスクリプタレンジのアドレス
//	rootparam.DescriptorTable.NumDescriptorRanges = 2;//デスクリプタレンジ数
//	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダから見える
//
//
//
//	rootSignatureDesc.pParameters = &rootparam;//ルートパラメータの先頭アドレス
//	rootSignatureDesc.NumParameters = 1;//ルートパラメータ数
//
//	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
//	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し
//	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し
//	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し
//	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
//	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//補間しない(ニアレストネイバー)
//	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
//	samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//オーバーサンプリングの際リサンプリングしない？
//	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ可視
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
//	viewport.Width = img_width2;//出力先の幅(ピクセル数)
//	viewport.Height = img_height2;//出力先の高さ(ピクセル数)
//	viewport.TopLeftX = 0;//出力先の左上座標X
//	viewport.TopLeftY = 0;//出力先の左上座標Y
//	viewport.MaxDepth = 1.0f;//深度最大値
//	viewport.MinDepth = 0.0f;//深度最小値
//
//
//	D3D12_RECT scissorrect = {};
//	scissorrect.top = 0;//切り抜き上座標
//	scissorrect.left = 0;//切り抜き左座標
//	scissorrect.right = scissorrect.left + window_width;//切り抜き右座標
//	scissorrect.bottom = scissorrect.top + window_height;//切り抜き下座標
//
//
//	TexMetadata metadata = {};
//	ScratchImage scratchImg = {};
//	//result = LoadFromWICFile(L"dat/boad/board.png", WIC_FLAGS_NONE, &metadata, scratchImg);
//	result = LoadFromWICFile(L"dat/boad/titlechar.png", WIC_FLAGS_NONE, &metadata, scratchImg);
//	auto img = scratchImg.GetImage(0, 0, 0);//生データ抽出
//
//	D3D12_HEAP_PROPERTIES uploadHeapProp = {};
//	uploadHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//Upload用
//	uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//	uploadHeapProp.CreationNodeMask = 0;//単一アダプタのため0
//	uploadHeapProp.VisibleNodeMask = 0;//単一アダプタのため0
//
//	D3D12_RESOURCE_DESC resDesc = {};
//	resDesc.Format = DXGI_FORMAT_UNKNOWN;
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;//単なるバッファとして
//	auto pixelsize = scratchImg.GetPixelsSize();
//	resDesc.Width = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT)*img->height;//データサイズ
//
//	resDesc.Height = 1;//
//	resDesc.DepthOrArraySize = 1;//
//	resDesc.MipLevels = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;//連続したデータですよ
//	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;//とくにフラグなし
//	resDesc.SampleDesc.Count = 1;//通常テクスチャなのでアンチェリしない
//	resDesc.SampleDesc.Quality = 0;//
//
//
//	//中間バッファ作成
//	ID3D12Resource* uploadbuff = nullptr;
//	result = _dev->CreateCommittedResource(
//		&uploadHeapProp,
//		D3D12_HEAP_FLAG_NONE,//特に指定なし
//		&resDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,//CPUから書き込み可能
//		nullptr,
//		IID_PPV_ARGS(&uploadbuff)
//	);
//
//	//次にテクスチャのためのヒープ設定
//	D3D12_HEAP_PROPERTIES texHeapProp = {};
//	texHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;//テクスチャ用
//	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
//	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
//	texHeapProp.CreationNodeMask = 0;//単一アダプタのため0
//	texHeapProp.VisibleNodeMask = 0;//単一アダプタのため0
//
//	//リソース設定(変数は使いまわし)
//	resDesc.Format = metadata.format;
//	resDesc.Width = metadata.width;//幅
//	resDesc.Height = metadata.height;//高さ
//	resDesc.DepthOrArraySize = metadata.arraySize;//2Dで配列でもないので１
//	resDesc.MipLevels = metadata.mipLevels;//ミップマップしないのでミップ数は１つ
//	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);//2Dテクスチャ用
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
//
//
//
//
//
//	//テクスチャバッファ作成
//	ID3D12Resource* texbuff = nullptr;
//	result = _dev->CreateCommittedResource(
//		&texHeapProp,
//		D3D12_HEAP_FLAG_NONE,//特に指定なし
//		&resDesc,
//		D3D12_RESOURCE_STATE_COPY_DEST,//コピー先
//		nullptr,
//		IID_PPV_ARGS(&texbuff)
//	);
//	uint8_t* mapforImg = nullptr;//image->pixelsと同じ型にする
//	result = uploadbuff->Map(0, nullptr, (void**)&mapforImg);//マップ
//	auto srcAddress = img->pixels;
//	auto rowPitch = AlignmentedSize(img->rowPitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
//	for (int y = 0; y < img->height; ++y) {
//		std::copy_n(srcAddress,
//			rowPitch,
//			mapforImg);//コピー
//		//1行ごとの辻褄を合わせてやる
//		srcAddress += img->rowPitch;
//		mapforImg += rowPitch;
//	}
//	uploadbuff->Unmap(0, nullptr);//アンマップ
//
//	D3D12_TEXTURE_COPY_LOCATION src = {}, dst = {};
//	dst.pResource = texbuff;
//	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
//	dst.SubresourceIndex = 0;
//
//	src.pResource = uploadbuff;//中間バッファ
//	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;//フットプリント指定
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
//		//_cmdAllocator->Reset();//キューをクリア
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
//		//コマンドリストの実行
//		ID3D12CommandList* cmdlists[] = { _cmdList };
//		_cmdQueue->ExecuteCommandLists(1, cmdlists);
//		////待ち
//		_cmdQueue->Signal(_fence, ++_fenceVal);
//
//		if (_fence->GetCompletedValue() != _fenceVal) {
//			auto event = CreateEvent(nullptr, false, false, nullptr);
//			_fence->SetEventOnCompletion(_fenceVal, event);
//			WaitForSingleObject(event, INFINITE);
//			CloseHandle(event);
//		}
//		_cmdAllocator->Reset();//キューをクリア
//		_cmdList->Reset(_cmdAllocator, nullptr);
//
//
//
//	}
//
//
//
//	//定数バッファ作成
//	auto worldMat = XMMatrixRotationY(XM_PIDIV4);
//	XMFLOAT3 eye(0, 10, -15);//<< eye(0,0,-5);
//	XMFLOAT3 target(0, 10, 0);
//	XMFLOAT3 up(0, 1, 0);
//	auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
//	auto projMat = XMMatrixPerspectiveFovLH(XM_PIDIV2,//画角は90°
//		static_cast<float>(window_width) / static_cast<float>(window_height),//アス比
//		1.0f,//近い方  << 1.0f
//		100.0f//遠い方
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
//	XMMATRIX* mapMatrix;//マップ先を示すポインタ
//	result = constBuff->Map(0, nullptr, (void**)&mapMatrix);//マップ
//	*mapMatrix = worldMat * viewMat*projMat;
//
//	ID3D12DescriptorHeap* basicDescHeap = nullptr;
//	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
//	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
//	descHeapDesc.NodeMask = 0;//マスクは0
//	descHeapDesc.NumDescriptors = 2;//SRV1つとCBV1つ
//	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;//デスクリプタヒープ種別
//	result = _dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));//生成
//
//	//通常テクスチャビュー作成
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = metadata.format;//DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA(0.0f～1.0fに正規化)
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;//後述
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
//	srvDesc.Texture2D.MipLevels = 1;//ミップマップは使用しないので1
//
//	//デスクリプタの先頭ハンドルを取得しておく
//	auto basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();
//	//シェーダリソースビューの作成
//	_dev->CreateShaderResourceView(texbuff, //ビューと関連付けるバッファ
//		&srvDesc, //先ほど設定したテクスチャ設定情報
//		basicHeapHandle
//	);
//	basicHeapHandle.ptr +=
//		_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
//	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
//	cbvDesc.SizeInBytes = constBuff->GetDesc().Width;
//	//定数バッファビューの作成
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
//		//もうアプリケーションが終わるって時にmessageがWM_QUITになる
//		if (msg.message == WM_QUIT) {
//			break;
//		}
//
//		angle -= 0.05f;
//		worldMat = XMMatrixRotationY(angle);
//		*mapMatrix = worldMat * viewMat * projMat;
//
//
//		//DirectX処理
//		//バックバッファのインデックスを取得
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
//		//レンダーターゲットを指定
//		auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
//		rtvH.ptr += bbIdx * _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//		_cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
//
//		//画面クリア
//
//
//		float clearColor[] = { 1.0f,1.0f,1.0f,1.0f };//黄色
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
//		//命令のクローズ
//		_cmdList->Close();
//
//
//
//		//コマンドリストの実行
//		ID3D12CommandList* cmdlists[] = { _cmdList };
//		_cmdQueue->ExecuteCommandLists(1, cmdlists);
//		////待ち
//		_cmdQueue->Signal(_fence, ++_fenceVal);
//
//		if (_fence->GetCompletedValue() != _fenceVal) {
//			auto event = CreateEvent(nullptr, false, false, nullptr);
//			_fence->SetEventOnCompletion(_fenceVal, event);
//			WaitForSingleObject(event, INFINITE);
//			CloseHandle(event);
//		}
//		_cmdAllocator->Reset();//キューをクリア
//		_cmdList->Reset(_cmdAllocator, _pipelinestate);//再びコマンドリストをためる準備
//
//
//		//フリップ
//		_swapchain->Present(1, 0);
//
//	}
//	//もうクラス使わんから登録解除してや
//	UnregisterClass(w.lpszClassName, w.hInstance);
//	return 0;
//}
