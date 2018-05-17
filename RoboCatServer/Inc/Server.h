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
	void	DrawTileMap(Vector3 pos);
	void	SetupSpawnLocation();
	Vector3	GetSpawnLocation(int inPlayerId);
	void	DropMoney(Vector3 pos);

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();
	float	mTimeElapsed;
	vector<Vector3>	mSpawnLocations;

};