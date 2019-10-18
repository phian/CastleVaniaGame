#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

//#include "Mario.h"
//#include "Brick.h"
//#include "Goomba.h"

#include "Simon.h"
#include "PillarOfFire.h"
#include "Scene.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castle Vania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 380

#define MAX_FRAME_RATE 120

//#define ID_TEX_MARIO 0
//#define ID_TEX_ENEMY 10
//#define ID_TEX_MISC 20

#define ID_TEX_SIMON	  0
#define ID_TEX_PILLARFIRE 10
#define ID_TEX_BBOX		  20
#define ID_TEX_SCENE	  30

CGame * game;

CSimon* Simon;
CPillarFire* PillarFire;
CScene* Scene;

//CMario* mario;
//CGoomba* goomba;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (KeyCode)
	{
	//case DIK_D: // reset
	//	/*Simon->SetState(SIMON_STATE_IDLE);
	//	Simon->SetPosition(50.0f, 0.0f);
	//	Simon->SetSpeed(0, 0);*/
	//	if (Simon->GetState() == SIMON_STATE_SITDOWN) 
	//		Simon->SetState(SIMON_STATE_USE_WHIP_SIT);
	//	break;
	//case DIK_S:
	//	if (Simon->GetState() == SIMON_STATE_IDLE) Simon->SetState(SIMON_STATE_USE_WHIP_STAND);
	//	break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	// disable control key when Mario die 
	/*if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	else
		mario->SetState(MARIO_STATE_IDLE);*/

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_SPACE))
		Simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_SPACE))
		Simon->SetState(SIMON_STATE_WALKING_LEFT);
	else if (game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_D))
	{
		if (game->IsKeyDown(DIK_RIGHT))
		{
			Simon->SetState(SIMON_STATE_SITDOWN_RIGHT);

			return;
		}
		if (game->IsKeyDown(DIK_LEFT))
		{
			Simon->SetState(SIMON_STATE_SITDOWN_LEFT);

			return;
		}
		
		Simon->SetState(SIMON_STATE_SITDOWN);
	}
	else if (game->IsKeyDown(DIK_SPACE) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
		Simon->SetState(SIMON_STATE_JUMP);
	else if (game->IsKeyDown(DIK_DOWN) && game->IsKeyDown(DIK_D) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
		Simon->SetState(SIMON_STATE_USE_WHIP_SIT);
	else if (game->IsKeyDown(DIK_S) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
		Simon->SetState(SIMON_STATE_USE_WHIP_STAND);
	else
		if(Simon->GetState() != SIMON_STATE_JUMP) // if previous state is not jump
			Simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	/*textures->Add(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));*/

	textures->Add(ID_TEX_SIMON, L"Textures\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PILLARFIRE, L"Textures\\Candle.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SCENE, L"Textures\\CastlevaniaMapLevel01a(1).png", D3DCOLOR_XRGB(164, 0, 0));

	//textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPANIMATION ani;

	//--------------------------------Scene---------------------------------//
	LPDIRECT3DTEXTURE9 texScene = textures->Get(ID_TEX_SCENE);

	sprites->Add(10001, 1, 1, 1465, 339, texScene);

	ani = new CAnimation(100);
	ani->Add(10001);
	animations->Add(0, ani);

	Scene = new CScene();

	Scene->AddAnimation(0);
	Scene->SetPosition(0.0f, 0.0f);

	objects.push_back(Scene);

	// ----------------------------------------------------------------------//

	//--------------------------------Candle---------------------------------//
	LPDIRECT3DTEXTURE9 texCandle = textures->Get(ID_TEX_PILLARFIRE);

	sprites->Add(10001, 1, 1, 32, 63, texCandle);
	sprites->Add(10002, 33, 1, 64, 63, texCandle);

	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	animations->Add(0, ani);

	for (int i = 0; i < 5; i++)
	{
		PillarFire = new CPillarFire();

		PillarFire->AddAnimation(0);
		PillarFire->SetPosition(177.0f + i * 240, 245.0f);

		objects.push_back(PillarFire);
	}
	
	//-----------------------------------------------------------------------//

	// ----------------------------Simon-------------------------------------//
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	sprites->Add(10001, 316, 199, 346, 261, texSimon); // idle right
	sprites->Add(10002, 135, 3, 167, 64, texSimon); // idle left

	sprites->Add(10013, 135, 3, 167, 64, texSimon); // walking left
	sprites->Add(10014, 77, 0, 106, 63, texSimon);
	sprites->Add(10015, 11, 3, 45, 63, texSimon);

	sprites->Add(10010, 316, 199, 346, 261, texSimon); // walking right
	sprites->Add(10011, 371, 197, 404, 260, texSimon);
	sprites->Add(10012, 434, 199, 468, 262, texSimon);
	 
	sprites->Add(10020, 195, 202, 228, 263, texSimon); // sitdown right
	sprites->Add(10021, 249, 2, 285, 64, texSimon); // sitdown left

	sprites->Add(10030, 195, 202, 228, 263, texSimon); // sit use whip right
	sprites->Add(10031, 0, 265, 49, 329, texSimon); 
	sprites->Add(10032, 432, 333, 470, 394, texSimon); 
	sprites->Add(10033, 373, 332, 420, 393, texSimon);
	 
	sprites->Add(10040, 249, 2, 285, 64, texSimon); // sit use whip left
	sprites->Add(10041, 428, 66, 479, 131, texSimon); 
	sprites->Add(10042, 10, 133, 43, 195, texSimon); 
	sprites->Add(10043, 61, 133, 106, 197, texSimon);

	sprites->Add(10050, 316, 199, 346, 261, texSimon); // stand use whip right
	sprites->Add(10051, 119, 201, 169, 263, texSimon);
	sprites->Add(10052, 74, 200, 107, 262, texSimon);
	sprites->Add(10053, 15, 202, 61, 262, texSimon);

	sprites->Add(10060, 135, 3, 167, 64, texSimon); // stand use whip left
	sprites->Add(10061, 310, 3, 360, 64, texSimon);
	sprites->Add(10062, 373, 3, 408, 63, texSimon);
	sprites->Add(10063, 421, 3, 467, 64, texSimon);

	sprites->Add(10070, 194, 215, 229, 263, texSimon); // jump right
	sprites->Add(10071, 250, 15, 285, 65, texSimon); // jump left
	

	ani = new CAnimation(100); // idle right
	ani->Add(10001);
	animations->Add(0, ani);

	ani = new CAnimation(100); // idle left
	ani->Add(10002);
	animations->Add(1, ani);

	ani = new CAnimation(100); // sitdown right
	ani->Add(10020);
	animations->Add(2, ani);

	ani = new CAnimation(100); // sitdown left
	ani->Add(10021);
	animations->Add(3, ani);

	ani = new CAnimation(100); // walking right
	ani->Add(10010);
	ani->Add(10011);
	ani->Add(10012);
	animations->Add(4, ani);

	ani = new CAnimation(100); // walking left
	ani->Add(10013);
	ani->Add(10014);
	ani->Add(10015);
	animations->Add(5, ani);

	ani = new CAnimation(100); // sit use whip right
	ani->Add(10030);
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(6, ani);

	ani = new CAnimation(100); // sit use whip left
	ani->Add(10040);
	ani->Add(10041);
	ani->Add(10042);
	ani->Add(10043);
	animations->Add(7, ani);

	ani = new CAnimation(100);  // stand use whip right
	ani->Add(10050);
	ani->Add(10051);
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(8, ani);

	ani = new CAnimation(100);  // stand use whip left
	ani->Add(10060);
	ani->Add(10061);
	ani->Add(10062);
	ani->Add(10063);
	animations->Add(9, ani);

	ani = new CAnimation(100); // jump right
	ani->Add(10070);
	animations->Add(10, ani);

	ani = new CAnimation(100); // jump left
	ani->Add(10071);
	animations->Add(11, ani);

	Simon = new CSimon();

	Simon->AddAnimation(0); // idle right
	Simon->AddAnimation(1); // idle left

	Simon->AddAnimation(2); // sitdown right
	Simon->AddAnimation(3); // sitdown left

	Simon->AddAnimation(4); // walking right
	Simon->AddAnimation(5); // walking left

	Simon->AddAnimation(6); // sit use whip right
	Simon->AddAnimation(7); // sit use whip left

	Simon->AddAnimation(8); // stand use whip right
	Simon->AddAnimation(9); // stand use whip left

	Simon->AddAnimation(10); // jump right
	Simon->AddAnimation(11); // jump left

	Simon->SetPosition(80.0f, 245.0f);

	objects.push_back(Simon);

	// ----------------------------------------------------------------------//


	//LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);


	//mario->SetPosition(50.0f, 0);
	//objects.push_back(mario);

	//for (int i = 0; i < 5; i++)
	//{
	//	CBrick* brick = new CBrick();
	//	brick->AddAnimation(601);
	//	brick->SetPosition(100.0f + i * 60.0f, 74.0f);
	//	objects.push_back(brick);

	//	brick = new CBrick();
	//	brick->AddAnimation(601);
	//	brick->SetPosition(100.0f + i * 60.0f, 90.0f);
	//	objects.push_back(brick);

	//	brick = new CBrick();
	//	brick->AddAnimation(601);
	//	brick->SetPosition(84.0f + i * 60.0f, 90.0f);
	//	objects.push_back(brick);
	//}


	//for (int i = 0; i < 90; i++)
	//{
	//	CBrick* brick = new CBrick();

	//	if (i < 30)
	//	{
	//		brick->AddAnimation(601);
	//		brick->SetPosition(0 + i * 16.0f, 150);
	//		objects.push_back(brick);
	//	}
	//	
	//	else if (i < 60)
	//	{
	//		brick->AddAnimation(601);
	//		brick->SetPosition((-700) + i * 16.0f, 200);
	//		objects.push_back(brick);
	//	}

	//	else
	//	{
	//		brick->AddAnimation(601);
	//		brick->SetPosition((-960) + i * 16.0f, 250);
	//		objects.push_back(brick);
	//	}
	//}

	// and Goombas 
	/*for (int i = 0; i < 5; i++)
	{
		goomba = new CGoomba();
		goomba->AddAnimation(701);
		goomba->AddAnimation(702);
		goomba->SetPosition(200 + i * 60, 110);
		goomba->SetState(GOOMBA_STATE_WALKING);
		objects.push_back(goomba);
	}*/

}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// Update camera to follow Simon
	float cx, cy;
	Simon->GetPosition(cx, cy);

	/*cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;*/

	if (cx <= 1065 && cx > 100) // check position to stop camera
	{
		cx -= SCREEN_WIDTH - 400;
		cy -= SCREEN_HEIGHT - 400;
	
		CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	}
}

/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();

	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}