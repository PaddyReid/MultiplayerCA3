#include <RoboCatServerPCH.h>
#include <fstream>
#include <iostream>
#include <sstream>

bool Server::StaticInit()
{
	sInstance.reset(new Server());

	return true;
}

Server::Server() :
	mTimeElapsed(0),
	mSpawnLocations(),
	mHighScores()
{
	SetupSpawnLocation();

	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('GRAS', MapServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('HEAL', HealthServer::StaticCreate);

	InitNetworkManager();
	//NetworkManagerServer::sInstance->SetDropPacketChance( 0.8f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.1f );

}


int Server::Run()
{
	SetupWorld();
	LoadHighScores();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}


namespace
{

	void CreateRandomMice(int inMouseCount)
	{
		Vector3 mouseMin( -23.f, -18.f, 0.f );
		Vector3 mouseMax( 17.f, 12.f, 0.f );
		GameObjectPtr go;

		//make a mouse somewhere- where will these come from?
		for (int i = 0; i < inMouseCount; ++i)
		{
			go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
			Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
			go->SetLocation(mouseLocation);
		}
	}
}

void Server::SpawnHealthPacks()
{
	GameObjectPtr go;
	go = GameObjectRegistry::sInstance->CreateGameObject('HEAL');
	go->SetLocation(Vector3(0, -2, 0));
}


void Server::DrawTileMap(Vector3 pos)
{
	GameObjectPtr go;
	go = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	go->SetLocation(Vector3(-25, 15, 0));

	go = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	go->SetLocation(Vector3(20, 16, 0));

	go = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	go->SetLocation(Vector3(0, 16, 0));

	go = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	go->SetLocation(Vector3(-25, 15, 0));

	go = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	go->SetLocation(Vector3(-25, -16, 0));
}

void Server::SetupSpawnLocation()
{
	mSpawnLocations.push_back(Vector3(0, -0.65, 0));	//1
	mSpawnLocations.push_back(Vector3(0, -16, 0));	    //2
	mSpawnLocations.push_back(Vector3(0, 16, 0));		//3
	mSpawnLocations.push_back(Vector3(27, -16, 0));		//4
	mSpawnLocations.push_back(Vector3(-28, -16, 0));	//5
	mSpawnLocations.push_back(Vector3(27, 16, 0));		//6
	mSpawnLocations.push_back(Vector3(-28, 16, 0));		//7
	mSpawnLocations.push_back(Vector3(27, -0.5, 0));	//8
	mSpawnLocations.push_back(Vector3(-28, 0, 0));		//9
	mSpawnLocations.push_back(Vector3(15, 12, 0));		//10
	mSpawnLocations.push_back(Vector3(-15, 12, 0));		//11
	mSpawnLocations.push_back(Vector3(27, 12, 0));		//12
	mSpawnLocations.push_back(Vector3(-28, 12, 0));		//13
	mSpawnLocations.push_back(Vector3(-27, -12, 0));	//14
	mSpawnLocations.push_back(Vector3(27, -12, 0));		//15
	mSpawnLocations.push_back(Vector3(-0, -8, 0));		//16
}
void Server::DropMoney(Vector3 pos)
{
	GameObjectPtr go;
	go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
	go->SetLocation(pos);
}

void Server::LoadHighScores()
{
	std::string line;
	std::ifstream scorefile;
	scorefile.open("../Assets/scores.txt");
	while (getline(scorefile, line, (char)'\n')) {
		HighScore score;
		std::stringstream ss(line);
		std::string val;

		getline(ss, val, (char)',');
		score.playerName = val;
		getline(ss, val, (char)',');
		score.score = stoi(val);
		mHighScores.push_back(score);
	}

	scorefile.close();
}

void Server::UpdateHighScores()
{
	for (ScoreBoardManager::Entry entry : ScoreBoardManager::sInstance->GetEntries()) {
		if (!UpdateHighScore(entry.GetPlayerId(), entry.GetPlayerName(), entry.GetScore())) {
			HighScore hs;
			hs.playerName = entry.GetPlayerName();
			hs.score = entry.GetScore();
		}
	}
}

ClientProxyPtr Server::GetIPAddr(int inPlayerId)
{
	return ClientProxyPtr();
}

bool Server::UpdateHighScore(int playerId, string playerName, int score)
{
	for (HighScore sc : mHighScores) {
		if (playerName == sc.playerName) {
			sc.score = score;
			return true;
		}
	}
	return false;
}

void Server::SaveHighScores()
{
	UpdateHighScores();
	std::ofstream out("../Assets/score.txt");

	for (HighScore sc : mHighScores) {
		out << sc.playerName << ',' << sc.score;
		out << '\n';
	}
}


int Server::GetHighScore(int playerId, string playerName)
{
	for (HighScore sc : mHighScores) {
		if (playerName == sc.playerName)
			return sc.score;
	}
	return -1;
}

int Server::GetHighScore(string playerName)
{
	for (HighScore sc : mHighScores) {
		if (playerName == sc.playerName)
			return sc.score;
	}
	return -1;
}

Vector3 Server::GetSpawnLocation(int inPlayerId)
{
	if (inPlayerId < 17)
	{
		return mSpawnLocations.at(inPlayerId - 1);
	}
	else
	{
		return mSpawnLocations.at(inPlayerId - 1) + Vector3(0 + (int)(inPlayerId / 16), 0 + (int)(inPlayerId / 16), 0);
	}
}


void Server::SetupWorld()
{
	//spawn some random mice
	CreateRandomMice(10);

	//spawn more random mice!
	//ReturnZone
	CreateRandomMice( 5 );

	SpawnHealthPacks();

	//spawnHelth()

	DrawTileMap(Vector3(0, 0, 0));
}

void Server::DoFrame()
{
	float deltaTime = Timing::sInstance.GetDeltaTime();
	mTimeElapsed += deltaTime;
	if (mTimeElapsed > 1) {
		if (LobbyManager::sInstance->IsGamePlaying()) {
			//handle game timer
			if (LobbyManager::sInstance->GetMatchTimer() == 0) {
				LobbyManager::sInstance->ResetGame();
				for (LobbyManager::LobbyPlayer lp : LobbyManager::sInstance->GetEntries()) {
					static_cast< RoboCatServer* >(GetCatRefForPlayer(lp.GetPlayerId()))->Kill();
					LobbyManager::sInstance->ChangeReadyState(lp.GetPlayerId(), false);
				}
			}
			else {
				LobbyManager::sInstance->DecrementMatchTimer();
			}

		}
		else if (LobbyManager::sInstance->IsEveryoneReady())
		{
			if (LobbyManager::sInstance->GetTimeToGameStart() == 0)
				LobbyManager::sInstance->StartGame();
			else
				LobbyManager::sInstance->DecrementTimeToGameStart();
		}
		mTimeElapsed = 0;
	}
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();
}

void Server::HandleNewClient(ClientProxyPtr inClientProxy)
{
	mClientProxies.push_back(inClientProxy);
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	int oldScore = GetHighScore(playerId, inClientProxy->GetName());
	if (oldScore != -1) //restore old score
		ScoreBoardManager::sInstance->IncScore(playerId, oldScore);
	LobbyManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCatForPlayer(playerId);
}

void Server::SpawnCatForPlayer(int inPlayerId)
{
	RoboCatPtr cat = std::static_pointer_cast< RoboCat >(GameObjectRegistry::sInstance->CreateGameObject('RCAT'));
	cat->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	LobbyManager::sInstance->ChangeReadyState(inPlayerId, false);
	cat->SetPlayerId(inPlayerId);

	//gotta pick a better spawn location than this...
	DropMoney(cat->GetLocation());

	//TO-DO set spawn locations

	cat->SetLocation(GetSpawnLocation(inPlayerId));
}

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();
	UpdateHighScores();
	int count = 0;

	//remove client from tracking list
	for (ClientProxyPtr cProxy : mClientProxies) {
		if (cProxy->GetPlayerId() == playerId) {
			mClientProxies.erase(mClientProxies.begin() + count);
		}
		count++;
	}

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	LobbyManager::sInstance->RemoveEntry(playerId);
	RoboCatPtr cat = GetCatForPlayer(playerId);
	if (cat)
	{
		cat->SetDoesWantToDie(true);
	}
}

RoboCatPtr Server::GetCatForPlayer(int inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		RoboCat* cat = go->GetAsCat();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast< RoboCat >(go);
		}
	}

	return nullptr;

}

RoboCat* Server::GetCatRefForPlayer(int inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		RoboCat* cat = go->GetAsCat();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return cat;
		}
	}

	return nullptr;

}