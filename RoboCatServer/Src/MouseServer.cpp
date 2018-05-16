#include <RoboCatServerPCH.h>


MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


bool MouseServer::HandleCollisionWithCat( RoboCat* inCat )
{
	ScoreBoardManager::Entry* currentEntry = ScoreBoardManager::sInstance->GetEntry(inCat->GetPlayerId());

	if (currentEntry != nullptr)
	{
		if (!currentEntry->GetHasMoney())
		{
			//kill yourself!
			SetDoesWantToDie(true);

			//LOG("Player %d has Money %d", inCat->GetPlayerId(), currentEntry->GetHasMoney());
			ScoreBoardManager::sInstance->ChangeHasMoney(inCat->GetPlayerId(), true);
			//LOG("Player %d has Money %d", inCat->GetPlayerId(), currentEntry->GetHasMoney());

			ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 500);
		}
	}
	return false;
}


