#pragma once

#include "res/Resource.h"
#include "draw/Painter.h"

namespace q
{
namespace draw
{

	class SpriteBase : public res::Resource, public std::enable_shared_from_this<SpriteBase>
	{
		friend class Sprite;
		DEFINE_RTTI_CLASS(q::draw::SpriteBase, q::res::Resource);
	public:
		SpriteBase(Path const& path);
		virtual ~SpriteBase();

		enum class DataType : uint8_t
		{
			MODULE				= 1 << 0,
			ELLIPSE				= 1 << 1,
			LINE				= 1 << 2,
			RECTANGLE			= 1 << 3,
			ROUNDED_RECTANGLE	= 1 << 4,
			MARKER				= 1 << 5
		};

		virtual void		unload();
		virtual bool		is_valid() const;

		//////////////////////////////////////////////////////////////////////////
		// Building the data

		uint32_t		addTexture(String const& name, video::Texture_cptr const& tex);

		uint32_t		addModule(uint32_t textureIdx, math::vec2u32 const& size, math::vec2f const& uv, math::vec2f const& uvSize);
		uint32_t		addEllipse(math::vec2f const& size, float startAngle, float endAngle, uint32_t color);
		uint32_t		addLine(math::vec2f const& end, uint32_t color);
		uint32_t		addRectangle(math::vec2f const& size, uint32_t color);
		uint32_t		addRoundedRectangle(math::vec2f const& size, math::vec2f const& arcSize, uint32_t color);
		uint32_t		addMarker(math::vec2f const& size);

		uint32_t		addAnimation(String const& name);
        void            addAnimationData(uint32_t animationIdx, math::mat3f const& transform, uint32_t frameIdx);
		
		uint32_t		addFrame(String const& name);
        void            addFrameData(uint32_t frameIdx, math::mat3f const& transform, DataType type, uint32_t dataIdx);

		//////////////////////////////////////////////////////////////////////////
		// Getters

		video::Texture_cptr getTexture(uint32_t idx) const;
        String          getTextureName(uint32_t idx) const;

        int             findAnimationIdxByName(String const& name) const;
        String          getAnimationName(uint32_t idx) const;

		uint32_t		getAnimationCount() const;
		uint32_t		getFrameCount(uint32_t animationIdx) const;
		uint32_t		getFrameDataCount(uint32_t animationIdx, uint32_t frameIdx) const;

