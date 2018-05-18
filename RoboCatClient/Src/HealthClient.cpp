#include <RoboCatClientPCH.h>

HealthClient::HealthClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetScale(Vector3(50, 50, 0));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("health"));
}

