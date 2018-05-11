#include <RoboCatClientPCH.h>

MapClient::MapClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("grass"));
}