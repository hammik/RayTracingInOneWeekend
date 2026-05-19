#pragma once

#include "ray.h"

class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_unit_normal)
    {
        front_face = dot(r.direction(), outward_unit_normal) < 0;
        normal = front_face ? outward_unit_normal : -outward_unit_normal;
    }
};

class hittable {
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0; // virtual -> 基底クラスの型で子クラスの関数を呼び出せる（animalで犬や猫が鳴くと呼び出したい．（関数自体の実装はそれぞれの子クラスによって行われる。ワンやニャーなどのように））
};