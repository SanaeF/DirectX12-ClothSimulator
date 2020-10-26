#pragma once
#include<d3dx12.h>
#include<DirectXMath.h>

class ShaderManager {
private:
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
public:

	virtual void LoadShader();

	void LoadVertexShaderFile(
		LPCWSTR Path,
		LPCSTR function,
		LPCSTR Version
	);

	void LoadPixelShaderFile(
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