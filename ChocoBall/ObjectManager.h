#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "BulletPhysics.h"
#include "Assert.h"

#define OBJECTNAME_MAX 255

// ゲームオブジェクト構造体
typedef struct OBJECT_DATA{
	CHAR objectname[OBJECTNAME_MAX + 1];	// 生成するオブジェクト名
	CGameObject* object;	// ゲームオブジェクトのポインタ
	short priority;			// 実行優先度(数字が小さいほど優先度が高い　最小値：0)
}OBJECT_DATA;

class CObjectManager
{
	SINGLETON_DECL(CObjectManager)
public:

	template<class T>
	//指定したオブジェクトを自動生成する関数(優先度を指定したい場合)
	//引き数: LPCSTR型 インスタンスの名前 
	//		  PRIORTY型 更新優先度(0が最小、数字が大きいほど優先度が低い)
	//		  bool型 常駐フラグ指定(trueならシーン切り替え時に削除されない)
	//返り値: T*型 生成したクラスのオブジェクトを返却
	//呼び出し例: SINSTANCE(CObjectManager)->GenerationObject<生成したいクラス名>(_T("インスタンスの名前"),0,false);
	//※インスタンスの名前はインスタンスごとに被らないようにユニークなものをプログラマーが指定してください
	//※インスタンスの名前は255文字以内としてください
	//※オブジェクトのインスタンスはCObjectManagerクラスの外部でdeleteしないこと
	//※必ずCObjectManagerクラスのDeleteGameObject関数を呼び出して行うこと
	T* GenerationObject(LPCSTR ObjectName,PRIORTY priorty,bool common){
		T* Object = new T;
		Object->ActiveManagerNewFlg();	// ObjectManagerクラス内でnewしたため、フラグをtrueにする
		Object->SetCommon(common);
		if (priorty > PRIORTY::MAX_PRIORTY){
			priorty = PRIORTY::MAX_PRIORTY;
		}
		this->Add(Object, ObjectName,priorty);
		return Object;
	}

	template<class T>
	//指定したオブジェクトを自動生成する関数(優先度なし：自動的に優先度は最低になります)
	//引き数: LPCSTR型 インスタンスの名前
	//		  bool型 常駐フラグ指定(trueならシーン切り替え時に削除されない)
	//返り値: T*型 生成したクラスのオブジェクトを返却
	//呼び出し例: SINSTANCE(CObjectManager)->GenerationObject<生成したいクラス名>(_T("インスタンスの名前"),false);
	//※インスタンスの名前はインスタンスごとに被らないようにユニークなものをプログラマーが指定してください
	//※インスタンスの名前は255文字以内としてください
	//※オブジェクトのインスタンスはCObjectManagerクラスの外部でdeleteしないこと
	//※必ずCObjectManagerクラスのDeleteGameObject関数を呼び出して行うこと
	T* GenerationObject(LPCSTR ObjectName,bool common){
		T* Object = new T;
		Object->ActiveManagerNewFlg();	// ObjectManagerクラス内でnewしたため、フラグをtrueにする
		Object->SetCommon(common);
		PRIORTY priorty = PRIORTY::MAX_PRIORTY;
		this->Add(Object,ObjectName ,priorty);
		return Object;
	}

	//すでに生成されているオブジェクトをマネージャークラスに登録する関数(優先度を指定したい場合)
	//引き数: CGameObject*型 登録するGameObjectのポインタ
	//		　short型 更新優先度(0が最小、数字が大きいほど優先度が低い)
	//		  bool型 常駐フラグ指定(trueならシーン切り替え時に削除されない)
	//返り値: なし
	//呼び出し例: SINSTANCE(CObjectManager)->AddObject(追加したいインスタンスのポインタ,_T("インスタンスの名前"),0);
	//※インスタンスの名前はインスタンスごとに被らないようにユニークなものをプログラマーが指定してください
	//※インスタンスの名前は255文字以内としてください
	//※この関数で登録したインスタンスをnewで生成している場合は、必ずCObjectManagerクラスのDeleteGameObject関数を呼び出した後に
	//  CObjectManagerクラスの外部できちんとdeleteしてください
	void AddObject(CGameObject*,LPCSTR, PRIORTY,bool);

