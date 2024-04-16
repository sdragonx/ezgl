#ifndef _EZGL_MESH_H
#define _EZGL_MESH_H

#include "aabb3.h"
#include "vertex.h"

extern void draw_arrays(int shape, const vertex* vs, size_t pos, size_t size);
extern void draw_elements(int shape, const vertex* vs, const uint32_t* indices, size_t size);

//
// Mesh
//

class Mesh
{
public:
    GLenum primitive;
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;

public:
    Mesh() :
        primitive(GL_NONE),
        vertices(),
        indices()
    {
    }

    void dispose()
    {
        primitive = GL_NONE;
        vertices.clear();
        indices.clear();
    }

    // »æÖÆ
    void draw()
    {
        if (indices.empty()) {
            draw_arrays(primitive, vertices.data(), 0, vertices.size());
        }
        else {
            draw_elements(primitive, vertices.data(), indices.data(), indices.size());
        }
    }

    // ·µ»Ø°üÎ§ºÐ
    aabb3f bounding_box() const
    {
        aabb3f box;
        for (size_t i = 0; i < vertices.size(); ++i) {
            box.append(vertices[i].position);
        }
        return box;
    }
};

#endif // _EZGL_MESH_H
