#pragma once

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class DXRenderer;
class ResourceManager;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();
	
	
	static void LoadModel(Model* model, aiScene const *scene, DXRenderer* dxrenderer, ResourceManager* resourceManager);
	static void CalculateModelTangents(Model& model);
private:
	static void LoadMaterialTextures(const aiScene* scene, Model& model, Mesh& mesh, aiMaterial *mat, uint64_t mesh_index, ResourceManager* resourceManager);
private:
	static void ProcessNode(aiNode *node, const aiScene *scene, Model& model, 
		uint32_t& totalVertexCount, uint32_t& totalIndexCount, ResourceManager* resourceManager);
	static void ProcessMesh(aiMesh *mesh, const aiScene *scene, Model& model,
		uint32_t& totalVertexCount, uint32_t& totalIndexCount, ResourceManager* resourceManager);

	static void PushVertexData(Model& model, const VertexData& vertex_data);

	static Texture* ProcessAssimpTextureToResourceManager(const std::string& key_name, const aiTexture* ai_texture, ResourceManager* resourceManager);

};

