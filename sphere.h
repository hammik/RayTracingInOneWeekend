#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable{

public:
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override { //virtual, =0 （純粋仮想関数のoverride, 継承したクラス側で実装をしないといけない）
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;

        if (discriminant < 0) return false;

        auto sqrtd = std::sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || root >= ray_tmax)
        {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || root >= ray_tmax) return false;
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_unit_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_unit_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};


// bool front_face;　//外向きの法線を仮定して，レイと同じ向きなら逆にして，レイと逆向きならそのままにする
// if (dot(ray_direction, outward_normal) > 0.0) {　//レイと法線の向きが同じなら，逆にする
//     // ray is inside the sphere
//     normal = -outward_normal;
//     front_face = false;
// } else {
//     // ray is outside the sphere
//     normal = outward_normal;　//逆向きならそのまま
//     front_face = true;
// }