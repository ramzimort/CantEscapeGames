/**
 * @file CustomSystem.h
 * @author your name (you@domain.com)
 * @brief System that handles Update for the scripted component of GameObjects
 * @date 2019-11-21
 */

#pragma once

#include "../BaseSystem.h"

class CustomComponent;
class AppRenderer;

/**
 * @brief CustomSystem will only update CustomComponent. 
 One gameObj can have multiple CustomComponents, so that has to be accounted for in the node
 * 
 */
struct CustomCompNode : BaseSystemCompNode
{
	/**
	 * @brief In this case, we can have an arbitrary number of custom components
	 * 
	 */
	std::vector<CustomComponent*> n_components;

	/**
	 * @brief Construct a new Custom Comp Node object
	 * 
	 * @param compList 
	 */
	CustomCompNode(std::vector<CustomComponent*> const& compList)
	{
		for (CustomComponent *comp : compList)
		{
			n_components.push_back(comp);
		}
	}
};


/**
 * @brief System that handles Update for the scripted component of GameObjects
 * 
 */
class CustomSystem : public BaseSystem
{

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	/**
	 * @brief Construct a new Custom System object
	 * 
	 */
	CustomSystem();
	/**
	 * @brief Destroy the Custom System object
	 * 
	 */
	virtual ~CustomSystem() {}

	/**
	 * @brief Registers a GameObject onto this system. Only GameObjects with custom components can register
	 * 
	 * @param go 
	 */
	virtual void Register_GameObject(GameObject *go);
	/**
	 * @brief Calls update on all the scripts of the GameObjects registered with this system
	 * 
	 * @param dt 
	 * @param compNode 
	 */
	virtual void Update(float dt, BaseSystemCompNode *compNode);

	/**
	 * @brief Calls draw on all the scripts of the GameObjects registered with this system
	 *
	 * 
	 * @param compNode
	 */
	virtual void Draw(float dt, BaseSystemCompNode *compNode) override;


	void RegisterAppRenderer(AppRenderer* appRenderer);
public:
	/**
	 * @brief To compare when using templates
	 * 
	 */
	static unsigned int const static_type;

private:
	AppRenderer* m_appRenderer;

};