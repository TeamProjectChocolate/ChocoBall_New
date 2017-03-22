// DirectX.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "DirectX.h"
#include "MainScene.h"
#include "Effect.h"
#include "Camera.h"
#include "ImageManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "RenderContext.h"
#include "GameManager.h"
#include "ShadowRender.h"
#include "SceneTitle.h"
#include "SceneResult.h"
#include "BulletPhysics.h"
#include "StageManager.h"
#include "tkStopwatch.h"
#include "Icon.h"
#include "DirectFont.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

HWND g_hWnd;
CGraphicsDevice g_graphicsDevice;
CSceneTitle SceneTitle;
CMainScene MainScene;
CSceneResult SceneResult;
CCamera g_camera;
void addScene();
void Initialize();
void Update();
void Draw();

CStopwatch g_watch;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIRECTX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX));

	Initialize();

	// メイン メッセージ ループ:
	do
	{
		//メッセージが存在するか検証
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージに変換
			DispatchMessage(&msg);		//メッセージをウィンドウプロシージャに送出
		}
		else
		{
			//if (SINSTANCE(CInputManager)->GetCurrentInput()->IsTriggerEscape()){
			//	PostQuitMessage(0);
			//}
			Update();
			Draw();	
			SINSTANCE(CShadowRender)->ExcuteDeleteObjects();
			SINSTANCE(CGameManager)->SetNextScene();
			SINSTANCE(CObjectManager)->ExcuteDeleteObjects();

			
			//MessageBox(nullptr, text, _T("メッセージ"), MB_OK);
		}

	} while (msg.message != WM_QUIT);

	g_graphicsDevice.FreeDX();

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DIRECTX);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void AddScene(){	// ゲームで使用するシーンを登録
	SINSTANCE(CGameManager)->AddScene(&SceneTitle, _T("Title"));
	SINSTANCE(CGameManager)->AddScene(&MainScene, _T("Main"));
	SINSTANCE(CGameManager)->AddScene(&SceneResult, _T("Result"));
}

// 物理エンジンを扱うためのクラス。
CBulletPhysics g_BulletPhysics;

void Initialize()
{
	graphicsDevice().InitD3d(g_hWnd);
	CGameManager::CreateInstance();			// シングルトンクラス:ゲーム管理クラスのインスタンスを生成
	CEffect::CreateInstance();				// シングルトンクラス:エフェクトファイル管理クラスのインスタンスを生成
	CImageManager::CreateInstance();		// シングルトンクラス:オブジェクトのモデル情報管理クラスのインスタンスを生成
	CShadowRender::CreateInstance();		// シングルトンクラス:影生成するオブジェクト管理クラスのインスタンスを生成
	SINSTANCE(CShadowRender)->Initialize();
	CObjectManager::CreateInstance();		// シングルトンクラス:オブジェクト管理クラスのインスタンスを生成
	SINSTANCE(CObjectManager)->OnCreate();	// オブジェクトマネージャー初期化。
	CInputManager::CreateInstance();		// シングルトンクラス:入力インタフェース管理クラスのインスタンスを生成
	CRenderContext::CreateInstance();		// シングルトンクラス:現在設定中カメラの管理クラスのインスタンスを生成
	SINSTANCE(CRenderContext)->CreateRenderingTerget();
	CStageManager::CreateInstance();		// シングルトンクラス:ステージ管理クラスのインスタンスを生成
  	SINSTANCE(CInputManager)->DI_Init();
	SINSTANCE(CInputManager)->CreateInput(g_hWnd);
	CDirectFont::CreateInstance();		// シングルトンクラス:文字出力クラスのインスタンスを生成。
	SINSTANCE(CDirectFont)->Initialize();
	CAudio* pAudio = new CAudio;
	pAudio->Initialize("Audio/Audio.xgs", "Audio/Audio.xwb", "Audio/Audio.xsb");
	SINSTANCE(CStageManager)->SetAudio(pAudio);
	SINSTANCE(CGameManager)->SetAudio(pAudio);
	AddScene();
	SINSTANCE(CGameManager)->ChangeScene(_T("Title"/*"Main"*/));
	// コリジョンワールドを持つクラスのインスタンスを登録(シーン切り替えで削除されない)。
	SINSTANCE(CObjectManager)->AddObject(&g_BulletPhysics, _T("BulletPhysics"),OBJECT::PRIORTY::CONFIG, true);
	//SINSTANCE(CObjectManager)->GenerationObject<CIcon>(_T("Kill_Icon"), PRIORTY::OBJECT2D_ALPHA, true);
	//SINSTANCE(CObjectManager)->GenerationObject<CIcon>(_T("Clear_Icon"), PRIORTY::OBJECT2D_ALPHA, true)->SetFileName(_T("Image/CLEAR_icon.png"));
	SINSTANCE(CGameManager)->SetNextScene();
}

CStopwatch g_Watch;
string g_text;

void Update()
{
#ifdef TEISYUTU_YOU
#else
	g_Watch.Start();
#endif
	SINSTANCE(CInputManager)->Update();
	SINSTANCE(CGameManager)->Update();		//シーン更新
}

void Draw()
{
	//(*graphicsDevice()).Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (SUCCEEDED((*graphicsDevice()).BeginScene()))
	{
		(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, true);
		(*graphicsDevice()).SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		SINSTANCE(CGameManager)->Draw();		// シーン描画
#ifdef TEISYUTU_YOU
#else
		SINSTANCE(CDirectFont)->Draw(g_text.c_str(), 730, 20);
#endif
		(*graphicsDevice()).EndScene();
	}

	(*graphicsDevice()).Present(NULL, NULL, NULL, NULL);
#ifdef TEISYUTU_YOU
#else
	g_Watch.Stop();
	double counter = g_Watch.GetElapsed();
	counter = 1.0 / counter;
	g_text = "FPS = ";
	g_text = g_text + to_string(counter);
#endif
}
