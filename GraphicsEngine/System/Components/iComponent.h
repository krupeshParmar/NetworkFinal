#pragma once
#include <string>
#include <glm/glm.hpp>

// hack
struct PoRoSc {
	glm::vec3 pos, rot, sca;
};

class iComponent
{
public:
	std::string gameObjectName;
	std::string componentType;
	virtual void Reset() = 0;
	virtual void CopyComponent() = 0;
	virtual void GetParentValues() = 0;
	virtual void Update(PoRoSc porosc) = 0;
};
