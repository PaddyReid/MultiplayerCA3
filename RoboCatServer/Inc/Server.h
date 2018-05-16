class Server : public Engine
{
	struct HighScore{
		string playerName;
		int playerId;
		int score;
	};
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
	void	LoadHighScores();
	void	SaveHighScores();
	int		GetHighScore(int playerId);
	Vector3	GetSpawnLocation(int inPlayerId);
	void	DropMoney(Vector3 pos);

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();
	float	mTimeElapsed;
	vector<Vector3>	mSpawnLocations;
	vector<HighScore> highScores;
};