#pragma once

#include "Buffer.h"

namespace q
{
namespace video
{

	class Uniform_Buffer : public Buffer
	{
	public:
		virtual ~Uniform_Buffer() {}

		static Uniform_Buffer_ptr		create();
	};


}
}
