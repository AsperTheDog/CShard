#include "Collider.hpp"

Collider::Collider() : Collider({COLLIDE_SPHERE, {1.f}}){}

Collider::Collider(ColliderData data) : hasCollided(false)
{

}

void Collider::testCollisions()
{
	
}
