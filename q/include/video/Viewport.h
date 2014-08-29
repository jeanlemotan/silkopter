#pragma once

namespace q
{
namespace video
{
	struct Viewport
	{
		Viewport();
		Viewport(math::vec2u32 const& position, math::vec2u32 const& size);
		bool operator==(Viewport const& other) const;
		bool operator!=(Viewport const& other) const;

		math::vec2u32 position;
		math::vec2u32 size;
	};

	//////////////////////////////////////////////////////////////////////////

	inline Viewport::Viewport() 
	{
	}
	inline Viewport::Viewport(math::vec2u32 const& position, math::vec2u32 const& size)
		: position(position)
		, size(size) 
	{
	}

	inline bool Viewport::operator==(Viewport const& other) const
	{
		return position == other.position && size == other.size;
	}
	inline bool Viewport::operator!=(Viewport const& other) const
	{
		return !operator==(other);
	}
	
}
}