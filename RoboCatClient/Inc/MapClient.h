class MapClient : public Map
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new MapClient()); }
	void	SetSpriteTexture(string textureName);
	int		GetTextureWidth();
	int		GetTextureHeight();

	SDL_Rect textureSource;

protected:
	MapClient();

private:

	SpriteComponentPtr	mSpriteComponent;
	string textureName;
	
}; 
