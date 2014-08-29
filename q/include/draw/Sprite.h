#pragma once

namespace q
{
namespace draw
{

	class Sprite : util::Noncopyable
	{
		friend class Painter;
	public:
		Sprite(SpriteBase_cptr const& base);
		~Sprite();

		SpriteBase const&	getBase() const;

		//////////////////////////////////////////////////////////////////////////
		// Transform

		void				setPosition(math::vec2f const& pos);
		math::vec2f const&	getPosition() const;

		void				setRotation(float rot);
		float				getRotation() const;

		void				setScale(math::vec2f const& scale);
		math::vec2f const&	getScale() const;

		void				setTransform(math::vec2f const& pos, float rot);
		void				setTransform(math::vec2f const& pos, float rot, math::vec2f const& scale);

		//////////////////////////////////////////////////////////////////////////
		// Animation

		void				setFps(uint32_t fps);
		uint32_t			getFps() const;

		void				setPlay(bool yes);
		bool				isPlaying() const;

		void				setMaxLoopCount(uint32_t count);
		uint32_t			getMaxLoopCount() const;
		uint32_t			getLoopCount() const;

		void				setTime(float time);
		float				getTime() const;

		void				setFrame(uint32_t frame);
		uint32_t					getFrame() const;

		void				setAnimation(uint32_t animation);
		uint32_t					getAnimation() const;
		String 				getAnimationName() const;

		void				update(float dt);

	protected:
		void				render(Painter& painter) const;

	private:
		SpriteBase_cptr		m_base;
		math::vec2f			mPosition;
		float				mRotation;
		math::vec2f			mScale;

		float				mTime;
		uint32_t					mFrame;
		uint32_t					mAnimation;
		uint32_t					mMaxLoopCount;
		uint32_t					mLoopCount;
		uint32_t					mFps;
		float				mTpf; //time per frame
		float				mInvTpf; //1 / tpf
		uint32_t					mFrameCount;
		float				mDuration;
		bool				mIsPlaying;
	};

	inline SpriteBase const& Sprite::getBase() const
	{
		return *m_base;
	}

	inline void Sprite::setPosition(math::vec2f const& pos)
	{
		mPosition = pos;
	}
	inline math::vec2f const& Sprite::getPosition() const
	{
		return mPosition;
	}

	inline void Sprite::setRotation(float rot)
	{
		mRotation = rot;
	}
	inline float Sprite::getRotation() const
	{
		return mRotation;
	}

	inline void Sprite::setScale(math::vec2f const& scale)
	{
		mScale = scale;
	}
	inline math::vec2f const& Sprite::getScale() const
	{
		return mScale;
	}

	inline void Sprite::setTransform(math::vec2f const& pos, float rot)
	{
		mPosition = pos;
		mRotation = rot;
	}
	inline void Sprite::setTransform(math::vec2f const& pos, float rot, math::vec2f const& scale)
	{
		mPosition = pos;
		mRotation = rot;
		mScale = scale;
	}

	inline void Sprite::setFps(uint32_t fps)
	{
		float oldFrame = mTime * mInvTpf;

		mFps = math::max(fps, 1u);
		mTpf = 1.f / (float)mFps;
		mInvTpf = 1 / mTpf;

		mTime = oldFrame * mTpf;
		mDuration = (float)mFrameCount * mTpf;
	}
	inline uint32_t Sprite::getFps() const
	{
		return mFps;
	}

	inline void Sprite::setPlay(bool yes)
	{
		mIsPlaying = yes;
	}
	inline bool Sprite::isPlaying() const
	{
		return mIsPlaying;
	}

	inline void Sprite::setMaxLoopCount(uint32_t count)
	{
		mMaxLoopCount = count;
	}
	inline uint32_t Sprite::getMaxLoopCount() const
	{
		return mMaxLoopCount;
	}
	inline uint32_t Sprite::getLoopCount() const
	{
		return mLoopCount;
	}

	inline void Sprite::setTime(float time)
	{
		mTime = math::clamp(time, 0.f, mDuration);
		mFrame = (uint32_t)(mTime * mInvTpf);
	}
	inline float Sprite::getTime() const
	{
		return mTime;
	}

	inline void Sprite::setFrame(uint32_t frame)
	{
		mFrame = math::clamp(frame, 0u, (uint32_t)(mFrameCount - 1));
		mTime = mFrame * mTpf;
	}
	inline uint32_t Sprite::getFrame() const
	{
		return mFrame;
	}

	inline void Sprite::setAnimation(uint32_t animation)
	{
		QASSERT(animation < m_base->getAnimationCount());
		mAnimation = animation;
		mTime = 0;
		mFrame = 0;
		mLoopCount = 0;
		mFrameCount = m_base->getFrameCount(animation);
		mDuration = (float)mFrameCount * mTpf;
	}
	inline uint32_t Sprite::getAnimation() const
	{
		return mAnimation;
	}
	inline String Sprite::getAnimationName() const
	{
		return m_base->getAnimationName(getAnimation());
	}

}
}