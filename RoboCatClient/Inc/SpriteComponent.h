class SpriteComponent
{
public:

	SpriteComponent( GameObject* inGameObject );
	~SpriteComponent();

	virtual void		Draw( const SDL_Rect& inViewTransform );

			void		SetTexture( TexturePtr inTexture )			{ mTexture = inTexture; }

			Vector3		GetOrigin()					const			{ return mOrigin; }
			void		SetOrigin( const Vector3& inOrigin )		{ mOrigin = inOrigin; }

			Vector3		GetScale()					const { return mScale; }
			void		SetScale(const Vector3& inScale) { mScale = inScale; }


private:

	Vector3											mOrigin;
	Vector3											mScale;

	TexturePtr										mTexture;

	//don't want circular reference...
	GameObject*										mGameObject;
};

typedef shared_ptr< SpriteComponent >	SpriteComponentPtr;