		math::mat3f const& getFrameDataTransform(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const;
        DataType        getFrameDataType(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const;
		uint32_t		getFrameDataIdx(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const;

        math::vec2f&    getFrameDataSize(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const;

		//////////////////////////////////////////////////////////////////////////

        Sprite_ptr      createInstance() const;

	protected:
        void            render(Painter& painter, math::vec2f const& pos, float rot, math::vec2f const& scale, uint32_t animIdx, uint32_t frameIdx) const;

	private:
		struct Texture
		{
			String	name;
			video::Texture_cptr texture;
		};
		struct AnimData
		{
			uint32_t frameIdx;
			math::mat3f transform;
			math::vec2f	position;
			bool		hasRotScale;
		};
		struct Anim
		{
			String name;
			std::vector<AnimData> data;
		};
		struct FrameData
		{
			uint16_t dataIdx;
			math::mat3f transform;
			math::vec2f	position;
			bool		hasRotScale;
			DataType type;
		};
		struct Frame
		{
			String name;
			std::vector<FrameData> data;
		};
		struct Module
		{
			uint32_t			textureIdx;
			math::vec2u32 size;
			math::vec2f uv;
			math::vec2f uvSize;
		};
		struct Ellipse
		{
			math::vec2f size;
			float startAngle;
			float endAngle;
			uint32_t color;
		};
		struct Line
		{
			math::vec2f end;
			uint32_t color;
		};
		struct Rect
		{
			math::vec2f size;
			uint32_t color;
		};
		struct RoundedRect
		{
			math::vec2f size;
			math::vec2f arcSize;
			uint32_t color;
		};
		struct Marker
		{
			math::vec2f size;
		};

		typedef std::vector<Anim>		AnimArray;
		typedef std::vector<Anim>		AnimDataArray;
		typedef std::vector<Frame>		FrameArray;
		typedef std::vector<FrameData>	FrameDataArray;

		typedef std::vector<Module>		ModuleArray;
		typedef std::vector<Ellipse>	EllipseArray;
		typedef std::vector<Line>		LineArray;
		typedef std::vector<Rect>		RectArray;
		typedef std::vector<RoundedRect>RoundedRectArray;
		typedef std::vector<Marker>		MarkerArray;

		AnimArray		mAnims;
		FrameArray		mFrames;
		ModuleArray		mModules;
		EllipseArray	mEllipses;
		LineArray		mLines;
		RectArray		mRects;
		RoundedRectArray mRoundedRects;
		MarkerArray		mMarkers;
		std::vector<Texture> mTextures;
	};

	inline uint32_t SpriteBase::addTexture(String const& name, video::Texture_cptr const& tex)
	{
		QASSERT(tex);
		Texture t;
		t.name = name;
		t.texture = tex;
		mTextures.push_back(t);
		return mTextures.size() - 1;
	}
	inline uint32_t SpriteBase::addModule(uint32_t textureIdx, math::vec2u32 const& size, math::vec2f const& uv, math::vec2f const& uvSize)
	{
		QASSERT(textureIdx < mTextures.size());
		Module d;
		d.textureIdx = textureIdx;
		d.size = size;
		d.uv = uv;
		d.uvSize = uvSize;
		mModules.push_back(d);
		return mModules.size() - 1;
	}
	inline uint32_t SpriteBase::addEllipse(math::vec2f const& size, float startAngle, float endAngle, uint32_t color)
	{
		Ellipse d;
		d.size = size;
		d.startAngle = startAngle;
		d.endAngle = endAngle;
		d.color = color;
		mEllipses.push_back(d);
		return mEllipses.size() - 1;
	}
	inline uint32_t SpriteBase::addLine(math::vec2f const& end, uint32_t color)
	{
		Line d;
		d.end = end;
		d. color = color;
		mLines.push_back(d);
		return mLines.size() - 1;
	}
	inline uint32_t SpriteBase::addRectangle(math::vec2f const& size, uint32_t color)
	{
		Rect d;
		d.size = size;
		d.color = color;
		mRects.push_back(d);
		return mRects.size() - 1;
	}
	inline uint32_t SpriteBase::addRoundedRectangle(math::vec2f const& size, math::vec2f const& /*arcSize*/, uint32_t color)
	{
		RoundedRect d;
		d.size = size;
		d.arcSize = size;
		d.color = color;
		mRoundedRects.push_back(d);
		return mRoundedRects.size() - 1;
	}
	inline uint32_t SpriteBase::addMarker(math::vec2f const& size)
	{
		Marker d;
		d.size = size;
		mMarkers.push_back(d);
		return mMarkers.size() - 1;
	}

	inline uint32_t SpriteBase::addAnimation(String const& name)
	{
		Anim a;
		a.name = name;
		mAnims.push_back(a);
		return mAnims.size() - 1;
	}
	inline void SpriteBase::addAnimationData(uint32_t animationIdx, math::mat3f const& transform, uint32_t frameIdx)
	{
		QASSERT(frameIdx < mFrames.size());
		AnimData d;
		d.transform = transform;
		QASSERT(0);
		//d.position = transform.getTranslation();
		d.hasRotScale = !math::is_identity(math::mat2f(transform));
		d.frameIdx = frameIdx;
		mAnims[animationIdx].data.push_back(d);
	}

	inline uint32_t SpriteBase::addFrame(String const& name)
	{
		Frame f;
		f.name = name;
		mFrames.push_back(f);
		return mFrames.size() - 1;
	}
	inline void SpriteBase::addFrameData(uint32_t frameIdx, math::mat3f const& transform, DataType type, uint32_t dataIdx)
	{
		FrameData d;
		d.transform = transform;
		QASSERT(0);
		//d.position = transform.getTranslation();
		d.hasRotScale = !math::is_identity(math::mat2f(transform));
		d.type = type;
		d.dataIdx = static_cast<uint16_t>(dataIdx);
		mFrames[frameIdx].data.push_back(d);
	}

	//////////////////////////////////////////////////////////////////////////

	inline video::Texture_cptr SpriteBase::getTexture(uint32_t idx) const
	{
		return mTextures[idx].texture;
	}
	inline String SpriteBase::getTextureName(uint32_t idx) const
	{
		return mTextures[idx].name;
	}


	inline String SpriteBase::getAnimationName(uint32_t idx) const
	{
		return mAnims[idx].name;
	}
	inline int SpriteBase::findAnimationIdxByName(String const& name) const
	{
		for (uint32_t i = 0; i < mAnims.size(); i++)
		{
			if (mAnims[i].name == name)
			{
				return i;
			}
		}
		return -1;
	}
	inline uint32_t SpriteBase::getAnimationCount() const
	{
		return mAnims.size();
	}
	inline uint32_t SpriteBase::getFrameCount(uint32_t animationIdx) const
	{
		return mAnims[animationIdx].data.size();
	}
	inline uint32_t SpriteBase::getFrameDataCount(uint32_t animationIdx, uint32_t frameIdx) const
	{
		return mFrames[mAnims[animationIdx].data[frameIdx].frameIdx].data.size();
	}

	inline math::mat3f const& SpriteBase::getFrameDataTransform(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const
	{
		return mFrames[mAnims[animationIdx].data[frameIdx].frameIdx].data[frameDataIdx].transform;
	}
	inline SpriteBase::DataType SpriteBase::getFrameDataType(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const
	{
		return mFrames[mAnims[animationIdx].data[frameIdx].frameIdx].data[frameDataIdx].type;
	}
	inline uint32_t SpriteBase::getFrameDataIdx(uint32_t animationIdx, uint32_t frameIdx, uint32_t frameDataIdx) const
	{
		return mFrames[mAnims[animationIdx].data[frameIdx].frameIdx].data[frameDataIdx].dataIdx;
	}

}
}
