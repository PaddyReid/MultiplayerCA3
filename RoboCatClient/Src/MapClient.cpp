#include <RoboCatClientPCH.h> 

MapClient::MapClient() :
	textureName("vanOne")
{
	mSpriteComponent.reset(new SpriteComponent(this));
	SetSpriteTexture(textureName);
}

void MapClient::SetSpriteTexture(string _textureName) {
	textureName = _textureName;
	if (_textureName == "world")
	{
		mSpriteComponent->SetScale(Vector3(128, 128, 0));
	}
	else
	{
		mSpriteComponent->SetScale(Vector3(550, 400, 0));
	}
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(textureName));
}

int MapClient::GetTextureWidth() {
	return TextureManager::sInstance->GetTexture(textureName)->GetWidth();
}

int MapClient::GetTextureHeight() {
	return TextureManager::sInstance->GetTexture(textureName)->GetHeight();
}

bool MapClient::HandleCollisionWithCat(RoboCat* inCat)
{
	if (GetCollidable())
	{
		//To-Do add wall collision
	}
	return false;
}