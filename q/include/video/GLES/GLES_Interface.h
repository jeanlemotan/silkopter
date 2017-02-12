#pragma once

namespace q
{
namespace video
{
namespace gles
{
	typedef void             iGLvoid;
	typedef unsigned int     iGLenum;
	typedef unsigned char    iGLboolean;
	typedef unsigned int     iGLbitfield;
	typedef uint8_t          iGLbyte;
	typedef short            iGLshort;
	typedef int              iGLint;
	typedef int              iGLsizei;
	typedef uint8_t          iGLubyte;
	typedef unsigned short   iGLushort;
	typedef unsigned int     iGLuint;
	typedef float            iGLfloat;
	typedef float            iGLclampf;
	typedef uint32_t         iGLfixed;
	typedef char             iGLchar;

	/* iGL types for handling large vertex buffer objects */
	typedef int              iGLintptr;
	typedef int              iGLsizeiptr;


	/* OpenGL ES core versions */
	static const uint32_t iGL_ES_VERSION_2_0                 = 1;

	/* ClearBufferMask */
	static const uint32_t iGL_DEPTH_BUFFER_BIT               = 0x00000100;
	static const uint32_t iGL_STENCIL_BUFFER_BIT             = 0x00000400;
	static const uint32_t iGL_COLOR_BUFFER_BIT               = 0x00004000;

	/* Boolean */
	static const uint32_t iGL_FALSE                          = 0;
	static const uint32_t iGL_TRUE                           = 1;

	/* BeginMode */
	static const uint32_t iGL_POINTS                         = 0x0000;
	static const uint32_t iGL_LINES                          = 0x0001;
	static const uint32_t iGL_LINE_LOOP                      = 0x0002;
	static const uint32_t iGL_LINE_STRIP                     = 0x0003;
	static const uint32_t iGL_TRIANGLES                      = 0x0004;
	static const uint32_t iGL_TRIANGLE_STRIP                 = 0x0005;
	static const uint32_t iGL_TRIANGLE_FAN                   = 0x0006;

	/* AlphaFunction (not supported in ES20) */
	/*      iGL_NEVER */
	/*      iGL_LESS */
	/*      iGL_EQUAL */
	/*      iGL_LEQUAL */
	/*      iGL_GREATER */
	/*      iGL_NOTEQUAL */
	/*      iGL_GEQUAL */
	/*      iGL_ALWAYS */

	/* BlendingFactorDest */
	static const uint32_t iGL_ZERO                           = 0;
	static const uint32_t iGL_ONE                            = 1;
	static const uint32_t iGL_SRC_COLOR                      = 0x0300;
	static const uint32_t iGL_ONE_MINUS_SRC_COLOR            = 0x0301;
	static const uint32_t iGL_SRC_ALPHA                      = 0x0302;
	static const uint32_t iGL_ONE_MINUS_SRC_ALPHA            = 0x0303;
	static const uint32_t iGL_DST_ALPHA                      = 0x0304;
	static const uint32_t iGL_ONE_MINUS_DST_ALPHA            = 0x0305;

	/* BlendingFactorSrc */
	/*      iGL_ZERO */
	/*      iGL_ONE */
	static const uint32_t iGL_DST_COLOR                      = 0x0306;
	static const uint32_t iGL_ONE_MINUS_DST_COLOR            = 0x0307;
	static const uint32_t iGL_SRC_ALPHA_SATURATE             = 0x0308;
	/*      iGL_SRC_ALPHA */
	/*      iGL_ONE_MINUS_SRC_ALPHA */
	/*      iGL_DST_ALPHA */
	/*      iGL_ONE_MINUS_DST_ALPHA */

	/* BlendEquationSeparate */
	static const uint32_t iGL_FUNC_ADD                       = 0x8006;
	static const uint32_t iGL_BLEND_EQUATION                 = 0x8009;
	static const uint32_t iGL_BLEND_EQUATION_RGB             = 0x8009;    /* same as BLEND_EQUATION */
	static const uint32_t iGL_BLEND_EQUATION_ALPHA           = 0x883D;

	/* BlendSubtract */
	static const uint32_t iGL_FUNC_SUBTRACT                  = 0x800A;
	static const uint32_t iGL_FUNC_REVERSE_SUBTRACT          = 0x800B;

	/* Separate Blend Functions */
	static const uint32_t iGL_BLEND_DST_RGB                  = 0x80C8;
	static const uint32_t iGL_BLEND_SRC_RGB                  = 0x80C9;
	static const uint32_t iGL_BLEND_DST_ALPHA                = 0x80CA;
	static const uint32_t iGL_BLEND_SRC_ALPHA                = 0x80CB;
	static const uint32_t iGL_CONSTANT_COLOR                 = 0x8001;
	static const uint32_t iGL_ONE_MINUS_CONSTANT_COLOR       = 0x8002;
	static const uint32_t iGL_CONSTANT_ALPHA                 = 0x8003;
	static const uint32_t iGL_ONE_MINUS_CONSTANT_ALPHA       = 0x8004;
	static const uint32_t iGL_BLEND_COLOR                    = 0x8005;

	/* Buffer Objects */
	static const uint32_t iGL_ARRAY_BUFFER                   = 0x8892;
	static const uint32_t iGL_ELEMENT_ARRAY_BUFFER           = 0x8893;
	static const uint32_t iGL_ARRAY_BUFFER_BINDING           = 0x8894;
	static const uint32_t iGL_ELEMENT_ARRAY_BUFFER_BINDING   = 0x8895;

	static const uint32_t iGL_STREAM_DRAW                    = 0x88E0;
	static const uint32_t iGL_STATIC_DRAW                    = 0x88E4;
	static const uint32_t iGL_DYNAMIC_DRAW                   = 0x88E8;

	static const uint32_t iGL_STREAM_READ                    = 0x88E1;

	static const uint32_t iGL_BUFFER_SIZE                    = 0x8764;
	static const uint32_t iGL_BUFFER_USAGE                   = 0x8765;
	static const uint32_t iGL_READ_ONLY                      = 0x88B8;
	static const uint32_t iGL_WRITE_ONLY                     = 0x88B9;
	static const uint32_t iGL_READ_WRITE                     = 0x88BA;

	// Map Buffer Range
	static const uint32_t iGL_READ_BIT                      = 0x0001;
	static const uint32_t iGL_WRITE_BIT                     = 0x0002;
	static const uint32_t iGL_MAP_INVALIDATE_RANGE_BIT      = 0x0004;
	static const uint32_t iGL_MAP_INVALIDATE_BUFFER_BIT     = 0x0008;
	static const uint32_t iGL_MAP_FLUSH_EXPLICIT_BIT        = 0x0010;
	static const uint32_t iGL_MAP_UNSYNCHRONIZED_BIT        = 0x0020;

	static const uint32_t iGL_CURRENT_VERTEX_ATTRIB          = 0x8626;

	/* CullFaceMode */
	static const uint32_t iGL_FRONT                          = 0x0404;
	static const uint32_t iGL_BACK                           = 0x0405;
	static const uint32_t iGL_FRONT_AND_BACK                 = 0x0408;

	/* DepthFunction */
	/*      iGL_NEVER */
	/*      iGL_LESS */
	/*      iGL_EQUAL */
	/*      iGL_LEQUAL */
	/*      iGL_GREATER */
	/*      iGL_NOTEQUAL */
	/*      iGL_GEQUAL */
	/*      iGL_ALWAYS */

