#pragma once

namespace q
{
namespace util
{


	//specialize this for your enum results
	template<class ENUM> std::string get_result_description(ENUM value)
	{
		return format2<std::string>("Unknown result code: {}", static_cast<int>(value));
	}

	//This represents a result. IT needs an enum with codes and the success code
	template <class ENUM, ENUM SUCCESS>
	class Result_T
	{
	public:
		typedef ENUM Enum_t;
		static const Enum_t s_success = SUCCESS;

		Result_T() = default;
		Result_T(Enum_t code) : m_code(code) {}

		auto get_code() const -> Enum_t { return m_code; }
		auto is_ok() const -> bool { return m_code == s_success; }

	private:
		Enum_t m_code = s_success;
	};

	//This represents a result. IT needs an enum with codes and the success code
	template <class ENUM, ENUM SUCCESS, class... Inners>
	class Result_Inner_T
	{
	public:
		typedef ENUM Enum_t;
		static const Enum_t s_success = SUCCESS;

		Result_T() = default;
		Result_T(Enum_t code) : m_code(code) {}
		
		template<class Inners...> 
		Result_T(Enum_t code, Inners const&... inners) : m_code(code) 
		{
			set_inners(inners...);
		}

		auto get_code() const -> Enum_t { return m_code; }
		auto is_ok() const -> bool { return m_code == s_success; }

		template<class Inner> 
		auto get_inner() const -> boost::optional<Inner&>
		{ 
			auto& inner = std::get<boost::optional>();
			return inner ? boost::optional<Inner&>(*inner) : boost::none;
		}
		template<class Inner> 
		auto has_inner() const -> bool
		{ 
			auto& inner = std::get<boost::optional>();
			return inner != false;
		}

	private:
		void set_inners()
		{
		}
		
		template<class Inner, class Inners...> 
		void set_inners(Inner const& inner, Inners const&... inners)
		{
			std::get<boost::optional<Inner>>() = inner;
			set_inners(inners...);
		}

		Enum_t m_code = s_success;
		std::tuple<boost::optional<Inners>...> m_inners;
	};

}
}


namespace q
{
namespace util
{
namespace formatting
{

	template<class Dst_Adapter, class Placeholder, class ENUM, ENUM SUCCESS>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, Result_T<ENUM, SUCCESS> const& p)
	{
		format_string(dst, ph, get_result_description(p.get_code()));
	}

	template<class Dst_Adapter, class Placeholder, class ENUM, ENUM SUCCESS, class INNER_RESULT>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, Result_Inner_T<ENUM, SUCCESS, INNER_RESULT> const& p)
	{
		format_string(dst, ph, get_result_description(p.get_code()));
	}

	//formatting the inner results
	template<class Dst_Adapter, class Placeholder, class Result, class Head>
	void format_inner(Dst_Adapter& dst, Placeholder const& ph, Result const& p)
	{
		format_string(dst, ph, p.get_inner<Head>());
	}

	template<class Dst_Adapter, class Placeholder, class Result, class Head, class... Tail>
	void format_inner(Dst_Adapter& dst, Placeholder const& ph, Result const& p)
	{
		format_string(dst, ph, p.get_inner<Head>());
		format_string(dst, ph, " ** ");
		format_inner<Dst_Adapter, Placeholder, Result, Tail...>(dst, ph, p);
	}
	//formatting the inner results

	template<class Dst_Adapter, class Placeholder, class ENUM, ENUM SUCCESS, class INNER_RESULT, class Head>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, Result_Inner_T<ENUM, SUCCESS, INNER_RESULT, Head> const& p)
	{
		format_string(dst, ph, get_result_description(p.get_code()));
		format_string(dst, ph, " ** ");
		format_string(dst, ph, p.get_inner<Head>());
	}

	template<class Dst_Adapter, class Placeholder, class ENUM, ENUM SUCCESS, class INNER_RESULT, class Head, class... Tail>
	void format_string(Dst_Adapter& dst, Placeholder const& ph, Result_Inner_T<ENUM, SUCCESS, INNER_RESULT, Head, Tail...> const& p)
	{
		format_string(dst, ph, get_result_description(p.get_code()));
		format_string(dst, ph, " ** ");
		format_inner<Dst_Adapter, Placeholder, Result, Head, Tail...>(dst, ph, p);
	}

}
}
}
