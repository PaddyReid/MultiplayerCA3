
#include <RoboCatClientPCH.h>

bool Client::StaticInit( )
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();

	if( WindowManager::StaticInit() == false )
	{
		return false;
	}
	
	if( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) == false )
	{
		return false;
	}

	TextureManager::StaticInit();
	RenderManager::StaticInit();
	InputManager::StaticInit();

	HUD::StaticInit();

	sInstance.reset( client );

	return true;
}

Client::Client() :
	isStarted(false)
{
	
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'GRAS', MapClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatClient::StaticCreate);

	string destination = StringUtils::GetCommandLineArg( 1 );
	string name = StringUtils::GetCommandLineArg( 2 );

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString( destination );

	NetworkManagerClient::StaticInit( *serverAddress, name );

	//NetworkManagerClient::sInstance->SetDropPacketChance( 0.6f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.1f );
}

int lev[20][25]
{
	{ 0,1,1,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};

void Client::DrawTileMap()
{
	GameObjectPtr tile;
	float x = 0, y = 0;
	int type = 0;
	float width = 0.63f, height = 0.63f;

	for (int row = 0; row < 20; row++) {
		for (int column = 0; column < 25; column++) {
			type = lev[row][column];

			tile = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
			std::shared_ptr<MapClient> tilePiece = std::dynamic_pointer_cast<MapClient>(tile);
			Vector3 tilelocation = Vector3(x, y, 0.f);
			tilePiece->SetLocation(tilelocation);
			
			//NEED A SetTexture or SetSprite function in Map class
			
			switch (type)
			{
			case 0: //grass
				break;
			case 1: //wall - needs collsion
				tilePiece->SetSpriteTexture("wall");
				//tilePiece->SetCollisionRadius(0.5f);
				break;
			case 2: //window
				tilePiece->SetSpriteTexture("window");
				break;
			}
			width = (tilePiece->GetTextureWidth() * 0.01f) - 0.01f;
			height = (tilePiece->GetTextureHeight() * 0.01f) -0.01f;
			x += width;
		}
		x = 0;
		y += height;
	}
}

void Client::DoFrame()
{
	if (!isStarted) {
		isStarted = true;
		DrawTileMap();
	}

	InputManager::sInstance->Update();

	Engine::DoFrame();

	NetworkManagerClient::sInstance->ProcessIncomingPackets();
	
	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::HandleEvent( SDL_Event* inEvent )
{
	switch( inEvent->type )
	{
	case SDL_KEYDOWN:
			InputManager::sInstance->HandleInput(EIA_Pressed, inEvent->key.keysym.sym);
		break;
	case SDL_KEYUP:
			InputManager::sInstance->HandleInput(EIA_Released, inEvent->key.keysym.sym);
		break;
	//case SDL_MOUSEBUTTONDOWN:
	//	InputManager::sInstance->HandleInput(EIA_Pressed, inEvent->key.keysym.sym);
	//	break;
	//case SDL_MOUSEBUTTONUP:
	//	InputManager::sInstance->HandleInput(EIA_Released, inEvent->key.keysym.sym);
	//	break;
	default:
		break;
	}
}

