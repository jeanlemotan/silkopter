#include "QStdAfx.h"
#include "res/Resource.h"
#include "anim/Pose.h"
#include "anim/Animation_Node.h"
#include "anim/Animation.h"

using namespace q;
using namespace anim;

static const uint32_t TIME_PRECISION				= 3;

Animation::Animation(Path const& path)
	: Resource(path)
	, m_duration(0)
{
	m_channels.reserve(16);
	m_version = (int)(size_t)this;
}
Animation::~Animation()
{
	delete_all_channels_data();
}

void Animation::delete_channel_data(size_t channel_idx)
{
	auto& ch = m_channels[channel_idx];
	switch (ch.type)
	{
	case Channel_Type::FLOAT: delete ch.float_data; break;
	case Channel_Type::QUANTIZED_FLOAT: delete ch.quantized_float_data; break;
	case Channel_Type::VEC2: delete ch.vec2_data; break;
	case Channel_Type::QUANTIZED_VEC2: delete ch.quantized_vec2_data; break;
	case Channel_Type::VEC3: delete ch.vec3_data; break;
	case Channel_Type::QUANTIZED_VEC3: delete ch.quantized_vec3_data; break;
	case Channel_Type::QUAT: delete ch.quat_data; break;
	case Channel_Type::QUANTIZED_QUAT: delete ch.quantized_quat_data; break;
    case Channel_Type::NONE: break;
	}
	ch.float_data = 0;
	ch.type = Channel_Type::NONE;
}

void Animation::delete_all_channels_data()
{
	for (size_t i = 0; i < m_channels.size(); i++)
	{
		delete_channel_data(i);
	}
	m_channels.clear();
}

uint32_t Animation::get_version() const
{
	return m_version;
}

Pose::Data Animation::get_channel_data(uint32_t channel_idx, uint32_t kf_idx) const
{
	Pose::Data data;
	static const float inv32767 = 1.f / 32767.f;
	auto const& ch = m_channels[channel_idx];
	switch (ch.type)
	{
	case Channel_Type::FLOAT:
		data.data.x = (*ch.float_data)[kf_idx].data;
		break;
	case Channel_Type::QUANTIZED_FLOAT:
		data.data.x = (float)(*ch.quantized_float_data)[kf_idx].quantized_data * inv32767;
		break;
	case Channel_Type::VEC2:
		(math::vec2f&)data.data = (*ch.vec2_data)[kf_idx].data;
		break;
	case Channel_Type::QUANTIZED_VEC2:
		(math::vec2f&)data.data = math::vec2f(*(math::vec2s16*)(*ch.quantized_vec2_data)[kf_idx].quantized_data) * inv32767;
		break;
	case Channel_Type::VEC3:
		(math::vec3f&)data.data = (*ch.vec3_data)[kf_idx].data;
		break;
	case Channel_Type::QUANTIZED_VEC3:
		(math::vec3f&)data.data = math::vec3f(*(math::vec3s16*)(*ch.quantized_vec3_data)[kf_idx].quantized_data) * inv32767;
		break;
	case Channel_Type::QUAT:
		(math::vec4f&)data.data = *(math::vec4f*)&(*ch.quat_data)[kf_idx].data;
		break;
	case Channel_Type::QUANTIZED_QUAT:
		(math::vec4f&)data.data = math::vec4f(*(math::vec4s16*)(*ch.quantized_quat_data)[kf_idx].quantized_data) * inv32767;
		break;
	default:
		QASSERT(0);
	}
	return data;
}
Pose::Data Animation::get_interpolated_channel_data(uint32_t channel_idx, uint32_t kf_idx, float ratio) const
{
	Pose::Data data;
	static const float inv32767 = 1.f / 32767.f;
	auto const& ch = m_channels[channel_idx];
	switch (ch.type)
	{
	case Channel_Type::FLOAT:
		{
			data.data.x = math::lerp(
				(*ch.float_data)[kf_idx].data,
				(*ch.float_data)[kf_idx + 1].data,
				ratio);
		}
		break;
	case Channel_Type::QUANTIZED_FLOAT:
		{
			data.data.x = math::lerp(
				(float)(*ch.quantized_float_data)[kf_idx].quantized_data * inv32767,
				(float)(*ch.quantized_float_data)[kf_idx + 1].quantized_data * inv32767,
				ratio);
		}
		break;
	case Channel_Type::VEC2:
		{
			(math::vec2f&)data.data = math::lerp(
				(*ch.vec2_data)[kf_idx].data,
				(*ch.vec2_data)[kf_idx + 1].data,
				ratio);
		}
		break;
	case Channel_Type::QUANTIZED_VEC2:
		{
			(math::vec2f&)data.data = math::lerp(
				math::vec2f(*(math::vec2s16*)(*ch.quantized_vec2_data)[kf_idx].quantized_data) * inv32767,
				math::vec2f(*(math::vec2s16*)(*ch.quantized_vec2_data)[kf_idx + 1].quantized_data) * inv32767,
				ratio);
		}
		break;
	case Channel_Type::VEC3:
		{
			(math::vec3f&)data.data = math::lerp(
				(*ch.vec3_data)[kf_idx].data,
				(*ch.vec3_data)[kf_idx + 1].data,
				ratio);
		}
		break;
	case Channel_Type::QUANTIZED_VEC3:
		{
			(math::vec3f&)data.data = math::lerp(
				math::vec3f(*(math::vec3s16*)(*ch.quantized_vec3_data)[kf_idx].quantized_data) * inv32767,
				math::vec3f(*(math::vec3s16*)(*ch.quantized_vec3_data)[kf_idx + 1].quantized_data) * inv32767,
				ratio);
		}
		break;
	case Channel_Type::QUAT:
		{
			(math::quatf&)data.data = math::nlerp(
				*(math::quatf*)&(*ch.quat_data)[kf_idx].data,
				*(math::quatf*)&(*ch.quat_data)[kf_idx + 1].data,
				ratio);
		}
		break;
	case Channel_Type::QUANTIZED_QUAT:
		{
			math::vec4s16 const& v0 = *(math::vec4s16*)(*ch.quantized_quat_data)[kf_idx].quantized_data;
			math::quatf q0((float)v0.x * inv32767, (float)v0.y * inv32767, (float)v0.z * inv32767, (float)v0.w * inv32767);
			math::vec4s16 const& v1 = *(math::vec4s16*)(*ch.quantized_quat_data)[kf_idx + 1].quantized_data;
			math::quatf q1((float)v1.x * inv32767, (float)v1.y * inv32767, (float)v1.z * inv32767, (float)v1.w * inv32767);
			(math::quatf&)data.data = math::nlerp(q0, q1, ratio);
		}
		break;
	default:
		QASSERT(0);
	}
	return data;
}

