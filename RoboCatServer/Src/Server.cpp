#include <RoboCatServerPCH.h>
#include <fstream>
#include <iostream>
#include <sstream>

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server() :
	mTimeElapsed(0),
	mSpawnLocations(),
	highScores()
{
	SetupSpawnLocation();
	
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'GRAS', MapServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', RoboCatServer::StaticCreate);

	InitNetworkManager();
	LoadHighScores();
	//NetworkManagerServer::sInstance->SetDropPacketChance( 0.8f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.1f );

}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg( 1 );
	uint16_t port = stoi( portString );

	return NetworkManagerServer::StaticInit( port );
}


namespace
{
	
	void CreateRandomMice( int inMouseCount )
	{
		Vector3 mouseMin( -5.f, -3.f, 0.f );
		Vector3 mouseMax( 5.f, 3.f, 0.f );
		GameObjectPtr go;

		//make a mouse somewhere- where will these come from?
		for( int i = 0; i < inMouseCount; ++i )
		{
			go = GameObjectRegistry::sInstance->CreateGameObject( 'MOUS' );
			Vector3 mouseLocation = RoboMath::GetRandomVector( mouseMin, mouseMax );
			go->SetLocation( mouseLocation );
		}
	}
}


void Server::DrawTileMap()
{
	GameObjectPtr tile;
	tile = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	Vector3 tilelocation = Vector3(0.f, 0.f, 0.f);
	tile->SetLocation(tilelocation);
	
}

void Server::SetupSpawnLocation()
{
	mSpawnLocations.push_back(Vector3(0, 0, 0)); //1
	mSpawnLocations.push_back(Vector3(3, 0, 0)); //2
	mSpawnLocations.push_back(Vector3(6, 0, 0)); //3
	mSpawnLocations.push_back(Vector3(9, 0, 0)); //4
	mSpawnLocations.push_back(Vector3(12, 0, 0)); //5
	mSpawnLocations.push_back(Vector3(0, 3, 0)); //6
	mSpawnLocations.push_back(Vector3(0, 6, 0)); //7
	mSpawnLocations.push_back(Vector3(0, 9, 0)); //8
	mSpawnLocations.push_back(Vector3(0, 12, 0)); //9
	mSpawnLocations.push_back(Vector3(3, 12, 0)); //10
	mSpawnLocations.push_back(Vector3(6, 12, 0)); //11
	mSpawnLocations.push_back(Vector3(9, 12, 0)); //12
	mSpawnLocations.push_back(Vector3(12, 12, 0)); //13
	mSpawnLocations.push_back(Vector3(12, 3, 0)); //14
	mSpawnLocations.push_back(Vector3(12, 6, 0)); //15
	mSpawnLocations.push_back(Vector3(12, 9, 0)); //16
}

void Server::LoadHighScores()
{
	std::string buffer;
	std::ifstream scorefile;
	scorefile.open("../Assets/score.txt");

	scorefile.close();
}

void Server::SaveHighScores()
{
}

int Server::GetHighScore(int playerId)
{
	return 0;
}

Vector3 Server::GetSpawnLocation(int inPlayerId)
{
	if(inPlayerId<17)
		return mSpawnLocations.at(inPlayerId - 1);
	else
		return mSpawnLocations.at(0);
}

void Server::SetupWorld()
{
	//spawn some random mice
	CreateRandomMice( 10 );
	
	//spawn more random mice!
	CreateRandomMice( 10 );

	//Draw tile map
	//Change to power ups??
	//DrawTileMap();
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

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	LobbyManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCatForPlayer( playerId );
}

void Server::SpawnCatForPlayer( int inPlayerId )
{
	RoboCatPtr cat = std::static_pointer_cast< RoboCat >( GameObjectRegistry::sInstance->CreateGameObject( 'RCAT' ) );
	cat->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	cat->SetPlayerId( inPlayerId );
	cat->SetLocation(GetSpawnLocation(inPlayerId));
}

void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	LobbyManager::sInstance->RemoveEntry(playerId);
	RoboCatPtr cat = GetCatForPlayer( playerId );
	if( cat )
	{
		cat->SetDoesWantToDie( true );
	}
}

RoboCatPtr Server::GetCatForPlayer( int inPlayerId )
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for( int i = 0, c = gameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = gameObjects[ i ];
		RoboCat* cat = go->GetAsCat();
		if( cat && cat->GetPlayerId() == inPlayerId )
		{
			return std::static_pointer_cast< RoboCat >( go );
		}
	}

	return nullptr;

}