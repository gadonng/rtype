#include "Player.h"

Player::Player(AObject::objectType type, int idObject, int x, int y, int height, int width, int Vx, int Vy)
	: AObject(type, idObject, x, y, height, width, Vx, Vy)
{
}

Player::~Player()
{
}