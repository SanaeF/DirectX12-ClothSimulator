#include "ShaderManager.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")


void ShaderManager::LoadShader() {

}

void ShaderManager::LoadVertexShaderFile(
	LPCWSTR Path,
	LPCSTR function,
	LPCSTR Version
) {
	auto result = D3DCompileFromFile(
		Path,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		function, Version,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob,
		&errorBlob
	);

	if (!SUCCEEDED(result)) {
		assert(0 && "VertexShader 2D Error!" && Path);
	}
}

void ShaderManager::LoadPixelShaderFile(
	LPCWSTR Path,
	LPCSTR function,
	LPCSTR Version
) {
	auto result = D3DCompileFromFile(
		Path,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		function, Version,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob,
		&errorBlob
	);

	if (!SUCCEEDED(result)) {
		assert(0 && "PicelShader 2D Error!" && Path);
	}
}

void ShaderManager::setVertShaderBlob(ID3DBlob* vsBlob) {
	this->vsBlob = vsBlob;
}

void ShaderManager::setPixelShaderBlob(ID3DBlob* psBlob) {
	this->psBlob = psBlob;
}

void ShaderManager::setErrorShaderBlob(ID3DBlob* errorBlob) {
	this->errorBlob = errorBlob;
}


ID3DBlob* ShaderManager::getVertShaderBlob() {
	return vsBlob;
}

ID3DBlob* ShaderManager::getPixelShaderBlob() {
	return psBlob;
}

ID3DBlob* ShaderManager::getErrorShaderBlob() {
	return errorBlob;
}