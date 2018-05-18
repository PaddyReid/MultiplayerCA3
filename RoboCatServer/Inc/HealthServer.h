class HealthServer : public Health
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new HealthServer()); }

	void HandleDying() override;
	virtual bool		HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	HealthServer();

};