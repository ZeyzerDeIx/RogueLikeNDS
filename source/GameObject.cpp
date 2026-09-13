#include "GameObject.h"
#include <utility>

using namespace std;

vector<unique_ptr<GameObject>> GameObject::s_GameObjects;

void GameObject::Update(int dt) {}


void GameObject::UpdateAllGameObjects(int dt)
{
	for(unique_ptr<GameObject>& go : s_GameObjects)
		go->Update(dt);
}

GameObject::GameObject(string name): m_Name(std::move(name)) {}