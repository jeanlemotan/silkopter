#include "QStdAfx.h"
#include "Zup_Sprite_Loader.h"

#include "../../pugixml/src/pugixml.hpp"
#define PUGIXML_HEADER_ONLY
#include "../../pugixml/src/pugixml.cpp"


using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace draw;

bool ZupSpriteLoader::can_load_from_source(data::Source& source) const
{
	source.seek(0);

	uint32_t h[3];
	uint32_t s = source.read((uint8_t*)h, sizeof(h));
	if (s != sizeof(h))
	{
		return false;
	}
	if (h[0] == 8 && h[1] == 2 && h[2] == 1)
	{
		return true;
	}

	return false;
}

void ZupSpriteLoader::load(Path const& /*path*/, data::Source& source, SpriteBase& sprite_base) const
{
	sprite_base.unload();
	source.seek(0);

	uint32_t f1, f2, f3;
	source >> f1 >> f2 >> f3;
	if (f1 != 8 || f2 != 2 || f3 != 1)
	{
		return;
	}

	uint32_t ver;
	source >> ver;

	std::map<uint32_t, uint32_t> textureIdToIdx;
	std::map<uint32_t, uint32_t> moduleIdToIdx;
	std::map<uint32_t, uint32_t> frameIdToIdx;
	std::map<uint32_t, uint32_t> animIdToIdx;

	uint32_t textureCount;
	source >> textureCount;
	for (uint32_t i = 0; i < textureCount; i++)
	{
		uint32_t id;
		String name, texName;
		source >> name >> id >> texName;
		auto tex = System::inst().get_factory().load_by_name<video::Texture>(texName);
		if (!tex)
		{
			//sprite->set_error_string(util::format<String>("cannot load texture '{}'", texName));
			return;
		}

		textureIdToIdx[id] = sprite_base.addTexture(name, tex);
	}

	uint32_t moduleCount;
	source >> moduleCount;
	for (uint32_t i = 0; i < moduleCount; i++)
	{
		String name;
		uint32_t id, textureId, x, y, w, h;
		source >> name >> id >> textureId >> x >> y >> w >> h;

		uint32_t textureIdx = textureIdToIdx[textureId];
		video::Texture_cptr tex = sprite_base.getTexture(textureIdx);
		math::vec2f uv = math::vec2f((float)x, (float)y) / math::vec2f(tex->get_size());
		math::vec2f uvSize = math::vec2f((float)w, (float)h) / math::vec2f(tex->get_size());
		moduleIdToIdx[id] = sprite_base.addModule(textureIdx, math::vec2u32(w, h), uv, uvSize);
	}

	uint32_t frameCount;
	source >> frameCount;
	for (uint32_t i = 0; i < frameCount; i++)
	{
		String name;
		uint32_t id, fmCount;
		source >> name >> id >> fmCount;

		uint32_t frameIdx = sprite_base.addFrame(name);
		frameIdToIdx[id] = frameIdx;

		for (uint32_t j = 0; j < fmCount; j++)
		{
			uint32_t moduleId;
			math::vec2f pos, scale;
			float rot;
			source >> moduleId >> pos >> rot >> scale;
			math::mat3f trans;
			QASSERT(0);
			//trans.setTranslation(pos);
			//trans.setRotationPart(rot);
			//trans.postScale(scale);
			sprite_base.addFrameData(frameIdx, trans, SpriteBase::DataType::MODULE, moduleIdToIdx[moduleId]);
		}
	}

	uint32_t animCount;
	source >> animCount;
	for (uint32_t i = 0; i < animCount; i++)
	{
		String name;
		uint32_t id, afCount;
		source >> name >> id >> afCount;

		uint32_t animIdx = sprite_base.addAnimation(name);
		animIdToIdx[id] = animIdx;

		for (uint32_t j = 0; j < afCount; j++)
		{
			uint32_t frameId;
			math::vec2f pos, scale;
			float rot;
			source >> frameId >> pos >> rot >> scale;
			math::mat3f trans;
			QASSERT(0);
// 			trans.setTranslation(pos);
// 			trans.setRotationPart(rot);
// 			trans.postScale(scale);
			sprite_base.addAnimationData(animIdx, trans, frameIdToIdx[frameId]);
		}
	}

	//sprite_base.set_is_valid(true);
}

