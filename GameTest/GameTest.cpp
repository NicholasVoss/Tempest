//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include <ctime>
//------------------------------------------------------------------------
#include "app\app.h"
#include "Bullet.h"
#include "Enemy.h"
//------------------------------------------------------------------------

const short maxBullets = 5;
const short maxEnemies = 5;
CSimpleSprite* shipSprite;
Bullet* bullets[maxBullets];
Enemy* enemies[maxEnemies];
short score = 0;
short lives = 5;
short loopNum = 0;
bool gameOver = false;
bool gameStarted = false;

//variables for for stage creation/player positioning
const int maxLines = 16;
float sxValues[maxLines + 1];
float syValues[maxLines + 1];
float exValues[maxLines + 1];
float eyValues[maxLines + 1];
short shipPosition = 0;
short numLines = 4;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	srand(time(0));

	shipSprite = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	shipSprite->SetPosition(510.0f, 685.0f);	
	shipSprite->SetFrame(2);
	shipSprite->SetScale(1.0f);
	shipSprite->SetAngle(PI);

	for (int i = 0; i < maxBullets; i++)
	{
		bullets[i] = nullptr;
	}

	for (int i = 0; i < maxEnemies; i++)
	{
		enemies[i] = nullptr;
	}
	//------------------------------------------------------------------------
}

void deleteBullets()
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullets[i] != nullptr)
		{
			delete bullets[i];
			bullets[i] = nullptr;
		}
	}
}

void deleteEnemies()
{
	for (int i = 0; i < maxEnemies; i++)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}
}

bool bulletEnemyCollision(Bullet* bullet, Enemy* enemy)
{
	if (cosf(bullet->getAngle()) > 0 && sinf(bullet->getAngle()) > 0)
	{
		if (bullet->getX() < enemy->getX()
			&& enemy->getX() < bullet->getX() + bullet->getLength() * cosf(bullet->getAngle())
			&& bullet->getY() < enemy->getY()
			&& enemy->getY() < bullet->getY() + bullet->getLength() * sinf(bullet->getAngle()))
		{
			return true;
		}
	}

	if (cosf(bullet->getAngle()) > 0 && sinf(bullet->getAngle()) < 0)
	{
		if (bullet->getX() < enemy->getX()
			&& enemy->getX() < bullet->getX() + bullet->getLength() * cosf(bullet->getAngle())
			&& bullet->getY() > enemy->getY()
			&& enemy->getY() > bullet->getY() + bullet->getLength() * sinf(bullet->getAngle()))
		{
			return true;
		}
	}

	if (cosf(bullet->getAngle()) < 0 && sinf(bullet->getAngle()) < 0)
	{
		if (bullet->getX() > enemy->getX()
			&& enemy->getX() > bullet->getX() + bullet->getLength() * cosf(bullet->getAngle())
			&& bullet->getY() > enemy->getY()
			&& enemy->getY() > bullet->getY() + bullet->getLength() * sinf(bullet->getAngle()))
		{
			return true;
		}
	}

	if (cosf(bullet->getAngle()) < 0 && sinf(bullet->getAngle()) > 0)
	{
		if (bullet->getX() > enemy->getX()
			&& enemy->getX() > bullet->getX() + bullet->getLength() * cosf(bullet->getAngle())
			&& bullet->getY() < enemy->getY()
			&& enemy->getY() < bullet->getY() + bullet->getLength() * sinf(bullet->getAngle()))
		{
			return true;
		}
	}


	return false;
}

