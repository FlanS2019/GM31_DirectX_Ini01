#pragma once
#include <list>

class GameObject;

class Manager
{
private:
	static std::list<GameObject*> g_GameObject;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	template<typename T>//テンプレート関数
	static T* AddGameObject()
	{
		// 生成は派生型ポインタで行い、リストには基底ポインタで格納してから派生型ポインタを返す
		T* obj = new T();
		obj->Init();
		g_GameObject.push_back(static_cast<GameObject*>(obj));
		return obj;
	}
};