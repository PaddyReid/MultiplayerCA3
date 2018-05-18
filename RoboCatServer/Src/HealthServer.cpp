#include <RoboCatServerPCH.h>


HealthServer::HealthServer()
{
}

void HealthServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool HealthServer::HandleCollisionWithCat(RoboCat* inCat)
{

	//destroy health item
	SetDoesWantToDie(true);
	//tell clients to ad health
	static_cast< RoboCatServer* >(inCat)->AddHealth();

	return false;
}


