#ifndef HELLO_MR_FEENEY
#define HELLO_MR_FEENEY

#include <string>
#include <map>
#include "../include/glm/glm.hpp"


struct sVertex
{
	float x, y, z;
	float r, g, b;
	float nx, ny, nz;
};

struct sModelDrawInfo
{
	sModelDrawInfo();
	
	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	sVertex* pVertices;

	unsigned int* pIndices;

	glm::vec3 maxValues;
	glm::vec3 minValues;
};

class cVAOManager {
public:

	bool LoadModelIntoVAO(std::string filename, sModelDrawInfo& drawInfo, unsigned int shaderProgramID);
	bool FindDrawInfoByModelName(std::string filename, sModelDrawInfo& drawInfo);
	std::string getLastError(bool bAndClear = true);

private:
	std::map< std::string /*model name*/,
			sModelDrawInfo/*info needed to draw*/>
		m_map_ModelName_to_VAOID;
};

#endif
