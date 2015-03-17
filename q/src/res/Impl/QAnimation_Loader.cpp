#include "QStdAfx.h"
#include "res/Loader.h"
#include "res/Resource.h"
#include "anim/Animation.h"
#include "QAnimation_Loader.h"

using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace anim;

bool QAnimation_Loader::can_load_from_source(data::Source& source) const
{
	source.seek(0);

	uint8_t h[2];
	size_t s = source.read(h, sizeof(h));
	if (s != sizeof(h))
	{
		return false;
	}
	if (h[0] != 'q' || h[1] != 'a')
	{
		return false;
	}

	return true;
}

void QAnimation_Loader::load(Path const& /*path*/, data::Source& source, anim::Animation& animation) const
{
	animation.unload();
	source.seek(0);

	{
		uint8_t q, a;
		source >> q >> a;
		if (q != 'q' && a != 'a')
		{
			return;
		}
	}

	uint32_t version;
	source >> version;

	uint32_t fps;
	source >> fps;

	uint32_t channel_count;
	source >> channel_count;

	std::vector<Animation::Header> headers;
	std::vector<Animation::Float> float_key_frames;
	std::vector<Animation::Vec2> vec2_key_frames;
	std::vector<Animation::Vec3> vec3_key_frames;
	std::vector<Animation::Quat> quat_key_frames;

	for (uint32_t i = 0; i < channel_count; i++)
	{
		String chPath;
		source >> chPath;

		uint8_t type;
		source >> type;

		uint32_t count;
		source >> count;

		if (type == 0) //float
		{
			headers.resize(count);
			float_key_frames.resize(count);
			for (uint32_t k = 0; k < count; k++)
			{
				headers[k].step = false;
				source >> headers[k].time;
				source >> float_key_frames[k].data;
			}
			int idx = animation.add_channel(Path(chPath), headers);
			if (idx >= 0)
			{
				animation.set_channel_data(idx, float_key_frames);
			}
		}
		else if (type == 1) //vec2
		{
			headers.resize(count);
			vec2_key_frames.resize(count);
			for (uint32_t k = 0; k < count; k++)
			{
				headers[k].step = false;
				source >> headers[k].time;
				source >> vec2_key_frames[k].data;
			}
			int idx = animation.add_channel(Path(chPath), headers);
			if (idx >= 0)
			{
				animation.set_channel_data(idx, vec2_key_frames);
			}
		}
		else if (type == 2) //vec3
		{
			headers.resize(count);
			vec3_key_frames.resize(count);
			for (uint32_t k = 0; k < count; k++)
			{
				headers[k].step = false;
				source >> headers[k].time;
				source >> vec3_key_frames[k].data;
			}
			int idx = animation.add_channel(Path(chPath), headers);
			if (idx >= 0)
			{
				animation.set_channel_data(idx, vec3_key_frames);
			}
		}
		else if (type == 3) //quat
		{
			headers.resize(count);
			quat_key_frames.resize(count);
			for (uint32_t k = 0; k < count; k++)
			{
				headers[k].step = false;
				source >> headers[k].time;
				source >> quat_key_frames[k].data;
			}
			int idx = animation.add_channel(Path(chPath), headers);
			if (idx >= 0)
			{
				animation.set_channel_data(idx, quat_key_frames);
			}
		}
	}

	//animation.set_is_valid(true);
}
