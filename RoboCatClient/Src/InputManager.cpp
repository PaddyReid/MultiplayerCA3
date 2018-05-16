#include <RoboCatClientPCH.h>

unique_ptr< InputManager >	InputManager::sInstance;

namespace
{
	float kTimeBetweenInputSamples = 0.03f;
}

void InputManager::StaticInit()
{
	sInstance.reset( new InputManager() );
}


namespace
{
	inline void UpdateDesireVariableFromKey( EInputAction inInputAction, bool& ioVariable )
	{
		if( inInputAction == EIA_Pressed || inInputAction == EIA_PressedOnce )
		{
			ioVariable = true;
		}
		else if( inInputAction == EIA_Released || inInputAction == EIA_ReleasedOnce)
		{
			ioVariable = false;
		}
	}

	inline void UpdateDesireFloatFromKey(EInputAction inInputAction, float& ioVariable)
	{
		if (inInputAction == EIA_Pressed || inInputAction == EIA_PressedOnce)
		{
			ioVariable = 1.f;
		}
		else if (inInputAction == EIA_Released || inInputAction == EIA_ReleasedOnce)
		{
			ioVariable = 0.f;
		}
	}

	inline void UpdateDesireVariableFromKeyPress(EInputAction inInputAction, bool& ioVariable)
	{
		if (LobbyManager::sInstance->GetEntries().size()>1) {
			if (inInputAction == EIA_Pressed || inInputAction == EIA_PressedOnce)
			{
				ioVariable = !ioVariable;
			}
		}
	}
}

void InputManager::HandleInput(EInputAction inInputAction, int inKeyCode)
{
	if (LobbyManager::sInstance->IsGamePlaying()) {
		switch (inKeyCode)
		{
		case SDLK_a:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredLeftAmount);
			break;
		case SDLK_d:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredRightAmount);
			break;
		case SDLK_w:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredForwardAmount);
			break;
		case SDLK_s:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredBackAmount);
			break;
		case SDLK_SPACE:
			UpdateDesireVariableFromKey(inInputAction, mCurrentState.mIsShooting);
			break;
		}
	}
	else {
		switch (inKeyCode)
		{
		case SDLK_a:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredLeftAmount);
			break;
		case SDLK_d:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredRightAmount);
			break;
		}
	}
	
}

void InputManager::HandleInputOnce(EInputAction inInputAction, int inKeyCode)
{
	
	if (LobbyManager::sInstance->IsGamePlaying()) {
		switch (inKeyCode)
		{
		case SDLK_a:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredLeftAmount);
			break;
		case SDLK_d:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredRightAmount);
			break;
		case SDLK_w:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredForwardAmount);
			break;
		case SDLK_s:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredBackAmount);
			break;
		case SDLK_SPACE:
			UpdateDesireVariableFromKey(inInputAction, mCurrentState.mIsShooting);
			break;
		}
	}
	else {
		switch (inKeyCode)
		{
		case SDLK_a:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredLeftAmount);
			break;
		case SDLK_d:
			UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredRightAmount);
			break;
		case SDLK_SPACE:
			UpdateDesireVariableFromKeyPress(inInputAction, mCurrentState.mIsReady);
			break;
		}
	}
}



InputManager::InputManager() :
	mNextTimeToSampleInput( 0.f ),
	mPendingMove( nullptr )
{

}

const Move& InputManager::SampleInputAsMove()
{
	return mMoveList.AddMove( GetState(), Timing::sInstance.GetFrameStartTime() );
}

bool InputManager::IsTimeToSampleInput()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if( time > mNextTimeToSampleInput )
	{
		mNextTimeToSampleInput = mNextTimeToSampleInput + kTimeBetweenInputSamples;
		return true;
	}

	return false;
}

void InputManager::Update()
{
	if( IsTimeToSampleInput() )
	{
		mPendingMove = &SampleInputAsMove();
	}
}