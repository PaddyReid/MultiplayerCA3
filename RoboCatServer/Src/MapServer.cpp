#include <RoboCatServerPCH.h>


MapServer::MapServer()
{
}

void MapServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool MapServer::HandleCollisionWithCat(RoboCat* inCat)
{
	ScoreBoardManager::Entry* currentEntry = ScoreBoardManager::sInstance->GetEntry(inCat->GetPlayerId());

	if (currentEntry != nullptr)
	{
		if (currentEntry->GetHasMoney())
		{
			//kill yourself!
			ScoreBoardManager::sInstance->ChangeHasMoney(inCat->GetPlayerId(), false);
			ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 500);
		}
	}
	return false;
}


