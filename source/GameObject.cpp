#include "GameObject.h"
#include <utility>

using namespace std;

void GameObject::update(float dt) {}


void GameObject::updateAllGameObjects(float dt)
{
	for(unique_ptr<GameObject>& go : m_gameObjects)
		go->update(dt);
}

GameObject::GameObject(string name): m_name(name) {}