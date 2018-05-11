class MapServer : public Map
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new MapServer()); }

	void HandleDying() override;
	virtual bool		HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	MapServer();

};