	/* EnableCap */
	static const uint32_t iGL_TEXTURE_2D                     = 0x0DE1;
	static const uint32_t iGL_CULL_FACE                      = 0x0B44;
	static const uint32_t iGL_BLEND                          = 0x0BE2;
	static const uint32_t iGL_DITHER                         = 0x0BD0;
	static const uint32_t iGL_STENCIL_TEST                   = 0x0B90;
	static const uint32_t iGL_DEPTH_TEST                     = 0x0B71;
	static const uint32_t iGL_SCISSOR_TEST                   = 0x0C11;
	static const uint32_t iGL_POLYGON_OFFSET_FILL            = 0x8037;
#if defined(Q_WINDOWS) || defined(Q_LINUX)
	static const uint32_t iGL_POLYGON_OFFSET_LINE            = 0x2A02;
#endif
	static const uint32_t iGL_SAMPLE_ALPHA_TO_COVERAGE       = 0x809E;
	static const uint32_t iGL_SAMPLE_COVERAGE                = 0x80A0;

	/* ErrorCode */
	static const uint32_t iGL_NO_ERROR                       = 0;
	static const uint32_t iGL_INVALID_ENUM                   = 0x0500;
	static const uint32_t iGL_INVALID_VALUE                  = 0x0501;
	static const uint32_t iGL_INVALID_OPERATION              = 0x0502;
	static const uint32_t iGL_OUT_OF_MEMORY                  = 0x0505;

	/* FrontFaceDirection */
	static const uint32_t iGL_CW                             = 0x0900;
	static const uint32_t iGL_CCW                            = 0x0901;

	/* GetPName */
	static const uint32_t iGL_LINE_WIDTH                     = 0x0B21;
	static const uint32_t iGL_ALIASED_POINT_SIZE_RANGE       = 0x846D;
	static const uint32_t iGL_ALIASED_LINE_WIDTH_RANGE       = 0x846E;
	static const uint32_t iGL_CULL_FACE_MODE                 = 0x0B45;
	static const uint32_t iGL_FRONT_FACE                     = 0x0B46;
	static const uint32_t iGL_DEPTH_RANGE                    = 0x0B70;
	static const uint32_t iGL_DEPTH_WRITEMASK                = 0x0B72;
	static const uint32_t iGL_DEPTH_CLEAR_VALUE              = 0x0B73;
	static const uint32_t iGL_DEPTH_FUNC                     = 0x0B74;
	static const uint32_t iGL_STENCIL_CLEAR_VALUE            = 0x0B91;
	static const uint32_t iGL_STENCIL_FUNC                   = 0x0B92;
	static const uint32_t iGL_STENCIL_FAIL                   = 0x0B94;
	static const uint32_t iGL_STENCIL_PASS_DEPTH_FAIL        = 0x0B95;
	static const uint32_t iGL_STENCIL_PASS_DEPTH_PASS        = 0x0B96;
	static const uint32_t iGL_STENCIL_REF                    = 0x0B97;
	static const uint32_t iGL_STENCIL_VALUE_MASK             = 0x0B93;
	static const uint32_t iGL_STENCIL_WRITEMASK              = 0x0B98;
	static const uint32_t iGL_STENCIL_BACK_FUNC              = 0x8800;
	static const uint32_t iGL_STENCIL_BACK_FAIL              = 0x8801;
	static const uint32_t iGL_STENCIL_BACK_PASS_DEPTH_FAIL   = 0x8802;
	static const uint32_t iGL_STENCIL_BACK_PASS_DEPTH_PASS   = 0x8803;
	static const uint32_t iGL_STENCIL_BACK_REF               = 0x8CA3;
	static const uint32_t iGL_STENCIL_BACK_VALUE_MASK        = 0x8CA4;
	static const uint32_t iGL_STENCIL_BACK_WRITEMASK         = 0x8CA5;
	static const uint32_t iGL_VIEWPORT                       = 0x0BA2;
	static const uint32_t iGL_SCISSOR_BOX                    = 0x0C10;
	/*      iGL_SCISSOR_TEST */
	static const uint32_t iGL_COLOR_CLEAR_VALUE              = 0x0C22;
	static const uint32_t iGL_COLOR_WRITEMASK                = 0x0C23;
	static const uint32_t iGL_UNPACK_ALIGNMENT               = 0x0CF5;
	static const uint32_t iGL_PACK_ALIGNMENT                 = 0x0D05;
	static const uint32_t iGL_MAX_TEXTURE_SIZE               = 0x0D33;
	static const uint32_t iGL_MAX_VIEWPORT_DIMS              = 0x0D3A;
	static const uint32_t iGL_SUBPIXEL_BITS                  = 0x0D50;
	static const uint32_t iGL_RED_BITS                       = 0x0D52;
	static const uint32_t iGL_GREEN_BITS                     = 0x0D53;
	static const uint32_t iGL_BLUE_BITS                      = 0x0D54;
	static const uint32_t iGL_ALPHA_BITS                     = 0x0D55;
	static const uint32_t iGL_DEPTH_BITS                     = 0x0D56;
	static const uint32_t iGL_STENCIL_BITS                   = 0x0D57;
	static const uint32_t iGL_POLYGON_OFFSET_UNITS           = 0x2A00;
	/*      iGL_POLYGON_OFFSET_FILL */
	static const uint32_t iGL_POLYGON_OFFSET_FACTOR          = 0x8038;
	static const uint32_t iGL_TEXTURE_BINDING_2D             = 0x8069;
	static const uint32_t iGL_SAMPLE_BUFFERS                 = 0x80A8;
	static const uint32_t iGL_SAMPLES                        = 0x80A9;
	static const uint32_t iGL_SAMPLE_COVERAGE_VALUE          = 0x80AA;
	static const uint32_t iGL_SAMPLE_COVERAGE_INVERT         = 0x80AB;

	/* GetTextureParameter */
	/*      iGL_TEXTURE_MAG_FILTER */
	/*      iGL_TEXTURE_MIN_FILTER */
	/*      iGL_TEXTURE_WRAP_S */
	/*      iGL_TEXTURE_WRAP_T */

	static const uint32_t iGL_NUM_COMPRESSED_TEXTURE_FORMATS = 0x86A2;
	static const uint32_t iGL_COMPRESSED_TEXTURE_FORMATS     = 0x86A3;

	/* HintMode */
	static const uint32_t iGL_DONT_CARE                      = 0x1100;
	static const uint32_t iGL_FASTEST                        = 0x1101;
	static const uint32_t iGL_NICEST                         = 0x1102;

	/* HintTarget */
	static const uint32_t iGL_GENERATE_MIPMAP_HINT           = 0x8192;

	/* DataType */
	static const uint32_t iGL_BYTE                           = 0x1400;
	static const uint32_t iGL_UNSIGNED_BYTE                  = 0x1401;
	static const uint32_t iGL_SHORT                          = 0x1402;
	static const uint32_t iGL_UNSIGNED_SHORT                 = 0x1403;
	static const uint32_t iGL_INT                            = 0x1404;
	static const uint32_t iGL_UNSIGNED_INT                   = 0x1405;
	static const uint32_t iGL_FLOAT                          = 0x1406;
	static const uint32_t iGL_HALF_FLOAT					 = 0x140B;
	static const uint32_t iGL_FIXED                          = 0x140C;

	/* PixelFormat */
	static const uint32_t iGL_DEPTH_COMPONENT                = 0x1902;
	static const uint32_t iGL_ALPHA                          = 0x1906;
	static const uint32_t iGL_RGB                            = 0x1907;
	static const uint32_t iGL_RGBA                           = 0x1908;
	static const uint32_t iGL_LUMINANCE                      = 0x1909;
	static const uint32_t iGL_LUMINANCE_ALPHA                = 0x190A;
	static const uint32_t iGL_BGR                            = 0x80E0;
	static const uint32_t iGL_BGRA                           = 0x80E1;

