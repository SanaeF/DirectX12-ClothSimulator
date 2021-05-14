#include<sstream>
#include "PMDModel.h"
#include"Dx12Wrapper.h"

#include "DxVertex3D.h"

int PMDModel::LoadHeader(FILE* fp) {
	struct PMDHeader {
		float version;
		char model_name[20];
		char comment[256];
	};
	PMDHeader mPMDHeader;
	char signature[3] = {};

	fread(signature, sizeof(signature), 1, fp);
	fread(&mPMDHeader, sizeof(mPMDHeader), 1, fp);

}

int PMDModel::Load(std::shared_ptr<Dx12Wrapper> DxWrap) {
	auto fp = fopen("model/çÎ‡Vî¸çÁÉ~ÉN.pmd", "rb");
	LoadHeader(fp);
	DxVertex3D vertex;
	vertex.LoadVertex(DxWrap, fp);
	VertexBuff= vertex.getVertexBuff();
	fclose(fp);
	return 0;
}

PMDModel::PMDModel(){

}

PMDModel::~PMDModel() {

}