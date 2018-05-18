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
	SetDoesWantToDie(true);
	//incremanet health


	return false;
}


