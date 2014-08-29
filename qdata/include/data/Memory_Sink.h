#pragma once

namespace q
{
namespace data
{
	class Memory_Source;

	class Memory_Sink : public Sink
	{
		friend class Memory_Source;
	public:
		Memory_Sink(size_t reserve_size = 0u);
		virtual ~Memory_Sink();

		size_t		get_size() const;
		size_t		get_capacity() const;
		void		set_size(size_t size);

		size_t		write(uint8_t const* buffer, size_t size);
		size_t		get_offset() const;
		size_t		seek(size_t offset);
		size_t		seek_relative(int offset);

		void		swap_data(Memory_Source& source);

		uint8_t const*	get_data_ptr() const;
		std::vector<uint8_t> get_data_copy() const;

	private:
		void		refresh_cache() const;
		std::shared_ptr<std::vector<uint8_t>> m_data;
		mutable uint8_t*			m_data_ptr;
		mutable size_t		m_data_size;
		size_t		m_offset;
	};


}
}