#ifndef _EZGL_AABB3_H
#define _EZGL_AABB3_H

#include "public.h"

template<typename T>
class aabb3
{
public:
    typedef aabb3 this_type;
    typedef T value_type;
    typedef glm::vec<3, value_type> vec3;

public:
    vec3 a;
    vec3 b;

public:
    aabb3() :
        a(std::numeric_limits<value_type>::max()),
        b(-std::numeric_limits<value_type>::max())
    {
    }
    
    aabb3(const vec3& a, const vec3& b)
    {
        reset();
        append(a);
        append(b);
    }
    
    aabb3(const std::vector<vec3>& points)
    {
        this->assign(points);
    }

    value_type width()const
    {
        return b.x - a.x;
    }

    value_type height()const
    {
        return b.y - a.y;
    }

    value_type depth()const
    {
        return b.z - a.z;
    }

    vec3 position()const
    {
        return a;
    }

    vec3 size()const
    {
        return b - a;
    }

    vec3 center()const
    {
        return glm::mix(a, b, T(0.5));
    }

    void reset()
    {
        a = vec3(std::numeric_limits<value_type>::max());
        b = vec3(-std::numeric_limits<value_type>::max());
    }

    bool is_valid()const
    {
        return a.x < b.x
            && a.y < b.y
            && a.z < b.z;
    }

    void append(value_type vx, value_type vy, value_type vz)
    {
        a.x = std::min(a.x, vx);
        a.y = std::min(a.y, vy);
        a.z = std::min(a.z, vz);

        b.x = std::max(b.x, vx);
        b.y = std::max(b.y, vy);
        b.z = std::max(b.z, vz);
    }

    void append(const vec3& v)
    {
        this->append(v.x, v.y, v.z);
    }

    void append(const vec3* points, size_t size)
    {
        for (size_t i = 0; i < size; ++i) {
            this->append(points[i].x, points[i].y, points[i].z);
        }
    }

    void append(const std::vector<vec3>& points)
    {
        for (size_t i = 0; i < points.size(); ++i) {
            this->append(points[i].x, points[i].y, points[i].z);
        }
    }

    void expand(value_type n)
    {
        a -= n;
        b += n;
    }

    bool contains(T vx, T vy, T vz)const
    {
        return vx >= a.x && vx < b.x
            && vy >= a.y && vy < b.y
            && vz >= a.z && vz < b.z;
    }

    bool contains(const vec3& v)const
    {
        return this->contains(v.x, v.y, v.z);
    }

    this_type operator&(const this_type& other)const
    {
        this_type temp;

        temp.a = vec3(
            std::max(a.x, other.a.x),
            std::max(a.y, other.a.y),
            std::max(a.z, other.a.z)
        );

        temp.b = vec3(
            std::min(b.x, other.b.x),
            std::min(b.y, other.b.y),
            std::min(b.z, other.b.z)
        );

        return temp;
    }

    this_type& operator&=(const this_type& other)
    {
        a.x = std::max(a.x, other.a.x);
        a.y = std::max(a.y, other.a.y);
        a.z = std::max(a.z, other.a.z);

        b.x = std::min(b.x, other.b.x);
        b.y = std::min(b.y, other.b.y);
        b.z = std::min(b.z, other.b.z);

        return *this;
    }

    this_type operator|(const this_type& other)const
    {
        this_type temp;

        temp.a = vec3(
            std::min(a.x, other.a.x),
            std::min(a.y, other.a.y),
            std::min(a.z, other.a.z)
        );

        temp.b = vec3(
            std::max(b.x, other.b.x),
            std::max(b.y, other.b.y),
            std::max(b.z, other.b.z)
        );

        return temp;
    }

    this_type& operator|=(const this_type& other)
    {
        a.x = std::min(a.x, other.a.x);
        a.y = std::min(a.y, other.a.y);
        a.z = std::min(a.z, other.a.z);

        b.x = std::max(b.x, other.b.x);
        b.y = std::max(b.y, other.b.y);
        b.z = std::max(b.z, other.b.z);

        return *this;
    }

    bool intersect(const this_type& other)const
    {
        return ((*this) & other).is_valid();
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const aabb3<T>& aabb)
{
    out << aabb.a << " - " << aabb.b;
    return out;
}

typedef aabb3<int>   aabb3i;
typedef aabb3<float> aabb3f;

#endif // _EZGL_AABB3_H
