//HEADER


#include "FBXLoader.h"
#include "Graphics/Models/Model.h"
#include "Animation/AnimModel.h"


namespace FBXLoader
{
	// Reads the assimp FBX and starts the unwrapping process
	void LoadSkeletalData(Model *mdl, aiScene const *scene)
	{
		//Animation model. Holds the skeleton and animation info
		AnimModel *model = static_cast<AnimModel*>(mdl);

		//Other useful vars and structures
		int totalNumberOfVertices = CalculateTotalVertexCount(scene);
		std::unordered_map<std::string, Matrix>	BoneOffsetMap;
		std::unordered_map<std::string, int> BoneNameIdMap;


		// Save bone data
		ProcessBoneHierarchy(scene->mRootNode, "", model, BoneNameIdMap, 0);
		//Loop through bones and id them
		int index = 0;
		for (auto& node : model->boneMap)
		{
			Bone& bone = node.second;
			bone.index = index;
			BoneNameIdMap[bone.name] = index;
			++index;
		}



		// Stores all the mesh info
		for (unsigned i = 0; i < scene->mNumMeshes; ++i)
			GetPerVertexData(model, scene->mMeshes[i], BoneOffsetMap, BoneNameIdMap);



		//Retrieve the offset matrix for the bones we care about
		for (auto& node : model->boneMap)
		{
			Bone& bone = node.second;

			auto iter = BoneOffsetMap.find(bone.name);
			if (iter != BoneOffsetMap.end())
				bone.offsetMatrix = iter->second;
			else
				bone.offsetMatrix = Matrix::Identity;
		}

	}


	// Reads the assimp FBX and starts the unwrapping process
	void LoadAnimationData(Model *mdl, aiScene const *scene, std::string const& name)
	{
		//Animation model. Holds the skeleton and animation info
		AnimModel *model = static_cast<AnimModel*>(mdl);

		// NOTE - Right now we are loading one anim per scene
		// Prints all the anim info for each animation
		for (int i = 0; i < scene->mNumAnimations; ++i)
			ProcessAnimation(scene->mAnimations[i], model, name);
	}


	// Process the aiMesh and get all the information
	void GetPerVertexData(AnimModel *model, aiMesh* mesh,
		std::unordered_map<std::string, Matrix>& BoneOffsetMap,
		std::unordered_map<std::string, int> const& BonesNameIdMap)
	{
		//Uninitialized data vectors
		std::vector< std::vector<int>> bones_indices(mesh->mNumVertices);
		std::vector< std::vector<float> > bones_weights(mesh->mNumVertices);

		//BONES PROCESSING
		for (unsigned int i = 0; i < mesh->mNumBones; ++i)
		{
			aiBone *bone = mesh->mBones[i];
			int boneIndex = BonesNameIdMap.find(bone->mName.C_Str())->second;

			//Once we have the bone, we need to get the vertex info and store it per vertex
			for (unsigned j = 0; j < bone->mNumWeights; ++j)
			{
				aiVertexWeight node = bone->mWeights[j];
				bones_indices[node.mVertexId].push_back(boneIndex);
				bones_weights[node.mVertexId].push_back(node.mWeight);
			}

			//Also, hash the offset matrix to the name of the bone
			aiMatrix4x4 const& om = bone->mOffsetMatrix;
			Matrix offsetMatrix = aiMat_To_Matrix(om);
			BoneOffsetMap[bone->mName.C_Str()] = offsetMatrix;
		}

		//We have to pass the indices and weights info to the animModel
		model->PassIndicesAndWeightsPerMesh(bones_indices, bones_weights);
	}




	// Process an animation and stores all the channels and info in myAnimList
	void ProcessAnimation(aiAnimation* aiAnim, AnimModel *model, std::string const& name)
	{
		Animation animation;
		animation.animName = name;// aiAnim->mName.C_Str();
		animation.ticksPerSecond = static_cast<float>(aiAnim->mTicksPerSecond);
		animation.duration = static_cast<float>(aiAnim->mDuration);

		// Animations Channels (One per aiNode)
		for (unsigned i = 0; i < aiAnim->mNumChannels; ++i)
		{
			aiNodeAnim* animNode = aiAnim->mChannels[i];

			//Create a new animation
			AnimChannel anim;
			///anim.animName = aiAnim->mName.C_Str();
			///anim.ticksPerSecond = aiAnim->mTicksPerSecond;
			///anim.duration = aiAnim->mDuration;

			anim.boneName = animNode->mNodeName.C_Str();
			anim.PositionKeys = GetPositionKeyFrames(animNode);		//Vector of PosKey
			anim.RotationKeys = GetRotationKeyFrames(animNode);		//Vector of RotKey
			anim.ScalingKeys = GetScalingKeyFrames(animNode);		//Vector of ScaKey 

			animation.channels.push_back(anim);
		}

		model->animMap[animation.animName] = animation;
	}




