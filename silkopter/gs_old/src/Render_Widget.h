#pragma once

class QGLFunctions;

class Render_Widget : public QWidget
{
public:
	enum class Flag : uint8_t
	{
		DEPTH_ACCESS = 1 << 0
	};
	typedef q::util::Flag_Set<Flag, uint8_t> Flags;

	Render_Widget(QWidget* parent = 0);
	Render_Widget(Flags flags, QWidget* parent = 0);

	void init();
	bool is_initialized() { return m_is_initialized; }

	~Render_Widget();

	Flags get_flags() const;
	void set_flags(Flags flags);

	void begin_rendering();
	void end_rendering();

	float get_depth_at(const math::vec2u32& coords);

private:
	void paintEvent(QPaintEvent* event);

	bool m_is_initialized = false;
	Flags m_flags;

	struct Buffer
	{
		uint32_t color_pbo = 0;
		uint32_t depth_pbo = 0;
		q::video::Render_Target_ptr framebuffer;
		std::vector<uint8_t> color_data;
		std::vector<float> depth_data;
		bool color_is_resolved = false;
		bool depth_is_resolved = false;
		math::vec2u32 size;
	};

	void resolve_color_buffer(Buffer& buffer);
	void resolve_depth_buffer(Buffer& buffer);
	void finish_buffer_transfer(Buffer& buffer);

	std::array<Buffer, 2> m_buffers;
    std::atomic<size_t> m_read_buffer_idx{0};
    std::atomic<size_t> m_write_buffer_idx{0};
	bool m_buffers_full;

	std::vector<uint8_t> m_temp_row_data;
};

