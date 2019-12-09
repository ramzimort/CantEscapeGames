/**
 * @file BaseComponent.h
 * @author Jose Rosenbluth
 * @brief Base class for all components (engine and scripted)
 * @date 2019-11-21
 */

#pragma once


///Forward dec
class GameObject;
class ResourceManager;
class DXRenderer;
#define MAX_NUM_COMPONENTS			32	//Max number of components (for bit string)

class GameObjectManager;


/**
 * @brief Base class for all components (engine and scripted)
 * 
 */
class BaseComponent 
{

//Friend classes
public:
	friend class Factory;
	friend class GameObject;
	typedef unsigned int ComponentId;

//Public interface
public:
	/**
	 * @brief Construct a new Base Component object
	 * 
	 * @param owner 
	 * @param type 
	 */
	BaseComponent(GameObject *owner, ComponentId type)
	{
		this->m_owner = owner;
		this->m_type = type;
	}
	/**
	 * @brief Destroy the Base Component object
	 * 
	 */
	virtual ~BaseComponent() { }

	/**
	 * @brief Get the Type object
	 * 
	 * @return ComponentId 
	 */
	ComponentId GetType() const { return m_type; }

	/**
	 * @brief Get the Owner object
	 * 
	 * @return GameObject* 
	 */
	GameObject* GetOwner() const { return m_owner; }

private:
	/**
	 * @brief Construct a new Base Component object
	 * 
	 * @param rhs 
	 */
	BaseComponent(BaseComponent& rhs);

	/**
	 * @brief Init will be called on GameObject creation, after the component has been overwritten and the GameObject has had all its components created
	 * 
	 * @param res Resource manager, used if necessary to get any resource
	 * @param dxrenderer Graphic's Manager
	 */
	virtual void Init(ResourceManager* res, DXRenderer* dxrenderer) = 0;
	/**
	 * @brief Begin will be called in a frame after all Objects have been spawned that frame. 
	 This is the place to put code that could ask for any data outside of the owner's GameObject
	 * 
	 * @param goMgr 
	 */
	virtual void Begin(GameObjectManager *goMgr) = 0;


protected:
	/**
	 * @brief Static member, number of unique component types
	 * 
	 */
	static ComponentId numberOfTypes;


protected:
	/**
	 * @brief Id unique per component type
	 * 
	 */
	ComponentId m_type;
	/**
	 * @brief Pointer to GameObject owner of this component
	 * 
	 */
	GameObject *m_owner;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};