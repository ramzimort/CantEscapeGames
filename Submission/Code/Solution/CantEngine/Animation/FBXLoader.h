/**
 * @file FBXLoader.h
 * @author Jose Rosenbluth
 * @brief In charge of loading the animation information from FBX format
 * @date 2019-11-21
 */

#pragma once

//Includes
#include "Bone.h"
#include "Animation.h"

class Model;
class AnimModel;

/**
 * @brief Class in charge of loading
 * 
 */
namespace FBXLoader
{
	/**
	 * @brief Reads the assimp FBX and starts the unwrapping process
	 * 
	 * @param model Model pointer
	 * @param scene aiScene, the whole scene as stored by assimp
	 */
	void LoadSkeletalData(Model *model, aiScene const *scene);

	/**
	 * @brief Reads the assimp FBX and starts the unwrapping process
	 * 
	 * @param animMap Hash table that maps the animation name to the Animation
	 * @param scene aiScene, the whole scene as stored by assimp
	 * @param name Name of the animation
	 */
	void LoadAnimationData(std::unordered_map<std::string, Animation>& animMap, 
		aiScene const *scene, std::string const& name);

	/**
	 * @brief Process the aiMesh and get all the information
	 * 
	 * @param model animModel pointer
	 * @param mesh aiMesh pointer from which we will get per vertex data
	 * @param BoneOffsetMap Hash table that maps bone name to Offset transformation
	 * @param BonesNameIdMap Hash table that maps bone name to bone id
	 */
	void GetPerVertexData(AnimModel *model, aiMesh* mesh,
		std::unordered_map<std::string, Matrix>& BoneOffsetMap,
		std::unordered_map<std::string, int> const& BonesNameIdMap);


	/**
	 * @brief Process an animation and stores all the channels and info in myAnimList
	 * 
	 * @param aiAnim pointer to aiAnimation object with which we will get data to create an Animation object
	 * @param animMap Hash table that maps animation anme to Animation object
	 * @param name Name of the animation
	 */
	void ProcessAnimation(aiAnimation* aiAnim, 
		std::unordered_map<std::string, Animation>& animMap, std::string const& name);


	/**
	 * @brief Process the nodes and get some bone information from here
	 * 
	 * @param node Current node evaluated
	 * @param parentName Name of the parent node
	 * @param model AnimModel pointer
	 * @param BoneNameIdMap Hash table between bone name and id
	 * @param boneIndex Bone index
	 */
	void ProcessBoneHierarchy(aiNode const *node,
		std::string const& parentName, AnimModel *model,
		std::unordered_map<std::string, int>& BoneNameIdMap, int boneIndex);


	/**
	 * @brief Convert from aiMatrix4x4 to Matrix4
	 * 
	 * @param inMat 
	 * @return Matrix 
	 */
	Matrix aiMat_To_Matrix(aiMatrix4x4 const& inMat);


	/**
	 * @brief Calculates the sum of vertices from all meshes
	 * 
	 * @param scene 
	 * @return int 
	 */
	int CalculateTotalVertexCount(aiScene const *scene);


	/**
	 * @brief Get the Position Key Frames object
	 * 
	 * @param animNode 
	 * @return std::vector<PosKey> 
	 */
	std::vector<PosKey> GetPositionKeyFrames(aiNodeAnim *animNode);
	/**
	 * @brief Get the Rotation Key Frames object
	 * 
	 * @param animNode 
	 * @return std::vector<RotKey> 
	 */
	std::vector<RotKey> GetRotationKeyFrames(aiNodeAnim *animNode);
	/**
	 * @brief Get the Scaling Key Frames object
	 * 
	 * @param animNode 
	 * @return std::vector<ScaKey> 
	 */
	std::vector<ScaKey> GetScalingKeyFrames(aiNodeAnim *animNode);
}