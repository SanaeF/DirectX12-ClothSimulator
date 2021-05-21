#include "Layout.hlsli"
#include "Vector.hlsli"
#include "PhisicasParam.hlsli"

struct Vertex {
	float3 vert;
	float2 uv;
};

RWStructuredBuffer<Vertex> vertex : register(u0);
RWStructuredBuffer<uint> index : register(u1);
int vertexNum : register(u2);
int indexNum : register(u3);

struct VirtualData {
	float3 vertex[5]: SV_POSITION;
	uint index[4]: SV_POSITION;
};

VirtualData createVirtualPointData(int vertNum);

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void ClothPhisical(uint3 DTid : SV_DispatchThreadID)
{
	int step = 1;
	Vector vec;
	PhisicasParam param;
	uint beltNum = 128;

	param.mass = 1.f;
	param.gravity = 9.8f;
	param.resistance = 0.1f;
	param.acc = 5.f;
	param.wind = 0;
	param.spring.constant = 1.f;
	param.spring.shrink = 0.5f;
	param.spring.stretch = 0.15f;
	param.force3.x = 1;
	param.force3.y = 1;
	param.force3.z = 1;

	//param.acc++;
	int vertMax = vertexNum - beltNum;
	[loop] for (int vNum = 0; vNum < vertMax; vNum++) {
		VirtualData vir = createVirtualPointData(vNum);
		float Natulength = vec.dist(vir.vertex[0], vir.vertex[3]);

		float g = param.gravity * param.mass;
		float w = param.wind * param.mass;
		float k = param.spring.constant * param.mass;
		param.force3.y += g;
		param.force3.x += w * (sin(param.acc) * 0.5 + 0.5);
		//param.force3 = vec.scale(param.force3, step * step * 0.5 / param.mass);//‚¢‚ç‚È‚¢
		param.resistance = 1.f - param.resistance * step;

		float3 dtX = vec.sub(vir.vertex[0], vir.vertex[vNum]);
		dtX = vec.add(dtX, param.force3);
		dtX = vec.scale(dtX, param.resistance);
		vir.vertex[0] = vec.add(vir.vertex[0], dtX);

		[loop] for (int ite = 0; ite < step; ite++) {
			float d = vec.dist(vir.vertex[0], vir.vertex[3]);
			float f = (d - Natulength) * k;
			if (f >= 0) f = f * param.spring.shrink;
			else f = f * param.spring.stretch;
			float3 dtX = vec.sub(vir.vertex[3], vir.vertex[0]);
			dtX = vec.normalize(dtX);
			dtX = vec.scale(dtX, f);
			dtX = vec.scale(dtX, step * step * 0.5 / param.mass);

			float3 dx_p1 = vec.scale(dtX, 1.f);
			vir.vertex[0] = vec.add(vir.vertex[0], dx_p1);
		}
		vertex[vNum].vert = float3(1,1,1);
	}
}

VirtualData createVirtualPointData(int vertNum){
	int data[6];
	int count;
	VirtualData vir;
	for (int num = 0; num < indexNum; num++) {
		if (index[num] == vertNum) {
			data[count] = num;
			count++;
		}
		if (count > 6)break;
	}
	for (int num = 0; num < 6; num++) {
		if (data[num] % 6 == 0)vir.index[0] = data[num] + 5;//P3
		if (data[num] % 6 == 1)vir.index[1] = data[num] - 1;//P4
		if (data[num] % 6 == 2)vir.index[2] = data[num] - 1;//P1
		if (data[num] % 6 == 3)vir.index[3] = data[num] + 2;//P2
	}
	vir.vertex[0] = vertex[vertNum].vert;
	if (between(vir.index[2], 0, vertexNum))vir.vertex[1] = vertex[vir.index[2]].vert;
	if (between(vir.index[3], 0, vertexNum))vir.vertex[2] = vertex[vir.index[3]].vert;
	if (between(vir.index[0], 0, vertexNum))vir.vertex[3] = vertex[vir.index[0]].vert;
	if (between(vir.index[1], 0, vertexNum))vir.vertex[4] = vertex[vir.index[1]].vert;
	return vir;
}
