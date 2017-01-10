#include "stdafx.h"
#include "MapObjectManager.h"


void CMapObjectManager::Initialize() {
	//int size = sizeof(MapObjects_Boss) / sizeof(MapObjects_Boss[0]);
	//for (MapObject Obj : MapObjects_Boss) {
	//	CGameObject* WorkObj = new CGameObject;
	//	WorkObj->SetFileName(Obj.FileName);
	//	WorkObj->Initialize();
	//	WorkObj->SetPos(Obj.Position);
	//	WorkObj->SetQuaternion(Obj.Rotation);
	//	WorkObj->SetScale(Obj.Scale);
	//	m_MapObjects.push_back(WorkObj);
	//}
}

void CMapObjectManager::Update() {
	for (auto obj : m_MapObjects) {
		obj->Update();
	}
}

void CMapObjectManager::Draw() {
	for (auto obj : m_MapObjects) {
		obj->Draw();
	}
}

void CMapObjectManager::DrawShadow(CCamera* camera) {
	for (auto obj : m_MapObjects) {
		obj->DrawShadow(camera);
	}
}

void CMapObjectManager::Draw_EM(CCamera* camera) {
	for (auto obj : m_MapObjects) {
		obj->Draw_EM(camera);
	}
}
