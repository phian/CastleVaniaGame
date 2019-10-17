#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

//#include "Goomba.h"

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	if (y > 150)
	{
		vy = 0; 
		y = 150.0f;
	}

	x += dx;
	y += dy;
	
	// simple screen edge collision!!!
	if (vx > 0 && x > 500) x = 500;
	if (vx < 0 && x < 0) x = 0;

	/*vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();*/

	// turn off collision when die 
	//if (state != MARIO_STATE_DIE)
		//CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	//if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	//{
	//	untouchable_start = 0;
	//	untouchable = 0;
	//}

	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;

	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	//	// block 
	//	x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
	//	y += min_ty * dy + ny * 0.4f;

	//	if (nx != 0) vx = 0;
	//	if (ny != 0) vy = 0;

	//	// Collision logic with Goombas
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

			//if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny < 0)
			//	{
			//		if (goomba->GetState() != GOOMBA_STATE_DIE)
			//		{
			//			goomba->SetState(GOOMBA_STATE_DIE);
			//			vy = -MARIO_JUMP_DEFLECT_SPEED;
			//		}
			//	}
			//	else if (e->nx != 0)
			//	{
			//		if (untouchable == 0)
			//		{
			//			if (goomba->GetState() != GOOMBA_STATE_DIE)
			//			{
			//				if (level > MARIO_LEVEL_SMALL)
			//				{
			//					level = MARIO_LEVEL_SMALL;
			//					StartUntouchable();
			//				}
			//				else
			//					SetState(MARIO_STATE_DIE);
			//			}
			//		}
			//	}
			//}
		/*}
	}*/

	// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	int ani;
	if (vx == 0 && GetState() == SIMON_STATE_IDLE)
	{
		if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
		else ani = SIMON_ANI_IDLE_LEFT;
	}
	else if (vx == 0 && GetState() == SIMON_STATE_SITDOWN)
	{
		if (nx > 0) ani = SIMON_ANI_SITDOWN_RIGHT;
		else ani = SIMON_ANI_SITDOWN_LEFT;
	}
	else if (vx == 0 && GetState() == SIMON_STATE_USE_WHIP_SIT)
	{
		if (nx > 0) ani = SIMON_ANI_USE_WHIP_SIT_RIGHT;
		else ani = SIMON_ANI_USE_WHIP_SIT_LEFT;
	}
	else if (vx == 0 && GetState() == SIMON_STATE_USE_WHIP_STAND)
	{
		if (nx > 0) ani = SIMON_ANI_USE_WHIP_STAND_RIGHT;
		else ani = SIMON_ANI_USE_WHIP_STAND_LEFT;
	}
	else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
	else ani = SIMON_ANI_WALKING_LEFT;

	animations[ani]->Render(x, y);

	/*int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x, y, alpha);*/

	//RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
		case SIMON_STATE_WALKING_RIGHT:
			vx = SIMON_WALKING_SPEED;
			nx = 1;
			break;
		case SIMON_STATE_WALKING_LEFT:
			vx = -SIMON_WALKING_SPEED;
			nx = -1;
			break;
		case SIMON_STATE_IDLE:
			vx = 0;
			break;
		case SIMON_STATE_SITDOWN:
			vx = 0;
			break;
		case SIMON_STATE_JUMP:
			if(y > 10)
				vy = -SIMON_JUMP_SPEED_Y;
			break;
		case SIMON_STATE_USE_WHIP_SIT:
			vx = 0;
			break;
		case SIMON_STATE_USE_WHIP_STAND:
			vx = 0;
			break;
	}
}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMOB_BBOX_HEIGHT;
}