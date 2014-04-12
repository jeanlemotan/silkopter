#pragma once

namespace q
{
namespace video
{

	class Render_State
	{
		friend struct std::hash<q::video::Render_State>;
	protected:
		enum class Flag : uint8_t
		{
			CULL			= 1 << 0,
			DEPTH_TEST		= 1 << 1,
			DEPTH_WRITE		= 1 << 2,
			STENCIL			= 1 << 3
		};
		typedef util::Flag_Set<Flag, uint8_t> Flags;

	public:
		struct Blend_Formula
		{
			enum class Preset : uint8_t
			{
				SOLID,
				ALPHA,
				ADDITIVE,
				MULTIPLY
			};

			enum class Operation : uint8_t
			{
				ADD,
				SUBSTRACT
			};

			enum class Factor : uint8_t
			{
				ZERO,
				ONE,
				SRC_COLOR,
				INV_SRC_COLOR,
				DST_COLOR,
				INV_DST_COLOR,
				SRC_ALPHA,
				INV_SRC_ALPHA,
				DST_ALPHA,
				INV_DST_ALPHA,
				SRC_ALPHA_SATURATED
			};
			Blend_Formula();
			Blend_Formula(Preset preset);
			Blend_Formula(Factor src_color, Factor src_alpha, Factor dst_color, Factor dst_alpha);
			Blend_Formula(Factor src, Factor dst);
			Blend_Formula(Factor src_color, Factor src_alpha, Factor dst_color, Factor dst_alpha, Operation operation);
			Blend_Formula(Factor src, Factor dst, Operation operation);

			boost::optional<Preset> getPreset() const;

			bool operator==(Blend_Formula const& other) const;
			bool operator!=(Blend_Formula const& other) const;

			Factor src_color;
			Factor src_alpha;
			Factor dst_color;
			Factor dst_alpha;
			Operation operation;
		};

		enum class Compare_Func : uint8_t
		{
			LESS,
			LEQUAL,
			EQUAL,
			GEQUAL,
			GREATER,
			NOT_EQUAL,
			ALWAYS,
			NEVER
		};

		enum class Stencil_Op : uint8_t
		{
			KEEP,
			CLEAR,
			REPLACE,
			INVERT,
			INCREMENT,
			DECREMENT,
			INCREMENT_WRAP,
			DECREMENT_WRAP
		};

		enum class Cull_Face : uint8_t
		{
			BACK,
			FRONT
		};

		enum class Winding : uint8_t
		{
			WINDING_CW,
			WINDING_CCW
		};

		Render_State();
		Render_State(Render_State const& other);
		~Render_State();

		Render_State&			operator=(Render_State const& other);
		bool					operator==(Render_State const& other) const;
		DEFINE_EQ_OPERATORS(Render_State);

		void					set_blend_formula(Blend_Formula formula);
		Blend_Formula			get_blend_formula() const;

		void					set_culling(bool enabled);
		bool					get_culling() const;

		void					set_cull_face(Cull_Face face);
		Cull_Face				get_cull_face() const;

		void					set_winding(Winding wind);
		Winding					get_winding() const;

		void					set_depth_test(bool enabled);
		bool					get_depth_test() const;

		void					set_depth_func(Compare_Func func);
		Compare_Func			get_depth_func() const;

		void					set_depth_write(bool enabled);
		bool					get_depth_write() const;

		// Stencil
		void					set_stencil(bool enabled);
		bool					get_stencil() const;

		void					set_stencil_params(Compare_Func func, int32_t ref = 1, uint32_t mask = ~0);
		Compare_Func			get_stencil_func() const;
		int32_t						get_stencil_ref()  const;
		uint32_t				get_stencil_mask() const;

		void					set_stencil_ops(Stencil_Op s_fail, Stencil_Op z_fail, Stencil_Op pass);
		void					set_stencil_ops(Cull_Face face, Stencil_Op s_fail, Stencil_Op z_fail, Stencil_Op pass);

		Stencil_Op				get_stencil_front_pass_op()  const;
		Stencil_Op				get_stencil_front_s_fail_op() const;
		Stencil_Op				get_stencil_front_z_fail_op() const;
		Stencil_Op				get_stencil_back_pass_op()   const;
		Stencil_Op				get_stencil_back_s_fail_op()  const;
		Stencil_Op				get_stencil_back_z_fail_op()  const;

	protected:
		Flags					get_flags() const;

		Blend_Formula::Factor	parse_blend_factor(String const& str, Blend_Formula::Factor def);
		Compare_Func			parse_compare_func(String const& str, Compare_Func def);
		Stencil_Op				parse_stencil_op(String const& str, Stencil_Op def);

