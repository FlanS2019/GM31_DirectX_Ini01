#pragma once
#include "vector3.h"
#include "component.h"
#include <d3d11.h>
#include <list>
#include <type_traits>


class GameObject
{
protected: // 継承先でアクセスできるようにする
	Vector3 m_Position{ 0,0,0 };
	Vector3 m_Rotation{ 0,0,0 };
	Vector3 m_Scale{ 1,1,1 };

	// 各 GameObject ごとに持つコンポーネントのリスト（static ではなくインスタンスメンバ）
	std::list<Component*> m_Components;

public:
	void SetPosition(const Vector3& position) { m_Position = position; }

	// 派生クラスでオーバーライド可能にする
	virtual void Init(){}

	virtual void Uninit()
	{
		for (Component* component : m_Components)
		{
			if (component)
			{
				component->Uninit();
				delete component;
			}
		}
		m_Components.clear();
	}

	virtual void Update()
	{
		for (Component* component : m_Components)
		{
			if (component) component->Update();
		}
	}

	virtual void Draw()
	{
		for (Component* component : m_Components)
		{
			if (component) component->Draw();
		}
	}

	template <typename T>
	T* AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
		T* component = new T(this);
		m_Components.push_back(component);
		// 追加直後に初期化する
		component->Init();
		return component;
	}
};
