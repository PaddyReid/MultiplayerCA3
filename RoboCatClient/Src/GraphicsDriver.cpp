#include <RoboCatClientPCH.h>

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
	if( mRenderer == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to create hardware-accelerated renderer." );
		return false;
	}


	//Render Pixel world here

	// Cornflower blue background, cause why not?
	SDL_SetRenderDrawColor(mRenderer, 100, 149, 237, SDL_ALPHA_OPAQUE);
	//SDL_SetRenderTarget(mRenderer, );
	// Set the logical size to 1280x720 so everything will just auto-scale
	SDL_RenderSetLogicalSize( mRenderer, 1280, 720 );
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