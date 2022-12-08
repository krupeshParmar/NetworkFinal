#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	componentType = "boxcollider";
	box_model_path = "assets/models/bounding_box.ply";
	//boundingBox = BoundingBox();
}

void BoxCollider::Reset()
{
}

void BoxCollider::CopyComponent()
{
}

void BoxCollider::GetParentValues()
{
}

void BoxCollider::Update()
{
	//Check for collisions

}

void BoxCollider::Update(PoRoSc porosc)
{
	
}
