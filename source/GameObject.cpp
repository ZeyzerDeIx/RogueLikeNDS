#include "GameObject.h"
#include <utility>

using namespace std;

vector<unique_ptr<GameObject>> GameObject::s_GameObjects;

void GameObject::Update(float dt) {}


void GameObject::UpdateAllGameObjects(float dt)
{
	for(unique_ptr<GameObject>& go : s_GameObjects)
		go->Update(dt);
}

GameObject::GameObject(string name): m_Name(name) {}