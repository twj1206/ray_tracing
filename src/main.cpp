#include <iostream>
#include <fstream>
#include "color.h"
#include "vec3.h"
#include "ray.h"
using namespace std;

// 判定射线与球的交点  求方根公式
double hit_sphere(const point3 &center, double radius, const ray &r)
{
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
// 计算插值颜色
color ray_color(const ray &r)
{ // 球心在（0,0，-1），半径为0.5
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0) // 相交
    {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        cout << "eucces";
        return 0.5 * color(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0);
    }
    // 背景色
    vec3 unit_direction = unit_vector(r.direction());                   // 单位化
    t = 0.5 * (unit_direction.y() + 1.0);                               // 将y分量映射到[0, 1]
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // 插值公式 白色&蓝色
}
int main()
{
    ofstream file("image.ppm", ios::binary | ios::out);
    // image size
    const int image_width = 400;
    const int image_height = 200;

    // Render
    file << "P3\n"
         << image_width << " " << image_height << "\n255\n";

    // Camera
    vec3 lower_left_corner(-2.0, -1.0, -1.0); // 左下角作为开始位置
    vec3 horizontal(4.0, 0.0, 0.0);           // 屏幕水平宽度
    vec3 vertical{0, 2.0, 0};                 // 屏幕垂直高度
    vec3 origin(0, 0, 0);                     // 眼睛位置
    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / (image_width);
            auto v = double(j) / (image_height);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);

            vec3 pixel_color = ray_color(r); // 得到插值颜色（rgb)
            int ir = static_cast<int>(255.999 * pixel_color[0]);
            int ig = static_cast<int>(255.999 * pixel_color[1]);
            int ib = static_cast<int>(255.999 * pixel_color[2]);
            file << ir << " " << ig << " " << ib << endl;
        }
    }
    file.close();
}