#include <RoboCatClientPCH.h>

MouseClient::MouseClient()
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetScale(Vector3(25, 20, 0));
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "money" ) );
}