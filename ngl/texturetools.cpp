#include <algorithm>
#include <cstdio>

#include "gl.h"
#include "texturetools.h"

class ScopedFclose {
public:
    ScopedFclose(FILE *fp) : fp(fp) {}
    ~ScopedFclose() { fclose(fp); }
private:
    FILE *fp;
};

TEXTURE* newTexture(const uint16_t w, const uint16_t h, const COLOR fill, const bool transparent, const COLOR transparent_color)
{
    // TODO: Don't leak on exception
    TEXTURE *ret = new TEXTURE;

    ret->width = w;
    ret->height = h;
    if(fill == 0)
        ret->bitmap = new COLOR[w * h]{fill};
    else
    {
        ret->bitmap = new COLOR[w * h];
        std::fill(ret->bitmap, ret->bitmap + w * h, fill);
    }

    ret->has_transparency = transparent;
    ret->transparent_color = transparent_color;

    return ret;
}

void deleteTexture(TEXTURE *tex)
{
    delete[] tex->bitmap;
    delete tex;
}

void copyTexture(const TEXTURE &src, TEXTURE &dest)
{
#ifndef NDEBUG
    if(src.width != dest.width || src.height != dest.height)
    {
        puts("Error: textures don't have the same resolution!");
        return;
    }
#endif
    std::copy(src.bitmap, src.bitmap + src.width*src.height, dest.bitmap);
}

struct RGB24 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __attribute__((packed));

#if 0
static bool skip_space(FILE *file)
{
    char c;
    bool in_comment = false;
    do {
        int i = getc(file);
        if(i == EOF)
            return false;
        if(i == '#')
            in_comment = true;
        if(i == '\n')
            in_comment = false;
        c = i;
    } while(in_comment || c == '\n' || c == '\r' || c == '\t' || c == ' ');

    ungetc(c, file);
    return true;
}
#endif

//PPM-Loader without support for ascii
TEXTURE* loadTextureFromFile(const char* filename)
{
#if 0
    FILE *texture_file = fopen(filename, "rb");
    if(!texture_file)
        return nullptr;

    ScopedFclose fc(texture_file);

    char magic[3];
    magic[2] = 0;
    unsigned int width, height, pixel_max, pixels;
    RGB24 *buffer, *ptr24;
    uint16_t *ptr16;
    TEXTURE *texture = nullptr;

    if(fread(magic, 1, 2, texture_file) != 2 || strcmp(magic, "P6"))
        return texture;

    if(!skip_space(texture_file))
        return texture;

    if(fscanf(texture_file, "%u", &width) != 1)
        return texture;

    if(!skip_space(texture_file))
        return texture;

    if(fscanf(texture_file, "%u", &height) != 1)
        return texture;

    if(!skip_space(texture_file))
        return texture;

    if(fscanf(texture_file, "%u", &pixel_max) != 1 || pixel_max != 255)
        return texture;

    if(!skip_space(texture_file))
        return texture;

    texture = newTexture(width, height);
    if(!texture)
        return texture;

    pixels = width * height;
    buffer = new RGB24[pixels];

    if(fread(buffer, sizeof(RGB24), pixels, texture_file) != pixels)
    {
        delete[] buffer;
        return texture;
    }

    //Convert to RGB565
    ptr24 = buffer;
    ptr16 = texture->bitmap;
    while(pixels--)
    {
        *ptr16 = (ptr24->r & 0b11111000) << 8 | (ptr24->g & 0b11111100) << 3 | (ptr24->b & 0b11111000) >> 3;
        ptr24++;
        ptr16++;
    }

    delete[] buffer;
    return texture;
#else
	return NULL;
#endif
}

bool saveTextureToFile(const TEXTURE &texture, const char *filename)
{
#if 0
    FILE *f = fopen(filename, "wb");
    if(!f)
        return false;

    ScopedFclose fc(f);

    if(fprintf(f, "P6 %d %d %d ", texture.width, texture.height, 255) < 0)
        return false;

    unsigned int pixels = texture.width * texture.height;
    RGB24 *buffer24 = new RGB24[pixels];

    //Convert to RGB24
    RGB24 *ptr24 = buffer24;
    COLOR *ptr16 = texture.bitmap;
    while(pixels--)
    {
        ptr24->r = (*ptr16 & 0b1111100000000000) >> 8;
        ptr24->g = (*ptr16 & 0b0000011111100000) >> 3;
        ptr24->b = (*ptr16 & 0b0000000000011111) << 3;
        ++ptr24;
        ++ptr16;
    }

    bool ret = fwrite(buffer24, sizeof(RGB24), texture.width * texture.height, f) == static_cast<unsigned int>(texture.width) * texture.height;

    delete[] buffer24;

    return ret;
#else
	return false;
#endif
}

