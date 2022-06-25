#pragma once

#include <string>

namespace Stella
{

enum class TextureWrap
{
	REPEAT,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE
};

enum class TextureFilter
{
	LINEAR,
	NEAREST,
	NEAREST_MIPMAP_NEAREST,
	LINEAR_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR
};

enum class TextureFormat
{
	RGB,
	RGBA
};

enum class TextureDataType
{
	UNSIGNED_BYTE
};

struct Texture2DInfo
{
	TextureWrap m_wrap;
	TextureFilter m_minFilter;
	TextureFilter m_magFilter;
	TextureFormat m_internalFormat;
	bool m_generateMipMap;
	bool m_flip;
	std::string m_fileName;
	TextureFormat m_dataFormat;
	TextureDataType m_dataType;
	uint8_t * m_pData;
	uint32_t m_width;
	uint32_t m_height;

	Texture2DInfo(
		TextureWrap wrap, 
		TextureFilter min_filter, 
		TextureFilter mag_filter, 
		TextureFormat internal_format, 
		TextureFormat data_format, 
		TextureDataType data_type, 
		bool mipmap, 
		bool flip,
		std::string fileName
	) : m_wrap(wrap), m_minFilter(min_filter), m_magFilter(mag_filter), 
		m_internalFormat(internal_format), m_dataFormat(data_format), m_dataType(data_type),
		m_generateMipMap(mipmap), m_flip(flip), m_fileName(fileName), m_pData(nullptr) { }

	Texture2DInfo(
		TextureWrap wrap, 
		TextureFilter min_filter, 
		TextureFilter mag_filter, 
		TextureFormat internal_format, 
		TextureFormat data_format, 
		TextureDataType data_type,
		bool mipmap, 
		bool flip, 
		uint32_t w, uint32_t h, 
		uint8_t * p_data
	) : m_wrap(wrap), m_minFilter(min_filter), m_magFilter(mag_filter), 
		m_width(w), m_height(h), m_internalFormat(internal_format), m_dataFormat(data_format), m_dataType(data_type),
		m_generateMipMap(mipmap), m_flip(flip), m_pData(p_data) { }
};

class Texture2D
{

public:

	Texture2D() = default;

	static bool create(Texture2D * p_tex, const Texture2DInfo & info);
	static void destroy(Texture2D * p_tex);

	void bind();
	void unbind();

private:

	static int to_render_api_wrap(TextureWrap wrapping);
	static int to_render_api_filter(TextureFilter filter);
	static int to_render_api_format(TextureFormat format);
	static int to_render_api_data_type(TextureDataType type);

	static uint8_t texture_format_channel_count(TextureFormat format);
	static void flip_vertical(uint32_t width, uint32_t height, uint32_t channels, uint8_t * to_flip, uint8_t * out);

private:

	uint32_t m_id;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_channels;
};

    
} // namespace Stella
