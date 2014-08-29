#include "QStdAfx.h"
#include <boost/xpressive/xpressive_dynamic.hpp>

#include <regex>

using namespace q;
using namespace video;

Shader_Source::Shader_Source(String const& src)
	: m_source(src.begin(), src.end())
{
	process();
}

Shader_Source::Shader_Source(Shader_Source const& other)
	: m_source(other.m_source)
{
	operator=(other);
}

Shader_Source::Shader_Source(Shader_Source&& other)
	: m_source(std::move(other.m_source))
	, m_tokens(std::move(other.m_tokens))
{
}

Shader_Source& Shader_Source::operator=(Shader_Source const& other)
{
	if (this != &other)
	{
		m_source = other.m_source;
		m_tokens = other.m_tokens;
	}
	return *this;
}
bool Shader_Source::operator==(Shader_Source const& other) const
{
	return m_source == other.m_source;
}
bool Shader_Source::operator!=(Shader_Source const& other) const
{
	return !operator==(other);
}

String Shader_Source::get_source() const
{
	return String(m_source);
}

void Shader_Source::process()
{
	using namespace boost::xpressive;

	//mTokens.resize(0);
	m_tokens.clear();

	//\$\(\s*(.*?)\s*\)
	//find tags

	//split mumu.bibi[23]
	//([a-zA-Z0-9_\.]+)(?:\[(.*)\])?

	//validate mumu.bibi (^[a-zA-z][\.a-zA-Z_]*[a-zA-Z]$)

	//validate [x]
	//^\s*([a-zA-Z_]+[0-9a-zA-Z_]+|[0-9]+)\s*$

	//#if maca == -0.21 || bubu <= 3 && m == 1
	//#if\s+(\S+)\s*(==|>=|<=|!=|<|>|)\s*([-+]?[0-9]*\.?[0-9]+)(?:\s*(&&|\|\|)+\s*(\S+)\s*(==|>=|<=|!=|<|>)\s*([-+]?[0-9]*\.?[0-9]+))

	//search for auto uniforms
	auto tag_rex = sregex::compile("\\$\\((.*?)\\)"); //Matches $(x) and returns x
	auto splitter_rex = sregex::compile("([a-zA-Z0-9_]+)(?:\\[(.*?)\\])?"); //Matches $(x) and returns x
	auto name_validator_rex = sregex::compile("(^[a-zA-Z0-9_]+$)");
	auto index_validator_rex = sregex::compile("^\\s*([a-zA-Z_]+[0-9a-zA-Z_]+|[0-9]+)\\s*$");

    //auto time = Clock::now();

	sregex_iterator it(m_source.begin(), m_source.end(), tag_rex), end;
	for (; it != end; ++it)
	{
		auto const& tag_match = *it;

		std::string const& tag = tag_match[0];
		std::string name, index;
		size_t name_start_idx = 0, name_length = 0;
		size_t index_start_idx = 0, index_length = 0;
		{
			smatch m;
			if (regex_search(tag, m, splitter_rex))
			{
				name = m[1];
				name_start_idx = tag_match.position(0) + m.position(1);
				name_length = m[1].length();
				index = m[2];
				index_start_idx = tag_match.position(0) + m.position(2);
				index_length = m[2].length();
			}
		}
		//part0 is compulsory
		if (name.empty())
		{
			auto pos = util::find_line_char_by_offset(m_source, tag_match.position());
			QLOG_ERR("Q", "Syntax error at {}: bad tag - {}", pos, tag);
			return;
		}

		//validate part0
		{
			if (!regex_match(name, name_validator_rex))
			{
				auto pos = util::find_line_char_by_offset(m_source, name_start_idx);
				QLOG_ERR("Q", "Syntax error at {}: bad tag - {}", pos, tag);
				return;
			}
		}

		//validate and extract part1 ::::  not needed as the index is already extracted above
		
// 		if (!index.empty())
// 		{
// 			smatch m;
// 			if (!regex_match(index, m, index_validator_rex))
// 			{
// 				std::pair<size_t, size_t> pos = util::find_line_char_by_offset(m_source, index_start_idx);
// 				QLOG_ERR("Q", "Syntax error at {}: bad tag - {}", pos, tag);
// 				return;
// 			}
// 			index = m[1];
// 			index_start_idx = index_start_idx + m.position(1);
// 			index_length = m[1].length();
// 		}

		auto* t = new Expression;
		t->type = Token::Type::EXPRESSION;
		t->value = Literal(String(tag), tag_match.position(), tag_match.length());
		t->name = Literal(String(name), name_start_idx, name_length);
		t->index = Literal(String(index), index_start_idx, index_length);
		m_tokens.emplace_back(t);
	}

	//QLOGI("Tokenizing {} items: {}ms", mTokens.size(), (float)(System::inst().getUTime() - time) / 1000.f);
}

size_t Shader_Source::get_token_count() const
{
	return m_tokens.size();
}
Shader_Source::Token const& Shader_Source::get_token(size_t idx) const
{
	return *m_tokens[idx];
}
