#pragma once
class Enemy
{
public:
	Enemy(int lines);
	void move();
	float getSize();
	float getX();
	float getY();
private:
	float xPos;
	float yPos;
	float angle;
	float size;
	float position;


};

