#pragma once

namespace q
{
namespace anim
{


	class Animation : public res::Resource, public Animation_Node
	{
		DEFINE_RTTI_CLASS2(q::anim::Animation, Animation_Node, res::Resource);
	public:
		Animation(Path const& path);
		virtual ~Animation();

		//Node impl
		virtual uint32_t		get_version() const;
		virtual Pose const&		get_pose() const;
		virtual bool			update_pose(Pose& pose, Duration time) const;
		virtual Duration		get_duration() const;

		//Resource impl
		virtual void		unload();
		virtual bool		is_valid() const;

		//Self
		struct Header
		{
			Seconds	time;
			bool	step;
		};
		struct Quantized_Header
		{
			uint32_t	quantized_time:15; //time/8
			uint32_t	step:1; //0 - linear interp, 1 - step
		};

		struct Float
		{
			float data;
		};
		struct Quantized_Float
		{
			uint16_t	quantized_data; //clamp(data, -1, 1) / 32767
		};
		struct Vec2
		{
			math::vec2f data;
		};
		struct Quantized_Vec2
		{
			int16_t	quantized_data[2]; //clamp(data, -1, 1) / 32767
		};
		struct Vec3
		{
			math::vec3f data;
		};
		struct Quantized_Vec3
		{
			int16_t	quantized_data[3]; //clamp(data, -1, 1) / 32767
		};
		struct Quat
		{
			math::quatf data;
		};
		struct Quantized_Quat
		{
			int16_t	quantized_data[4]; //clamp(data, -1, 1) / 32767
		};

		int		add_channel(Path const& path, std::vector<Header> const& headers);
		int		add_channel(Path const& path, std::vector<Quantized_Header> const& headers);
		void	set_channel_data(uint32_t idx, std::vector<Float> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Quantized_Float> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Vec2> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Quantized_Vec2> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Vec3> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Quantized_Vec3> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Quat> const& keyFrames);
		void	set_channel_data(uint32_t idx, std::vector<Quantized_Quat> const& keyFrames);
		int		find_channel_idx_by_path(Path const& path) const;

	private:
		int				_add_channel(Path const& path, std::vector<Quantized_Header> const& headers);
		bool			update_channel(uint32_t time_ms, uint32_t pose_channel_idx, Pose& pose) const;
		uint32_t		find_start_key_frame(uint32_t time_ms, std::vector<Quantized_Header> const& headers, uint32_t& cache) const;
		void			delete_channel_data(uint32_t channel_idx);
		void			delete_all_channels_data();
		Pose::Data		get_channel_data(uint32_t channel_idx, uint32_t kf_idx) const;
		Pose::Data		get_interpolated_channel_data(uint32_t channel_idx, uint32_t kf_idx, float ratio) const;

		enum class Channel_Type : uint8_t
		{
			NONE,
			FLOAT,
			VEC2,
			VEC3,
			QUAT,
			QUANTIZED_FLOAT,
			QUANTIZED_VEC2,
			QUANTIZED_VEC3,
			QUANTIZED_QUAT,
		};

		struct Channel_Data
		{
			Channel_Data() : type(Channel_Type::NONE), float_data(0) {}
			Path path;
			std::vector<Quantized_Header> headers;
			Channel_Type type;
			union 
			{
				std::vector<Float>* float_data;
				std::vector<Quantized_Float>* quantized_float_data;
				std::vector<Vec2>* vec2_data;
				std::vector<Quantized_Vec2>* quantized_vec2_data;
				std::vector<Vec3>* vec3_data;
				std::vector<Quantized_Vec3>* quantized_vec3_data;
				std::vector<Quat>* quat_data;
				std::vector<Quantized_Quat>* quantized_quat_data;
			};
		};

		std::vector<Channel_Data> m_channels;
		mutable Pose m_pose;
		Duration m_duration;
		uint32_t	m_version;
	};

}
}