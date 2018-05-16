class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient( ClientProxyPtr inClientProxy );
	void HandleLostClient( ClientProxyPtr inClientProxy );

	RoboCatPtr	GetCatForPlayer( int inPlayerId );
	void	SpawnCatForPlayer( int inPlayerId );
	void	DrawTileMap();
	void	SetupSpawnLocation();
	Vector3	GetSpawnLocation(int inPlayerId);

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();
	float	mTimeElapsed;
	vector<Vector3>	mSpawnLocations;

};