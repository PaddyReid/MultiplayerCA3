#include <RoboCatClientPCH.h>

MouseClient::MouseClient()
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetScale(Vector3(75, 75, 0));
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "money" ) );
}