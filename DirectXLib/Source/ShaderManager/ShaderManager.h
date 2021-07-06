#pragma once
#include<d3dx12.h>
#include<DirectXMath.h>

namespace lib {
	class ShaderManager {
	private:
		ID3DBlob* m_pVS_blob = nullptr;
		ID3DBlob* m_pPS_blob = nullptr;
		ID3DBlob* m_pError_blob = nullptr;
	public:
		virtual void loadShader();
		void loadVertexShaderFile(
			LPCWSTR Path,
			LPCSTR function,
			LPCSTR Version
		);
		void loadPixelShaderFile(
			LPCWSTR Path,
			LPCSTR function,
			LPCSTR Version
		);
		void setVertShaderBlob(ID3DBlob* vsBlob);
		void setPixelShaderBlob(ID3DBlob* psBlob);
		void setErrorShaderBlob(ID3DBlob* errorBlob);
		ID3DBlob* getVertShaderBlob();
		ID3DBlob* getPixelShaderBlob();
		ID3DBlob* getErrorShaderBlob();
	};
}