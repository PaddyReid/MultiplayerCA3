#include <RoboCatClientPCH.h>

MapClient::MapClient() :
	textureName("grass")
{
	mSpriteComponent.reset(new SpriteComponent(this));
	SetSpriteTexture(textureName);
}

void MapClient::SetSpriteTexture(string _textureName) {
	textureName = _textureName;
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(textureName));
}

int MapClient::GetTextureWidth() {
	return TextureManager::sInstance->GetTexture(textureName)->GetWidth();
}

int MapClient::GetTextureHeight() {
	return TextureManager::sInstance->GetTexture(textureName)->GetHeight();
}
