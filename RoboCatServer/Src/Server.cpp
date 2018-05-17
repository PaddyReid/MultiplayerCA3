#include <RoboCatServerPCH.h>

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server() :
	mTimeElapsed(0),
	mSpawnLocations()
{
	SetupSpawnLocation();
	
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'GRAS', MapServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', RoboCatServer::StaticCreate );

	InitNetworkManager();
	
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
		Vector3 mouseMin( -28.f, -18.f, 0.f );
		Vector3 mouseMax( 17.f, 16.f, 0.f );
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


void Server::DrawTileMap(Vector3 pos)
{
	/*GameObjectPtr go;
	go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
	go->SetLocation(pos);*/

	//Draw Drop off zones
	GameObjectPtr col;

	col = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	col->SetLocation(Vector3(-28, -16, 0));

	col = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	col->SetLocation(Vector3(27, 16, 0));

	col = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	col->SetLocation(Vector3(-28, 16, 0));

	col = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
	col->SetLocation(Vector3(27, -16, 0));
}

void Server::SetupSpawnLocation()
{
	mSpawnLocations.push_back(Vector3(0, -0.65, 0)); //1
	mSpawnLocations.push_back(Vector3(0, -16, 0)); //2
	mSpawnLocations.push_back(Vector3(0, 16, 0)); //3
	mSpawnLocations.push_back(Vector3(27, -16, 0)); //4
	mSpawnLocations.push_back(Vector3(-28, -16, 0)); //5
	mSpawnLocations.push_back(Vector3(27, 16, 0)); //6
	mSpawnLocations.push_back(Vector3(-28, 16, 0)); //7
	mSpawnLocations.push_back(Vector3(27, -0.5, 0)); //8
	mSpawnLocations.push_back(Vector3(-28, 0, 0)); //9
	mSpawnLocations.push_back(Vector3(15, 12, 0)); //10
	mSpawnLocations.push_back(Vector3(-15, 12, 0)); //11
	mSpawnLocations.push_back(Vector3(27, 12, 0)); //12
	mSpawnLocations.push_back(Vector3(-28, 12, 0)); //13
	mSpawnLocations.push_back(Vector3(-27, -12, 0)); //14
	mSpawnLocations.push_back(Vector3(27, -12, 0)); //15
	mSpawnLocations.push_back(Vector3(-0, -8, 0)); //16
}
void Server::DropMoney(Vector3 pos)
{
	GameObjectPtr go;
	go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
	go->SetLocation(pos);
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
	//ReturnZone
	CreateRandomMice( 5 );

	DrawTileMap(Vector3(0, 0, 0));

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

	//gotta pick a better spawn location than this...
	//DropMoney(cat->GetLocation());

	//TO-DO set spawn locations

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