#include "ShaderManager.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace lib {
	void ShaderManager::loadShader() {

	}

	void ShaderManager::loadVertexShaderFile(
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
			&m_pVS_blob,
			&m_pError_blob
		);

		if (!SUCCEEDED(result)) {
			assert(0 && "VertexShader Error!" && Path);
		}
	}

	void ShaderManager::loadPixelShaderFile(
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
			&m_pPS_blob,
			&m_pError_blob
		);

		if (!SUCCEEDED(result)) {
			assert(0 && "PicelShader Error!" && Path);
		}
	}

	void ShaderManager::setVertShaderBlob(ID3DBlob* vsBlob) {
		this->m_pVS_blob = vsBlob;
	}

	void ShaderManager::setPixelShaderBlob(ID3DBlob* psBlob) {
		this->m_pPS_blob = psBlob;
	}

	void ShaderManager::setErrorShaderBlob(ID3DBlob* errorBlob) {
		this->m_pError_blob = errorBlob;
	}


	ID3DBlob* ShaderManager::getVertShaderBlob() {
		return m_pVS_blob;
	}

	ID3DBlob* ShaderManager::getPixelShaderBlob() {
		return m_pPS_blob;
	}

	ID3DBlob* ShaderManager::getErrorShaderBlob() {
		return m_pError_blob;
	}
}