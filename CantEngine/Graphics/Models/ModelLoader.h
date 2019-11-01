#pragma once

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class DXRenderer;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();
	
	
	static void LoadModel(Model* model, aiScene const *scene, DXRenderer* dxrenderer);
	static void CalculateModelTangents(Model& model);

private:
	static void ProcessNode(aiNode *node, const aiScene *scene, Model& model, 
		uint32_t& totalVertexCount, uint32_t& totalIndexCount, bool textured_model);
	static void ProcessMesh(aiMesh *mesh, const aiScene *scene, Model& model,
		uint32_t& totalVertexCount, uint32_t& totalIndexCount, bool textured_model);

	static void PushVertexData(Model& model, const VertexData& vertex_data);
};

