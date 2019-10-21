#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

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
#include "Knife.h"

using namespace std;

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
#define ID_TEX_KNIFE	  40

CGame * game;

CSimon* Simon;
CPillarFire* PillarFire;
CScene* Scene;
CKnife* Knife;


vector<string> result; // save data read from txt file
int Count = 0; // increase animation id 

bool isUsingKnife;

int temp;

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
	/*case DIK_S:
		if (Simon->GetState() == SIMON_STATE_WALKING_RIGHT || Simon->GetState() == SIMON_STATE_WALKING_LEFT) 
			Simon->SetSpeed(0, 0);
			Simon->SetState(SIMON_STATE_USE_WHIP_STAND);
		break;*/
	case DIK_F:
		if (Simon->vx > 0)
		{
			Simon->SetSpeed(0, 0);
			Knife->SetState(KNIFE_STATE_MOVING_RIGHT);
			Knife->SetPosition(Simon->x + 10, Simon->y + 15);
		}
		else if (Simon->vx < 0)
		{
			Simon->SetSpeed(0, 0);
			Knife->SetState(KNIFE_STATE_MOVING_LEFT);
			Knife->SetPosition(Simon->x + 10, Simon->y + 15);
		}
		else if (Simon->GetState() == SIMON_STATE_IDLE || Simon->GetState() == SIMON_STATE_JUMP || Simon->GetState() == SIMON_STATE_USE_WHIP_STAND)
		{
			if (Simon->nx > 0)
			{
				Knife->SetState(KNIFE_STATE_MOVING_RIGHT);
				Knife->SetPosition(Simon->x + 10, Simon->y + 15);
			}
			else
			{
				Knife->SetState(KNIFE_STATE_MOVING_LEFT);
				Knife->SetPosition(Simon->x + 10, Simon->y + 15);
			}
		}

		isUsingKnife = true;
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

	if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_SPACE) && !game->IsKeyDown(DIK_S) && !game->IsKeyDown(DIK_D))
	{
		if (Simon->vy == 0)
			Simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_SPACE) && !game->IsKeyDown(DIK_S) && !game->IsKeyDown(DIK_D))
	{

		if (Simon->GetState() == SIMON_STATE_JUMP) // Simon jump one time (jump again when simon is idle)
			return;
		if (Simon->vy == 0 && Simon->GetState() != SIMON_STATE_JUMP)
			Simon->SetState(SIMON_STATE_JUMP); // jump again when simon is idle
		if (Simon->y >= 225)
			Simon->SetSpeed(SIMON_WALKING_SPEED, -SIMON_JUMP_SPEED_Y);
	}
	else if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_SPACE) && !game->IsKeyDown(DIK_S) && !game->IsKeyDown(DIK_D))
	{
		if (Simon->vy == 0)
			Simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_LEFT) && game->IsKeyDown(DIK_SPACE) && !game->IsKeyDown(DIK_S) && !game->IsKeyDown(DIK_D))
	{
		if (Simon->GetState() == SIMON_STATE_JUMP)
			return;
		if (Simon->vy == 0 && Simon->GetState() != SIMON_STATE_JUMP)
			Simon->SetState(SIMON_STATE_JUMP); // jump again when simon is idle
		if (Simon->y >= 225) Simon->SetSpeed(-SIMON_WALKING_SPEED, -SIMON_JUMP_SPEED_Y);
	}
	else if (game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_D) && Simon->GetState() != SIMON_STATE_JUMP) // if Simon is jumping -> disable DIK_DOWN
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
		else
			Simon->SetState(SIMON_STATE_SITDOWN);
	}
	else if (game->IsKeyDown(DIK_SPACE) && !game->IsKeyDown(DIK_S) && !game->IsKeyDown(DIK_D))
	{
		if (Simon->GetState() == SIMON_STATE_JUMP)
			return;
		else if (Simon->GetState() != SIMON_STATE_JUMP) // jump again when simon is idle
			Simon->SetState(SIMON_STATE_JUMP);
	}
	else if (game->IsKeyDown(DIK_DOWN) && game->IsKeyDown(DIK_D) && !game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
		Simon->SetState(SIMON_STATE_USE_WHIP_SIT);
	else if (game->IsKeyDown(DIK_S))
	{
		if (Simon->GetState() == SIMON_STATE_WALKING_RIGHT || Simon->GetState() == SIMON_STATE_WALKING_LEFT)
		{
			Simon->SetSpeed(0, 0);
			Simon->SetState(SIMON_STATE_USE_WHIP_STAND);
		}
		else if (Simon->GetState() == SIMON_STATE_IDLE || Simon->GetState() == SIMON_STATE_JUMP)
			Simon->SetState(SIMON_STATE_USE_WHIP_STAND);
	}
	else if (game->IsKeyDown(DIK_F))
	{
		Simon->SetSpeed(0, 0);
		Simon->SetState(SIMON_STATE_USE_WHIP_STAND);
	}
	else
		if (Simon->GetState() != SIMON_STATE_JUMP) // if previous state is not jump
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

vector<string> ReadFile(int id)
{
	fstream outputFile;
	vector<string> data;
	string line;

	if (id == 0)
	{
		outputFile.open("TexturesData\\TexturesPath.txt", ios::in);

		while (!outputFile.eof())
		{
			getline(outputFile, line);

			data.push_back(line);
		}

		outputFile.close();
	}

	else if (id == 1)
	{
		outputFile.open("TexturesData\\SimonSprites.txt", ios::in);

		while (!outputFile.eof())
		{
			getline(outputFile, line);

			data.push_back(line);
		}
	}

	else if (id == 2)
	{
		outputFile.open("TexturesData\\CandleSprites.txt", ios::in);

		while (!outputFile.eof())
		{
			getline(outputFile, line);

			data.push_back(line);
		}
	}

	else if (id == 3)
	{
		outputFile.open("TexturesData\\BackgroundSprites.txt", ios::in);

		while (!outputFile.eof())
		{
			getline(outputFile, line);

			data.push_back(line);
		}
	}

	else if (id == 4)
	{
		outputFile.open("TexturesData\\KnifeSprites.txt");

		while (!outputFile.eof())
		{
			getline(outputFile, line);

			data.push_back(line);
		}
	}

	return data;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();

	result = ReadFile(0);

	wstring stemp = wstring(result[0].begin(), result[0].end());
	LPCWSTR path = stemp.c_str();
	textures->Add(ID_TEX_SIMON, path, D3DCOLOR_XRGB(255, 0, 255));

	stemp = wstring(result[1].begin(), result[1].end());
	path = stemp.c_str();
	textures->Add(ID_TEX_PILLARFIRE, path, D3DCOLOR_XRGB(255, 0, 255));

	stemp = wstring(result[2].begin(), result[2].end());
	path = stemp.c_str();
	textures->Add(ID_TEX_SCENE, path, D3DCOLOR_XRGB(164, 0, 0));

	stemp = wstring(result[3].begin(), result[3].end());
	path = stemp.c_str();
	textures->Add(ID_TEX_KNIFE, path, D3DCOLOR_XRGB(255, 0, 255));

	//textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	LPANIMATION ani;

	//--------------------------------Scene---------------------------------//
	LPDIRECT3DTEXTURE9 texScene = textures->Get(ID_TEX_SCENE);

	result = ReadFile(3);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };
		if (data.size() > 0)
			sprites->Add(stoi(data[0]), stoi(data[1]), stoi(data[2]), stoi(data[3]), stoi(data[4]), texScene);
	}

	ani = new CAnimation(100);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };

		if(data.size() > 0)
			ani->Add(stoi(data[0]));
		else if (data.size() == 0)
		{
			animations->Add(Count, ani);
			temp = Count;
			++Count;
			ani = new CAnimation(100);
		}
	}
	
	Scene = new CScene();
	
	for (int i = 0; i <= Count; i++)
	{
		Scene->AddAnimation(i);
	}

	Scene->SetPosition(0.0f, 0.0f);

	objects.push_back(Scene);

	Count = 0; // Reset to count for next sprite

	// ----------------------------------------------------------------------//

	//--------------------------------Candle---------------------------------//
	LPDIRECT3DTEXTURE9 texCandle = textures->Get(ID_TEX_PILLARFIRE);

	result = ReadFile(2);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };
		if (data.size() > 0)
			sprites->Add(stoi(data[0]), stoi(data[1]), stoi(data[2]), stoi(data[3]), stoi(data[4]), texCandle);
	}

	ani = new CAnimation(100);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };

		if (data.size() > 0)
			ani->Add(stoi(data[0]));
		else if (data.size() == 0)
		{
			animations->Add(Count, ani);
			++Count;
			ani = new CAnimation(100);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		PillarFire = new CPillarFire();

		for (int j = 0; j <= Count; j++)
		{
			PillarFire->AddAnimation(j);
		}

		PillarFire->SetPosition(177.0f + i * 240, 245.0f);

		objects.push_back(PillarFire);
	}

	Count = 0; // Reset to count for next sprite
	
	//-----------------------------------------------------------------------//

	// ----------------------------Simon-------------------------------------//
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	result = ReadFile(1);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> position{ istream_iterator<string>{iss},
									istream_iterator<string>{} };
		if (position.size() > 0)
			sprites->Add(stoi(position[0]), stoi(position[1]), stoi(position[2]), stoi(position[3]), stoi(position[4]), texSimon);
	}

	ani = new CAnimation(100);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };
		if (data.size() > 0)
			ani->Add(stoi(data[0]));
		else if(data.size() == 0)
		{
			animations->Add(Count, ani);
			++Count;
			ani = new CAnimation(100);
		}
	}

	Simon = new CSimon();

	for (int i = 0; i <= Count; i++)
	{
		Simon->AddAnimation(i);
	}

	Simon->SetPosition(80.0f, 245.0f);

	objects.push_back(Simon);

	Count = 0; // Reset to count for next sprite

	// ----------------------------------------------------------------------//

	// ----------------------------Knife-------------------------------------//
	LPDIRECT3DTEXTURE9 texKnife = textures->Get(ID_TEX_KNIFE);

	result = ReadFile(4);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };

		if (data.size() > 0)
			sprites->Add(stoi(data[0]), stoi(data[1]), stoi(data[2]), stoi(data[3]), stoi(data[4]), texKnife);
	}

	ani = new CAnimation(100);

	for (unsigned i = 0; i < result.size(); i++)
	{
		istringstream iss(result[i]);
		vector<string> data{ istream_iterator<string>{iss},
									istream_iterator<string>{} };

		if (data.size() > 0)
			ani->Add(stoi(data[0]));
		else
		{
			animations->Add(Count, ani);
			++Count;
			ani = new CAnimation(100);
		}
	}

	Knife = new CKnife();

	for (int i = 0; i <= Count; i++)
	{
		Knife->AddAnimation(i);
	}

	//Knife->SetPosition(Simon->x, Simon->y + 20);

	objects.push_back(Knife);

	Count = 0;

	// ----------------------------------------------------------------------//

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
		{
			if (objects[i] != Knife)
				objects[i]->Render();
			else if (objects[i] == Knife && isUsingKnife == true)
			{
				objects[i]->Render();

				if (Knife->x < 0 || Knife->x > 1465)
					isUsingKnife = false;
			}
		}

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

	/*wchar_t buffer[256];
	wsprintfW(buffer, L"%d", temp);

	MessageBox(hWnd, buffer, L"Show", MB_OK);*/

	Run();

	return 0;
}