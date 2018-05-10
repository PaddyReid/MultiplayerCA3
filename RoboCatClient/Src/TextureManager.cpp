#include <RoboCatClientPCH.h>
#include <SDL_image.h>

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset( new TextureManager() );
}


TextureManager::TextureManager()
{
	CacheTexture( "cat1", "../Assets/hitman_Idle.png");
	CacheTexture( "cat", "../Assets/hitman_Moving.png");
	CacheTexture( "mouse", "../Assets/mouse.png" );
<<<<<<< Updated upstream
	CacheTexture("yarn", "../Assets/yarn.png");
	
=======
	CacheTexture( "yarn", "../Assets/Bullet.png" );
>>>>>>> Stashed changes
}

TexturePtr	TextureManager::GetTexture( const string& inTextureName )
{
	return mNameToTextureMap[ inTextureName ];
}

bool TextureManager::CacheTexture( string inTextureName, const char* inFileName )
{
	SDL_Texture* texture = IMG_LoadTexture( GraphicsDriver::sInstance->GetRenderer(), inFileName );

	if( texture == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", inFileName );
		return false;
	}

	int w, h;
	SDL_QueryTexture( texture, nullptr, nullptr, &w, &h );

	// Set the blend mode up so we can apply our colors
	SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
	
	TexturePtr newTexture( new Texture( w, h, texture ) );

	mNameToTextureMap[ inTextureName ] = newTexture;

	return true;

}