#pragma once

namespace q
{
namespace data
{
	class Memory_Sink;

	class Memory_Source : public Source
	{
		friend class Memory_Sink;

	public:
		Memory_Source();
		Memory_Source(std::vector<uint8_t>&& data);
		Memory_Source(uint8_t const* data, size_t size); //copies the data
		virtual ~Memory_Source();

		size_t		get_size() const;
		size_t		get_capacity() const;

		bool		is_changed() const;

		size_t		read(uint8_t* buffer, size_t size);

		size_t		get_offset() const;
		size_t		seek(size_t offset);
		size_t		seek_relative(int offset);

		uint8_t const*	get_data_ptr() const;
		std::vector<uint8_t> get_data_copy() const;

		void		swap_data(Memory_Sink& sink);

	private:
		void		refresh_cache() const;

		size_t		m_offset;
		std::shared_ptr<std::vector<uint8_t>> m_data;
		mutable size_t		m_data_size;
		mutable uint8_t const*	m_data_ptr;
	};

}
}