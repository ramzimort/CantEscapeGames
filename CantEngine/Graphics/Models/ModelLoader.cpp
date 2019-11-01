#include "ModelLoader.h"
#include "Graphics/DXResourceLoader.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/D3D11_Renderer.h"

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}


void ModelLoader::LoadModel( Model* model, aiScene const *scene,DXRenderer* dxrenderer)
{
	uint32_t totalVertexCount = 0;
	uint32_t totalIndexCount = 0;

	ModelLoader::ProcessNode(scene->mRootNode, scene, *model, totalVertexCount, totalIndexCount, false);

	if (!model->m_has_tangent)
	{
		ModelLoader::CalculateModelTangents(*model);
	}
}

void ModelLoader::ProcessNode(aiNode *node, const aiScene *scene, Model& model, 
	uint32_t& totalVertexCount, uint32_t& totalIndexCount, bool textured_model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, model, totalVertexCount, totalIndexCount, textured_model);
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		aiNode *child = node->mChildren[i];
		ProcessNode(child, scene, model, totalVertexCount, totalIndexCount, textured_model);
	}
}

void ModelLoader::ProcessMesh(aiMesh *mesh, const aiScene *scene, Model& model, 
	uint32_t& totalVertexCount, uint32_t& totalIndexCount, bool textured_model)
{
	size_t mesh_index = model.m_meshes.size();
	model.m_meshes.push_back(Mesh());

	Mesh& last_mesh = model.m_meshes.back();

	last_mesh.m_start_index = totalIndexCount;
	last_mesh.m_start_vertex = totalVertexCount;


	if (mesh->mTangents && mesh->mBitangents)
	{
		model.m_has_tangent = true;
	}

	//VERTICES- NORMALS- UVs
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{

		aiVector3D vertex = mesh->mVertices[i];
		DirectX::XMFLOAT3 transf_vertex(vertex.x, vertex.y, vertex.z);


		aiVector3D normal = mesh->mNormals[i];
		DirectX::XMFLOAT3 transf_normal(normal.x, normal.y, normal.z);

		// process tangents
		Vector3 transf_tgt(0.f, 0.f, 0.f);

		if (mesh->mTangents)
		{
			aiVector3D tgt = mesh->mTangents[i];
			transf_tgt = DirectX::XMFLOAT3(tgt.x, tgt.y, tgt.z);
		}

		Vector3 transf_bitgt(0.f, 0.f, 0.f);
		// process bitangents
		if (mesh->mBitangents)
		{
			aiVector3D bitgt = mesh->mBitangents[i];
			transf_bitgt = Vector3(bitgt.x, bitgt.y, bitgt.z);
		}

		DirectX::XMFLOAT2 transf_uv(0.f, 0.f);
		if (mesh->mTextureCoords && mesh->mTextureCoords[0])
		{
			aiVector3D uv = mesh->mTextureCoords[0][i];
			transf_uv = Vector2(uv.x, uv.y);
		}

		PushVertexData(model, VertexData(transf_vertex, transf_normal, transf_tgt, transf_bitgt, transf_uv));

		++totalVertexCount;
	}
	uint32_t curIndexCount = 0;

	//INDICES PROCESSING
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace& face = mesh->mFaces[i];

		Model::Triangle triangle = {};

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{

			int triangle_index = (j % 3);
			triangle[triangle_index] = face.mIndices[j];

			if (triangle_index == 2)
			{
				model.m_triangle_indices.push_back(triangle);
			}


			//++totalIndexCount;
			++curIndexCount;
		}
	}

	last_mesh.m_index_count = curIndexCount;
	totalIndexCount += curIndexCount;

	if (textured_model && mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		//TODO: load material texture
	}


}

void ModelLoader::PushVertexData(Model& model, const VertexData& vertex_data)
{
	const Vector3& pos = vertex_data.m_position;
	model.m_aabb.m_Min.x = fmin(model.m_aabb.m_Min.x, pos.x);
	model.m_aabb.m_Min.y = fmin(model.m_aabb.m_Min.y, pos.y);
	model.m_aabb.m_Min.z = fmin(model.m_aabb.m_Min.z, pos.z);

	model.m_aabb.m_Max.x = fmax(model.m_aabb.m_Max.x, pos.x);
	model.m_aabb.m_Max.y = fmax(model.m_aabb.m_Max.y, pos.y);
	model.m_aabb.m_Max.z = fmax(model.m_aabb.m_Max.z, pos.z);

	model.m_vertices.push_back(vertex_data);
}


void ModelLoader::CalculateModelTangents(Model& model)
{
	if (model.m_has_tangent)
	{
		return;
	}

	std::vector<Vector3> raw_tangent_list, raw_bitangent_list;

	for (int32_t k = 0; k < model.m_triangle_indices.size(); ++k)
	{
		const Model::Triangle& model_triangle = model.m_triangle_indices[k];
		uint32_t v_index0 = model_triangle[0];
		uint32_t v_index1 = model_triangle[1];
		uint32_t v_index2 = model_triangle[2];

		const VertexData& vertex0 = model.m_vertices[v_index0];
		const VertexData& vertex1 = model.m_vertices[v_index1];
		const VertexData& vertex2 = model.m_vertices[v_index2];

		const Vector3& p0 = vertex0.m_position;
		const Vector3& p1 = vertex1.m_position;
		const Vector3& p2 = vertex2.m_position;

		const Vector2& uv0 = vertex0.m_tex_coords;
		const Vector2& uv1 = vertex1.m_tex_coords;
		const Vector2& uv2 = vertex2.m_tex_coords;

		Vector3 e1 = p1 - p0;
		Vector3 e2 = p2 - p0;

		float x1 = uv1.x - uv0.x;
		float x2 = uv2.x - uv0.x;
		float y1 = uv1.y - uv0.y;
		float y2 = uv2.y - uv0.y;

		float r = 1.f / (x1 * y2 - x2 * y1);
		
		Vector3 tangent_raw = (e1 * y2 - e2 * y1) * r;
		Vector3 bitangent_raw = (e2 * x1 - e1 * x2) * r;

		raw_tangent_list.push_back(tangent_raw);
		raw_tangent_list.push_back(tangent_raw);
		raw_tangent_list.push_back(tangent_raw);

		raw_bitangent_list.push_back(bitangent_raw);
		raw_bitangent_list.push_back(bitangent_raw);
		raw_bitangent_list.push_back(bitangent_raw);
	}

	for (int32_t i = 0; i < model.m_vertices.size(); ++i)
	{
		const Vector3& t = raw_tangent_list[i];
		const Vector3& b = raw_bitangent_list[i];
		const Vector3& n = model.m_vertices[i].m_normal;

		Vector3 final_tangent = t - (t.Dot(n)) * n;;
		final_tangent.Normalize();

		model.m_vertices[i].m_tangent = final_tangent;

		Vector3 final_bitangent = n.Cross(final_tangent);
		final_bitangent.Normalize();

		model.m_vertices[i].m_bitangent = final_bitangent;

		float sign = t.Cross(b).Dot(n);
		int k = 0;
	}
	


	model.m_has_tangent = true;
}