		Flags					m_flags;

		Blend_Formula			m_blend_formula;

		Cull_Face				m_cull_face;
		Winding					m_winding;

		Compare_Func			m_depth_func;

		// Stencil
		Compare_Func			m_stencil_func;
		int32_t						m_stencil_ref;
		uint32_t				m_stencil_mask;

		Stencil_Op				m_front_stencil_pass_op;
		Stencil_Op				m_front_stencil_s_fail_op;
		Stencil_Op				m_front_stencil_z_fail_op;

		Stencil_Op				m_back_stencil_pass_op;
		Stencil_Op				m_back_stencil_s_fail_op;
		Stencil_Op				m_back_stencil_z_fail_op;
	};

	inline Render_State::Blend_Formula::Blend_Formula()
		: src_color(Factor::ONE)
		, src_alpha(Factor::ONE)
		, dst_color(Factor::ZERO)
		, dst_alpha(Factor::ZERO)
		, operation(Operation::ADD)	{}
	inline Render_State::Blend_Formula::Blend_Formula(Preset preset)
	{
		switch (preset)
		{
		case Preset::SOLID: src_color = src_alpha = Factor::ONE; dst_color = dst_alpha = Factor::ZERO; break;
		case Preset::ALPHA: src_color = src_alpha = Factor::SRC_ALPHA; dst_color = dst_alpha = Factor::INV_SRC_ALPHA; break;
		case Preset::ADDITIVE: src_color = src_alpha = Factor::ONE; dst_color = dst_alpha = Factor::ONE; break;
		case Preset::MULTIPLY: src_color = src_alpha = Factor::DST_COLOR; dst_color = dst_alpha = Factor::ZERO; break;
		default: QASSERT(0); break;
		}
		operation = Operation::ADD;
	}
	inline Render_State::Blend_Formula::Blend_Formula(Factor src_color, Factor src_alpha, Factor dst_color, Factor dst_alpha)
		: src_color(src_color)
		, src_alpha(src_alpha)
		, dst_color(dst_color)
		, dst_alpha(dst_alpha) 
		, operation(Operation::ADD) {}
	inline Render_State::Blend_Formula::Blend_Formula(Factor src, Factor dst)
		: src_color(src)
		, src_alpha(src)
		, dst_color(dst)
		, dst_alpha(dst) 
		, operation(Operation::ADD) {}
	inline Render_State::Blend_Formula::Blend_Formula(Factor src_color, Factor src_alpha, Factor dst_color, Factor dst_alpha, Operation operation)
		: src_color(src_color)
		, src_alpha(src_alpha)
		, dst_color(dst_color)
		, dst_alpha(dst_alpha) 
		, operation(operation) {}
	inline Render_State::Blend_Formula::Blend_Formula(Factor src, Factor dst, Operation operation)
		: src_color(src)
		, src_alpha(src)
		, dst_color(dst)
		, dst_alpha(dst) 
		, operation(operation) {}
	inline boost::optional<Render_State::Blend_Formula::Preset> Render_State::Blend_Formula::getPreset() const
	{
		if (src_color == src_alpha && src_color == Factor::ONE && dst_color == dst_alpha && dst_color == Factor::ZERO)
		{
			return Preset::SOLID;
		}
		if (src_color == src_alpha && src_color == Factor::SRC_ALPHA && dst_color == dst_alpha && dst_color == Factor::INV_SRC_ALPHA)
		{
			return Preset::ALPHA;
		}
		if (src_color == src_alpha && src_color == Factor::ONE && dst_color == dst_alpha && dst_color == Factor::ONE)
		{
			return Preset::ADDITIVE;
		}
		if ((src_color == src_alpha && src_color == Factor::DST_COLOR && dst_color == dst_alpha && dst_color == Factor::ZERO) ||
			(src_color == src_alpha && src_color == Factor::ZERO && dst_color == dst_alpha && dst_color == Factor::SRC_COLOR))
		{
			return Preset::MULTIPLY;
		}
		return boost::optional<Preset>();
	}
	inline bool Render_State::Blend_Formula::operator==(Blend_Formula const& other) const
	{
		return src_color == other.src_color &&
			src_alpha == other.src_alpha &&
			dst_color == other.dst_color &&
			dst_alpha == other.dst_alpha &&
			operation == other.operation;
	}
	inline bool Render_State::Blend_Formula::operator!=(Blend_Formula const& other) const
	{
		return !(operator==(other));
	}



