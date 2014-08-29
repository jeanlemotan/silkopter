#pragma once

namespace q
{
namespace util
{
	void encode_base_64(data::Sink& sink, data::Source& source);
	void decode_base_64(data::Sink& sink, data::Source& source);
}
}