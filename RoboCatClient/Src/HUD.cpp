#include <RoboCatClientPCH.h>
#include <SDL_image.h>

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
	mScoreBoardOrigin(50.f, 60.f, 0.0f),
	mBandwidthOrigin(50.f, 10.f, 0.0f),
	mRoundTripTimeOrigin(800.f, 600.f, 0.0f),
	mScoreOffset(0.f, 50.f, 0.0f),
	mHealthOffset(1000, 10.f, 0.0f),
	mPositionOffset(600, 10.f, 0.0f),
	mHealth(0),
	mInLobby(true),
	mHasMoney(false)
{
	TTF_Init();
	mFont = TTF_OpenFont("../Assets/GROBOLD.TTF", 36);
	if (mFont == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font.");
	}
}


void HUD::StaticInit()
{
	sInstance.reset(new HUD());
}

void HUD::Render()
{
	//RenderPosition();
	//RenderRoundTripTime();
	if (LobbyManager::sInstance->IsGamePlaying())
	{
		RenderGameTimer();
		RenderHealth();
		RenderMoney();
		RenderScoreBoard();
	}
	else
	{
		RenderLobby();
	}


}

void HUD::RenderGameTimer() {
	const vector< LobbyManager::LobbyPlayer >& entries = LobbyManager::sInstance->GetEntries();

	Vector3 offset = Vector3(1300, 195, 0.0f);
	for (const auto& entry : entries)
	{
		if (entry.GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
		{
			float x = 1234;
			float y = 200;
			float h = 20;
			float w = 10;
			SDL_Color BGColor = { 255, 255, 255 };
			SDL_Color FGColor = { 0, 0, 0 };

			float Percent = LobbyManager::sInstance->GetMatchTimer();
			Percent = Percent > LobbyManager::sInstance->MATCH_TIMER ? LobbyManager::sInstance->MATCH_TIMER : Percent < 0.f ? 0.f : Percent;
			SDL_Color old;
			SDL_GetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), &old.r, &old.g, &old.g, &old.a);
			SDL_Rect bgrect = { x, y, w, h };
			SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);
			SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &bgrect);
			SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
			int ph = (int)((float)h * Percent);
			int py = y + (h - ph);
			SDL_Rect fgrect = { x, py, w, ph };
			SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &fgrect);
			SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), old.r, old.g, old.b, old.a);

			//Render image
			SDL_Surface* mBombImage = IMG_Load("../Assets/timer.png");
			SDL_Texture* bombImage = SDL_CreateTextureFromSurface(GraphicsDriver::sInstance->GetRenderer(), mBombImage);

			SDL_Rect src;
			SDL_Rect dest;
			src.x = src.y = 0;
			src.h = dest.h = 75;
			src.w = dest.w = 75;

			dest.x = 1200;
			dest.y = 200;
			SDL_RenderCopy(GraphicsDriver::sInstance->GetRenderer(), bombImage, &src, &dest);

			RenderText(entry.GetLobbyMessage(), offset, Colors::White);

		}
	}
}

void HUD::RenderLobby()
{
	const vector< LobbyManager::LobbyPlayer >& entries = LobbyManager::sInstance->GetEntries();
	Vector3 offset = mScoreBoardOrigin;

	for (const auto& entry : entries)
	{
		//entry.GetFormattedNameReadyState()
		RenderText(entry.GetFormattedNameReadyState(), offset, entry.GetColor());
		offset.mX += mScoreOffset.mX;
		offset.mY += mScoreOffset.mY;
		if (entry.GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
			RenderText(entry.GetLobbyMessage(), mBandwidthOrigin, Colors::White);

	}

}

void HUD::RenderHealth()
{
	if (mHealth > 0)
	{
		//string healthString = StringUtils::Sprintf("Health %d", mHealth);
		//RenderText(healthString, mHealthOffset, Colors::Red);

		//health bar    
		SDL_Color BGColor = { 0,0,0 };
		SDL_Color FGColor = { 255,0,0 };
		int w = 40;
		mHealth = mHealth > 10.f ? 10.f : mHealth < 0.f ? 0.f : mHealth;
		SDL_Color old;
		SDL_GetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), &old.r, &old.g, &old.g, &old.a);
		SDL_Rect bgrect = { 1000, 10.f, 0.0f };
		SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);
		SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &bgrect);
		SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
		int pw = (int)((float)w * mHealth);
		int px = 1000 + (w - pw);
		SDL_Rect fgrect = { px, 10, pw, 10 };
		SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &fgrect);
		SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), old.r, old.g, old.b, old.a);

	}
}