Pose const& Animation::get_pose() const
{
	if (!is_valid())
	{
		m_pose.clear();
		return m_pose;
	}

	if (m_pose.is_empty())
	{
		for (size_t i = 0; i < m_channels.size(); i++)
		{
			int idx = m_pose.add_channel(m_channels[i].path);
			m_pose.set_cache(idx, 0);
			m_pose.set_data(idx, get_channel_data(i, 0));
			m_pose.set_id(idx, i);
		}
	}

	return m_pose;
}
bool Animation::update_pose(Pose& pose, Duration time) const
{
	if (!is_valid())
	{
		return false;
	}

	float duration_s = Seconds(m_duration).count();
	float time_s = Seconds(time).count();

	time_s = math::clamp(time_s, 0.f, duration_s);
	uint32_t time_ms = (uint32_t)(time_s * 1000.f);

	bool res = false;
	for (size_t i = 0; i < pose.get_count(); i++)
	{
		res |= update_channel(time_ms, i, pose);
	}

	return res;
}

uint32_t Animation::find_start_key_frame(uint32_t time_ms, std::vector<Quantized_Header> const& headers, uint32_t& cache) const
{
	uint32_t count = headers.size();
	uint32_t crt_kf = cache;

	uint32_t ctime = time_ms >> TIME_PRECISION;

	//uint16_t* keyFramesTime = kfs->m_keyFramesTime.get();
	if (crt_kf >= count  ||  ctime < (headers[crt_kf].quantized_time))
	{
		//binary search for the time
		uint32_t start = 0;
		uint32_t end = count;
		uint32_t mid = 0;
		while (start < end)
		{
			mid = start + ((end - start) >> 1);
			if (ctime < (headers[mid].quantized_time))
			{
				end = mid;
			}
			else if (mid+1 < end  &&  ctime > (headers[mid+1].quantized_time))
			{
				start = mid + 1;
			}
			else
			{
				break;
			}
		}

		crt_kf = mid;
	}

	//advance if time passed the next kf
	for (uint32_t count2 = count - 1; crt_kf < count2  &&  ctime >= (headers[crt_kf + 1].quantized_time); crt_kf++)
	{;}

	//cache the current position keyframe index
	cache = crt_kf;
	return (uint32_t)crt_kf;
}

