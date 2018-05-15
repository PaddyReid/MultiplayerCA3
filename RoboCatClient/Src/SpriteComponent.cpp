#include <RoboCatClientPCH.h>

SpriteComponent::SpriteComponent( GameObject *inGameObject ) :
	mGameObject( inGameObject )
{

	//hardcoded at the moment...
	float textureWidth = 128.f, textureHeight = 128.f;
	//origin should be half texture size, but we're not loading the actual size at the moment
	mOrigin = Vector3( textureWidth * 0.5f, textureHeight * 0.5f, 0.f );

	//and add yourself to the rendermanager...
	RenderManager::sInstance->AddComponent( this );
}

SpriteComponent::~SpriteComponent()
{
	//don't render me, I'm dead!
	RenderManager::sInstance->RemoveComponent( this );
}

void SpriteComponent::Draw( const SDL_Rect& inViewTransform )
{
	if( mTexture )
	{
		// Texture color multiplier
		Vector3 color = mGameObject->GetColor();
		Uint8 r = static_cast<Uint8>( color.mX * 255 );
		Uint8 g = static_cast<Uint8>( color.mY * 255 );
		Uint8 b = static_cast<Uint8>( color.mZ * 255 );
		SDL_SetTextureColorMod( mTexture->GetData(), r, g, b );

		// Compute the destination rectangle
		Vector3 objLocation = mGameObject->GetLocation();
		float objScale = mGameObject->GetScale();
		SDL_Rect SrcRect;

		//Need to set when picked
		Vector3 x;
		x = mGameObject->GetSource();

		SrcRect.x = x.mX;
		SrcRect.y = x.mY;

		//Not make this magic
		SrcRect.h = mScale.mX;
		SrcRect.w = mScale.mY;



		//ability to set 
		SDL_Rect dstRect;
		dstRect.w = static_cast< int >(mScale.mX);
		dstRect.h = static_cast< int >(mScale.mY);
		dstRect.x = static_cast<int>( objLocation.mX * inViewTransform.w + inViewTransform.x - dstRect.w / 2 );
		dstRect.y = static_cast<int>( objLocation.mY * inViewTransform.h + inViewTransform.y - dstRect.h / 2 );
		
		// Blit the texture
		SDL_RenderCopyEx( GraphicsDriver::sInstance->GetRenderer(), mTexture->GetData(), &SrcRect,
			&dstRect, RoboMath::ToDegrees( mGameObject->GetRotation() ), nullptr, SDL_FLIP_NONE );
	}
}