	/* PixelType */
	/*      iGL_UNSIGNED_BYTE */
	static const uint32_t iGL_UNSIGNED_SHORT_4_4_4_4         = 0x8033;
	static const uint32_t iGL_UNSIGNED_SHORT_5_5_5_1         = 0x8034;
	static const uint32_t iGL_UNSIGNED_SHORT_5_6_5           = 0x8363;

	/* Shaders */
	static const uint32_t iGL_FRAGMENT_SHADER                  = 0x8B30;
	static const uint32_t iGL_VERTEX_SHADER                    = 0x8B31;
	static const uint32_t iGL_MAX_VERTEX_ATTRIBS               = 0x8869;
	static const uint32_t iGL_MAX_VERTEX_UNIFORM_VECTORS       = 0x8DFB;
	static const uint32_t iGL_MAX_VARYING_VECTORS              = 0x8DFC;
	static const uint32_t iGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
	static const uint32_t iGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS   = 0x8B4C;
	static const uint32_t iGL_MAX_TEXTURE_IMAGE_UNITS          = 0x8872;
	static const uint32_t iGL_MAX_FRAGMENT_UNIFORM_VECTORS     = 0x8DFD;
	static const uint32_t iGL_SHADER_TYPE                      = 0x8B4F;
	static const uint32_t iGL_DELETE_STATUS                    = 0x8B80;
	static const uint32_t iGL_LINK_STATUS                      = 0x8B82;
	static const uint32_t iGL_VALIDATE_STATUS                  = 0x8B83;
	static const uint32_t iGL_ATTACHED_SHADERS                 = 0x8B85;
	static const uint32_t iGL_ACTIVE_UNIFORMS                  = 0x8B86;
	static const uint32_t iGL_ACTIVE_UNIFORM_MAX_LENGTH        = 0x8B87;
	static const uint32_t iGL_ACTIVE_ATTRIBUTES                = 0x8B89;
	static const uint32_t iGL_ACTIVE_ATTRIBUTE_MAX_LENGTH      = 0x8B8A;
	static const uint32_t iGL_SHADING_LANGUAGE_VERSION         = 0x8B8C;
	static const uint32_t iGL_CURRENT_PROGRAM                  = 0x8B8D;

	/* StencilFunction */
	static const uint32_t iGL_NEVER                          = 0x0200;
	static const uint32_t iGL_LESS                           = 0x0201;
	static const uint32_t iGL_EQUAL                          = 0x0202;
	static const uint32_t iGL_LEQUAL                         = 0x0203;
	static const uint32_t iGL_GREATER                        = 0x0204;
	static const uint32_t iGL_NOTEQUAL                       = 0x0205;
	static const uint32_t iGL_GEQUAL                         = 0x0206;
	static const uint32_t iGL_ALWAYS                         = 0x0207;

	/* StencilOp */
	/*      iGL_ZERO */
	static const uint32_t iGL_KEEP                           = 0x1E00;
	static const uint32_t iGL_REPLACE                        = 0x1E01;
	static const uint32_t iGL_INCR                           = 0x1E02;
	static const uint32_t iGL_DECR                           = 0x1E03;
	static const uint32_t iGL_INVERT                         = 0x150A;
	static const uint32_t iGL_INCR_WRAP                      = 0x8507;
	static const uint32_t iGL_DECR_WRAP                      = 0x8508;

	/* StringName */
	static const uint32_t iGL_VENDOR                         = 0x1F00;
	static const uint32_t iGL_RENDERER                       = 0x1F01;
	static const uint32_t iGL_VERSION                        = 0x1F02;
	static const uint32_t iGL_EXTENSIONS                     = 0x1F03;

	/* TextureMagFilter */
	static const uint32_t iGL_NEAREST                        = 0x2600;
	static const uint32_t iGL_LINEAR                         = 0x2601;

	/* TextureMinFilter */
	/*      iGL_NEAREST */
	/*      iGL_LINEAR */
	static const uint32_t iGL_NEAREST_MIPMAP_NEAREST         = 0x2700;
	static const uint32_t iGL_LINEAR_MIPMAP_NEAREST          = 0x2701;
	static const uint32_t iGL_NEAREST_MIPMAP_LINEAR          = 0x2702;
	static const uint32_t iGL_LINEAR_MIPMAP_LINEAR           = 0x2703;

	/* TextureParameterName */
	static const uint32_t iGL_TEXTURE_MAG_FILTER             = 0x2800;
	static const uint32_t iGL_TEXTURE_MIN_FILTER             = 0x2801;
	static const uint32_t iGL_TEXTURE_WRAP_S                 = 0x2802;
	static const uint32_t iGL_TEXTURE_WRAP_T                 = 0x2803;

	static const uint32_t iGL_TEXTURE_MAX_LEVEL				= 0x813D;

	/* TextureTarget */
	/*      iGL_TEXTURE_2D */
	static const uint32_t iGL_TEXTURE                        = 0x1702;

	static const uint32_t iGL_TEXTURE_CUBE_MAP               = 0x8513;
	static const uint32_t iGL_TEXTURE_BINDING_CUBE_MAP       = 0x8514;
	static const uint32_t iGL_TEXTURE_CUBE_MAP_POSITIVE_X    = 0x8515;
	static const uint32_t iGL_TEXTURE_CUBE_MAP_NEGATIVE_X    = 0x8516;
	static const uint32_t iGL_TEXTURE_CUBE_MAP_POSITIVE_Y    = 0x8517;
	static const uint32_t iGL_TEXTURE_CUBE_MAP_NEGATIVE_Y    = 0x8518;
	static const uint32_t iGL_TEXTURE_CUBE_MAP_POSITIVE_Z    = 0x8519;
	static const uint32_t iGL_TEXTURE_CUBE_MAP_NEGATIVE_Z    = 0x851A;
	static const uint32_t iGL_MAX_CUBE_MAP_TEXTURE_SIZE      = 0x851C;

	/* TextureUnit */
	static const uint32_t iGL_TEXTURE0                       = 0x84C0;
	static const uint32_t iGL_TEXTURE1                       = 0x84C1;
	static const uint32_t iGL_TEXTURE2                       = 0x84C2;
	static const uint32_t iGL_TEXTURE3                       = 0x84C3;
	static const uint32_t iGL_TEXTURE4                       = 0x84C4;
	static const uint32_t iGL_TEXTURE5                       = 0x84C5;
	static const uint32_t iGL_TEXTURE6                       = 0x84C6;
	static const uint32_t iGL_TEXTURE7                       = 0x84C7;
	static const uint32_t iGL_TEXTURE8                       = 0x84C8;
	static const uint32_t iGL_TEXTURE9                       = 0x84C9;
	static const uint32_t iGL_TEXTURE10                      = 0x84CA;
	static const uint32_t iGL_TEXTURE11                      = 0x84CB;
	static const uint32_t iGL_TEXTURE12                      = 0x84CC;
	static const uint32_t iGL_TEXTURE13                      = 0x84CD;
	static const uint32_t iGL_TEXTURE14                      = 0x84CE;
	static const uint32_t iGL_TEXTURE15                      = 0x84CF;
	static const uint32_t iGL_TEXTURE16                      = 0x84D0;
	static const uint32_t iGL_TEXTURE17                      = 0x84D1;
	static const uint32_t iGL_TEXTURE18                      = 0x84D2;
	static const uint32_t iGL_TEXTURE19                      = 0x84D3;
	static const uint32_t iGL_TEXTURE20                      = 0x84D4;
	static const uint32_t iGL_TEXTURE21                      = 0x84D5;
	static const uint32_t iGL_TEXTURE22                      = 0x84D6;
	static const uint32_t iGL_TEXTURE23                      = 0x84D7;
	static const uint32_t iGL_TEXTURE24                      = 0x84D8;
	static const uint32_t iGL_TEXTURE25                      = 0x84D9;
	static const uint32_t iGL_TEXTURE26                      = 0x84DA;
	static const uint32_t iGL_TEXTURE27                      = 0x84DB;
	static const uint32_t iGL_TEXTURE28                      = 0x84DC;
	static const uint32_t iGL_TEXTURE29                      = 0x84DD;
	static const uint32_t iGL_TEXTURE30                      = 0x84DE;
	static const uint32_t iGL_TEXTURE31                      = 0x84DF;
	static const uint32_t iGL_ACTIVE_TEXTURE                 = 0x84E0;