bool Animation::update_channel(uint32_t time_ms, uint32_t pose_channel_idx, Pose& pose) const
{
	uint32_t channel_idx = pose.get_id(pose_channel_idx);
	if (channel_idx >= m_channels.size() || m_channels[channel_idx].type == Channel_Type::NONE)
	{
		return false;
	}

	auto const& ch = m_channels[channel_idx];
	uint32_t cache = pose.get_cache(pose_channel_idx);

	//special case for one-kf channels
	if (ch.headers.size() == 1)
	{
		if (cache != 1)
		{
			pose.set_data(pose_channel_idx, get_channel_data(channel_idx, 0));
			pose.set_cache(pose_channel_idx, 1);
		}
		return true;
	}

	int start_kf = find_start_key_frame(time_ms, ch.headers, cache);
	pose.set_cache(pose_channel_idx, cache);

	uint32_t start_time = ch.headers[start_kf].quantized_time << TIME_PRECISION;

	uint32_t end_kf = start_kf + 1;
	if (time_ms < start_time || end_kf >= ch.headers.size() || ch.headers[start_kf].step != 0)
	{
		pose.set_data(pose_channel_idx, get_channel_data(channel_idx, start_kf));
		return true;
	}

	uint32_t end_time = ch.headers[end_kf].quantized_time << TIME_PRECISION;
	QASSERT(time_ms >= start_time);
	QASSERT(end_time > start_time);
	uint32_t diff_time = time_ms - start_time;
	uint32_t total_time = end_time - start_time;
	QASSERT(diff_time < total_time);
	float ratio = (float)diff_time / (float)total_time;

	pose.set_data(pose_channel_idx, get_interpolated_channel_data(channel_idx, start_kf, ratio));
	return true;
}

Duration Animation::get_duration() const
{
	return m_duration;
}
void Animation::unload()
{
	delete_all_channels_data();
	m_duration = Duration(0);
}
bool Animation::is_valid() const
{
	return !m_channels.empty();
}
int Animation::add_channel(Path const& path, std::vector<Header> const& headers)
{
	int channel_idx = find_channel_idx_by_path(path);
	if (channel_idx >= 0)
	{
        QLOGE("Duplicated channel '{}'. Ignored.", path);
		return -1;
	}

	std::vector<Quantized_Header> qheaders;
	qheaders.resize(headers.size());
	for (uint32_t i = 0; i < headers.size(); i++)
	{
		float time = headers[i].time.count();
		bool step = headers[i].step;

		float t = time * (1000.f / 8.f);
		if (t > 32767.f)
		{
            QLOGE("Animation exceeding the max duration of 4.36 minutes. Clamped.");
			qheaders.resize(i);
			break;
		}
		uint32_t qtime = (uint32_t)t;
		Quantized_Header qh;
		qh.quantized_time = qtime;
		qh.step = step ? 1 : 0;
		qheaders[i] = qh;
	}

	return _add_channel(path, qheaders);
}
int Animation::add_channel(Path const& path, std::vector<Quantized_Header> const& headers)
{
	int channel_idx = find_channel_idx_by_path(path);
	if (channel_idx >= 0)
	{
        QLOGE("Duplicated channel '{}'. Ignored.", path);
		return -1;
	}

	return _add_channel(path, headers);
}

int Animation::_add_channel(Path const& path, std::vector<Quantized_Header> const& headers)
{
	m_channels.resize(m_channels.size() + 1);

	auto& data = m_channels.back();
	data.path = path;
	data.headers = headers;

	uint32_t maxTime = 0;
	for (auto const& h: headers)
	{
		maxTime = math::max(maxTime, (uint32_t)h.quantized_time << TIME_PRECISION);
	}
	m_duration = math::max(m_duration, Duration(std::chrono::milliseconds(maxTime)));
	m_version++;

	return m_channels.size() - 1;
}

int Animation::find_channel_idx_by_path(Path const& path) const
{
// 	auto it = std::find_if(mChannels.begin(), mChannels.end(), [&](ChannelData const& ch) { return ch.path == path; });
// 	if (it != mChannels.end())
// 	{
// 		return std::distance(mChannels.begin(), it);
// 	}

	for (size_t i = 0; i < m_channels.size(); i++)
	{
		if (m_channels[i].path == path)
		{
			return i;
		}
	}
	return -1;
}

void Animation::set_channel_data(uint32_t channel_idx, std::vector<Float> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::FLOAT;
	m_channels[channel_idx].float_data = new std::vector<Float>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Quantized_Float> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::QUANTIZED_FLOAT;
	m_channels[channel_idx].quantized_float_data = new std::vector<Quantized_Float>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Vec2> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::VEC2;
	m_channels[channel_idx].vec2_data = new std::vector<Vec2>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Quantized_Vec2> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::QUANTIZED_VEC2;
	m_channels[channel_idx].quantized_vec2_data = new std::vector<Quantized_Vec2>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Vec3> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::VEC3;
	m_channels[channel_idx].vec3_data = new std::vector<Vec3>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Quantized_Vec3> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::QUANTIZED_VEC3;
	m_channels[channel_idx].quantized_vec3_data = new std::vector<Quantized_Vec3>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Quat> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::QUAT;
	m_channels[channel_idx].quat_data = new std::vector<Quat>(keyFrames);
}
void Animation::set_channel_data(uint32_t channel_idx, std::vector<Quantized_Quat> const& keyFrames)
{
	delete_channel_data(channel_idx);
	m_channels[channel_idx].type = Channel_Type::QUANTIZED_QUAT;
	m_channels[channel_idx].quantized_quat_data = new std::vector<Quantized_Quat>(keyFrames);
}