	inline Render_State::Render_State()
		: m_flags(Flag::CULL, Flag::DEPTH_TEST, Flag::DEPTH_WRITE)
		, m_blend_formula(Blend_Formula(Blend_Formula::Preset::SOLID))
		, m_cull_face(Cull_Face::BACK)
		, m_winding(Winding::WINDING_CCW)
		, m_depth_func(Compare_Func::LEQUAL)
		, m_stencil_func(Compare_Func::LEQUAL)
		, m_stencil_ref(1)
		, m_stencil_mask(~0u)
		, m_front_stencil_pass_op(Stencil_Op::KEEP)
		, m_front_stencil_s_fail_op(Stencil_Op::KEEP)
		, m_front_stencil_z_fail_op(Stencil_Op::KEEP)
		, m_back_stencil_pass_op(Stencil_Op::KEEP)
		, m_back_stencil_s_fail_op(Stencil_Op::KEEP)
		, m_back_stencil_z_fail_op(Stencil_Op::KEEP)

	{
	}
	inline Render_State::~Render_State()
	{

	}
	inline Render_State::Render_State(Render_State const& other)
	{
		*this = other;
	}
	inline Render_State& Render_State::operator=(Render_State const& other)
	{
		m_flags = other.m_flags;
		m_blend_formula = other.m_blend_formula;
		m_cull_face = other.m_cull_face;
		m_winding = other.m_winding;
		m_depth_func = other.m_depth_func;
		m_stencil_func = other.m_stencil_func;
		m_stencil_ref = other.m_stencil_ref;
		m_stencil_mask = other.m_stencil_mask;
		m_front_stencil_pass_op = other.m_front_stencil_pass_op;
		m_front_stencil_s_fail_op = other.m_front_stencil_s_fail_op;
		m_front_stencil_z_fail_op = other.m_front_stencil_z_fail_op;
		m_back_stencil_pass_op = other.m_back_stencil_pass_op;
		m_back_stencil_s_fail_op = other.m_back_stencil_s_fail_op;
		m_back_stencil_z_fail_op = other.m_back_stencil_z_fail_op;

		return *this;
	}
	inline bool Render_State::operator==(Render_State const& other) const
	{
		if (m_flags != other.m_flags)
		{
			return false;
		}

		if (get_blend_formula() != other.get_blend_formula() ||
			get_cull_face() != other.get_cull_face() ||
			get_winding() != other.get_winding() ||
			get_depth_func() != other.get_depth_func() ||
			get_stencil() != other.get_stencil() ||
			get_stencil_func() != other.get_stencil_func() ||
			get_stencil_ref() != other.get_stencil_ref() ||
			get_stencil_mask() != other.get_stencil_mask() ||
			get_stencil_front_pass_op() != other.get_stencil_front_pass_op() ||
			get_stencil_front_s_fail_op() != other.get_stencil_front_s_fail_op() ||
			get_stencil_front_z_fail_op() != other.get_stencil_front_z_fail_op() ||
			get_stencil_back_pass_op() != other.get_stencil_back_pass_op() ||
			get_stencil_back_s_fail_op() != other.get_stencil_back_s_fail_op() ||
			get_stencil_back_z_fail_op() != other.get_stencil_back_z_fail_op()
			)
		{
			return false;
		}

		return true;
	}
	inline Render_State::Flags Render_State::get_flags() const
	{
		return m_flags;
	}
	inline void Render_State::set_blend_formula(Blend_Formula formula)
	{
		m_blend_formula = formula;
	}
	inline Render_State::Blend_Formula Render_State::get_blend_formula() const
	{
		return m_blend_formula;
	}
	inline void Render_State::set_culling(bool enabled)
	{
		m_flags.set(Flag::CULL, enabled);
	}
	inline bool Render_State::get_culling() const
	{
		return m_flags[Flag::CULL];
	}
	inline void Render_State::set_cull_face(Cull_Face face)
	{
		m_cull_face = face;
	}
	inline Render_State::Cull_Face Render_State::get_cull_face() const
	{
		return m_cull_face;
	}
	inline void Render_State::set_winding(Winding wind)
	{
		m_winding = wind;
	}
	inline Render_State::Winding Render_State::get_winding() const
	{
		return m_winding;
	}
	inline void Render_State::set_depth_test(bool enabled)
	{
		m_flags.set(Flag::DEPTH_TEST, enabled);
	}
	inline bool Render_State::get_depth_test() const
	{
		return m_flags[Flag::DEPTH_TEST];
	}
	inline void Render_State::set_depth_func(Compare_Func func)
	{
		m_depth_func = func;
	}
	inline Render_State::Compare_Func Render_State::get_depth_func() const
	{
		return m_depth_func;
	}
	inline void Render_State::set_depth_write(bool enabled)
	{
		m_flags.set(Flag::DEPTH_WRITE, enabled);
	}
	inline bool Render_State::get_depth_write() const
	{
		return m_flags[Flag::DEPTH_WRITE];
	}
	inline void Render_State::set_stencil(bool enabled)
	{
		m_flags.set(Flag::STENCIL, enabled);
	}
	inline bool	Render_State::get_stencil() const
	{
		return m_flags[Flag::STENCIL];
	}
	inline void	Render_State::set_stencil_params(Compare_Func func, int32_t ref, uint32_t mask)
	{
		m_stencil_func = func;
		m_stencil_ref  = ref;
		m_stencil_mask = mask;
	}
	inline Render_State::Compare_Func	Render_State::get_stencil_func() const
	{
		return m_stencil_func;
	}
	inline int32_t Render_State::get_stencil_ref() const
	{
		return m_stencil_ref;
	}
	inline uint32_t Render_State::get_stencil_mask() const
	{
		return m_stencil_mask;
	}
	inline void Render_State::set_stencil_ops(Stencil_Op s_fail, Stencil_Op z_fail, Stencil_Op pass)
	{
		m_front_stencil_s_fail_op = m_back_stencil_s_fail_op = s_fail;
		m_front_stencil_z_fail_op = m_back_stencil_z_fail_op = z_fail;
		m_front_stencil_pass_op  = m_back_stencil_pass_op  = pass;
	}
	inline void Render_State::set_stencil_ops(Cull_Face face, Stencil_Op s_fail, Stencil_Op z_fail, Stencil_Op pass)
	{
		if (face == Cull_Face::FRONT)
		{
			m_front_stencil_s_fail_op = s_fail;
			m_front_stencil_z_fail_op = z_fail;
			m_front_stencil_pass_op  = pass;
		}
		else
		{
			m_back_stencil_s_fail_op = s_fail;
			m_back_stencil_z_fail_op = z_fail;
			m_back_stencil_pass_op  = pass;
		}
	}
	inline Render_State::Stencil_Op Render_State::get_stencil_front_pass_op()  const
	{
		return m_front_stencil_pass_op;
	}
	inline Render_State::Stencil_Op Render_State::get_stencil_front_s_fail_op() const
	{
		return m_front_stencil_s_fail_op;
	}
	inline Render_State::Stencil_Op Render_State::get_stencil_front_z_fail_op() const
	{
		return m_front_stencil_z_fail_op;
	}
	inline Render_State::Stencil_Op Render_State::get_stencil_back_pass_op()   const
	{
		return m_back_stencil_pass_op;
	}
	inline Render_State::Stencil_Op Render_State::get_stencil_back_s_fail_op()  const
	{
		return m_back_stencil_s_fail_op;
	}
	inline Render_State::Stencil_Op Render_State::get_stencil_back_z_fail_op()  const
	{
		return m_back_stencil_z_fail_op;
	}

}
}

