//Header

#pragma once

//Includes
#include "Bone.h"
#include "Animation.h"

class Model;
class AnimModel;


namespace FBXLoader
{

	// Reads the assimp FBX and starts the unwrapping process
	void LoadSkeletalData(Model *model, aiScene const *scene);


	// Reads the assimp FBX and starts the unwrapping process
	void LoadAnimationData(Model *mdl, aiScene const *scene, std::string const& name);


	// Process the aiMesh and get all the information
	void GetPerVertexData(AnimModel *model, aiMesh* mesh, int& vertexIndexOffset,
		std::unordered_map<std::string, Matrix>& BoneOffsetMap,
		std::unordered_map<std::string, int> const& BonesNameIdMap);


	// Process an animation and stores all the channels and info in myAnimList
	void ProcessAnimation(aiAnimation* aiAnim, AnimModel *model, std::string const& name);


	// Process the nodes and get some bone information from here
	void ProcessBoneHierarchy(aiNode const *node,
		std::string const& parentName, AnimModel *model,
		std::unordered_map<std::string, int>& BoneNameIdMap, int boneIndex);


	// TODO - check if the convertion is actually done correctly
	Matrix aiMat_To_Matrix(aiMatrix4x4 const& inMat);


	//Calculates the sum of vertices from all meshes
	int CalculateTotalVertexCount(aiScene const *scene);


	//METHODS FOR GETTING KEY FRAMES
	std::vector<PosKey> GetPositionKeyFrames(aiNodeAnim *animNode);
	//METHODS FOR GETTING KEY FRAMES
	std::vector<RotKey> GetRotationKeyFrames(aiNodeAnim *animNode);
	//METHODS FOR GETTING KEY FRAMES
	std::vector<ScaKey> GetScalingKeyFrames(aiNodeAnim *animNode);
}