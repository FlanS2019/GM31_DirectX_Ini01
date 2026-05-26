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
		T* gameObject = new T();
		gameObject->Init();
		g_GameObject.push_back(gameObject);
		return gameObject;
	}
	template<typename T>
	static T* GetGameObject()
	{
		for (GameObject* gameObject : g_GameObject)
		{
			T* find = dynamic_cast<T*>(gameObject);
			if (find != nullptr) 
			return find;
		}
		return nullptr;
	}
};