	/* TextureWrapMode */
	static const uint32_t iGL_REPEAT                         = 0x2901;
	static const uint32_t iGL_CLAMP_TO_EDGE                  = 0x812F;
	static const uint32_t iGL_MIRRORED_REPEAT                = 0x8370;

	/* Uniform Types */
	static const uint32_t iGL_FLOAT_VEC2                     = 0x8B50;
	static const uint32_t iGL_FLOAT_VEC3                     = 0x8B51;
	static const uint32_t iGL_FLOAT_VEC4                     = 0x8B52;
	static const uint32_t iGL_INT_VEC2                       = 0x8B53;
	static const uint32_t iGL_INT_VEC3                       = 0x8B54;
	static const uint32_t iGL_INT_VEC4                       = 0x8B55;
	static const uint32_t iGL_BOOL                           = 0x8B56;
	static const uint32_t iGL_BOOL_VEC2                      = 0x8B57;
	static const uint32_t iGL_BOOL_VEC3                      = 0x8B58;
	static const uint32_t iGL_BOOL_VEC4                      = 0x8B59;
	static const uint32_t iGL_FLOAT_MAT2                     = 0x8B5A;
	static const uint32_t iGL_FLOAT_MAT3                     = 0x8B5B;
	static const uint32_t iGL_FLOAT_MAT4                     = 0x8B5C;
	static const uint32_t iGL_SAMPLER_2D                     = 0x8B5E;
	static const uint32_t iGL_SAMPLER_CUBE                   = 0x8B60;

	/* Vertex Arrays */
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_ENABLED        = 0x8622;
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_SIZE           = 0x8623;
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_STRIDE         = 0x8624;
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_TYPE           = 0x8625;
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_NORMALIZED     = 0x886A;
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_POINTER        = 0x8645;
	static const uint32_t iGL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;

	/* Read Format */
	static const uint32_t iGL_IMPLEMENTATION_COLOR_READ_TYPE   = 0x8B9A;
	static const uint32_t iGL_IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;

	/* Shader Source */
	static const uint32_t iGL_COMPILE_STATUS                 = 0x8B81;
	static const uint32_t iGL_INFO_LOG_LENGTH                = 0x8B84;
	static const uint32_t iGL_SHADER_SOURCE_LENGTH           = 0x8B88;
	static const uint32_t iGL_SHADER_COMPILER                = 0x8DFA;

	/* Shader Binary */
	static const uint32_t iGL_SHADER_BINARY_FORMATS          = 0x8DF8;
	static const uint32_t iGL_NUM_SHADER_BINARY_FORMATS      = 0x8DF9;

	/* Shader Precision-Specified Types */
	static const uint32_t iGL_LOW_FLOAT                      = 0x8DF0;
	static const uint32_t iGL_MEDIUM_FLOAT                   = 0x8DF1;
	static const uint32_t iGL_HIGH_FLOAT                     = 0x8DF2;
	static const uint32_t iGL_LOW_INT                        = 0x8DF3;
	static const uint32_t iGL_MEDIUM_INT                     = 0x8DF4;
	static const uint32_t iGL_HIGH_INT                       = 0x8DF5;

	/* Framebuffer Object. */
	static const uint32_t iGL_FRAMEBUFFER                    = 0x8D40;
	static const uint32_t iGL_RENDERBUFFER                   = 0x8D41;

	static const uint32_t iGL_RGB8                           = 0x8051;
	static const uint32_t iGL_RGBA8                          = 0x8058;
	static const uint32_t iGL_RGBA4                          = 0x8056;
	static const uint32_t iGL_RGB5_A1                        = 0x8057;
	static const uint32_t iGL_RGB565                         = 0x8D62;
	static const uint32_t iGL_DEPTH_COMPONENT16              = 0x81A5;
	static const uint32_t iGL_DEPTH_COMPONENT24				= 0x81A6;
	static const uint32_t iGL_DEPTH_COMPONENT32F				= 0x8CAC;
	
	static const uint32_t iGL_STENCIL_INDEX                  = 0x1901;
	static const uint32_t iGL_STENCIL_INDEX8                 = 0x8D48;
	static const uint32_t iGL_LUMINANCE8                     = 0x8040;
	static const uint32_t iGL_LUMINANCE8_ALPHA8              = 0x8045;
	static const uint32_t iGL_ALPHA8                         = 0x803C;

	// ARB_texture_float
	static const uint32_t iGL_TEXTURE_RED_TYPE           = 0x8C10;
	static const uint32_t iGL_TEXTURE_GREEN_TYPE         = 0x8C11;
	static const uint32_t iGL_TEXTURE_BLUE_TYPE          = 0x8C12;
	static const uint32_t iGL_TEXTURE_ALPHA_TYPE         = 0x8C13;
	static const uint32_t iGL_TEXTURE_LUMINANCE_TYPE     = 0x8C14;
	static const uint32_t iGL_TEXTURE_INTENSITY_TYPE     = 0x8C15;
	static const uint32_t iGL_TEXTURE_DEPTH_TYPE         = 0x8C16;
	static const uint32_t iGL_UNSIGNED_NORMALIZED        = 0x8C17;

	static const uint32_t iGL_RGBA32F						= 0x8814;
	static const uint32_t iGL_RGB32F							= 0x8815;
	static const uint32_t iGL_ALPHA32F						= 0x8816;
	static const uint32_t iGL_INTENSITY32F					= 0x8817;
	static const uint32_t iGL_LUMINANCE32F					= 0x8818;
	static const uint32_t iGL_LUMINANCE_ALPHA32F				= 0x8819;
	static const uint32_t iGL_RGBA16F						= 0x881A;
	static const uint32_t iGL_RGB16F							= 0x881B;
	static const uint32_t iGL_ALPHA16F						= 0x881C;
	static const uint32_t iGL_INTENSITY16F					= 0x881D;
	static const uint32_t iGL_LUMINANCE16F					= 0x881E;
	static const uint32_t iGL_LUMINANCE_ALPHA16F				= 0x881F;

	// ARB_draw_buffers
	static const uint32_t iGL_MAX_DRAW_BUFFERS           = 0x8824;
	static const uint32_t iGL_DRAW_BUFFER0               = 0x8825;
	static const uint32_t iGL_DRAW_BUFFER1               = 0x8826;
	static const uint32_t iGL_DRAW_BUFFER2               = 0x8827;
	static const uint32_t iGL_DRAW_BUFFER3               = 0x8828;
	static const uint32_t iGL_DRAW_BUFFER5               = 0x882A;
	static const uint32_t iGL_DRAW_BUFFER6               = 0x882B;
	static const uint32_t iGL_DRAW_BUFFER7               = 0x882C;
	static const uint32_t iGL_DRAW_BUFFER8               = 0x882D;
	static const uint32_t iGL_DRAW_BUFFER9               = 0x882E;
	static const uint32_t iGL_DRAW_BUFFER10              = 0x882F;
	static const uint32_t iGL_DRAW_BUFFER11              = 0x8830;
	static const uint32_t iGL_DRAW_BUFFER12              = 0x8831;
	static const uint32_t iGL_DRAW_BUFFER13              = 0x8832;
	static const uint32_t iGL_DRAW_BUFFER14              = 0x8833;
	static const uint32_t iGL_DRAW_BUFFER15              = 0x8834;

