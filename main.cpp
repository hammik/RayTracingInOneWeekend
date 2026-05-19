#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

double hit_sphere(const point3& sphere_center, double sphere_radius, const ray& r)
{
    vec3 oc = sphere_center - r.origin();
    auto a = dot(r.direction(), r.direction());
    //auto b = 2.0 * dot(oc, r.direction());
    auto h = dot(oc, r.direction());
    auto c = dot(oc, oc) - sphere_radius*sphere_radius;
    auto discriminant = h*h - a*c;
    // return (discriminant >= 0);//判別式が0以上ならば、レイは球と交差している

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - sqrt(discriminant)) / a; //交差点のt値を返す。交差点が複数ある場合は、近い方のt値を返す。
    }
}


color ray_color(const ray& r)
{   
    point3 sphere_center(0, 0, -1);
    double sphere_radius = 0.5;
    // if (hit_sphere(sphere_center, sphere_radius, r)){
    //     return color(1.0, 0.0, 0.0); //赤色
    // }
    auto t = hit_sphere(sphere_center, sphere_radius, r);
    if (t > 0.0)
    {
        point3 p = r.at(t);
        vec3 N = unit_vector(p - sphere_center); //球の表面の法線ベクトル
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1); //法線ベクトルの値は[-1,1]の範囲にあるので、[0,1]の範囲に変換してから、色として返す。
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0)
         + a * color(0.5, 0.7, 1.0);
}

int main()
{
    int image_width = 400;
    auto aspect_ratio = 16.0 / 9.0;

    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);

    //Calculate the vectors across the horizontal and donw the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    // camera_centerを原点として、focal_length分だけ奥に行き、そこからviewport_uの半分とviewport_vの半分を引いた位置が、ビューポートの左上の位置になる。(viewport_uは位置ベクトルではなく、移動量ベクトルであることに注意)
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); //左上のピクセルの位置


    //Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j){
        std::cerr << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i){
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            ray r(camera_center, pixel_center - camera_center);//カメラからピクセルの位置へのベクトルをレイの方向ベクトルとする

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\nDone.\n";    
}