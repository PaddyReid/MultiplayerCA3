
class GraphicsDriver
{
public:

	static bool StaticInit( SDL_Window* inWnd );
	GameObject				gameObject;

	static std::unique_ptr< GraphicsDriver >		sInstance;

	void					Clear();
	void					Present();
	void					LoadLevel(int arr[20][25]);
	void					DrawLevel();
	SDL_Rect&				GetLogicalViewport();
	SDL_Rect&				GetPlayerViewport();
	SDL_Renderer*			GetRenderer();

	~GraphicsDriver();
	//void SetViewport(Vector3 playerLocation);

private:

	GraphicsDriver();
	bool Init( SDL_Window* inWnd );

	int						map[20][25];
	SDL_Rect				src, dest;
	SDL_Texture*			test;
	SpriteComponentPtr		mSpriteComponent;
	SDL_Renderer*			mRenderer;
	SDL_Rect				mViewport;
};
