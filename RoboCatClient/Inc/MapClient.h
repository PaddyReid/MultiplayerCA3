class MapClient : public Map
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new MapClient()); }
	void	SetSpriteTexture(string textureName);
	int		GetTextureWidth();
	int		GetTextureHeight();
protected:
	MapClient();

private:

	SpriteComponentPtr	mSpriteComponent;
	string textureName;
}; 
