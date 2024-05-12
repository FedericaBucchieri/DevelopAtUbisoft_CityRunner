#pragma once
#include <unordered_map>
#include <typeindex>

//
// The CObject class serves as a foundational entity in the game, managing components and their life cycles. 
// It provides methods for initialization, activation, and dynamic addition/removal of components. 
// The class utilizes a mapping system for efficient component organization based on types.
//


class CObject {

public:
	CObject() : b_isActive(true) {};
	virtual ~CObject();
    virtual void Init();
    virtual void DeInit();
	virtual void OnBeginPlay();
	virtual void OnBeginDestroy();
	virtual void Activate();
	virtual void Deactivate();

	virtual void Update(float deltaTime);
    virtual void Render();

	virtual void AddComponent(CComponent* component);
	virtual void RemoveComponent(CComponent* component);

	int GetUniqueID() { return m_uniqueID; }

	template <typename T>
	T* GetFirstComponentOfClass()
	{
		auto typeIndex = std::type_index(typeid(T));

		auto it = m_componentMap.find(typeIndex);
		if (it != m_componentMap.end())
		{
			auto& componentsOfType = it->second;
			if (!componentsOfType.empty())
			{
				return static_cast<T*>(componentsOfType.begin()->second);
			}
		}

		return nullptr;
	}

	template <typename T>
	void AddComponentToMap(T* component)
	{
		auto typeIndex = std::type_index(typeid(*component));

		auto itType = m_componentMap.find(typeIndex);
		if (itType == m_componentMap.end())
		{
			m_componentMap[typeIndex] = std::map<int, CComponent*>();
		}

		auto& componentsOfType = m_componentMap[typeIndex];

		auto it = componentsOfType.find(component->GetUniqueID());
		if (it == componentsOfType.end())
		{
			componentsOfType.emplace(component->GetUniqueID(), component);
		}
	}

	template <typename T>
	void RemoveComponentFromMap(T* component)
	{
		auto typeIndex = std::type_index(typeid(*component));
		auto it = m_componentMap.find(typeIndex);
		if (it != m_componentMap.end())
		{
			auto& componentsOfType = it->second;
			componentsOfType.erase(component->GetUniqueID());
		}
	}

protected:
	std::vector<CComponent*> v_p_components;
	std::map<std::type_index, std::map<int, CComponent*>> m_componentMap;

	int m_uniqueID;
	bool b_isActive;
};
