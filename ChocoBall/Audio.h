#pragma once
#include "xact3.h"

typedef struct AUDIO_SET{
	IXACT3Engine* pEngine;	//XACTエンジン
	IXACT3WaveBank* pWaveBank;  //ウェーブバンク
	IXACT3SoundBank* pSoundBank;	//サウンドバンク
	const char* xactEngineName;	//XACTエンジン名
	const char* waveBankName;   //ウェーブバンク名
	const char* soundBankName;	//サウンドバンク名
	VOID* pbWaveBank;           //ウェーブバンクハンドル
	VOID* pbSoundBank; // サウンドバンクハンドル
	XACTINDEX cueIndex;                //キューインデックス

}AOUDI_SET;

typedef struct SOUND_DATA{
	char Name[FILENAME_MAX];
	void* Pointer;
};

//XACT初期化
class CAudio{
public:
	CAudio();
	~CAudio();
	//XACT初期化関数（エンジン名、ウェーブバンク名、サウンドバンク名)
	HRESULT Initialize(const char*, const char*, const char*);
	//エンジン作成
	HRESULT CreateEngine(const char*);
	//ウェーブバンク作成
	HRESULT CreateWaveBank(const char*);
	//サウンドバンク作成
	HRESULT CreateSoundBank(const char*);
	//XACTエンジンの周期的タスク実行用
	void Run();
	//XACT終了関数
	void CleanupXACT();

	//キュー再生関数(キュー名)
	//引き数:const char*型 サウンド名
	//		 bool型 音を重ねて再生するかのフラグ(trueなら重ねる,falseなら重ねない)
	void PlayCue(const char*,bool,void*);

	//キュー停止関数(キュー名)
	void StopCue(const char*,bool,void*);

	void AddSound(LPCSTR name,void* pointer){
		SOUND_DATA* work = new SOUND_DATA;
		strcpy(work->Name, name);
		work->Pointer = pointer;
		m_SoundDataArray.push_back(work);
	}

	bool DeleteSound(LPCSTR name,void* pointer){
		for (vector<SOUND_DATA*>::iterator itr = m_SoundDataArray.begin(); itr != m_SoundDataArray.end();){
			if ((*itr)->Pointer == pointer){
				if (!strcmp(name, (*itr)->Name)){
					(*itr)->Pointer = nullptr;
					SAFE_DELETE(*itr);
					itr = m_SoundDataArray.erase(itr);
					return true;
				}
				else{
					itr++;
				}
			}
			else{
				itr++;
			}
		}
		return false;
	}

	void DeleteAll(){
		for (vector<SOUND_DATA*>::iterator itr = m_SoundDataArray.begin(); itr != m_SoundDataArray.end();){
			(*itr)->Pointer = nullptr;
			SAFE_DELETE(*itr);
			itr = m_SoundDataArray.erase(itr);
		}
		m_SoundDataArray.clear();
	}

	bool FindSound(LPCSTR name,void* pointer){
		int size = m_SoundDataArray.size();
		for (int idx = 0; idx < size; idx++){
			if (m_SoundDataArray[idx]->Pointer == pointer){
				if (!strcmp(name, m_SoundDataArray[idx]->Name)){
					return true;
				}
			}
		}
		return false;
	}
private:
	AUDIO_SET m_audio;	//XACTデータ用
	vector<SOUND_DATA*> m_SoundDataArray;
};

//WaveBank生成
