#include <RoboCatClientPCH.h>
#include <SDL_image.h>

std::unique_ptr< GraphicsDriver >	GraphicsDriver::sInstance;


namespace
{
}


bool GraphicsDriver::StaticInit( SDL_Window* inWnd )
{
	GraphicsDriver* newGraphicsDriver = new GraphicsDriver();
	bool result = newGraphicsDriver->Init( inWnd );

	if( !result )
	{
		delete newGraphicsDriver;
	}
	else
	{
		sInstance.reset( newGraphicsDriver );
	}

	return result;
}

bool GraphicsDriver::Init( SDL_Window* inWnd )
{
	mRenderer = SDL_CreateRenderer( inWnd, -1, SDL_RENDERER_ACCELERATED );
	SDL_Renderer* tRenderer = SDL_CreateRenderer(inWnd, 2000, SDL_RENDERER_ACCELERATED);

	if( mRenderer == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to create hardware-accelerated renderer." );
		return false;
	}
	SDL_Surface* Loading_Test = SDL_LoadBMP("../Assets/test.bmp");
	SDL_Texture* BlueShape = SDL_CreateTextureFromSurface(mRenderer, Loading_Test);

	SDL_Surface* tempSurface = IMG_Load("../Assets/Tiles/wall.png");
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, tempSurface);
	src.x = src.y = 64;
	src.h = dest.h = 64;
	src.w = dest.w = 64;

	dest.x = 64;
	dest.y = 64;

	//int i;
	//int n;
	//for (i = 0; i < 2; ++i) {
	//	for (n = 0; n < 4; ++n) {
	//		src.x = 64 * (n % 2);
	//		if (n > 1) {
	//			src.y = 64;
	//		}
	//		else {
	//			src.y = 0;
	//		}

	//		/* render background, whereas NULL for source and destination
	//		rectangles just means "use the default" */
	//		SDL_RenderCopy(mRenderer, tex, NULL, NULL);

	//		/* render the current animation step of our shape */
	//		SDL_RenderCopy(mRenderer, BlueShape, &src, &dest);
	//		SDL_RenderPresent(mRenderer);
	//		SDL_Delay(500);
	//	}
	//}

	//LoadLevel(lev);


	
	//LoadLevel(lev);
	//DrawLevel();

	////Render Pixel world here

	////Cornflower blue background, cause why not?
	SDL_SetRenderDrawColor(mRenderer, 100, 0, 0, 255);
	//// Set the logical size to 1280x720 so everything will just auto-scale
	SDL_RenderClear(mRenderer);
	/*SDL_RenderCopy(mRenderer, tex, NULL, &dest);*/
	SDL_RenderSetLogicalSize(mRenderer, 1280, 720);
	SDL_RenderPresent(mRenderer);
	SDL_Delay(500);
	return true;
}

GraphicsDriver::GraphicsDriver()
	: mRenderer( nullptr )
{
}


GraphicsDriver::~GraphicsDriver()
{
	if( mRenderer != nullptr )
	{
		SDL_DestroyRenderer( mRenderer );
	}
}

void GraphicsDriver::Clear()
{
	SDL_RenderClear( mRenderer );
}

void GraphicsDriver::LoadLevel(int arr[20][25])
{
	for (int row = 0; row < 20; row++)
	{
		for (int colum = 0; colum < 25; colum++)
		{
			map[row][colum] = arr[row][colum];
		}
	}
}

void GraphicsDriver::DrawLevel()
{
	int type = 0;
	for (int row = 0; row < 20; row++)
	{
		for (int colum = 0; colum < 25; colum++)
		{
			type = map[row][colum];

			dest.x = colum * 32;
			dest.y = row * 32;

			switch (type)
			{
			case 0:
				break;
			}
		}
	}
}

void GraphicsDriver::Present()
{
	SDL_RenderPresent( mRenderer );
}

SDL_Rect& GraphicsDriver::GetLogicalViewport()
{
	SDL_RenderGetLogicalSize(mRenderer, &mViewport.w, &mViewport.h);

	return mViewport;
}

SDL_Rect& GraphicsDriver::GetPlayerViewport()
{
	return mViewport;
}

SDL_Renderer* GraphicsDriver::GetRenderer()
{
	return mRenderer;
}
//
//void GraphicsDriver::SetViewport(Vector3 playerLocation)
//{
//	mViewport.x = (playerLocation.mX*100) + mViewport.w*0.5;
//	mViewport.y = (playerLocation.mY*100) + mViewport.h*0.5;
//
//}