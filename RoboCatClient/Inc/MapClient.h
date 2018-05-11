class MapClient : public Map
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new MapClient()); }

protected:
	MapClient();

private:

	SpriteComponentPtr	mSpriteComponent;
}; 