	// GLES3
	static const uint32_t iGL_RGBA32UI                    = 0x8D70;
	static const uint32_t iGL_RGB32UI                     = 0x8D71;
	static const uint32_t iGL_RGBA16UI                    = 0x8D76;
	static const uint32_t iGL_RGB16UI                     = 0x8D77;
	static const uint32_t iGL_RGBA32I                     = 0x8D82;
	static const uint32_t iGL_RGB32I                      = 0x8D83;
	static const uint32_t iGL_RGBA16I                     = 0x8D88;
	static const uint32_t iGL_RGB16I                      = 0x8D89;
	static const uint32_t iGL_RED_INTEGER                  = 0x8D94;
	static const uint32_t iGL_GREEN_INTEGER                = 0x8D95;
	static const uint32_t iGL_BLUE_INTEGER                 = 0x8D96;
	static const uint32_t iGL_RGB_INTEGER                  = 0x8D98;
	static const uint32_t iGL_RGBA_INTEGER                 = 0x8D99;
	static const uint32_t iGL_BGR_INTEGER                  = 0x8D9A;
	static const uint32_t iGL_BGRA_INTEGER                 = 0x8D9B;

	 // ARB_texture_rg
	static const uint32_t iGL_R8                         = 0x8229;
	static const uint32_t iGL_R16                        = 0x822A;

	static const uint32_t iGL_RG8                        = 0x822B;
	static const uint32_t iGL_RG16                       = 0x822C;
	static const uint32_t iGL_R16F                       = 0x822D;
	static const uint32_t iGL_R32F                       = 0x822E;

	static const uint32_t iGL_RG16F                      = 0x822F;
	static const uint32_t iGL_RG32F                      = 0x8230;

	static const uint32_t iGL_R8I                        = 0x8231;
	static const uint32_t iGL_R8UI                       = 0x8232;
	static const uint32_t iGL_R16I                       = 0x8233;
	static const uint32_t iGL_R16UI                      = 0x8234;
	static const uint32_t iGL_R32I                       = 0x8235;
	static const uint32_t iGL_R32UI                      = 0x8236;

	static const uint32_t iGL_RG8I                       = 0x8237;
	static const uint32_t iGL_RG8UI                      = 0x8238;
	static const uint32_t iGL_RG16I                      = 0x8239;
	static const uint32_t iGL_RG16UI                     = 0x823A;
	static const uint32_t iGL_RG32I                      = 0x823B;
	static const uint32_t iGL_RG32UI                     = 0x823C;

	static const uint32_t iGL_RED                        = 0x1903;
	static const uint32_t iGL_COMPRESSED_RED             = 0x8225;
	static const uint32_t iGL_COMPRESSED_RG              = 0x8226;
	static const uint32_t iGL_RG                         = 0x8227;
	static const uint32_t iGL_RG_INTEGER                 = 0x8228;

	// GL_EXT_packed_depth_stencil: for multisampling on win
	// GL_OES_packed_depth_stencil: for multisampling on ios

	static const uint32_t iGL_DEPTH24_STENCIL8				= 0x88F0;
	static const uint32_t iGL_DEPTH_STENCIL					= 0x84F9;
	static const uint32_t iGL_UNSIGNED_INT_24_8				= 0x84FA;

	// GL_EXT_framebuffer_blit on win
	// GL_APPLE_framebuffer_multisample on ios
	static const uint32_t iGL_DRAW_FRAMEBUFFER_BINDING       = 0x8CA6;
	static const uint32_t iGL_READ_FRAMEBUFFER	            = 0x8CA8;
	static const uint32_t iGL_DRAW_FRAMEBUFFER	            = 0x8CA9;
	static const uint32_t iGL_READ_FRAMEBUFFER_BINDING       = 0x8CAA;

	static const uint32_t iGL_RENDERBUFFER_WIDTH             = 0x8D42;
	static const uint32_t iGL_RENDERBUFFER_HEIGHT            = 0x8D43;
	static const uint32_t iGL_RENDERBUFFER_INTERNAL_FORMAT   = 0x8D44;
	static const uint32_t iGL_RENDERBUFFER_RED_SIZE          = 0x8D50;
	static const uint32_t iGL_RENDERBUFFER_GREEN_SIZE        = 0x8D51;
	static const uint32_t iGL_RENDERBUFFER_BLUE_SIZE         = 0x8D52;
	static const uint32_t iGL_RENDERBUFFER_ALPHA_SIZE        = 0x8D53;
	static const uint32_t iGL_RENDERBUFFER_DEPTH_SIZE        = 0x8D54;
	static const uint32_t iGL_RENDERBUFFER_STENCIL_SIZE      = 0x8D55;

	static const uint32_t iGL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE           = 0x8CD0;
	static const uint32_t iGL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME           = 0x8CD1;
	static const uint32_t iGL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL         = 0x8CD2;
	static const uint32_t iGL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;

	static const uint32_t iGL_COLOR_ATTACHMENT0              = 0x8CE0;
	static const uint32_t iGL_COLOR_ATTACHMENT1              = 0x8CE1;
	static const uint32_t iGL_COLOR_ATTACHMENT2              = 0x8CE2;
	static const uint32_t iGL_COLOR_ATTACHMENT3              = 0x8CE3;
	static const uint32_t iGL_DEPTH_ATTACHMENT               = 0x8D00;
	static const uint32_t iGL_STENCIL_ATTACHMENT             = 0x8D20;

	static const uint32_t iGL_NONE                           = 0;

	static const uint32_t iGL_FRAMEBUFFER_COMPLETE                      = 0x8CD5;
	static const uint32_t iGL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT         = 0x8CD6;
	static const uint32_t iGL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
	static const uint32_t iGL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS         = 0x8CD9;
	static const uint32_t iGL_FRAMEBUFFER_UNSUPPORTED                   = 0x8CDD;

	static const uint32_t iGL_FRAMEBUFFER_BINDING            = 0x8CA6;
	static const uint32_t iGL_RENDERBUFFER_BINDING           = 0x8CA7;
	static const uint32_t iGL_MAX_RENDERBUFFER_SIZE          = 0x84E8;

	static const uint32_t iGL_INVALID_FRAMEBUFFER_OPERATION  = 0x0506;

	static const uint32_t iGL_TEXTURE_MAX_ANISOTROPY = 0x84FE;
	static const uint32_t iGL_MAX_TEXTURE_MAX_ANISOTROPY = 0x84FF;

	static const uint32_t iGL_VERTEX_ARRAY_BINDING		= 0x85B5;

	static const uint32_t iGL_SAMPLES_PASSED				= 0x8914;
	static const uint32_t iGL_QUERY_COUNTER_BITS			= 0x8864;
	static const uint32_t iGL_CURRENT_QUERY				= 0x8865;
	static const uint32_t iGL_QUERY_RESULT				= 0x8866;
	static const uint32_t iGL_QUERY_RESULT_AVAILABLE		= 0x8867;