namespace std 
{
	template <> 
	struct hash<q::video::Render_State>
	{
		size_t operator()(q::video::Render_State const& rs) const
		{
			size_t hash = 0;
			q::util::hash_combine(hash, (int)rs.m_flags);
			q::util::hash_combine(hash, (int)rs.m_blend_formula.src_color);
			q::util::hash_combine(hash, (int)rs.m_blend_formula.src_alpha);
			q::util::hash_combine(hash, (int)rs.m_blend_formula.dst_color);
			q::util::hash_combine(hash, (int)rs.m_blend_formula.dst_alpha);
			q::util::hash_combine(hash, (int)rs.m_blend_formula.operation);
			q::util::hash_combine(hash, (int)rs.m_cull_face);
			q::util::hash_combine(hash, (int)rs.m_winding);
			q::util::hash_combine(hash, (int)rs.m_depth_func);
			q::util::hash_combine(hash, (int)rs.m_stencil_func);
			q::util::hash_combine(hash, rs.m_stencil_ref);
			q::util::hash_combine(hash, rs.m_stencil_mask);
			q::util::hash_combine(hash, (int)rs.m_front_stencil_pass_op);
			q::util::hash_combine(hash, (int)rs.m_front_stencil_s_fail_op);
			q::util::hash_combine(hash, (int)rs.m_front_stencil_z_fail_op);
			q::util::hash_combine(hash, (int)rs.m_back_stencil_pass_op);
			q::util::hash_combine(hash, (int)rs.m_back_stencil_s_fail_op);
			q::util::hash_combine(hash, (int)rs.m_back_stencil_z_fail_op);
			return hash;
		}
	};
}

