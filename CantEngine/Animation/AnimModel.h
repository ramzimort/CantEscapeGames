#pragma once
#include "Physics/Geometry/Aabb.h"
#include "Helper/Hash.h"
#include "Graphics/Model.h"



struct AnimVertexData
{
	VertexData m_vertexData;
	uint32_t m_boneIndices[4];
	float m_boneWeights[4];
};



class LoadedMesh;
class Texture;
class DXRenderer;

class AnimModel : public Model 
{
public:
	friend class FBXLoader;
public:
	AnimModel() {};
	~AnimModel() {};


private:
	
private:

	

	
};