	//すでに生成されているオブジェクトをマネージャークラスに登録する関数(優先度なし：自動的に優先度は最低になります)
	//引き数: CGameObject*型 登録するGameObjectのポインタ
	//		  bool型 常駐フラグ指定(trueならシーン切り替え時に削除されない)
	//返り値: なし
	//呼び出し例: SINSTANCE(CObjectManager)->AddObject(追加したいインスタンスのポインタ,_T("インスタンスの名前"));
	//※インスタンスの名前はインスタンスごとに被らないようにユニークなものをプログラマーが指定してください
	//※インスタンスの名前は255文字以内としてください
	//※この関数で登録したインスタンスをnewで生成している場合は、必ずCObjectManagerクラスのDeleteGameObject関数を呼び出した後に
	//  CObjectManagerクラスの外部できちんとdeleteしてください
	void AddObject(CGameObject*, LPCSTR,bool);

	template<class T>
	//ObjectManagerクラスに登録されているGameObjectのインスタンスを名前で検索する関数
	//引き数: LPCSTR型 インスタンスの名前
	//返り値: CGameObject*型 引き数の名前と一致したオブジェクトのポインタ(どれか一つ)
	//呼び出し例: SINSTANCE(CObjectManager)->FindGameObject<検索したいクラス名>(_T("インスタンスの名前"));
 	//※複数名前が一致した場合どれか一つが返されます
	//※確実にそのインスタンスを取得したい場合はプログラマーがきちんとユニークな名前で登録してください
	//※関数が重いため多用しないこと
	T* FindGameObject(LPCSTR ObjectName)
	{
		int size = m_GameObjects.size();
		for (int idx = 0; idx < size; idx++){
			if (!strcmp(m_GameObjects[idx]->objectname,ObjectName)){
				return (T*)m_GameObjects[idx]->object;
			}
		}
		//MessageBox(NULL, "オブジェクトが登録されていません", 0, 0);
		return nullptr;
	}

	//Objectmanagerクラスに登録されているGameObjectのインスタンスを名前で検索し、一致したものを一件削除する関数
	//※ここでは削除リストにプールされるだけで、削除自体はCObjectManagerクラスのExcuteDeleteObjects関数にて行われる
	//引き数: LPCSTR型 インスタンスの名前
	//返り値: なし
	//呼び出し例: SINSTANCE(CObjectManager)->DeleteGameObject(_T("インスタンスの名前"));
	//※最初にインスタンスの名前が一致したものが一件だけ削除されます
	//※確実にそのインスタンスを削除したい場合はプログラマーがきちんとユニークな名前で登録してください
	//※オブジェクトのインスタンスはCObjectManagerクラスの外部でdeleteしないこと
	//※オブジェクトの削除は必ずこの関数を呼び出して行うこと
	//※関数が重いため多用しないこと
	void DeleteGameObject(LPCSTR);

	//Objectmanagerクラスに登録されているGameObjectのインスタンスを一件削除する関数
	//※ここでは削除リストにプールされるだけで、削除自体はCObjectManagerクラスのExcuteDeleteObjects関数にて行われる
	//引き数: CGameObject*型 削除したいオブジェクトのポインタ
	//返り値: なし
	//呼び出し例: SINSTANCE(CObjectManager)->DeleteGameObject(_T("インスタンスの名前"));
	//※オブジェクトのインスタンスはCObjectManagerクラスの外部でdeleteしないこと
	//※オブジェクトの削除は必ずこの関数を呼び出して行うこと
	//※関数が重いため多用しないこと
	void DeleteGameObject(CGameObject*);
	//こちらは即時デリート。
	void DeleteGameObjectImmediate(CGameObject*);
	//削除リストに登録されている、常駐GameObject以外を削除リストにすべてプールする関数
	//引き数: なし
	//返り値: なし
	//呼び出し例: SINSTANCE(CObjectManager)->CleanManager();
	//※シーン切り替えの際に呼び出す
	//※この関数では常駐オブジェクトは削除できない
	void CObjectManager::CleanManager();

	//削除リストに登録されているGameObjectを削除する関数
	//引き数: なし
	//返り値: なし
	//呼び出し例: SINSTANCE(CObjectManager)->ExcuteDeleteObject();
	//※必ずメインループで最後に呼び出す
	void ExcuteDeleteObjects();

	void Intialize();
	void Update();
	void Draw();
	const vector<OBJECT_DATA*>& GetObjectList(){
		return m_GameObjects;
	}
private:
	void Add(CGameObject*,LPCSTR, PRIORTY);
	vector<OBJECT_DATA*> m_GameObjects;	// GameObject*のリスト
	vector<CGameObject*> m_DeleteObjects;	// 削除リスト

	//vectorに登録された要素をすべて削除する関数
	//※デストラクタにて呼び出される
	void DeleteAll();
};