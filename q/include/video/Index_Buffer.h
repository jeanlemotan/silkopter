#pragma once

#include "Buffer.h"
#include "Ptr_Fw_Declaration.h"

namespace q
{
namespace video
{

	class Index_Buffer : public Buffer
	{
	public:
        virtual ~Index_Buffer() {}
        
		static Index_Buffer_ptr		create();
	};




}
}