TextureAtlasEntry textureArea(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h)
{
    return TextureAtlasEntry{
        .left = x,
        .right = x+w,
        .top = y,
        .bottom = y+h,
    };
}

void drawTexture(const TEXTURE &src, TEXTURE &dest,
				 uint16_t src_x, uint16_t src_y, uint16_t src_w, uint16_t src_h,
				 uint16_t dest_x, uint16_t dest_y, uint16_t dest_w, uint16_t dest_h)
{
	#ifndef NDEBUG
	if(src_x + src_w > src.width || src_y + src_h > src.height || dest_x + dest_w > dest.width || dest_y + dest_h > dest.height)
		return;
	#endif
	
	uint16_t *dest_ptr = dest.bitmap + dest_x + dest_y * dest.width;
	const unsigned int dest_nextline = dest.width - dest_w;
	
	//Special cases, for better performance
	if(src_w == dest_w && src_h == dest_h)
	{
		dest_w = std::min(src_w, static_cast<uint16_t>(dest.width - dest_x));
		dest_h = std::min(src_h, static_cast<uint16_t>(dest.height - dest_y));
		
		const uint16_t *src_ptr = src.bitmap + src_x + src_y * src.width;
		const unsigned int src_nextline = src.width - dest_w;
		
		if(!src.has_transparency)
		{
			for(unsigned int i = dest_h; i--;)
			{
				for(unsigned int j = dest_w; j--;)
					*dest_ptr++ = *src_ptr++;
				
				dest_ptr += dest_nextline;
				src_ptr += src_nextline;
			}
		}
		else
		{
			for(unsigned int i = dest_h; i--;)
			{
				for(unsigned int j = dest_w; j--;)
				{
					uint16_t c = *src_ptr++;
					if(c != src.transparent_color)
						*dest_ptr = c;
					
					++dest_ptr;
				}
				
				dest_ptr += dest_nextline;
				src_ptr += src_nextline;
			}
		}
		
		return;
	}
	
	const GLFix dx_src = GLFix(src_w) / dest_w, dy_src = GLFix(src_h) / dest_h;
	uint16_t *dest_line_end = std::min(dest_ptr + dest_w, dest.bitmap + dest.width * dest.height);
	const uint16_t *dest_ptr_end = std::min(dest_ptr + dest.width * dest_h, dest.bitmap + dest.height * dest.width);
	
	GLFix src_fx = src_x, src_fy = src_y;
	
	if(!src.has_transparency)
	{
		while(dest_ptr < dest_ptr_end)
		{
			src_fx = src_x;
			
			while(dest_ptr < dest_line_end)
			{	
				*dest_ptr++ = src.bitmap[(src_fy.floor() * src.width) + src_fx.floor()];
				
				src_fx += dx_src;
			}
			
			dest_ptr += dest_nextline;
			dest_line_end += dest.width;
			src_fy += dy_src;
		}
	}
	else
	{
		while(dest_ptr < dest_ptr_end)
		{
			src_fx = src_x;
			
			while(dest_ptr < dest_line_end)
			{
				uint16_t c = src.bitmap[(src_fy.floor() * src.width) + src_fx.floor()];
				if(c != src.transparent_color)
					*dest_ptr = c;
				
				src_fx += dx_src;
				++dest_ptr;
			}
			
			dest_ptr += dest_nextline;
			dest_line_end += dest.width;
			src_fy += dy_src;
		}
	}
}

