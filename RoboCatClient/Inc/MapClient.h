class MapClient : public Map
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new MapClient()); }
	void	SetSpriteTexture(string textureName);
	int		GetTextureWidth();
	int		GetTextureHeight();

	bool	GetCollidable() { return isCollidable; }
	void	setCollidable(bool colide) { isCollidable = colide; }


	bool HandleCollisionWithCat(RoboCat * inCat);

	SDL_Rect textureSource;

protected:
	MapClient();

private:

	SpriteComponentPtr	mSpriteComponent;
	string textureName;
	bool isCollidable;
}; 