void HUD::RenderMoney()
{
	ScoreBoardManager::Entry* currentEntry = ScoreBoardManager::sInstance->GetEntry(NetworkManagerClient::sInstance->GetPlayerId());

	if (currentEntry != nullptr)
	{
		if (currentEntry->GetHasMoney())
		{
			string healthString = StringUtils::Sprintf("$$");
			RenderText(healthString, Vector3(1200, 10.f, 0.0f), Colors::Green);
		}
	}
}

void HUD::RenderBandWidth()
{
	string bandwidth = StringUtils::Sprintf("In %d  Bps, Out %d Bps",
		static_cast< int >(NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue()),
		static_cast< int >(NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue()));
	RenderText(bandwidth, mBandwidthOrigin, Colors::White);
}

void HUD::RenderPosition()
{
	string position = StringUtils::Sprintf("Pos: [%d,%d]",
		static_cast< int >(RenderManager::sInstance->GetViewTransform().x),
		static_cast< int >(RenderManager::sInstance->GetViewTransform().y));

	RenderText(position, mPositionOffset, Colors::White);
}

void HUD::RenderRoundTripTime()
{
	float rttMS = NetworkManagerClient::sInstance->GetAvgRoundTripTime().GetValue() * 1000.f;

	string roundTripTime = StringUtils::Sprintf("RTT %d ms", (int)rttMS);
	RenderText(roundTripTime, mRoundTripTimeOrigin, Colors::White);
}

void HUD::RenderScoreBoard()
{
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	Vector3 offset = mScoreBoardOrigin;

	for (const auto& entry : entries)
	{
		RenderText(entry.GetFormattedNameScore(), offset, entry.GetColor());
		offset.mX += mScoreOffset.mX;
		offset.mY += mScoreOffset.mY;
	}
}

