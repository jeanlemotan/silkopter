#include "QStdAfx.h"
#include "res/Resource.h"
#include "Ptr_Fw_Declaration.h"
#include "draw/Sprite_Base.h"
#include "draw/Sprite.h"
#include "draw/Painter.h"



using namespace q;
using namespace draw;

SpriteBase::SpriteBase(Path const& path)
	: Resource(path)
{

}

SpriteBase::~SpriteBase()
{
}

void SpriteBase::unload()
{
	QASSERT(0);
}
bool SpriteBase::is_valid() const
{
	return true;
}

Sprite_ptr SpriteBase::createInstance() const
{
	SpriteBase_cptr base = shared_from_this();
	Sprite_ptr s = std::make_shared<Sprite>(base);
	return s;
}

void SpriteBase::render(Painter& painter, math::vec2f const& pos, float /*rot*/, math::vec2f const& /*scale*/, uint32_t animIdx, uint32_t frameIdx) const
{
	using namespace math;

	mat3f spriteTrans;
	mat3f frameTrans;
	QASSERT(0);
	//spriteTrans.setRotationPart(rot);
	//spriteTrans.postScale(scale);
	//spriteTrans.setTranslation(pos);
	bool hasRotScale = !math::is_identity(math::mat2f(spriteTrans));

	Anim const& anim = mAnims[animIdx];
	if (anim.data.empty())
	{
		return;
	}

	frameIdx = math::min(frameIdx, (uint32_t)anim.data.size() - 1);

	//uint32_t fcount = anim.data.size();
	Frame const& frame = mFrames[anim.data[frameIdx].frameIdx];
	uint32_t fdcount = frame.data.size();
	for (uint32_t i = 0; i < fdcount; i++)
	{
		FrameData const& data = frame.data[i];
		DataType type = data.type;
		uint32_t didx = data.dataIdx;
		switch (type)
		{
		case DataType::MODULE:
			{
				Module const& module = mModules[didx];
				video::Texture_cptr tex = mTextures[module.textureIdx].texture;
				//painter.setTexture(tex);
				//TODO

				vec2f s(module.size);

				vec2f p1, p2, p3, p4;

				if (hasRotScale | data.hasRotScale)
				{
					math::mat3f t(spriteTrans * data.transform);
					QASSERT(0);
					//p1 = t.getTranslation();
					p2 = math::transform(t, vec2f(s.x, 0));
					p3 = math::transform(t, s);
					p4 = math::transform(t, vec2f(0, s.y));
				}
// 					else if (data.hasRotScale)
// 					{
// 						math::mat3f const& t = data.transform;
// 						p1 = vec2f(t.getTranslation()) + pos;
// 						p2 = vec2f(t * vec3f(vec2f(s.x, 0), 1)) + pos;
// 						p3 = vec2f(t * vec3f(s, 1)) + pos;
// 						p4 = vec2f(t * vec3f(vec2f(0, s.y), 1)) + pos;
// 					}
// 					else if (hasRotScale)
// 					{
// 						math::mat3f const& t = spriteTrans;
// 						p1 = vec2f(t.getTranslation()) + data.position;
// 						p2 = vec2f(t * vec3f(vec2f(s.x, 0), 1)) + data.position;
// 						p3 = vec2f(t * vec3f(s, 1)) + data.position;
// 						p4 = vec2f(t * vec3f(vec2f(0, s.y), 1)) + data.position;
// 					}
				else
				{
					p1 = pos + data.position;
					p2 = p1 + vec2f(s.x, 0);
					p3 = p1 + s;
					p4 = p1 + vec2f(0, s.y);
				}

				vec2f const& uv = module.uv;
				vec2f const& uvSize = module.uvSize;
				vec2f t1(uv);
				vec2f t2(uv + vec2f(uvSize.x, 0));
				vec2f t3(uv + uvSize);
				vec2f t4(uv + vec2f(0, uvSize.y));

				painter.fill_quad(Vertex(p1, t1), Vertex(p2, t2), Vertex(p3, t3), Vertex(p4, t4));
			}
			break;
		default:
			QASSERT(0);
			break;
		}
	}
}
