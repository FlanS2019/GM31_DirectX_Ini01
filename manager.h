#pragma once

class GameObject;

class Manager
{
private:
	static std::list<GameObject*> g_GameObjectList;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

};