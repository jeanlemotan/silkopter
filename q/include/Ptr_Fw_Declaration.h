#pragma once

namespace q
{

	template<class T>
	inline void noop_deleter(T*)
	{
		;
	}

namespace res
{
	class Resource;
	DECLARE_CLASS_PTR(Resource);

}

namespace anim
{
	class Animation_Node;
	DECLARE_CLASS_PTR(Animation_Node);

	class Animation;
	DECLARE_CLASS_PTR(Animation);
}

namespace scene
{
	class Model_Base;
	DECLARE_CLASS_PTR(Model_Base);

	class Model;
	DECLARE_CLASS_PTR(Model);

	class Model2D;
	DECLARE_CLASS_PTR(Model2D);

	class Scene_Node;
	DECLARE_CLASS_PTR(Scene_Node);

	class Render_Job_Scene_Node;
	DECLARE_CLASS_PTR(Render_Job_Scene_Node);

	class Scene;
	DECLARE_CLASS_PTR(Scene);
}

namespace video
{
	class Material;
	DECLARE_CLASS_PTR(Material);

	class Renderer;
	DECLARE_CLASS_PTR(Renderer);

	class Post_Fx;
	DECLARE_CLASS_PTR(Post_Fx);

	class Buffer;
	DECLARE_CLASS_PTR(Buffer);

	class Uniform_Buffer;
	DECLARE_CLASS_PTR(Uniform_Buffer);

	class Vertex_Buffer;
	DECLARE_CLASS_PTR(Vertex_Buffer);

	class Index_Buffer;
	DECLARE_CLASS_PTR(Index_Buffer);

	class Render_Target;
	DECLARE_CLASS_PTR(Render_Target);

	class Render_Job;
	//DECLARE_CLASS_PTR(RenderJob);

	class Texture;
	DECLARE_CLASS_PTR(Texture);

	class Dynamic_Image;
	DECLARE_CLASS_PTR(Dynamic_Image);

	class Sampler;
	DECLARE_CLASS_PTR(Sampler);

	class Render_State;
	DECLARE_CLASS_PTR(Render_State);

	class Technique;
	DECLARE_CLASS_PTR(Technique);

	class Shader;
	DECLARE_CLASS_PTR(Shader);
}

namespace thread
{
	class Task;
	DECLARE_CLASS_PTR(Task);
	class Thread;
	DECLARE_CLASS_PTR(Thread);
}

namespace text
{
	class Font;
	DECLARE_CLASS_PTR(Font);
}

namespace draw
{
	class SpriteBase;
	DECLARE_CLASS_PTR(SpriteBase);
	class Sprite;
	DECLARE_CLASS_PTR(Sprite);
}

namespace ps
{
	class Emitter;
	DECLARE_CLASS_PTR(Emitter);
	class ParticleSystem;
	DECLARE_CLASS_PTR(ParticleSystem);
	class Emitter;
	DECLARE_CLASS_PTR(Emitter);
	class Influence;
	DECLARE_CLASS_PTR(Influence);
	class ParticleDefinition;
	DECLARE_CLASS_PTR(ParticleDefinition);
	class Shape;
	DECLARE_CLASS_PTR(Shape);
}

namespace game
{
	class GameStateMgr;
	DECLARE_CLASS_PTR(GameStateMgr);
	class GameState;
	DECLARE_CLASS_PTR(GameState);
}

}
