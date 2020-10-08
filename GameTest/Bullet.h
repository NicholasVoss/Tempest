#pragma once
class Bullet
{
public:
	Bullet(float x, float y, float angle);
	void move();
	float getLength();
	float getAngle();
	float getX();
	float getY();
private:
	float xPos = 0;
	float yPos = 0;
	float moveAngle = 0;
	float length = 0;

};

