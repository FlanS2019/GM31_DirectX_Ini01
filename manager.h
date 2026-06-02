#pragma once
#include <list>
#include <vector>

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

	template<typename T>
	static T* AddGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		g_GameObject.push_back(gameObject);
		return gameObject;
	}

	// 単一の最初の一致を返す
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

	// 複数一致を返す（名前を変更）
	template<typename T>
	static std::vector<T*> GetGameObjects()
	{
		std::vector<T*> gameObjects;
		for (GameObject* gameObject : g_GameObject)
		{
			T* find = dynamic_cast<T*>(gameObject);
			if (find != nullptr)
				gameObjects.push_back(find);
		}
		return gameObjects;
	}
};