void drawTextureOverlay(const TEXTURE &src, const unsigned int src_x, const unsigned int src_y, TEXTURE &dest, const unsigned int dest_x, const unsigned int dest_y, unsigned int w, unsigned int h)
{
#ifndef LOWEND
#ifndef NDEBUG
    if(dest_x >= dest.width || dest_y >= dest.height)
        return;

    if(src_x >= src.width || src_y >= src.height)
        return;
#endif
    // Clip
    w = std::min(w, dest.width - dest_x);
    h = std::min(h, dest.height - dest_y);
    w = std::min(w, src.width - src_x);
    h = std::min(h, src.height - src_y);

    COLOR *dest_ptr = dest.bitmap + dest_x + dest_y * dest.width;
    const COLOR *src_ptr = src.bitmap + src_x + src_y * src.width;
    const unsigned int nextline_dest = dest.width - w, nextline_src = src.width - w;

    for(unsigned int i = h; i--;)
    {
        for(unsigned int j = w ; j--;)
        {
            const COLOR srcc = *src_ptr++;
            COLOR *dest = dest_ptr++;

            if(src.has_transparency && srcc == src.transparent_color)
                continue;

#ifdef BGR_TEXTURE
            const unsigned int r_o = (*dest >> 0) & 0b11111;
            const unsigned int g_o = (*dest >> 5) & 0b11111;
            const unsigned int b_o = (*dest >> 10) & 0b11111;

            const unsigned int r_n = (srcc >> 0) & 0b11111;
            const unsigned int g_n = (srcc >> 5) & 0b11111;
            const unsigned int b_n = (srcc >> 10) & 0b11111;
            //Generate 50% opacity
            const unsigned int r = (r_n + r_o) >> 1;
            const unsigned int g = (g_n + g_o) >> 1;
            const unsigned int b = (b_n + b_o) >> 1;

            *dest = (r << 0) | (g << 5) | (b << 10);
#elif defined(N64)
			/* No alpha transparency because i can't figure out that shit and besides, it has a small performance penalty */
            const unsigned int r = (srcc >> 10) & 0b11111;
            const unsigned int g = (srcc >> 5) & 0b11111;
            const unsigned int b = (srcc >> 0) & 0b11111;
            *dest = (r << 11) | (g << 6) | (b << 1) | (127 >> 7);
#else
            const unsigned int r_o = (*dest >> 11) & 0b11111;
            const unsigned int g_o = (*dest >> 5) & 0b111111;
            const unsigned int b_o = (*dest >> 0) & 0b11111;

            const unsigned int r_n = (srcc >> 11) & 0b11111;
            const unsigned int g_n = (srcc >> 5) & 0b111111;
            const unsigned int b_n = (srcc >> 0) & 0b11111;
            //Generate 50% opacity
            const unsigned int r = (r_n + r_o) >> 1;
            const unsigned int g = (g_n + g_o) >> 1;
            const unsigned int b = (b_n + b_o) >> 1;

            *dest = (r << 11) | (g << 5) | (b << 0);
#endif
        }

        dest_ptr += nextline_dest;
        src_ptr += nextline_src;
    }
#endif
}

TEXTURE* resizeTexture(const TEXTURE &src, const uint16_t w, const uint16_t h)
{
    TEXTURE *ret = newTexture(w, h);

	// Gameblabla - Is this really necesarry ?
#ifndef NDEBUG
    if(w == src.width && h == src.height)
    {
        copyTexture(src, *ret);
        return ret;
    }
#endif

    COLOR *ptr = ret->bitmap;

    for(unsigned int dsty = 0; dsty < h; dsty++)
        for(unsigned int dstx = 0; dstx < w; dstx++)
            *ptr++ = src.bitmap[(dstx * src.width / w) + (dsty * src.height / h) * src.width];

    return ret;
}

void greyscaleTexture(TEXTURE &tex)
{
    unsigned int pixels = tex.width * tex.height;
    COLOR *ptr16 = tex.bitmap;
    while(pixels--)
    {
        const RGB rgb = rgbColor(*ptr16);
        //Somehow only the lowest 5 bits are used
        *ptr16 = (rgb.r.value + rgb.g.value + rgb.g.value + rgb.b.value) >> 5;
        ptr16++;
    }
}

void drawRectangle(TEXTURE &tex, const unsigned int x, const unsigned int y, unsigned int w, unsigned int h, const COLOR c)
{
#ifndef NDEBUG
    if(x >= tex.width || y >= tex.height || w < 2 || h < 2)
        return;
#endif
    w = std::min(w, tex.width - x);
    h = std::min(h, tex.height - y);

    //Draw top and bottom lines
    unsigned int w1 = w;
    COLOR *line_start_top = tex.bitmap + y * tex.width + x,
            *line_start_bot = tex.bitmap + (y+h-1) * tex.width + x;
    while(w1--)
    {
        *line_start_top++ = c;
        *line_start_bot++ = c;
    }

    //Draw left and right lines
    unsigned int h1 = h - 2; //Already drew top and bottom pixels
    line_start_top = tex.bitmap + y * tex.width + x;
    line_start_bot = tex.bitmap + y * tex.width + x + w - 1;
    while(h1--)
    {
        *(line_start_top += tex.width) = c;
        *(line_start_bot += tex.width) = c;
    }
}
