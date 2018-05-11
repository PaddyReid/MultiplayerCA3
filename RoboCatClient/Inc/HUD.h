//I take care of rendering things!

#include <SDL_ttf.h>

class HUD
{
public:

	static void StaticInit();
	static std::unique_ptr< HUD >	sInstance;

	void Render();

	void RenderLobby();

	void			SetPlayerHealth(int inHealth) { mHealth = inHealth; }
	bool			InLobby() const { return mInLobby; }

private:

	HUD();

	void	RenderBandWidth();
	void	RenderPosition();
	void	RenderRoundTripTime();
	void	RenderScoreBoard();
	void	RenderHealth();
	void	RenderText( const string& inStr, const Vector3& origin, const Vector3& inColor );

	Vector3										mBandwidthOrigin;
	Vector3										mRoundTripTimeOrigin;
	Vector3										mScoreBoardOrigin;
	Vector3										mScoreOffset;
	Vector3										mHealthOffset;
	SDL_Rect									mViewTransform;

	TTF_Font*									mFont;
	int											mHealth;
	bool										mInLobby;
};

