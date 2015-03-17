#include "QStdAfx.h"
#include "res/Resource.h"
#include "Ptr_Fw_Declaration.h"
#include "draw/Sprite_Base.h"
#include "draw/Sprite.h"
#include "draw/Painter.h"


using namespace q;
using namespace draw;

Sprite::Sprite(SpriteBase_cptr const& base)
{
	m_base = base;
	mScale = math::vec2f(1);
	mRotation = 0.f;

	mTime = 0;
	mFrame = 0;
	mAnimation = 0;
	mMaxLoopCount = 999999999;
	mLoopCount = 0;
	mIsPlaying = true;
	setFps(20);
	setAnimation(0);
}

Sprite::~Sprite()
{
}

void Sprite::update(float dt)
{
	if (mIsPlaying)
	{
		mTime += dt;
		mFrame = (uint32_t)(mTime * mInvTpf);
		if (mFrame >= mFrameCount)
		{
			if (mLoopCount < mMaxLoopCount)
			{
				mFrame = 0;
				mTime -= mDuration;
				mLoopCount++;
			}
			else
			{
				mFrame = mFrameCount - 1;
				mTime = (float)mFrame * mTpf;
			}
		}
	}
}

void Sprite::render(Painter& painter) const
{
	m_base->render(painter, mPosition, mRotation, mScale, mAnimation, mFrame);
}
