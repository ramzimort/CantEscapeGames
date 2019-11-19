#pragma once

/**
 * @file Serialization.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief  Reflection Helpers and Functions related to Serializing and Deserializing from disk
 */

/**
 * @brief Reflection Helpers and Functions
 * 
 */
namespace CantReflect
{
	/**
	 * @brief Convert any object or component to a JSON string format
	 * 
	 * @param instance 
	 * @return const std::string 
	 */
	const std::string ToJson(rttr::instance instance);
	/**
	 * @brief Load an obj and set its parameters based on the JSON string
	 * 
	 * @param json 
	 * @param obj 
	 * @return true 
	 * @return false 
	 */
	bool FromJson(const std::string& json, rttr::instance obj);
	/**
	 * @brief Load JSON string from file
	 * 
	 * @param filePath 
	 * @return const std::string 
	 */
	const std::string StringifyJson(const std::string& filePath);
}