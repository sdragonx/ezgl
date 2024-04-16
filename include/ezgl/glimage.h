#ifndef _EZGL_IMAGE_H
#define _EZGL_IMAGE_H

#include "public.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STB_IMAGE_STATIC

#if defined(__BORLANDC__) && !defined(__clang__)
#define STBI_NO_SIMD
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC

#include "stb_image.h"
#include "stb_image_write.h"

class glimage
{
private:
    GLuint m_id;

public:
    glimage() : m_id()
    {

    }

    ~glimage()
    {

    }

    // 返回纹理 ID
    GLuint id() const
    {
        return m_id;
    }

    operator GLuint() const
    {
        return m_id;
    }

    // 判断是否为空
    bool is_null() const
    {
        return m_id == GL_NONE;
    }

    // 创建纹理
    int create(
        int width,                          // 纹理宽度
        int height,                         // 纹理高度
        GLenum format,                      // 纹理格式
        const void* sourceData = nullptr,   // 源数据指针
        GLenum sourceFormat = GL_RGBA,      // 源数据格式
        bool mipmaps = false,               // 是否使用 mipmaps
        bool linear = true,                 // 是否使用线性过滤
        bool repeats = false                // 是否使用重复寻址模式
    )
    {
        // 申请id
        glGenTextures(1, &m_id);

        // 绑定id
        glBindTexture(GL_TEXTURE_2D, m_id);

        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, sourceData);

        if (mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);

            // 设置纹理过滤模式
            GLint filter = linear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        }
        else {
            // 设置纹理过滤模式
            GLint filter = linear ? GL_LINEAR : GL_NEAREST;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        }

        // 设置纹理环绕模式
        GLint wrap = repeats ? GL_REPEAT : GL_CLAMP;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        glBindTexture(GL_TEXTURE_2D, GL_NONE);

        return 0;
    }

    // 从文件加载纹理
    int load(
        const std::string& filename,    // 文件名
        bool flip = false,              // 是否上下翻转
        bool mipmaps = false,           // 是否使用 mipmaps
        bool linear = true,             // 是否使用线性过滤
        bool repeats = false            // 是否使用重复寻址模式
    )
    {
        std::vector<uint8_t> buf;
        if (!load_file(buf, filename)) {
            return -1;
        }

        int w, h, channels;

        stbi_set_flip_vertically_on_load(flip ? GL_TRUE : GL_FALSE);

        GLubyte* image = stbi_load_from_memory(
            &buf[0],
            static_cast<int>(buf.size()),
            &w,
            &h,
            &channels,
            0);

        if (image) {
            GLenum format = convertFormat(channels);
            this->create(w, h, format, image, format, mipmaps, linear, repeats);
            stbi_image_free(image);
            printf("glimage> load: \"%s\" %dx%d %d-bit.\n", filename.c_str(), w, h, channels * 8);
            return 0;
        }
        else {
            printf("glimage> load \"%s\" failed.\n", filename.c_str());
            return -1;
        }
    }

    // 从内存数据加载纹理
    int load_memory(
        const void* data,       // 纹理数据
        size_t size,            // 纹理数据大小
        bool flip = false,      // 是否上下翻转
        bool mipmaps = false,   // 是否使用 mipmaps
        bool linear = true,     // 是否使用线性过滤
        bool repeats = false    // 是否使用重复寻址模式
    )
    {
        int w, h, channels;

        stbi_set_flip_vertically_on_load(flip ? GL_TRUE : GL_FALSE);

        GLubyte* image = stbi_load_from_memory(
            static_cast<const stbi_uc*>(data),
            static_cast<int>(size),
            &w,
            &h,
            &channels,
            4);

        if (image) {
            GLenum format = convertFormat(channels);
            this->create(w, h, format, image, format, mipmaps, true, repeats);
            stbi_image_free(image);
            printf("glimage> load data: %dx%d %d-bit.\n", w, h, channels * 8);
            return 0;
        }
        else {
            printf("glimage> load image data failed.\n");
            return -1;
        }
    }

    // 释放
    void dispose()
    {
        if (m_id) {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }

    // 绑定
    void bind(GLint slot = 0) const
    {
        bind(slot, m_id);
    }

    // 绑定纹理到纹理单元。slot = [0, 31]
    static void bind(GLint slot, GLuint id)
    {
        // GL_TEXTURE0~31
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    // 返回纹理宽度
    int width() const
    {
        return getLevelInt(GL_TEXTURE_WIDTH);
    }

    // 返回纹理高度
    int height() const
    {
        return getLevelInt(GL_TEXTURE_HEIGHT);
    }

    // 返回像素格式
    GLenum format() const
    {
        return getLevelInt(GL_TEXTURE_INTERNAL_FORMAT);
    }

    // 保存图片。format = GL_RGB | GL_RGBA
    int save(const std::string& filename, GLenum format = GL_RGBA)
    {
        if (this->is_null()) {
            printf("glimage> image is null.\n");
            return -1;
        }

        if (format != GL_RGB && format != GL_RGBA) {
            printf("glimage> image format error.\n");
            return -1;
        }

        GLint w, h;
        GLint channels = (format == GL_RGB) ? 3 : 4;

        GLuint id = this->pushBind();
        glBindTexture(GL_TEXTURE_2D, m_id);

        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

        void* data = malloc(w * h * channels);
        if (data) {
            // read pixels
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, data);

            // extract extension
            std::string ext = filename.substr(filename.find_last_of('.'));
            std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

            if (ext == ".bmp") {
                stbi_write_bmp(filename.c_str(), w, h, channels, data);
                printf("glimage> save: \"%s\" %dx%d %d-bit\n", filename.c_str(), w, h, channels * 8);
            }
            else if (ext == ".jpg" || ext == ".jpeg") {
                stbi_write_jpg(filename.c_str(), w, h, channels, data, 90);
                printf("glimage> save: \"%s\" %dx%d %d-bit\n", filename.c_str(), w, h, channels * 8);
            }
            else if (ext == ".tga") {
                stbi_write_tga(filename.c_str(), w, h, channels, data);
                printf("glimage> save: \"%s\" %dx%d %d-bit\n", filename.c_str(), w, h, channels * 8);
            }
            else { // default: png format
                stbi_write_png_compression_level = 9;
                stbi_write_png(filename.c_str(), w, h, channels, data, 0);
                printf("glimage> save: \"%s\" %dx%d %d-bit\n", filename.c_str(), w, h, channels * 8);
            }

            free(data);
        }

        this->popBind(id);

        return 0;
    }

private:
    // stb_image 格式转 OpenGL 纹理格式
    GLenum convertFormat(int channels)
    {
        switch (channels) {
        case STBI_grey:
            return GL_ALPHA;
        case STBI_grey_alpha:
            return GL_LUMINANCE_ALPHA;
        case STBI_rgb:
            return GL_RGB;
        case STBI_rgb_alpha:
            return GL_RGBA;
        default:
            assert(0);  // error
            break;
        }
    }

    GLuint pushBind() const
    {
        GLint id;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        return static_cast<GLuint>(id);
    }

    void popBind(GLuint id) const
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    int getLevelInt(GLenum e) const
    {
        if (m_id) {
            GLint n = 0;
            GLuint id = this->pushBind();
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, e, &n);
            this->popBind(id);
            return static_cast<int>(n);
        }
        return 0;
    }
};

#endif // _EZGL_IMAGE_H
