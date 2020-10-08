#include "stdafx.h"
#include "Bullet.h"
#include "app\app.h"

Bullet::Bullet(float x, float y, float angle) : xPos(x), yPos(y)
{
	xPos = x;
	yPos = y;
	moveAngle = angle + PI / 2;
	length = 100;
}

void Bullet::move()
{
	xPos += 10 * cosf(moveAngle);
	yPos += 10 * sinf(moveAngle);
	App::DrawLine(xPos, yPos, xPos + length * cosf(moveAngle), yPos + (length * sinf(moveAngle)), 1.0f, 0, 0);
	length =  .4 * (sqrt(pow(xPos - APP_INIT_WINDOW_WIDTH / 2, 2) + pow(yPos - APP_INIT_WINDOW_HEIGHT / 2, 2)));
	
}

float Bullet::getLength()
{
	return length;
}

float Bullet::getAngle()
{
	return moveAngle;
}

float Bullet::getX()
{
	return xPos;
}

float Bullet::getY()
{
	return yPos;
}