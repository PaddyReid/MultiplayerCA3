class Client : public Engine
{
public:

	static bool StaticInit( );
	void DrawTileMap();
protected:

	Client();

	

	virtual void	DoFrame() override;
	virtual void	HandleEvent( SDL_Event* inEvent ) override;

private:
	bool isStarted;


};