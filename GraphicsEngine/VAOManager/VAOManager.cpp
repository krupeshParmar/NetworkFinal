#include "../inc_opengl.h"
#include "VAOManager.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <sstream>

sModelDrawInfo::sModelDrawInfo()
{
    this->VAO_ID = 0;
    this->VertexBufferID = 0;
    this->numberOfVertices = 0;
    this->IndexBufferID = 0;
    this->IndexBuffer_Start_Index = 0;
    this->numberOfIndices = 0;
    this->numberOfTriangles = 0;

    this->pVertices = 0;
    this->pIndices = 0;

    return;
}


bool cVAOManager::LoadModelIntoVAO(std::string filename, sModelDrawInfo& drawInfo, unsigned int shaderProgramID)
{
    drawInfo.meshName = filename;

    // new buffer id by opengl
    glGenVertexArrays(1, &(drawInfo.VAO_ID));

    // bind this buffer: make this current vao buffer
    glBindVertexArray(drawInfo.VAO_ID);

    glGenBuffers(1, &(drawInfo.VertexBufferID));

    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sVertex) * drawInfo.numberOfVertices,
        (GLvoid*)drawInfo.pVertices,
        GL_STATIC_DRAW);

    // Copy the index buffer into the video card, too
    // Create an index buffer.
    glGenBuffers(1, &(drawInfo.IndexBufferID));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    // set the vertex attributes

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");
    GLint vnor_location = glGetAttribLocation(shaderProgramID, "vNormal");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, x));

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, r));

    glEnableVertexAttribArray(vnor_location);
    glVertexAttribPointer(vnor_location, 3,
        GL_FLOAT, GL_FALSE,
        sizeof(sVertex),
        (void*)offsetof(sVertex, nx));

    // set vao to zero
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vnor_location);

    // store the draw information into the map
    this->m_map_ModelName_to_VAOID[drawInfo.meshName] = drawInfo;

    return true;
}

bool cVAOManager::FindDrawInfoByModelName(std::string filename, sModelDrawInfo& drawInfo)
{
    std::map<std::string, sModelDrawInfo>::iterator itDrawInfo = this->m_map_ModelName_to_VAOID.find(filename);

    if (itDrawInfo == this->m_map_ModelName_to_VAOID.end())
        return false;

    drawInfo = itDrawInfo->second;
    return true;
}
