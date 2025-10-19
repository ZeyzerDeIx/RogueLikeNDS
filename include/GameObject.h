#pragma once

#include <vector>
#include <memory>
#include <string>

class GameObject
{
public:
	virtual ~GameObject() = default;
	
	virtual void update(float dt);

	template <class T>
	static T* instantiateGO(std::string name)
	{
		static_assert(std::is_base_of_v<GameObject, T>);
		m_gameObjects.push_back(std::make_unique<T>(std::move(name)));
		return static_cast<T*>(m_gameObjects.back().get());
	}

	static void updateAllGameObjects(float dt);

private:
	GameObject(std::string name);

	std::string m_name;

	static std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};