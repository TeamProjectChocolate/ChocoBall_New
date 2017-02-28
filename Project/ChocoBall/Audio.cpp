#include "stdafx.h"
#include "Audio.h"

CAudio::CAudio(){}//コンストラクタ
CAudio::~CAudio()//デストラクタ
{
	DeleteAll();
	CleanupXACT();
}

//グローバル設定ファイルを用いたXACTエンジン初期化
HRESULT CAudio::Initialize(const char* engineName, const char* wbName, const char* sbName)
{
	if (FAILED(CreateEngine(engineName))){ return E_FAIL; }
	if (FAILED(CreateWaveBank(wbName))){ return E_FAIL; }//ウェーブバンク
	if (FAILED(CreateSoundBank(sbName))){ return E_FAIL; }//サウンドバンク
	return S_OK;	//成功
}

HRESULT CAudio::CreateEngine(const char* engineFileName)
{
	HANDLE hFile;	//ファイルハンドル
	HRESULT hr = E_FAIL;	//結果

					/*エンジン作成*/
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(XACT3CreateEngine(0, &m_audio.pEngine))){ return E_FAIL; }

	m_audio.xactEngineName = engineFileName;


//xgsファイル読み込み
	void* xgsBuf = NULL;//xgsバッファー
	DWORD xgsFileSize;    //xgsファイルサイズ
	bool isSuccess = false;//成功判定
	//ファイル生成
	hFile = CreateFile(engineFileName, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile){
		xgsFileSize = GetFileSize(hFile, NULL);        //ファイルサイズを取得
		if (xgsFileSize != INVALID_FILE_SIZE){       //ファイルサイズ取得判定
			xgsBuf = CoTaskMemAlloc(xgsFileSize);    //ファイルサイズ分メモリ確保
			if (xgsBuf){    //メモリ確保成功
				DWORD byteRead;
				//xgsファイル読込
				if (NULL != ReadFile(hFile, xgsBuf, xgsFileSize, &byteRead, NULL)){
					isSuccess = true;    //成功
				}
			}
		}
		CloseHandle(hFile);        //ファイルハンドルを閉じる
	}

	if (!isSuccess){ return E_FAIL; }

	//読み込んだxgsファイルからエンジン初期化
	XACT_RUNTIME_PARAMETERS xactParams = { 0 };
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	xactParams.pGlobalSettingsBuffer = xgsBuf;
	xactParams.globalSettingsBufferSize = xgsFileSize;
	xactParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;

	if (FAILED(m_audio.pEngine->Initialize(&xactParams))){
		return E_FAIL;
	}
	return S_OK;
}

