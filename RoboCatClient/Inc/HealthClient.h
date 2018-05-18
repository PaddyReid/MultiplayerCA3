class HealthClient : public Health
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new HealthClient()); }
protected:
	HealthClient();

private:
	SpriteComponentPtr	mSpriteComponent;
};