	static const uint32_t iGL_TEXTURE_IMMUTABLE_FORMAT   = 0x912F;

	//PBO
	static const uint32_t iGL_PIXEL_PACK_BUFFER			= 0x88EB;
	extern "C"
	{
#if defined(_WIN32)
#	define GLAPIENTRY __stdcall
#else
#	define GLAPIENTRY
#endif

	}

	class TL_Interface;

	//Creates a Interface for the current thread
	//The GL context MUST be set before calling this function
	extern void init_for_thread(void* context); 
	extern void shutdown_for_thread(void* context);
	//sets the current context. This assumes the context was initialized for the current thread with initForThread
	extern void set_context(void* context);

	//////////////////////////////////////////////////////////////////////////
	// Helper class that has a member with the current TLInterface
	class Interface
	{
	public:
		void iglActiveTexture (iGLenum texture);
		void iglAttachShader (iGLuint program, iGLuint shader);
		void iglBindAttribLocation (iGLuint program, iGLuint index, const char* name);
		void iglBindBuffer (iGLenum target, iGLuint buffer);
		void iglBindFramebuffer (iGLenum target, iGLuint framebuffer);
		void iglBindRenderbuffer (iGLenum target, iGLuint renderbuffer);
		void iglBindTexture (iGLenum target, iGLuint texture);
		void iglBlendColor (iGLclampf red, iGLclampf green, iGLclampf blue, iGLclampf alpha);
		void iglBlendEquation (iGLenum mode);
		void iglBlendEquationSeparate (iGLenum modeRGB, iGLenum modeAlpha);
		void iglBlendFunc (iGLenum sfactor, iGLenum dfactor);
		void iglBlendFuncSeparate (iGLenum srcRGB, iGLenum dstRGB, iGLenum srcAlpha, iGLenum dstAlpha);
		void iglBufferData (iGLenum target, iGLsizeiptr size, const void* data, iGLenum usage);
		void iglBufferSubData (iGLenum target, iGLintptr offset, iGLsizeiptr size, const void* data);
		iGLenum iglCheckFramebufferStatus (iGLenum target);
		void iglClear (iGLbitfield mask);
		void iglClearColor (iGLclampf red, iGLclampf green, iGLclampf blue, iGLclampf alpha);
		void iglClearDepthf (iGLclampf depth);
		void iglClearStencil (iGLint s);
		void iglColorMask (iGLboolean red, iGLboolean green, iGLboolean blue, iGLboolean alpha);
		void iglCompileShader (iGLuint shader);
		void iglCompressedTexImage2D (iGLenum target, iGLint level, iGLenum internalformat, iGLsizei width, iGLsizei height, iGLint border, iGLsizei imageSize, const void* data);
		void iglCompressedTexSubImage2D (iGLenum target, iGLint level, iGLint xoffset, iGLint yoffset, iGLsizei width, iGLsizei height, iGLenum format, iGLsizei imageSize, const void* data);
		void iglCopyTexImage2D (iGLenum target, iGLint level, iGLenum internalformat, iGLint x, iGLint y, iGLsizei width, iGLsizei height, iGLint border);
		void iglCopyTexSubImage2D (iGLenum target, iGLint level, iGLint xoffset, iGLint yoffset, iGLint x, iGLint y, iGLsizei width, iGLsizei height);
		iGLuint iglCreateProgram (void);
		iGLuint iglCreateShader (iGLenum type);
		void iglCullFace (iGLenum mode);
		void iglDeleteBuffers (iGLsizei n, const iGLuint* buffers);
		void iglDeleteFramebuffers (iGLsizei n, const iGLuint* framebuffers);
		void iglDeleteProgram (iGLuint program);
		void iglDeleteRenderbuffers (iGLsizei n, const iGLuint* renderbuffers);
		void iglDeleteShader (iGLuint shader);
		void iglDeleteTextures (iGLsizei n, const iGLuint* textures);
		void iglDepthFunc (iGLenum func);
		void iglDepthMask (iGLboolean flag);
		void iglDepthRangef (iGLclampf zNear, iGLclampf zFar);
		void iglDetachShader (iGLuint program, iGLuint shader);
		void iglDisable (iGLenum cap);
		void iglDisableVertexAttribArray (iGLuint index);
		//void iglDiscardFramebuffer(iGLenum target, iGLsizei numAttachments, const iGLenum *attachments);
		void iglDrawArrays (iGLenum mode, iGLint first, iGLsizei count);
		void iglDrawElements (iGLenum mode, iGLsizei count, iGLenum type, const void* indices);
		void iglEnable (iGLenum cap);
		void iglEnableVertexAttribArray (iGLuint index);
		void iglFinish (void);
		void iglFlush (void);
		void iglFramebufferRenderbuffer (iGLenum target, iGLenum attachment, iGLenum renderbuffertarget, iGLuint renderbuffer);
		void iglFramebufferTexture2D (iGLenum target, iGLenum attachment, iGLenum textarget, iGLuint texture, iGLint level);
		void iglDrawBuffers(iGLsizei n, const iGLenum* bufs);
		void iglFrontFace (iGLenum mode);
		void iglGenBuffers (iGLsizei n, iGLuint* buffers);
		void iglGenerateMipmap (iGLenum target);
		void iglGenFramebuffers (iGLsizei n, iGLuint* framebuffers);
		void iglGenRenderbuffers (iGLsizei n, iGLuint* renderbuffers);
		void iglGenTextures (iGLsizei n, iGLuint* textures);
		void iglGetActiveAttrib (iGLuint program, iGLuint index, iGLsizei bufsize, iGLsizei* length, iGLint* size, iGLenum* type, char* name);
		void iglGetActiveUniform (iGLuint program, iGLuint index, iGLsizei bufsize, iGLsizei* length, iGLint* size, iGLenum* type, char* name);
		void iglGetAttachedShaders (iGLuint program, iGLsizei maxcount, iGLsizei* count, iGLuint* shaders);
		int  iglGetAttribLocation (iGLuint program, const char* name);
		void iglGetBooleanv (iGLenum pname, iGLboolean* params);
		void iglGetBufferParameteriv (iGLenum target, iGLenum pname, iGLint* params);
		iGLenum iglGetError (void);
		void iglGetFloatv (iGLenum pname, iGLfloat* params);
		void iglGetFramebufferAttachmentParameteriv (iGLenum target, iGLenum attachment, iGLenum pname, iGLint* params);
		void iglGetIntegerv (iGLenum pname, iGLint* params);
		void iglGetProgramiv (iGLuint program, iGLenum pname, iGLint* params);
		void iglGetProgramInfoLog (iGLuint program, iGLsizei bufsize, iGLsizei* length, char* infolog);
		void iglGetRenderbufferParameteriv (iGLenum target, iGLenum pname, iGLint* params);
		void iglGetShaderiv (iGLuint shader, iGLenum pname, iGLint* params);
		void iglGetShaderInfoLog (iGLuint shader, iGLsizei bufsize, iGLsizei* length, char* infolog);
		void iglGetShaderPrecisionFormat (iGLenum shadertype, iGLenum precisiontype, iGLint* range, iGLint* precision);
		void iglGetShaderSource (iGLuint shader, iGLsizei bufsize, iGLsizei* length, char* source);
		const iGLubyte* iglGetString (iGLenum name);
		void iglGetTexParameterfv (iGLenum target, iGLenum pname, iGLfloat* params);
		void iglGetTexParameteriv (iGLenum target, iGLenum pname, iGLint* params);
		void iglGetUniformfv (iGLuint program, iGLint location, iGLfloat* params);
		void iglGetUniformiv (iGLuint program, iGLint location, iGLint* params);
		int  iglGetUniformLocation (iGLuint program, const char* name);
		void iglGetVertexAttribfv (iGLuint index, iGLenum pname, iGLfloat* params);
		void iglGetVertexAttribiv (iGLuint index, iGLenum pname, iGLint* params);
		void iglGetVertexAttribPointerv (iGLuint index, iGLenum pname, void** pointer);
		void iglHint (iGLenum target, iGLenum mode);
		iGLboolean iglIsBuffer (iGLuint buffer);
		iGLboolean iglIsEnabled (iGLenum cap);
		iGLboolean iglIsFramebuffer (iGLuint framebuffer);
		iGLboolean iglIsProgram (iGLuint program);
		iGLboolean iglIsRenderbuffer (iGLuint renderbuffer);
		iGLboolean iglIsShader (iGLuint shader);
		iGLboolean iglIsTexture (iGLuint texture);
		void iglLineWidth (iGLfloat width);
		void iglLinkProgram (iGLuint program);
		void iglPixelStorei (iGLenum pname, iGLint param);
		void iglPolygonOffset (iGLfloat factor, iGLfloat units);
		void iglReadPixels (iGLint x, iGLint y, iGLsizei width, iGLsizei height, iGLenum format, iGLenum type, void* pixels);
		void iglReleaseShaderCompiler (void);
		void iglRenderbufferStorage (iGLenum target, iGLenum internalformat, iGLsizei width, iGLsizei height);
		void iglRenderbufferStorageMultisample(iGLenum target, iGLsizei samples, iGLenum internalformat, iGLsizei width, iGLsizei height);
		void iglBlitFramebuffer(iGLint srcX0, iGLint srcY0, iGLint srcX1, iGLint srcY1, iGLint dstX0, iGLint dstY0, iGLint dstX1, iGLint dstY1, iGLbitfield mask, iGLenum filter);
		void iglSampleCoverage (iGLclampf value, iGLboolean invert);
		void iglScissor (iGLint x, iGLint y, iGLsizei width, iGLsizei height);
		void iglShaderBinary (iGLsizei n, const iGLuint* shaders, iGLenum binaryformat, const void* binary, iGLsizei length);
		void iglShaderSource (iGLuint shader, iGLsizei count, const char** string, const iGLint* length);
		void iglStencilFunc (iGLenum func, iGLint ref, iGLuint mask);
		void iglStencilFuncSeparate (iGLenum face, iGLenum func, iGLint ref, iGLuint mask);
		void iglStencilMask (iGLuint mask);
		void iglStencilMaskSeparate (iGLenum face, iGLuint mask);
		void iglStencilOp (iGLenum fail, iGLenum zfail, iGLenum zpass);
		void iglStencilOpSeparate (iGLenum face, iGLenum fail, iGLenum zfail, iGLenum zpass);
		void iglTexImage2D (iGLenum target, iGLint level, iGLint internalformat, iGLsizei width, iGLsizei height, iGLint border, iGLenum format, iGLenum type, const void* pixels);
		void iglTexParameterf (iGLenum target, iGLenum pname, iGLfloat param);
		void iglTexParameterfv (iGLenum target, iGLenum pname, const iGLfloat* params);
		void iglTexParameteri (iGLenum target, iGLenum pname, iGLint param);
		void iglTexParameteriv (iGLenum target, iGLenum pname, const iGLint* params);
		void iglTexSubImage2D (iGLenum target, iGLint level, iGLint xoffset, iGLint yoffset, iGLsizei width, iGLsizei height, iGLenum format, iGLenum type, const void* pixels);
		void iglUniform1f (iGLint location, iGLfloat x);
		void iglUniform1fv (iGLint location, iGLsizei count, const iGLfloat* values);
		void iglUniform1i (iGLint location, iGLint x);
		void iglUniform1iv (iGLint location, iGLsizei count, const iGLint* values);
		void iglUniform2f (iGLint location, iGLfloat x, iGLfloat y);
		void iglUniform2fv (iGLint location, iGLsizei count, const iGLfloat* values);
		void iglUniform2i (iGLint location, iGLint x, iGLint y);
		void iglUniform2iv (iGLint location, iGLsizei count, const iGLint* values);
		void iglUniform3f (iGLint location, iGLfloat x, iGLfloat y, iGLfloat z);
		void iglUniform3fv (iGLint location, iGLsizei count, const iGLfloat* values);
		void iglUniform3i (iGLint location, iGLint x, iGLint y, iGLint z);
		void iglUniform3iv (iGLint location, iGLsizei count, const iGLint* values);
		void iglUniform4f (iGLint location, iGLfloat x, iGLfloat y, iGLfloat z, iGLfloat w);
		void iglUniform4fv (iGLint location, iGLsizei count, const iGLfloat* values);
		void iglUniform4i (iGLint location, iGLint x, iGLint y, iGLint z, iGLint w);
		void iglUniform4iv (iGLint location, iGLsizei count, const iGLint* values);
		void iglUniformMatrix2fv (iGLint location, iGLsizei count, iGLboolean transpose, const iGLfloat* value);
		void iglUniformMatrix3fv (iGLint location, iGLsizei count, iGLboolean transpose, const iGLfloat* value);
		void iglUniformMatrix4fv (iGLint location, iGLsizei count, iGLboolean transpose, const iGLfloat* value);
		void iglUseProgram (iGLuint program);
		void iglValidateProgram (iGLuint program);
		void iglVertexAttrib1f (iGLuint index, iGLfloat x);
		void iglVertexAttrib1fv (iGLuint index, const iGLfloat* values);
		void iglVertexAttrib2f (iGLuint index, iGLfloat x, iGLfloat y);
		void iglVertexAttrib2fv (iGLuint index, const iGLfloat* values);
		void iglVertexAttrib3f (iGLuint index, iGLfloat x, iGLfloat y, iGLfloat z);
		void iglVertexAttrib3fv (iGLuint index, const iGLfloat* values);
		void iglVertexAttrib4f (iGLuint index, iGLfloat x, iGLfloat y, iGLfloat z, iGLfloat w);
		void iglVertexAttrib4fv (iGLuint index, const iGLfloat* values);
		void iglVertexAttribPointer (iGLuint index, iGLint size, iGLenum type, iGLboolean normalized, iGLsizei stride, const void* ptr);
		void iglViewport (iGLint x, iGLint y, iGLsizei width, iGLsizei height);
		iGLvoid* iglMapBuffer(iGLenum target, iGLenum access);
		iGLvoid* iglMapBufferRange(iGLenum target, iGLintptr offset, iGLsizeiptr length, iGLbitfield access);
		iGLvoid iglFlushMappedBufferRange(iGLenum target, iGLintptr offset, iGLsizeiptr length);
		iGLboolean iglUnmapBuffer(iGLenum target); 
		void iglBindVertexArray(iGLuint array);
		void iglDeleteVertexArrays(iGLsizei n, const iGLuint* arrays);
		void iglGenVertexArrays(iGLsizei n, iGLuint* arrays);
		iGLboolean iglIsVertexArray(iGLuint array);

