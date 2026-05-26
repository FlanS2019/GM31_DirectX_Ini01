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
	AddGameObject<Polygon2D>();

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
	// キー状態を先に更新してから各オブジェクトを更新する（Input の状態に依存する Update の正しい動作のため）
	Input::Update();

	for (GameObject* gameObject : g_GameObject)
	{
		gameObject->Update();
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
