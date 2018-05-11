#include "RoboCatPCH.h"

std::unique_ptr< LobbyManager >	LobbyManager::sInstance;


void LobbyManager::StaticInit()
{
	sInstance.reset(new LobbyManager());
}

LobbyManager::LobbyManager()
{
	mDefaultColors.push_back(Colors::LightYellow);
	mDefaultColors.push_back(Colors::LightBlue);
	mDefaultColors.push_back(Colors::LightPink);
	mDefaultColors.push_back(Colors::LightGreen);
}

LobbyManager::LobbyPlayer::LobbyPlayer(uint32_t inPlayerId, const string& inPlayerName, const Vector3& inColor, bool readyBool) :
	mPlayerId(inPlayerId),
	mPlayerName(inPlayerName),
	mColor(inColor)
{
	SetReadyState(readyBool);
}

void LobbyManager::LobbyPlayer::SetReadyState(bool inReadyState)
{
	mReadyState = inReadyState;

	char	buffer[256];
	snprintf(buffer, 256, "%s %s", mPlayerName.c_str(), mReadyState ? "ready" : "not ready");
	mFormattedNameReadyState = string(buffer);
	LobbyManager::sInstance->CheckPlayerCount();
}

void LobbyManager::LobbyPlayer::SetLobbyMessage(string lobbyMessage)
{
	mLobbyMessage = lobbyMessage;
}


LobbyManager::LobbyPlayer* LobbyManager::GetEntry(uint32_t inPlayerId)
{
	for (LobbyPlayer &entry : mEntries)
	{
		if (entry.GetPlayerId() == inPlayerId)
		{
			return &entry;
		}
	}

	return nullptr;
}

bool LobbyManager::RemoveEntry(uint32_t inPlayerId)
{
	for (auto eIt = mEntries.begin(), endIt = mEntries.end(); eIt != endIt; ++eIt)
	{
		if ((*eIt).GetPlayerId() == inPlayerId)
		{
			mEntries.erase(eIt);
			return true;
		}
	}

	CheckPlayerCount();
	return false;
}

void LobbyManager::AddEntry(uint32_t inPlayerId, const string& inPlayerName)
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
	RemoveEntry(inPlayerId);

	mEntries.emplace_back(inPlayerId, inPlayerName, mDefaultColors[inPlayerId % mDefaultColors.size()],false);

	CheckPlayerCount();
}

void LobbyManager::ChangeReadyState(uint32_t inPlayerId, bool inReadyState)
{
	LobbyPlayer* entry = GetEntry(inPlayerId);
	if (entry)
	{
		entry->SetReadyState(inReadyState);
	}
}

void LobbyManager::CheckPlayerCount()
{
	int readyPlayers = 0;
	
	if (!IsEveryoneReady()) {
		for (LobbyPlayer& entry : mEntries)
		{
			if (entry.GetReadyState())
				readyPlayers++;

			//if all players are ready mark that all are ready
			if (readyPlayers != mEntries.size())
				SetEveryoneReady(false);
			else 
				SetEveryoneReady(true);
		}
		if (IsEveryoneReady()) {
			//Changes to ready
			ResetTimeToGameStart();
		}
	}
	else {
		for (LobbyPlayer& entry : mEntries)
		{
			if (entry.GetReadyState())
				readyPlayers++;

			//if all players are ready mark that all are ready
			if (readyPlayers != mEntries.size())
				SetEveryoneReady(false);
			else
				SetEveryoneReady(true);
		}
		if (!IsEveryoneReady()) {
			//Changes to not ready
		}
	}

	//when not everyone ready
	if (!IsEveryoneReady()) {
		for (LobbyPlayer& entry : mEntries)
		{
			std::string message = "Click Space to ready up";
			if (entry.GetReadyState()) {
				message = "Waiting on " + readyPlayers;
				message += " number of players to ready up";
			}
			else if (mEntries.size() < 2)
				message = "Waiting for 2nd Player";
			entry.SetLobbyMessage(message);
		}
	} //when everyone ready
	else {
		for (LobbyPlayer& entry : mEntries)
		{
			entry.SetLobbyMessage("Match Begining in " + GetTimeToGameStart());
		}
	}
}

void LobbyManager::ResetTimeToGameStart()
{
	mTimeToGameStart = 15;
}



bool LobbyManager::Write(OutputMemoryBitStream& inOutputStream) const
{
	int entryCount = mEntries.size();

	//we don't know our player names, so it's hard to check for remaining space in the packet...
	//not really a concern now though
	inOutputStream.Write(entryCount);
	for (const LobbyPlayer& entry : mEntries)
	{
		entry.Write(inOutputStream);
	}

	return true;
}



bool LobbyManager::Read(InputMemoryBitStream& inInputStream)
{
	int entryCount;
	inInputStream.Read(entryCount);
	//just replace everything that's here, it don't matter...
	mEntries.resize(entryCount);
	for (LobbyPlayer& entry : mEntries)
	{
		entry.Read(inInputStream);
	}

	return true;
}

void LobbyManager::SetEveryoneReady(bool inEveryoneReady)
{
	mEveryoneReady = inEveryoneReady;
}

int LobbyManager::GetTimeToGameStart() const
{
	return mTimeToGameStart;
}


bool LobbyManager::LobbyPlayer::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mColor);
	inOutputStream.Write(mPlayerId);
	inOutputStream.Write(mPlayerName);
	inOutputStream.Write(mReadyState);
	inOutputStream.Write(mLobbyMessage);
	inOutputStream.Write(mFormattedNameReadyState);
	return didSucceed;
}

bool LobbyManager::LobbyPlayer::Read(InputMemoryBitStream& inInputStream)
{
	bool didSucceed = true;

	inInputStream.Read(mColor);
	inInputStream.Read(mPlayerId);
	inInputStream.Read(mPlayerName);
	inInputStream.Read(mReadyState);
	inInputStream.Read(mLobbyMessage);
	inInputStream.Read(mFormattedNameReadyState);
	return didSucceed;
}