		void iglGenQueries(iGLsizei n, iGLuint *ids);
		void iglDeleteQueries(iGLsizei n, const iGLuint *ids);
		iGLboolean iglIsQuery(iGLuint id);
		void iglBeginQuery(iGLenum target, iGLuint id);
		void iglEndQuery(iGLenum target);
		void iglGetQueryiv(iGLenum target, iGLenum pname, iGLint *params);
		void iglGetQueryObjectiv(iGLuint id, iGLenum pname, iGLint *params);
		void iglGetQueryObjectuiv(iGLuint id, iGLenum pname, iGLuint *params);

		void iglTexStorage2D(iGLenum target, iGLsizei levels, iGLenum internalformat, iGLsizei width, iGLsizei height);

		//////////////////////////////////////////////////////////////////////////
		// New helper functions

		//breaks on error. 
		void iassertNoError();

		//returns true/false if error or not
		bool icheckError();

		//returns the last error. Use after ickeckError() returns true
		const char*	iglGetErrorString();

		bool ihasExtension(const char* extension);
		bool ihasExtensionPart(const char* extensionPart);
        
        void iflush();
        void isync();


		//////////////////////////////////////////////////////////////////////////

		Interface();
		TL_Interface* get_interface();

		TL_Interface* m_interface;
	};


