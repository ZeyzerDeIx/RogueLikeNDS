#pragma once

#include "pch.h"
#include <vector>
#include <memory>
#include <string>

class GameObject
{
public:
	virtual ~GameObject() = default;
	
	virtual void Update(FixedPoint dt);

	template <class T>
	static T* InstantiateGO(std::string name)
	{
		static_assert(std::is_base_of_v<GameObject, T>);
		s_GameObjects.push_back(std::unique_ptr<T>(new T(std::move(name))));
		return static_cast<T*>(s_GameObjects.back().get());
	}

	static void UpdateAllGameObjects(FixedPoint dt);

protected:
	GameObject(std::string name);

	std::string m_Name;

private:
	static std::vector<std::unique_ptr<GameObject>> s_GameObjects;
};