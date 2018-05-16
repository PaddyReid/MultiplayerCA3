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
	bool			SetHasMoney(int val) { if (val == 1) { mHasMoney = true; } else { mHasMoney = false; } }
	bool			InLobby() const { return mInLobby; }

private:

	HUD();

	void	RenderGameTimer();
	void	RenderBandWidth();
	void	RenderPosition();
	void	RenderRoundTripTime();
	void	RenderScoreBoard();
	void	RenderHealth();
	void    RenderMoney();
	void	RenderText( const string& inStr, const Vector3& origin, const Vector3& inColor );

	Vector3										mBandwidthOrigin;
	Vector3										mRoundTripTimeOrigin;
	Vector3										mScoreBoardOrigin;
	Vector3										mScoreOffset;
	Vector3										mHealthOffset;
	Vector3										mPositionOffset;
	SDL_Rect									mViewTransform;

	TTF_Font*									mFont;
	int											mHealth;
	bool										mInLobby;
	bool										mHasMoney;
};

