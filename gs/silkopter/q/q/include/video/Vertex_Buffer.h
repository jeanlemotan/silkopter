#pragma once

#include "Buffer.h"

namespace q
{
namespace video
{

	class Vertex_Buffer : public Buffer
	{
	public:
		virtual ~Vertex_Buffer() {}

		static Vertex_Buffer_ptr		create();
	};


}
}
