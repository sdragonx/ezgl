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

    // �������� ID
    GLuint id() const
    {
        return m_id;
    }

    operator GLuint() const
    {
        return m_id;
    }

    // �ж��Ƿ�Ϊ��
    bool is_null() const
    {
        return m_id == GL_NONE;
    }

    // ��������
    int create(
        int width,                          // ������
        int height,                         // ����߶�
        GLenum format,                      // �����ʽ
        const void* sourceData = nullptr,   // Դ����ָ��
        GLenum sourceFormat = GL_RGBA,      // Դ���ݸ�ʽ
        bool mipmaps = false,               // �Ƿ�ʹ�� mipmaps
        bool linear = true,                 // �Ƿ�ʹ�����Թ���
        bool repeats = false                // �Ƿ�ʹ���ظ�Ѱַģʽ
    )
    {
        // ����id
        glGenTextures(1, &m_id);

        // ��id
        glBindTexture(GL_TEXTURE_2D, m_id);

        // ��������
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, sourceFormat, GL_UNSIGNED_BYTE, sourceData);

        if (mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);

            // �����������ģʽ
            GLint filter = linear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        }
        else {
            // �����������ģʽ
            GLint filter = linear ? GL_LINEAR : GL_NEAREST;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        }

        // ����������ģʽ
        GLint wrap = repeats ? GL_REPEAT : GL_CLAMP;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

        glBindTexture(GL_TEXTURE_2D, GL_NONE);

        return 0;
    }

    // ���ļ���������
    int load(
        const std::string& filename,    // �ļ���
        bool flip = false,              // �Ƿ����·�ת
        bool mipmaps = false,           // �Ƿ�ʹ�� mipmaps
        bool linear = true,             // �Ƿ�ʹ�����Թ���
        bool repeats = false            // �Ƿ�ʹ���ظ�Ѱַģʽ
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

    // ���ڴ����ݼ�������
    int load_memory(
        const void* data,       // ��������
        size_t size,            // �������ݴ�С
        bool flip = false,      // �Ƿ����·�ת
        bool mipmaps = false,   // �Ƿ�ʹ�� mipmaps
        bool linear = true,     // �Ƿ�ʹ�����Թ���
        bool repeats = false    // �Ƿ�ʹ���ظ�Ѱַģʽ
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

    // �ͷ�
    void dispose()
    {
        if (m_id) {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }

    // ��
    void bind(GLint slot = 0) const
    {
        bind(slot, m_id);
    }

    // ����������Ԫ��slot = [0, 31]
    static void bind(GLint slot, GLuint id)
    {
        // GL_TEXTURE0~31
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    // ����������
    int width() const
    {
        return getLevelInt(GL_TEXTURE_WIDTH);
    }

    // ��������߶�
    int height() const
    {
        return getLevelInt(GL_TEXTURE_HEIGHT);
    }

    // �������ظ�ʽ
    GLenum format() const
    {
        return getLevelInt(GL_TEXTURE_INTERNAL_FORMAT);
    }

    // ����ͼƬ��format = GL_RGB | GL_RGBA
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
    // stb_image ��ʽת OpenGL �����ʽ
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
