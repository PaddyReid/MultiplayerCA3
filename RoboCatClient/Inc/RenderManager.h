//I take care of rendering things!
class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr< RenderManager >	sInstance;

	void Render();
	void SetCamera(Vector3 playerLocation);
	SDL_Rect GetViewTransform() const;
	void RenderComponents();

	//vert inefficient method of tracking scene graph...
	void AddComponent( SpriteComponent* inComponent );
	void RemoveComponent( SpriteComponent* inComponent );
	int	 GetComponentIndex( SpriteComponent* inComponent ) const;
	static SDL_Renderer *renderer;
	
private:

	RenderManager();

	//this can't be only place that holds on to component- it has to live inside a GameObject in the world
	vector< SpriteComponent* >		mComponents;

	SDL_Rect						mViewTransform;

};

