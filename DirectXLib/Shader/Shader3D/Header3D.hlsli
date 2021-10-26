struct Output {
	float4 svpos:SV_POSITION;
	float4 pos:POSITION;//システム用頂点座標
	float4 normal:NORMAL;
	float4 vnormal:NORMAL1;//法線ベクトル
	float2 uv:TEXCOORD;//UV値
	float3 ray:VECTOR;//ベクトル
};

Texture2D<float4> tex:register(t0);
Texture2D<float4> sph:register(t1);//1番スロットに設定されたテクスチャ(乗算)
Texture2D<float4> spa:register(t2);//2番スロットに設定されたテクスチャ(加算)
Texture2D<float4> toon:register(t3);//3番スロットに設定されたテクスチャ(トゥーン)

SamplerState smp : register(s0);

//定数バッファ
cbuffer SceneMat : register(b0) {
	matrix world;//ワールド変換行列
	matrix view;
	matrix proj;//ビュープロジェクション行列
	float3 eye;
};

cbuffer Material : register(b1) {
	float4 diffuse;//ディフューズ色
	float4 specular;//スペキュラ
	float3 ambient;//アンビエント
};