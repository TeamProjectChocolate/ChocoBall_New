#pragma once

#include "stdafx.h"
#include "Scene.h"

#include "Assert.h"

#define SCENENAME_MAX 255

typedef struct SCENE_DATA{
	CHAR SceneName[SCENENAME_MAX + 1];
	CScene* Scene;
}SCENE_DATA;

class CGameManager
{
	SINGLETON_DECL(CGameManager)
public:
	//引き数で渡したSceneのインスタンスをGameManagerクラスに追加する関数
	//引き数:CScene*型 追加する、CSceneクラスを継承したクラスのポインタ 
	//		 LPCSTR型 インスタンスの名前
	//返り値: なし
	//呼び出し例: SINSTANCE(CGameManager)->AddScene(&m_TitleScene,_T("インスタンスの名前"));
	//※インスタンスの名前はインスタンスごとに被らないようにユニークなものをプログラマーが指定してください
	//※インスタンスの名前は255文字以内としてください
	//※この関数で生成したインスタンスをnewで生成している場合は、CGameManagerクラスの外部で必ずdeleteしてください
	void AddScene(CScene*,LPCSTR);

	template<class T>
	//指定したSceneのインスタンスを自動生成する関数
	//引き数: LPCSTR型 インスタンスの名前
	//返り値: なし
	//呼び出し例: SINSTANCE(CGameManager)->GenerationObject<生成したいシーンのクラス名>(_T("インスタンスの名前"));
	//※インスタンスの名前はインスタンスごとに被らないようにユニークなものをプログラマーが指定してください
	//※インスタンスの名前は255文字以内としてください
	//※この関数で生成したインスタンスはCGameManagerクラスの外部でdeleteしないこと
	void GenerationScene(LPCSTR SceneName){
		CH_ASSERT(strlen(SceneName) <= SCENENAME_MAX);
		if (FindScene(SceneName) != nullptr){
			MessageBox(NULL, "その名前のSceneインスタンスはすでに登録されています", 0, 0);
			return;
		}
		T* Scene = new T;
		Scene->ActiveManagerNewFlg();	// ObjectManagerクラス内でnewしたため、フラグをtrueにする
		SCENE_DATA SceneData;
		strcpy(SceneData.SceneName, SceneName);
		SceneData.Scene = Scene;
		this->Add(&SceneData);
	}

	//GameManagerクラスに登録されているSceneクラスのインスタンスを名前で検索する関数
	//引き数: LPCSTR型 インスタンスの名前
	//返り値: CScene*型 引き数の名前と一致したオブジェクトのポインタ(どれか一つ)
	//呼び出し例: SINSTANCE(CGameManager)->FindScene(_T("インスタンスの名前"));
	//※関数が重いため多用しないこと
	CScene* FindScene(LPCSTR);

	//シーン切り替え関数
	//引き数: LPCSTR型 どのシーンに切り替えるか
	//返り値: なし
	//呼び出し例: SINSTANCE(CGameManager)->ChangeScene(_T("インスタンスの名前"));
	//※関数が重いため多用しないこと
	void ChangeScene(LPCSTR);
	void SetNextScene();
	void Update();
	void Draw();
	LPCSTR GetScene(){
		return m_NowSceneName;
	}

	void SetAudio(CAudio* audio){
		m_pAudio = audio;
	}
private:
	vector<SCENE_DATA*> m_Scenes;
	CScene* m_NowScene = nullptr;
	void Initialize();
	void Add(SCENE_DATA*);
	char m_NowSceneName[SCENENAME_MAX + 1];
	char m_NextSceneName[SCENENAME_MAX + 1];
	CAudio* m_pAudio;

	//vectorに登録された要素をすべて削除する関数
	//※デストラクタにて呼び出される
	void DeleteAll();
};

