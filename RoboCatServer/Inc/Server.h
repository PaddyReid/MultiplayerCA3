class Server : public Engine
{
	struct HighScore{
		string playerName;
		int score;
	};
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient( ClientProxyPtr inClientProxy );
	void HandleLostClient( ClientProxyPtr inClientProxy );

	RoboCatPtr	GetCatForPlayer( int inPlayerId );
	RoboCat *	GetCatRefForPlayer(int inPlayerId);
	void	SpawnCatForPlayer( int inPlayerId );
	void    SpawnHealthPacks();
	void	DrawTileMap(Vector3 pos);
	void	SetupSpawnLocation();
	void	LoadHighScores();
	void	UpdateHighScores();
	bool	UpdateHighScore(int playerId, string playerName, int score);
	void	SaveHighScores();
	int		GetHighScore(int playerId, string playerName);
	int		GetHighScore(string playerName);
	Vector3	GetSpawnLocation(int inPlayerId);
	ClientProxyPtr	GetIPAddr(int inPlayerId);
	void	DropMoney(Vector3 pos);

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();
	float	mTimeElapsed;
	vector<Vector3>	mSpawnLocations;
	vector<HighScore> mHighScores;
	vector<ClientProxyPtr> mClientProxies;
};