	//it's here to be inlined in release
	inline void Interface::iassertNoError()
	{
#		if !defined NDEBUG
			const char* errStr = iglGetErrorString();
			QASSERT_MSG(!errStr, errStr);
#		endif
	}


	//////////////////////////////////////////////////////////////////////////
	// Low level thread local stored data
	// DO not use directly - use it through an Interface object

	class GL_Ext_Functions;

	class TL_Interface : q::util::Noncopyable
	{
	public:
		TL_Interface();
		~TL_Interface();

		const char* get_extensions() const;

		//refreshes the internal state from OpenGL
		void sync();

		//writes the current internal state to OpenGl
		void flush();

		void reset();

		//load extensions. Do not call this explicitly. This is used internally from initForThread()
		void internal_load();


//////////////////////////////////////////////////////////////////////////
// shadow state
//////////////////////////////////////////////////////////////////////////

		GL_Ext_Functions* m_funcs;

		struct GL_State
		{
			GL_State() 
				: blendColor(0.f, 0.f, 0.f, 0.f)
				, clearColor(0.f, 0.f, 0.f, 0.f)
				, colorMask(0xFF, 0xFF, 0xFF, 0xFF)
				, scissorsBox(0, 0, 0, 0)
				, viewport(0, 0, 0, 0)
			{}

			//////////////////////////////////////////////////////////////////////////
			//ANY NEW FIELD ADDED HERE SHOULD BE INITIALIZED PROPERLY IN THE init METHOD
			// also, name the fields appropriately
			//////////////////////////////////////////////////////////////////////////

			bool buffersChanged;
			bool blendChanged;
			bool clearChanged;
			bool flagsChanged;
			bool stencilChanged;


			//void	glActiveTexture(uint32_t texture);
			uint32_t	activeTexture;

			//void	glBindBuffer(uint32_t target, uint32_t buffer);
			uint32_t bindArrayBuffer;
			uint32_t bindElementBuffer;

			//void glBindVertexArray(uint32_t array);
			uint32_t bindVertexArray;

			//void	glBindFramebuffer(uint32_t target, uint32_t framebuffer);
			uint32_t	bindFramebuffer;
			uint32_t	bindFramebufferRead;

			//void	glBindRenderbuffer(uint32_t target, uint32_t renderbuffer);
			uint32_t bindRenderbuffer;

			//void	glBindTexture(uint32_t target, uint32_t texture);
			std::vector<std::pair<uint32_t, bool>> bindTextures;
			int	bindTexturesLastChangedIdx;

			//void	glBlendColor(float red, float green, float blue, float alpha);
			math::vec4f blendColor;

			//void	glBlendEquationSeparate(uint32_t modeRGB, uint32_t modeAlpha);
			uint32_t blendEqRGB;
			uint32_t	blendEqAlpha;
			bool blendEqSeparate;

			//void	glBlendFunc(uint32_t sfactor, uint32_t dfactor);
			//void	glBlendFuncSeparate(uint32_t srcRGB, uint32_t dstRGB, uint32_t srcAlpha, uint32_t dstAlpha);
			uint32_t blendFuncSrcRGB;
			uint32_t	blendFuncDstRGB;
			uint32_t blendFuncSrcAlpha;
			uint32_t blendFuncDstAlpha;
			bool blendFuncSeparate;

			//void	glClearColor(float red, float green, float blue, float alpha);
			math::vec4f clearColor;

			//void	glClearDepthf(float depth);
			float clearDepth;

			//void	glClearStencil(int s);
			int clearStencil;

			//void	glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
			math::vec4<iGLboolean>	colorMask;

			//void	glCullFace(uint32_t mode);
			uint32_t cullFace;

			//void	glDepthFunc(uint32_t func);
			uint32_t depthFunc;

			//void	glDepthMask(GLboolean flag);
			iGLboolean depthMask;

			//void	glDepthRangef(float zNear, float zFar);
			float depthNear;
			float depthFar;

			//void	glDisable(uint32_t cap);
			//void	glEnable(uint32_t cap);
			bool		capsChanged;
			iGLboolean	capBlend;
			iGLboolean	capCullFace;
			iGLboolean	capDepthTest;
			iGLboolean	capDither;
			iGLboolean	capPolygonOffset;
			iGLboolean	capPolygonOffsetLine;
			iGLboolean	capSampleAlphaToCoverage;
			iGLboolean	capSampleCoverage;
			iGLboolean	capScissorTest;
			iGLboolean	capStencilTest;

			//void	glDisableVertexAttribArray(uint32_t index);
			//void	glEnableVertexAttribArray(uint32_t index);
			int			vertexAttribsLastChangedIdx;
			std::vector<uint8_t> vertexAttribs;

			//void	glFrontFace(uint32_t mode);
			uint32_t frontFace;

			//void	glHint(uint32_t target, uint32_t mode);
			uint32_t hintGenerateMipmap;

			//void	glLineWidth(float width);
			float lineWidth;

			//void	glPolygonOffset(float factor, float units);
			float polygonOffsetFactor;
			float polygonOffsetUnits;

			//void	glSampleCoverage(float value, GLboolean invert);
			float sampleCoverageValue;
			iGLboolean sampleCoverageInvert;

			//void	glSciior(int x, int y, int width, int height);
			math::vec4s32 scissorsBox;

			//void	glStencilFuncSeparate(uint32_t face, uint32_t func, int ref, uint32_t mask);
			uint32_t stencilFuncF;
			int stencilFuncRefF;
			uint32_t stencilFuncMaskF;
			uint32_t stencilFuncB;
			int stencilFuncRefB;
			uint32_t stencilFuncMaskB;
			bool stencilFuncSeparate;

			//void	glStencilMaskSeparate(uint32_t face, uint32_t mask);
			uint32_t stencilMaskF;
			uint32_t stencilMaskB;
			bool stencilMaskSeparate;

			//void	glStencilOpSeparate(uint32_t face, uint32_t fail, uint32_t zfail, uint32_t zpass);
			uint32_t stencilOpFailF;
			uint32_t stencilOpZFailF;
			uint32_t stencilOpZPassF;
			uint32_t stencilOpFailB;
			uint32_t stencilOpZFailB;
			uint32_t stencilOpZPassB;
			bool stencilOpSeparate;

			//void iglPixelStorei (iGLenum pname, iGLint param);
			int pixelStoreUnbpackAlignment;

			// void iglDrawBuffers(iGLsizei n, const iGLenum* bufs)
			uint32_t attachmentCount;
			std::array<gles::iGLenum, 4> attachments;
			//void	glUseProgram(uint32_t program);
			uint32_t	program;

			//void	glViewport(int x, int y, int width, int height);
			math::vec4s32 viewport;
		};

		GL_State m_crt_state;
		GL_State m_old_state;

		void* m_context;

        std::string m_extensions;
	};


}
}
}
