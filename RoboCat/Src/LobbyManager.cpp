#include "RoboCatPCH.h"

std::unique_ptr< LobbyManager >	LobbyManager::sInstance;


void LobbyManager::StaticInit()
{
	sInstance.reset(new LobbyManager());
}

LobbyManager::LobbyManager() :
	mEveryoneReady(false),
	mGamePlaying(false)
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
	if (mGamePlaying) 
	{
		for (LobbyPlayer& entry : mEntries)
		{
			//match running, don't mess with lobby setup
			// handle game Timer
			std::string message = std::to_string(GetMatchTimer());
			entry.SetLobbyMessage(message);
		}
	}
	else 
	{
		int readyPlayers = 0;
		bool alreadyEveryoneReady = IsEveryoneReady();
		for (LobbyPlayer& entry : mEntries)
		{
			if (entry.GetReadyState())
				readyPlayers++;
		}

		if (readyPlayers == mEntries.size() && readyPlayers != 0)
			SetEveryoneReady(true);
		else
			SetEveryoneReady(false);


		if (IsEveryoneReady() && !alreadyEveryoneReady) {
			//Changes to ready
			ResetTimeToGameStart();
		}
		else {
			//Changes to not ready
		}

		//when not everyone ready
		if (!IsEveryoneReady()) {
			for (LobbyPlayer& entry : mEntries)
			{
				std::string message = "Click Space to ready up";
				if (mEntries.size() < 2)
					message = "Waiting for 2nd Player";
				else if (entry.GetReadyState()) {
					message = "Waiting on " + std::to_string(mEntries.size() - readyPlayers) + " player(s) to ready up";
				}
				entry.SetLobbyMessage(message);
			}
		} //when everyone ready
		else {
			for (LobbyPlayer& entry : mEntries)
			{
				entry.SetLobbyMessage("Match Beginning in " + std::to_string(GetTimeToGameStart()));
			}
		}
	}
}

void LobbyManager::ResetTimeToGameStart()
{
	mTimeToGameStart = TIME_TO_GAME_START;
}

void LobbyManager::DecrementTimeToGameStart()
{
	mTimeToGameStart--;
	CheckPlayerCount();
}

void LobbyManager::ResetMatchTimer()
{
	mMatchTimer = MATCH_TIMER;
}

void LobbyManager::DecrementMatchTimer()
{
	mMatchTimer--;
	CheckPlayerCount();
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

	inOutputStream.Write(mTimeToGameStart);
	inOutputStream.Write(mMatchTimer);
	inOutputStream.Write(mGamePlaying);
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

	int timeToGameStart;
	inInputStream.Read(timeToGameStart);
	SetTimeToGameStart(timeToGameStart);

	int matchTimer;
	inInputStream.Read(matchTimer);
	SetMatchTimer(matchTimer);

	bool gamePlaying;
	inInputStream.Read(gamePlaying);
	SetGamePlaying(gamePlaying);

	return true;
}

void LobbyManager::SetEveryoneReady(bool inEveryoneReady)
{
	mEveryoneReady = inEveryoneReady;
}

void LobbyManager::SetGamePlaying(bool gamePlaying)
{
	mGamePlaying = gamePlaying;
}

int LobbyManager::GetTimeToGameStart() const
{
	return mTimeToGameStart;
}

void LobbyManager::SetTimeToGameStart(int newTime)
{
	mTimeToGameStart = newTime;
	CheckPlayerCount();
}

int LobbyManager::GetMatchTimer() const
{
	return mMatchTimer;
}

void LobbyManager::SetMatchTimer(int matchTimer)
{
	mMatchTimer = matchTimer;
	CheckPlayerCount();
}

void LobbyManager::StartGame()
{
	//read scores from file
	ResetMatchTimer();
	mGamePlaying = true;
	CheckPlayerCount();
}

void LobbyManager::ResetGame()
{
	mGamePlaying = false;
	ResetTimeToGameStart();
	SetEveryoneReady(false);
	for (LobbyPlayer entry : GetEntries()) {
		entry.SetReadyState(false);
	}
	//save scores to file
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
	int readyState;
	inInputStream.Read(readyState);
	if (didSucceed) {
		SetReadyState(readyState);
	}

	inInputStream.Read(mLobbyMessage);
	inInputStream.Read(mFormattedNameReadyState);
	return didSucceed;
}



