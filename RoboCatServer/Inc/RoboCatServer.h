enum ECatControlType
{
	ESCT_Human,
	ESCT_AI
};

class RoboCatServer : public RoboCat
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new RoboCatServer() ); }
	virtual void HandleDying() override;

	virtual void Update();

	void SetCatControlType( ECatControlType inCatControlType ) { mCatControlType = inCatControlType; }

	void Kill();
	void TakeDamage( int inDamagingPlayerId );
	void AddHealth();
	void SetReadyState(int inPlayerId, bool readyState);

protected:
	RoboCatServer();

private:

	void HandleShooting();


	ECatControlType	mCatControlType;


	float		mTimeOfNextShot;
	float		mTimeBetweenShots;

};