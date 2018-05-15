class Texture
{
public:
	Texture( uint32_t inWidth, uint32_t inHeight, SDL_Texture* inTexture );
	~Texture();

	uint32_t	GetWidth()	const						  { return mWidth; }
	uint32_t	GetHeight()	const						  { return mHeight; }
	void 		SetWidth(uint32_t width)				  { mWidth = width; }
	void 		SetHeight(uint32_t height)				  { mHeight = height; }
	SDL_Texture* GetData() const						  { return mTexture; }

private:
	uint32_t		mWidth;			
	uint32_t		mHeight;
	SDL_Texture*	mTexture;
};


typedef shared_ptr< Texture >	TexturePtr;