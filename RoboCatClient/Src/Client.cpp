
#include <RoboCatClientPCH.h>
#include <fstream>
#include <iostream>

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
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', RoboCatClient::StaticCreate);

	string destination = StringUtils::GetCommandLineArg( 1 );
	string name = StringUtils::GetCommandLineArg( 2 );

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString( destination );

	NetworkManagerClient::StaticInit( *serverAddress, name );

	//NetworkManagerClient::sInstance->SetDropPacketChance( 0.6f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.1f );
}


void Client::DrawTileMap()
{
	char mNumberFromFile;
	int xPosition, yPosition;
	std::string numb; 
	std::ifstream myfile;
	myfile.open("../Assets/Map.txt");
	GameObjectPtr tile;
	//TO_DO RUN UNTIL ITS THE SIZE OF FILE
	for (float y = -30; y < 30; y++)
	{
		for (float x = -47; x < 47; x++)
		{
			//Get X and Y Position from File;
			myfile.get(mNumberFromFile);
			numb = mNumberFromFile;
			xPosition = std::stoi(numb);
			myfile.get(mNumberFromFile);
			numb = mNumberFromFile;
			yPosition = std::stoi(numb);
			myfile.ignore();

			//Create game object for texture
			tile = GameObjectRegistry::sInstance->CreateGameObject('GRAS');
			std::shared_ptr<MapClient> tilePiece = std::dynamic_pointer_cast<MapClient>(tile);

			//Setup Texture
			Vector3 tilelocation = Vector3(x * 0.639f, y * 0.639f, 0.f);
			tilePiece->SetLocation(tilelocation);
			tilePiece->SetSpriteTexture("world");

			//Get texturemap co-ordinates
			xPosition *= 64;
			yPosition *= 64;
			tilePiece->SetSource(Vector3(xPosition, yPosition, 0));

		}
	}
	myfile.close();
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
	if (inEvent->key.repeat == 0) {
		switch (inEvent->type)
		{
		case SDL_KEYDOWN:
			InputManager::sInstance->HandleInputOnce(EIA_PressedOnce, inEvent->key.keysym.sym);
			break;
		case SDL_KEYUP:
			InputManager::sInstance->HandleInputOnce(EIA_ReleasedOnce, inEvent->key.keysym.sym);
			break;
		default:
			break;
		}
	}
	else {
		switch (inEvent->type)
		{
		case SDL_KEYDOWN:
			InputManager::sInstance->HandleInput(EIA_Pressed, inEvent->key.keysym.sym);
			break;
		case SDL_KEYUP:
			InputManager::sInstance->HandleInput(EIA_Released, inEvent->key.keysym.sym);
			break;
		default:
			break;
		}
	}
	
}

