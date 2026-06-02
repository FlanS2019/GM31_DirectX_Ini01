#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2d.h"
#include "field.h"
#include "camera.h"
#include "player.h"
#include "Input.h"
#include "gameObject.h"
#include "enemy.h"
#include "bullet.h"
#include "tree.h"
#include <list>

std::list<GameObject*> Manager::g_GameObject;//リストを使用する場合は、配列ではなくリストを宣言する必要があります。

//GameObject* g_GameObject[3];

void Manager::Init()
{
	Renderer::Init();
	Input::Init();

	GameObject* gameObject = nullptr;

	AddGameObject<Camera>();
	AddGameObject<Field>();
	AddGameObject<Player>();
	AddGameObject<enemy>()->SetPosition({ -2.0f, 0.0f, 1.0f });
	AddGameObject<Tree>()->SetPosition({ -5.0f, 0.0f, 5.0f });

	//AddGameObject<Bullet>();
	//AddGameObject<Polygon2D>();
}

void Manager::Uninit()
{
	for(GameObject* gameObject : g_GameObject)
	{
		gameObject->Uninit();
		delete gameObject;
	}
	Renderer::Uninit();
	Input::Uninit();
}

void Manager::Update()
{
	Input::Update();

	for (GameObject* gameObject : g_GameObject)
	{
		gameObject->Update();
	}

	// Destroyされたオブジェクトを削除
	for (auto it = g_GameObject.begin(); it != g_GameObject.end();)
	{
		if ((*it)->Destroy())
		{
			(*it)->Uninit();
			delete* it;
			it = g_GameObject.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	for (GameObject* gameObject : g_GameObject)
	{
		gameObject->Draw();
	}

	Renderer::End();
}