	// Process the nodes and get some bone information from here
	void ProcessBoneHierarchy(aiNode const *node,
		std::string const& parentName, AnimModel *model,
		std::unordered_map<std::string, int>& BoneNameIdMap, int boneIndex)
	{
		//HERE  WE GO THROUGH THE NODE HIERARCHY. NOTE, NOT ALL
		//NODES ARE NECESSARILY BONES!

		//Create bone and store node info in it
		Bone bone;
		bone.updatedVQS = false;
		bone.name = node->mName.C_Str();
		bone.parent = parentName;

		//Transformations
		bone.nodeTransformation = aiMat_To_Matrix(node->mTransformation);
		bone.vqs = Matrix::Identity;
		bone.accumTransformation = Matrix::Identity;

		// Recurse onto this node's children
		for (unsigned i = 0; i < node->mNumChildren; ++i)
		{
			bone.children.push_back(node->mChildren[i]->mName.C_Str());

			ProcessBoneHierarchy(node->mChildren[i], bone.name,
				model, BoneNameIdMap, boneIndex + i + 1);
		}

		//Push bone (copy) to the vector
		model->boneMap[bone.name] = bone;
	}




	// TODO - check if the convertion is actually done correctly
	Matrix aiMat_To_Matrix(aiMatrix4x4 const& inMat)
	{
		Matrix mat;

		//mat is a column major (first index is column?)
		mat._11 = inMat.a1;
		mat._12 = inMat.a2;
		mat._13 = inMat.a3;
		mat._14 = inMat.a4;

		mat._21 = inMat.b1;
		mat._22 = inMat.b2;
		mat._23 = inMat.b3;
		mat._24 = inMat.b4;

		mat._31 = inMat.c1;
		mat._32 = inMat.c2;
		mat._33 = inMat.c3;
		mat._34 = inMat.c4;

		mat._41 = inMat.d1;
		mat._42 = inMat.d2;
		mat._43 = inMat.d3;
		mat._44 = inMat.d4;

		//TODO - check if this is right
		return mat;
	}


	//Calculates the sum of vertices from all meshes
	int CalculateTotalVertexCount(aiScene const *scene)
	{
		int sum = 0;
		for (unsigned i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh *mesh = scene->mMeshes[i];
			sum += mesh->mNumVertices;
		}
		return sum;
	}


	//METHODS FOR GETTING KEY FRAMES
	std::vector<PosKey> GetPositionKeyFrames(aiNodeAnim *animNode)
	{
		std::vector<PosKey> positions;
		for (unsigned i = 0; i < animNode->mNumPositionKeys; ++i)
		{
			PosKey pos;
			aiVectorKey PKey = animNode->mPositionKeys[i];
			pos.position = Vector3(PKey.mValue.x, PKey.mValue.y, PKey.mValue.z);
			pos.time = static_cast<float>(PKey.mTime);
			positions.push_back(pos);
		}
		return positions;
	}

	//METHODS FOR GETTING KEY FRAMES
	std::vector<RotKey> GetRotationKeyFrames(aiNodeAnim *animNode)
	{
		std::vector<RotKey> quaternions;
		for (unsigned i = 0; i < animNode->mNumRotationKeys; ++i)
		{
			RotKey rot;
			aiQuatKey RKey = animNode->mRotationKeys[i];
			rot.quaternion = MathUtil::Quaternion(RKey.mValue.w, Vector3(RKey.mValue.x, RKey.mValue.y, RKey.mValue.z));
			rot.time = static_cast<float>(RKey.mTime);
			quaternions.push_back(rot);
		}
		return quaternions;
	}

	//METHODS FOR GETTING KEY FRAMES
	std::vector<ScaKey> GetScalingKeyFrames(aiNodeAnim *animNode)
	{
		std::vector<ScaKey> scales;
		for (unsigned i = 0; i < animNode->mNumScalingKeys; ++i)
		{
			ScaKey scale;
			aiVectorKey SKey = animNode->mScalingKeys[i];
			scale.scale = Vector3(SKey.mValue.x, SKey.mValue.y, SKey.mValue.z);
			scale.time = static_cast<float>(SKey.mTime);
			scales.push_back(scale);
		}
		return scales;
	}
}