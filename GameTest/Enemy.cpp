#include "stdafx.h"
#include "Enemy.h"
#include "app\app.h"

Enemy::Enemy(int lines)
{
	xPos = APP_INIT_WINDOW_WIDTH / 2;
	yPos = APP_INIT_WINDOW_HEIGHT / 2;
	position = (std::rand() % (lines)) + 0.5;
	angle = (position * 2 * PI / lines);
	size = 1;
}

void Enemy::move()
{
	//move enemy
	xPos += 1.5 * cosf(angle);
	yPos += 1.5 * sinf(angle);

	//draw enemy
	App::DrawLine(xPos - size, yPos - size, xPos + size, yPos - size, 1.0f, 0, 0);
	App::DrawLine(xPos - size, yPos + size, xPos + size, yPos + size, 1.0f, 0, 0);
	App::DrawLine(xPos - size, yPos + size, xPos - size, yPos - size, 1.0f, 0, 0);
	App::DrawLine(xPos + size, yPos + size, xPos + size, yPos - size, 1.0f, 0, 0);

	size += .1;
}

float Enemy::getSize()
{
	return size;
}

float Enemy::getX()
{
	return xPos;
}

float Enemy::getY()
{
	return yPos;
}