#pragma once

namespace q
{
namespace video
{

	class Shader_Source
	{
	public:
		struct Literal
		{
			Literal() : start(0), length(0) {}
			Literal(String const& value, size_t start, size_t length) : value(value), start(start), length(length) {}
			String value;
			size_t start;
			size_t length;
		};

		struct Token
		{
			virtual ~Token() {}
			enum class Type : uint8_t
			{
				EXPRESSION,
				CONDITIONAL,
			};
			Type	type;
			Literal value;
		};

		struct Expression : public Token
		{
			//syntax name[index] where:
			//  name := a.b.c
			//	index := number | variable

			Literal name; 
			Literal index;
		};

		struct Conditional : public Token
		{
		};

		Shader_Source();
		Shader_Source(String const& src);
		Shader_Source(Shader_Source const& other);
		Shader_Source(Shader_Source&& other);
		Shader_Source& operator=(Shader_Source const& other);
		bool operator==(Shader_Source const& other) const;
		bool operator!=(Shader_Source const& other) const;

		size_t			get_token_count() const;
		Token const&	get_token(size_t idx) const;

		String			get_source() const;

	private:
		void process();

		std::string m_source;
		std::vector<std::shared_ptr<Token>> m_tokens;
	};

	//clone semantics for Tokens
// 	inline ShaderSource::Token* new_clone(const ShaderSource::Token& r)
// 	{
// 		switch (r.type)
// 		{
// 		case ShaderSource::Token::Type::EXPRESSION: 
// 			return new ShaderSource::Expression(*reinterpret_cast<ShaderSource::Expression const*>(&r));
// 		case ShaderSource::Token::Type::CONDITIONAL:
// 			return new ShaderSource::Conditional(*reinterpret_cast<ShaderSource::Conditional const*>(&r));
// 		}
// 		QASSERT(0);
// 		return 0;
// 	}


}
}
