#pragma once

namespace q
{
namespace video
{

	class Atlaser
	{
	public:
		Atlaser();
		Atlaser(math::vec2u32 const& size);

		auto get_size() const->math::vec2u32 const&;

		void reset(math::vec2u32 const& size);
		void reset();

		auto get_region(math::vec2u32 const& size)->boost::optional<math::vec2u32>;

	private:
		auto fit(uint32_t index, math::vec2u32 const& size) -> int;
		void merge();

		math::vec2u32 m_size;
		std::vector<math::vec3s32> m_nodes;
	};


}
}

