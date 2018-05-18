class LobbyManager
{
public:

	static void StaticInit();
	static std::unique_ptr< LobbyManager >	sInstance;

	class LobbyPlayer
	{
	public:
		LobbyPlayer() {};

		LobbyPlayer(uint32_t inPlayerID, const string& inPlayerName, const Vector3& inColor, bool readyBool);

		const Vector3&	GetColor()		const { return mColor; }
		uint32_t		GetPlayerId()	const { return mPlayerId; }
		const string&	GetPlayerName()	const { return mPlayerName; }
		const string&	GetFormattedNameReadyState()	const { return mFormattedNameReadyState; }
		const string&	GetLobbyMessage() const { return mLobbyMessage; }
		bool			GetReadyState()		const { return mReadyState; }
	
		void			SetReadyState(bool inReadyState);
		void			SetLobbyMessage(string lobbyMessage);
		bool			Write(OutputMemoryBitStream& inOutputStream) const;
		bool			Read(InputMemoryBitStream& inInputStream);
		static uint32_t	GetSerializedSize();

	private:
		Vector3			mColor;

		uint32_t		mPlayerId;
		string			mPlayerName;

		int				mReadyState;

		string			mFormattedNameReadyState;
		string			mLobbyMessage;
	};

	LobbyPlayer*	GetEntry(uint32_t inPlayerId);
	bool	RemoveEntry(uint32_t inPlayerId);
	void	AddEntry(uint32_t inPlayerId, const string& inPlayerName);
	void	ChangeReadyState(uint32_t inPlayerId, bool inReadyState);
	void	CheckPlayerCount();
	void	ResetTimeToGameStart();
	void	DecrementTimeToGameStart();
	void	ResetMatchTimer();
	void	DecrementMatchTimer();

	bool	Write(OutputMemoryBitStream& inOutputStream) const;
	bool	Read(InputMemoryBitStream& inInputStream);
	bool	IsEveryoneReady() const { return mEveryoneReady; }
	bool	IsGamePlaying() const { return mGamePlaying; }
	bool	IsGameFinished() const { return mGameFinished; }
	void	SetEveryoneReady(bool inEveryoneReady);
	void	SetGamePlaying(bool gamePlaying);
	void	SetGameFinished(bool gameFinished);
	int		GetTimeToGameStart() const;
	void    SetTimeToGameStart(int newTime);
	int		GetMatchTimer() const;
	void    SetMatchTimer(int matchTimer);
	void	StartGame(); 
	void	ResetGame();

	const vector< LobbyPlayer >&	GetEntries()	const { return mEntries; }
	int const MATCH_TIMER = 120;
	int const TIME_TO_GAME_START = 5;

private:
	

	LobbyManager();

	vector< LobbyPlayer >	mEntries;

	vector< Vector3 >	mDefaultColors;

	int mTimeToGameStart;
	int mMatchTimer;
	bool mEveryoneReady;
	bool mGamePlaying;
	bool mGameFinished;
};