//WaveBank生成
HRESULT CAudio::CreateWaveBank(const char* wbFileName){
	HANDLE hFile;			//ファイルハンドル
	HANDLE hMapFile;		//ファイルマッピングハンドル
	DWORD dwFileSize;		//ファイルサイズ
	HRESULT hr = E_FAIL;	//結果

	m_audio.waveBankName = wbFileName;		//ウェーブバンク名セット

					/*ウェーブバンク作成*/
	//ウェーブバンクハンドル取得
	hFile = CreateFile(m_audio.waveBankName, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	//CreateFileに成功したか？
	if (hFile != INVALID_HANDLE_VALUE){
		dwFileSize = GetFileSize(hFile, NULL);	//ウェーブバンクファイルサイズ取得
		//ファイルサイズ取得に成功したか？
		if (dwFileSize != -1){
			//ウェーブバンクに対するファイルマッピングオブジェクトを読取り専用で作成
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
			//マッピングに成功したか？
			if (hMapFile){
				//ウェーブバンクファイルマップのビューを設定
				m_audio.pbWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
				//ビューの取得に成功したか？
				if (m_audio.pbWaveBank){
					//メモリ内ウェーブバンクを作成
					if (SUCCEEDED(
						m_audio.pEngine->CreateInMemoryWaveBank(
						m_audio.pbWaveBank, dwFileSize, 0, 0, &m_audio.pWaveBank)))
					{
						hr = S_OK;	//ウェーブバンク取得成功
					}
				}
				CloseHandle(hMapFile); //ファイルマッピングハンドルを閉じる
			}
		}
		CloseHandle(hFile); //ウェーブバンクファイルハンドルを閉じる
	}
	return hr;
}

//SoundBank生成
HRESULT CAudio::CreateSoundBank(const char* sbFileName){
	HANDLE hFile;			//ファイルハンドル
	HANDLE hMapFile;		//ファイルマッピングハンドル
	DWORD dwFileSize;		//ファイルサイズ
	HRESULT hr = E_FAIL;	//結果

	m_audio.soundBankName = sbFileName;		//サウンドバンク名セット

	//サウンドバンクハンドル取得
	hFile = CreateFile(m_audio.soundBankName,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//CreateFileに成功したか？
	if (hFile != INVALID_HANDLE_VALUE){
		dwFileSize = GetFileSize(hFile, NULL);
		//ファイルサイズ取得に成功したか？
		if (dwFileSize != -1){
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
			//マッピングに成功したか？
			if (hMapFile){
				m_audio.pbSoundBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);	//サウンドバンクファイルマップのビューを設定
				//ビューの取得に成功したか？
				if (m_audio.pbSoundBank){
					if (FAILED(m_audio.pEngine->CreateSoundBank(
						m_audio.pbSoundBank, dwFileSize,
						0, 0, &m_audio.pSoundBank))){
						return E_FAIL;	//サウンドバンク作成失敗
					}
					hr = S_OK;	//サウンドバンク生成成功
				}
				CloseHandle(hMapFile); //ファイルマッピングハンドルを閉じる
			}
		}
		CloseHandle(hFile); //サウンドバンクファイルハンドルを閉じる
	}
	return hr;
}

//XACTの周期制御
void CAudio::Run(){
	if (m_audio.pEngine != NULL){
		m_audio.pEngine->DoWork();	//周期的タスク実行
	}
}

//CleanupXACT関数実装
void CAudio::CleanupXACT(){
	if (m_audio.pEngine){
		m_audio.pEngine->ShutDown();    //XACTをシャットダウン
		SAFE_RELEASE(m_audio.pEngine);    //XACTエンジンを解放
	}
	if (m_audio.pbSoundBank){
		UnmapViewOfFile(m_audio.pbSoundBank);    //サウンドバンクのビューをアンマップ
	}
	m_audio.pbSoundBank = NULL;
	if (m_audio.pbWaveBank){
		UnmapViewOfFile(m_audio.pbWaveBank);    //ウェーブバンクのビューをアンマップ    
	}
	m_audio.pbWaveBank = NULL;

	m_audio.pWaveBank->Destroy();
	m_audio.pSoundBank->Destroy();
	CoUninitialize();
}

void CAudio::PlayCue(const char* cue,bool Isrepeat,void* pointer){
	bool work = FindSound(cue, pointer);

	if (!Isrepeat){
		if (work){
			return;
		}
		else{
			AddSound(cue, pointer);
		}
	}
	else{
		if(!work){
			AddSound(cue, pointer);
		}
	}
	
	if (m_audio.pSoundBank != NULL){
		//サウンドバンクから指定したキューのキューインデックス取得
		m_audio.cueIndex = m_audio.pSoundBank->GetCueIndex(cue);
		//キューインデックス取得成功？
		if (m_audio.cueIndex != XACTINDEX_INVALID){
			m_audio.pSoundBank->Play(m_audio.cueIndex, 0, 0, NULL);    //キュー再生
		}
	}
	
}

void CAudio::StopCue(const char* cue,bool Isrepert,void* pointer){
	if (!Isrepert){
		if (m_audio.pSoundBank != NULL){
			// サウンドバンクから指定したキューのキューインデクッス取得
			m_audio.cueIndex = m_audio.pSoundBank->GetCueIndex(cue);
			// キューインデックス取得成功か？
			if (m_audio.cueIndex != XACTINDEX_INVALID){
				m_audio.pSoundBank->Stop(m_audio.cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);		// キュー停止処理
			}
		}
	}
	else if (DeleteSound(cue, pointer)){
		if (m_audio.pSoundBank != NULL){
			// サウンドバンクから指定したキューのキューインデクッス取得
			m_audio.cueIndex = m_audio.pSoundBank->GetCueIndex(cue);
			// キューインデックス取得成功か？
			if (m_audio.cueIndex != XACTINDEX_INVALID){
				m_audio.pSoundBank->Stop(m_audio.cueIndex, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);		// キュー停止処理
			}
		}
	}
}