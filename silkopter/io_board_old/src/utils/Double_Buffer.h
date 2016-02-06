#pragma once

namespace util
{
	
	template<class T>
	class Double_Buffer
	{
	public:
		Double_Buffer()
		{
			m_data[0] = T();
			m_data[1] = T();
		}
		T& get()
		{
			uint8_t idx = m_idx;
			return m_data[idx];
		}
		T const& get() const
		{
			uint8_t idx = m_idx;
			return m_data[idx];
		}
		T& get_and_swap()
		{
			uint8_t idx = m_idx;
			m_idx = !m_idx;
			return m_data[idx];
		}
		
	private:
		volatile uint8_t m_idx = 0;
		T m_data[2];
	};
	
	
}