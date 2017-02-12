#pragma once

#include "res/Resource.h"
#include "Ptr_Fw_Declaration.h"

namespace q
{
namespace video
{

class Texture : public res::Resource
{
public:
    static Texture_ptr	create(Path const& path);

    enum class Type : int
    {
        LDR = 0,
        HDR = 0x100,
        COMPRESSED = 0x200,
        DEPTH = 0x300
    };

    enum class Format : int
    {
        //base types - always supported
        RGBA_4 = (int)Type::LDR,
        RGB_565,
        RGBA_5551,
        RGB_8,
        RGBA_8,
        I_8,
        A_8,
        AI_8,

        //float formats
        RGBA_16 = (int)Type::HDR,
        RGB_16,
        I_16,
        A_16,
        AI_16,

        RGBA_32,
        RGB_32,
        I_32,
        A_32,
        AI_32,

        //compressed formats - might not be supported
        RGBA_PVRTC2 = (int)Type::COMPRESSED,
        RGB_PVRTC2,
        RGBA_PVRTC4,
        RGB_PVRTC4,
        RGB_ETC,

        //depth formats - might not be supported
        DEPTH_HALF = (int)Type::DEPTH, //usually 16bpp
        DEPTH_FULL, //usually 24 or 32 bpp
    };

    Texture(Path const& path);
    virtual ~Texture() {}

    virtual math::vec2u32 const&	get_size() const = 0;
    virtual bool		is_npot() const = 0;

    virtual Format		get_format() const = 0;
    virtual bool		supports_format(Format format) const = 0;
    virtual Type		get_type() const = 0;

    virtual bool		has_mipmaps() const = 0;

    virtual bool		allocate(Format format, math::vec2u32 const& size) = 0;
    virtual void		upload_data(uint32_t mipmap, uint8_t const* data) = 0;
    virtual void		generate_mipmaps() = 0;
};

}
}
