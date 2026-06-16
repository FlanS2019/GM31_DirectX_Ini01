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
#include "grass.h"
#include "explosion.h"
#include <list>
#include "box.h"

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
	//木を10個増やす
	for (int i = 0; i < 10; i++)
	{
		AddGameObject<Tree>()->SetPosition({ -10.0f + i *2.0f, 0.0f, 10.0f });
		AddGameObject<enemy>()->SetPosition({ -2.0f + i * 2.0f, 0.0f, 1.0f });
	}
	//AddGameObject<Grass>()->SetPosition({ 5.0f, 0.0f, 3.0f });
	Box* box = AddGameObject<Box>();
	box->SetPosition({ 0.0f, 0.0f, 5.0f });
	box->SetScale({ 2.0f, 2.0f, 2.0f });
	//AddGameObject<Explosion>()->SetPosition({ 0.0f, 0.0f, 5.0f });

	//AddGameObjsect<Bullet>();
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
	if(Input::GetKeyTrigger(VK_F1))
	{
		AddGameObject<enemy>()->SetPosition({ 8.0f, 0.0f, 1.0f });
	}
}

void Manager::Draw()
{
	Renderer::Begin();
	Camera* camera = GetGameObject<Camera>();
	Vector3 forward = camera->GetForward();
	Vector3 position = camera->GetPosition();

	for ( GameObject* gameObject : g_GameObject )
	{
		gameObject->CalCameraZ(position, forward);
	}
	//z sort
	g_GameObject.sort([](GameObject* a, GameObject* b) {
		return a->GetCameraZ() > b->GetCameraZ(); // カメラから遠い順にソート
	});

	for (int i = 0 ; i < 4; i++) // ソート後の順番で描画
	{
		for (GameObject* gameObject : g_GameObject)
		{
			if (gameObject->GetLayer() == i)
			{
				gameObject->Draw();
			}
		}
	}
	//for(int layer = 0; layer <= 10; layer++) // レイヤー順に描画
	//{
	//	for (GameObject* gameObject : g_GameObject)
	//	{
	//		if(gameObject->GetLayer() == layer)
	//		{
	//			gameObject->Draw();
	//		}
	//	}
	//}
	Renderer::End();
}
