#pragma once
#include "vector3.h"
#include "component.h"
#include <d3d11.h>
#include <list>
#include <type_traits>


class GameObject
{
protected: // サブクラスが扱えるように protected
	Vector3 m_Position{ 0,0,0 };
	Vector3 m_Rotation{ 0,0,0 };
	Vector3 m_Scale{ 1,1,1 };

	std::list<Component*> m_Components;

public:
	void SetPosition(const Vector3& position) { m_Position = position; }
	Vector3 GetPosition() const { return m_Position; }

	void SetRotation(const Vector3& rotation) { m_Rotation = rotation; }
	Vector3 GetRotation() const { return m_Rotation; }

	void SetScale(const Vector3& scale) { m_Scale = scale; }
	Vector3 GetScale() const { return m_Scale; }

	// ライフサイクル
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
		component->Init();
		return component;
	}
};