void HUD::RenderText(const string& inStr, const Vector3& origin, const Vector3& inColor)
{
	// Convert the color
	SDL_Color color;
	color.r = static_cast<Uint8>(inColor.mX * 255);
	color.g = static_cast<Uint8>(inColor.mY * 255);
	color.b = static_cast<Uint8>(inColor.mZ * 255);
	color.a = 255;

	// Draw to surface and create a texture
	SDL_Surface* surface = TTF_RenderText_Blended(mFont, inStr.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(GraphicsDriver::sInstance->GetRenderer(), surface);

	// Setup the rect for the texture
	SDL_Rect dstRect;
	dstRect.x = static_cast<int>(origin.mX);
	dstRect.y = static_cast<int>(origin.mY);
	SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);

	// Draw the texture
	SDL_RenderCopy(GraphicsDriver::sInstance->GetRenderer(), texture, nullptr, &dstRect);

	// Destroy the surface/texture
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

//#include <RoboCatClientPCH.h>
//#include <SDL_image.h>
//
//bool SortByScore(ScoreBoardManager::Entry x, ScoreBoardManager::Entry y);
//std::unique_ptr< HUD >	HUD::sInstance;
//
//
//HUD::HUD() :
//mScoreBoardOrigin( 50.f, 60.f, 0.0f ),
//mBandwidthOrigin( 50.f, 10.f, 0.0f ),
//mRoundTripTimeOrigin( 800.f, 600.f, 0.0f ),
//mScoreOffset( 0.f, 50.f, 0.0f ),
//mHealthOffset( 1000, 10.f, 0.0f ),
//mPositionOffset(600, 10.f, 0.0f),
//mHealth( 0 ),
//mInLobby(true),
//mHasMoney(false)
//{
//	TTF_Init();
//	mFont = TTF_OpenFont( "../Assets/GROBOLD.TTF", 36 );
//	if( mFont == nullptr )
//	{
//		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to load font." );
//	}
//}
//
//
//void HUD::StaticInit()
//{
//	sInstance.reset( new HUD() );
//}
//
//void HUD::Render()
//{
//	//RenderPosition();
//	//RenderRoundTripTime();
//	if (LobbyManager::sInstance->IsGamePlaying()) 
//	{
//		RenderGameTimer();
//		RenderHealth();
//		RenderMoney();
//		RenderScoreBoard();
//	}
//	else 
//	{
//		RenderLobby();
//	}
//
//
//}
//
//void HUD::RenderGameTimer() {
//	const vector< LobbyManager::LobbyPlayer >& entries = LobbyManager::sInstance->GetEntries();
//
//	Vector3 offset = Vector3(1215, 225, 0.0f);
//	for (const auto& entry : entries)
//	{
//		if (entry.GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
//		{
//			float x = 1234;
//			float y = 200;
//			float h = 20;
//			float w = 10;
//			SDL_Color BGColor = { 255, 255, 255 };
//			SDL_Color FGColor = { 0, 0, 0 };
//
//			float Percent = LobbyManager::sInstance->GetMatchTimer() / LobbyManager::sInstance->MATCH_TIMER;
//			float countdown = LobbyManager::sInstance->GetMatchTimer() / LobbyManager::sInstance->MATCH_TIMER;
//			Percent = Percent > 1.0f ? 1.0f : Percent < 0.f ? 0.f : Percent;
//			SDL_Color old;
//			SDL_GetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), &old.r, &old.g, &old.g, &old.a);
//			SDL_Rect bgrect = { x, y, w, h };
//			SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);
//			SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &bgrect);
//			SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
//			float ph = h * countdown;
//			int py = y + (h - ph);
//			SDL_Rect fgrect = { x, py, w, ph };
//			SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &fgrect);
//			SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), old.r, old.g, old.b, old.a);
//
//			//Render image
//			SDL_Surface* mBombImage = IMG_Load("../Assets/timer.png");
//			SDL_Texture* bombImage = SDL_CreateTextureFromSurface(GraphicsDriver::sInstance->GetRenderer(), mBombImage);
//
//			SDL_Rect src;
//			SDL_Rect dest;
//			src.x = src.y = 0;
//			src.h = dest.h = 75;
//			src.w = dest.w = 75;
//
//			dest.x = 1200;
//			dest.y = 200;
//			SDL_RenderCopy(GraphicsDriver::sInstance->GetRenderer(), bombImage, &src, &dest);
//
//			RenderText(entry.GetLobbyMessage(), offset, Colors::White);
//
//		}	
//	}
//}
//
//void HUD::RenderLobby()
//{
//	const vector< LobbyManager::LobbyPlayer >& entries = LobbyManager::sInstance->GetEntries();
//	Vector3 offset = mScoreBoardOrigin;
//	offset.mX += 60;
//
//	for (const auto& entry : entries)
//	{
//		//entry.GetFormattedNameReadyState()
//		RenderText(entry.GetFormattedNameReadyState(), offset, entry.GetColor());
//
//		SDL_Rect src;
//		SDL_Rect dest;
//		src.x = src.y = 0;
//		src.h = dest.h = 50;
//		src.w = dest.w = 50;
//
//		dest.x = offset.mX - 60;
//		dest.y = offset.mY - 5;
//
//		if (entry.GetReadyState())
//		{
//			//Render tick
//			SDL_Surface* ready = IMG_Load("../Assets/playerReady.png");
//			SDL_Texture* tick = SDL_CreateTextureFromSurface(GraphicsDriver::sInstance->GetRenderer(), ready);
//
//			SDL_RenderCopy(GraphicsDriver::sInstance->GetRenderer(), tick, &src, &dest);
//		}
//		else
//		{
//			//render x
//			SDL_Surface* notready = IMG_Load("../Assets/playerNotReady.png");
//			SDL_Texture* not = SDL_CreateTextureFromSurface(GraphicsDriver::sInstance->GetRenderer(), notready);
//
//			SDL_RenderCopy(GraphicsDriver::sInstance->GetRenderer(), not, &src, &dest);
//		}
//		offset.mX += mScoreOffset.mX;
//		offset.mY += mScoreOffset.mY;
//		if(entry.GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
//			RenderText(entry.GetLobbyMessage(), mBandwidthOrigin, Colors::White);
//
//	}
//
//	SDL_Surface* Instructions = IMG_Load("../Assets/Instructions.png");
//	SDL_Texture* ins = SDL_CreateTextureFromSurface(GraphicsDriver::sInstance->GetRenderer(), Instructions);
//
//	SDL_Rect src;
//	SDL_Rect dest;
//	src.x = src.y = 0;
//	src.h = dest.h = 681;
//	src.w = dest.w = 455;
//
//	dest.x = 800;
//	dest.y = 20;
//
//	SDL_RenderCopy(GraphicsDriver::sInstance->GetRenderer(), ins, &src, &dest);
//}
//
//void HUD::RenderHealth()
//{
//	if (mHealth > 0)
//	{
//		//string healthString = StringUtils::Sprintf("Health %d", mHealth);
//		//RenderText(healthString, mHealthOffset, Colors::Red);
//
//		//health bar    
//		SDL_Color BGColor = { 0,0,0 };
//		SDL_Color FGColor = { 255,0,0 };
//		int w = 40;
//		mHealth = mHealth > 10.f ? 10.f : mHealth < 0.f ? 0.f : mHealth;
//		SDL_Color old;
//		SDL_GetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), &old.r, &old.g, &old.g, &old.a);
//		SDL_Rect bgrect = { 550, 30.f, 0.0f };
//		SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);
//		SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &bgrect);
//		SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
//		int pw = (int)((float)w * mHealth);
//		int px = 1000 + (w - pw);
//		SDL_Rect fgrect = { px, 10, pw, 10 };
//		SDL_RenderFillRect(GraphicsDriver::sInstance->GetRenderer(), &fgrect);
//		SDL_SetRenderDrawColor(GraphicsDriver::sInstance->GetRenderer(), old.r, old.g, old.b, old.a);
//
//	}
//}
//
//void HUD::RenderMoney()
//{
//	ScoreBoardManager::Entry* currentEntry = ScoreBoardManager::sInstance->GetEntry(NetworkManagerClient::sInstance->GetPlayerId());
//
//	if (currentEntry != nullptr)
//	{
//		if (currentEntry->GetHasMoney())
//		{
//			string healthString = StringUtils::Sprintf("$$");
//			RenderText(healthString, Vector3(1200, 10.f, 0.0f), Colors::Green);
//		}
//	}
//}
//
//void HUD::RenderBandWidth()
//{
//	string bandwidth = StringUtils::Sprintf( "In %d  Bps, Out %d Bps",
//		static_cast< int >(NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue()),
//		static_cast< int >(NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue()));
//	RenderText( bandwidth, mBandwidthOrigin, Colors::White );
//}
//
//void HUD::RenderPosition()
//{
//	string position = StringUtils::Sprintf("Pos: [%d,%d]",
//		static_cast< int >(RenderManager::sInstance->GetViewTransform().x),
//		static_cast< int >(RenderManager::sInstance->GetViewTransform().y));
//
//	RenderText( position, mPositionOffset, Colors::White);
//}
//
//void HUD::RenderRoundTripTime()
//{
//	float rttMS = NetworkManagerClient::sInstance->GetAvgRoundTripTime().GetValue() * 1000.f;
//
//	string roundTripTime = StringUtils::Sprintf( "RTT %d ms", ( int ) rttMS  );
//	RenderText( roundTripTime, mRoundTripTimeOrigin, Colors::White );
//}
//
//bool SortByScore(ScoreBoardManager::Entry x, ScoreBoardManager::Entry y) {
//	return (x.GetScore() > y.GetScore());
//}
//
//void HUD::RenderScoreBoard()
//{
//	vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
//	std::sort(entries.begin(), entries.end(), SortByScore);
//	Vector3 offset = mScoreBoardOrigin;
//	int count = 0; //only want the top 3
//	for( const auto& entry: entries )
//	{
//		RenderText( entry.GetFormattedNameScore(), offset, entry.GetColor() );
//		offset.mX += mScoreOffset.mX;
//		offset.mY += mScoreOffset.mY;
//		if (count == 2)
//			return;
//		count++;
//	}
//}
//
//
//
//void HUD::RenderText( const string& inStr, const Vector3& origin, const Vector3& inColor )
//{
//	// Convert the color
//	SDL_Color color;
//	color.r = static_cast<Uint8>( inColor.mX * 255 );
//	color.g = static_cast<Uint8>( inColor.mY * 255 );
//	color.b = static_cast<Uint8>( inColor.mZ * 255 );
//	color.a = 255;
//
//	// Draw to surface and create a texture
//	SDL_Surface* surface = TTF_RenderText_Blended( mFont, inStr.c_str(), color );
//	SDL_Texture* texture = SDL_CreateTextureFromSurface( GraphicsDriver::sInstance->GetRenderer(), surface );
//
//	// Setup the rect for the texture
//	SDL_Rect dstRect;
//	dstRect.x = static_cast<int>( origin.mX );
//	dstRect.y = static_cast<int>( origin.mY );
//	SDL_QueryTexture( texture, nullptr, nullptr, &dstRect.w, &dstRect.h );
//
//	// Draw the texture
//	SDL_RenderCopy( GraphicsDriver::sInstance->GetRenderer(), texture, nullptr, &dstRect );
//
//	// Destroy the surface/texture
//	SDL_DestroyTexture( texture );
//	SDL_FreeSurface( surface );
//}