void startGame()
{
	score = 0;
	lives = 5;
	numLines = 4;
	shipPosition = 0;
	gameOver = false;
	gameStarted = true;

	//initialize stage
	for (int i = 0; i < numLines; i++)
	{
		sxValues[i] = (APP_INIT_WINDOW_WIDTH / 2) + (50 * sinf(i * 2 * PI / numLines));;
		syValues[i] = (APP_INIT_WINDOW_HEIGHT / 2) + (50 * cosf(i * 2 * PI / numLines));
		exValues[i] = (APP_INIT_WINDOW_WIDTH / 2) + (300 * sinf(i * 2 * PI / numLines));
		eyValues[i] = (APP_INIT_WINDOW_HEIGHT / 2) + (300 * cosf(i * 2 * PI / numLines));
	}

	float x, y;
	shipSprite->GetPosition(x, y);
	x = (exValues[shipPosition] + exValues[shipPosition + 1]) / 2;
	y = (eyValues[shipPosition] + eyValues[shipPosition + 1]) / 2;
	shipSprite->SetPosition(x, y);
	shipSprite->SetAngle(PI + (shipPosition + .5) * -2 * PI / numLines);

	deleteEnemies();
	deleteBullets();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....

	shipSprite->Update(deltaTime);

	for (int i = 0; i < maxEnemies; i++)
	{
		//check if enemies reach the outside edge
		if (enemies[i] != nullptr && enemies[i]->getSize() >= 20)
		{
			delete enemies[i];
			enemies[i] = nullptr;
			lives--;
			if (lives <= 0)
			{
				gameOver = true;
			}
		}
		for (int j = 0; j < maxBullets; j++)
		{
			//check if player shoots enemies
			if (bullets[j] != nullptr && enemies[i] != nullptr && bulletEnemyCollision(bullets[j], enemies[i]) == true)
			{
				score++;
				if (score / 25 >= numLines / 4 && numLines < maxLines - 1)
				{
					numLines += 4;
					for (int l = 0; l < numLines; l++)
					{
						sxValues[l] = (APP_INIT_WINDOW_WIDTH / 2) + (50 * sinf(l * 2 * PI / numLines));;
						syValues[l] = (APP_INIT_WINDOW_HEIGHT / 2) + (50 * cosf(l * 2 * PI / numLines));
						exValues[l] = (APP_INIT_WINDOW_WIDTH / 2) + (300 * sinf(l * 2 * PI / numLines));
						eyValues[l] = (APP_INIT_WINDOW_HEIGHT / 2) + (300 * cosf(l * 2 * PI / numLines));
					}
					shipPosition = 0;
					float x, y;
					shipSprite->GetPosition(x, y);
					x = (exValues[shipPosition] + exValues[shipPosition + 1]) / 2;
					y = (eyValues[shipPosition] + eyValues[shipPosition + 1]) / 2;
					shipSprite->SetPosition(x, y);
					shipSprite->SetAngle(PI + (shipPosition + .5) * -2 * PI / numLines);

					deleteEnemies();
					deleteBullets();
				}
				delete enemies[i];
				enemies[i] = nullptr;
				delete bullets[j];
				bullets[j] = nullptr;
			}
		}
	}

	//spawn enemies
	for(int i = 0; i < maxEnemies; i++)
	{
		if (enemies[i] == nullptr && (std::rand() % 100 == 0))
		{
			enemies[i] = new Enemy(numLines);
			break;
		}
	}

	if (loopNum == 3) {
		loopNum = 0;

		//delete bullets when they get close to the center
		for (int i = 0; i < maxBullets; i++)
		{
			if (bullets[i] != nullptr && bullets[i]->getLength() <= 10)
			{
				delete bullets[i];
				bullets[i] = nullptr;
			}
		}

		if (App::GetController().GetLeftThumbStickX() > 0.5f)
		{
			float x, y;
			shipSprite->GetPosition(x, y);
			if (shipPosition < numLines - 1)
			{
				shipPosition++;
			}
			else
			{
				shipPosition = 0;
			}
			x = (exValues[shipPosition] + exValues[shipPosition + 1]) / 2;
			y = (eyValues[shipPosition] + eyValues[shipPosition + 1]) / 2;
			shipSprite->SetPosition(x, y);
			shipSprite->SetAngle(PI + (shipPosition + .5) * -2 * PI / numLines);
		}
		if (App::GetController().GetLeftThumbStickX() < -0.5f)
		{
			float x, y;
			shipSprite->GetPosition(x, y);
			if (shipPosition > 0)
			{
				shipPosition--;
			}
			else
			{
				shipPosition = numLines - 1;
			}
			x = (exValues[shipPosition] + exValues[shipPosition + 1]) / 2;
			y = (eyValues[shipPosition] + eyValues[shipPosition + 1]) / 2;
			shipSprite->SetPosition(x, y);
			shipSprite->SetAngle(PI + (shipPosition + .5) * -2 * PI / numLines);
		}
		if (App::GetController().GetLeftThumbStickY() > 0.5f)
		{
			float x, y;
			shipSprite->GetPosition(x, y);
			if (shipPosition < numLines - 1)
			{
				shipPosition++;
			}
			else
			{
				shipPosition = 0;
			}
			x = (exValues[shipPosition] + exValues[shipPosition + 1]) / 2;
			y = (eyValues[shipPosition] + eyValues[shipPosition + 1]) / 2;;
			shipSprite->SetPosition(x, y);
			shipSprite->SetAngle(PI + (shipPosition + .5) * -2 * PI / numLines);
		}
		if (App::GetController().GetLeftThumbStickY() < -0.5f)
		{
			float x, y;
			shipSprite->GetPosition(x, y);
			if (shipPosition > 0)
			{
				shipPosition--;
			}
			else
			{
				shipPosition = numLines - 1;
			}
			x = (exValues[shipPosition] + exValues[shipPosition + 1]) / 2;
			y = (eyValues[shipPosition] + eyValues[shipPosition + 1]) / 2;
			shipSprite->SetPosition(x, y);
			shipSprite->SetAngle(PI + (shipPosition + .5) * -2 * PI / numLines);
		}
	}
	else 
	{ 
		loopNum++;
	}
	
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		float x, y;
		shipSprite->GetPosition(x, y);

		//shoot
		if (!gameOver && gameStarted)
		{
			for (int i = 0; i < maxBullets; i++)
			{
				if (bullets[i] == nullptr)
				{
					bullets[i] = new Bullet(x, y, shipSprite->GetAngle());
					App::PlaySound(".\\TestData\\Test.wav");
					break;
				}
			}
		}

		if (gameOver)
		{
			startGame();
		}

		if (!gameStarted)
		{
 			startGame();
		}	
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	

	if (gameStarted && !gameOver)
	{

		//draw lines
		for (int i = 0; i < numLines; i++)
		{
			sxValues[i] = (APP_INIT_WINDOW_WIDTH / 2) + (50 * sinf(i * 2 * PI / numLines));;
			syValues[i] = (APP_INIT_WINDOW_HEIGHT / 2) + (50 * cosf(i * 2 * PI / numLines));
			exValues[i] = (APP_INIT_WINDOW_WIDTH / 2) + (300 * sinf(i * 2 * PI / numLines));
			eyValues[i] = (APP_INIT_WINDOW_HEIGHT / 2) + (300 * cosf(i * 2 * PI / numLines));
			if (shipPosition == i || shipPosition == i - 1 || i == 0 && shipPosition == numLines - 1)
			{
				App::DrawLine(sxValues[i], syValues[i], exValues[i], eyValues[i], 1.0f, 1.0f, 0);
			}
			else
			{
				App::DrawLine(sxValues[i], syValues[i], exValues[i], eyValues[i], 0, 0, 1.0f);
			}
		}
		exValues[numLines] = exValues[0];
		eyValues[numLines] = eyValues[0];
		sxValues[numLines] = sxValues[0];
		syValues[numLines] = syValues[0];

		//draw shape outline
		for (int i = 0; i < numLines; i++)
		{
			if (shipPosition != i)
			{
				App::DrawLine(sxValues[i], syValues[i], sxValues[i + 1], syValues[i + 1], 0, 0, 1.0f);
				App::DrawLine(exValues[i], eyValues[i], exValues[i + 1], eyValues[i + 1], 0, 0, 1.0f);
			}
			else
			{
				App::DrawLine(sxValues[i], syValues[i], sxValues[i + 1], syValues[i + 1], 1.0f, 1.0f, 0);
				App::DrawLine(exValues[i], eyValues[i], exValues[i + 1], eyValues[i + 1], 1.0f, 1.0f, 0);
			}
		}
		
		shipSprite->Draw();
		for (int i = 0; i < maxBullets; i++)
		{
			if (bullets[i] != nullptr)
			{
				bullets[i]->move();
			}
		}

		for (int i = 0; i < maxEnemies; i++)
		{
			if (enemies[i] != nullptr)
			{
				enemies[i]->move();
			}
		}

		//Print text

		char scoreString[16];
		char livesString[16];

		sprintf(scoreString, "Score: %d", score);
		sprintf(livesString, "Lives: %d", lives);

		App::Print(100, 725, scoreString);
		App::Print(100, 700, livesString);
	}
	else if (gameOver)
	{
		App::Print(APP_INIT_WINDOW_WIDTH / 2 - 75, APP_INIT_WINDOW_HEIGHT / 2, "Game Over");

		char scoreString[16];
		sprintf(scoreString, "Score: %d", score);
		App::Print(APP_INIT_WINDOW_WIDTH / 2 - 75, APP_INIT_WINDOW_HEIGHT / 2 - 50, scoreString);

		App::Print(APP_INIT_WINDOW_WIDTH / 2 - 75, APP_INIT_WINDOW_HEIGHT / 2 - 100, "Press Space to Restart");
	}
	else if (!gameStarted)
	{
		App::Print(APP_INIT_WINDOW_WIDTH / 2 - 75, APP_INIT_WINDOW_HEIGHT / 2, "Tempest");
		App::Print(APP_INIT_WINDOW_WIDTH / 2 - 200, APP_INIT_WINDOW_HEIGHT / 2 - 200, "Use Space to shoot");
		App::Print(APP_INIT_WINDOW_WIDTH / 2 + 50, APP_INIT_WINDOW_HEIGHT / 2 - 200, "Use WASD to move");
		App::Print(APP_INIT_WINDOW_WIDTH / 2 - 125, APP_INIT_WINDOW_HEIGHT / 2 - 50, "Press Space to Start Game");
	}

}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	deleteEnemies();
	deleteBullets();
	delete shipSprite;
	//------------------------------------------------------------------------
}