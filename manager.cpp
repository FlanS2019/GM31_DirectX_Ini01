#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "polygon2d.h"
#include "field.h"
#include "camera.h"

std::list<GameObject*> Manager::g_GameObjectList;//リストを使用する場合は、配列ではなくリストを宣言する必要があります。

//GameObject* g_GameObject[3];

void Manager::Init()
{
	Renderer::Init();

	GameObject* gameObject;

	gameObject = new Camera();
	gameObject->Init();
	g_GameObjectList.push_back(gameObject);

	gameObject = new Field();
	gameObject->Init();
	g_GameObjectList.push_back(gameObject);

	gameObject = new Polygon2D();
	gameObject->Init();
	g_GameObjectList.push_back(gameObject);

}

void Manager::Uninit()
{
	for(GameObject* gameObject : g_GameObjectList)
	{
		gameObject->Uninit();
		delete gameObject;
	}
	Renderer::Uninit();
}

void Manager::Update()
{
	for (GameObject* gameObject : g_GameObjectList)
	{
		gameObject->Update();
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	for (GameObject* gameObject : g_GameObjectList)
	{
		gameObject->Draw();
	}

	Renderer::End();
}
