
class GraphicsDriver
{
public:

	static bool StaticInit( SDL_Window* inWnd );

	static std::unique_ptr< GraphicsDriver >		sInstance;

	void					Clear();
	void					Present();
	SDL_Rect&				GetLogicalViewport();
	SDL_Rect&				GetPlayerViewport();
	SDL_Renderer*			GetRenderer();

	~GraphicsDriver();
	//void SetViewport(Vector3 playerLocation);

private:

	GraphicsDriver();
	bool Init( SDL_Window* inWnd );

	SDL_Renderer*			mRenderer;
	SDL_Rect				mViewport;
};
