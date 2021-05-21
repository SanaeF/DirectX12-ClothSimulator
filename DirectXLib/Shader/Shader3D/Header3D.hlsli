struct Output {
	float4 svpos:SV_POSITION;
	float4 normal:NORMAL;
	float2 uv:TEXCOORD;
	uint id:SV_InstanceID;
	matrix mat : MATRIX;
};

Texture2D<float4>tex:register(t0);
SamplerState smp : register(s0);

//定数バッファ
cbuffer cbuff0 : register(b0) {
	matrix world;//ワールド変換行列
	matrix viewproj;//ビュープロジェクション行列
	int worldTime;
};