#include <RoboCatServerPCH.h>

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server() :
	mTimeElapsed(0)
{

	
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'GRAS', MapServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatServer::StaticCreate);

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
		if (LobbyManager::sInstance->IsEveryoneReady()) {
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

	//TO-DO set spawn locations

	cat->SetLocation( Vector3( 1.f - static_cast< float >( inPlayerId ), 0.f, 0.f ) );


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