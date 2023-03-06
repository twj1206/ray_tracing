#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "vec3.h"
// point3 = vec3
class sphere : public hittable
{
public:
    sphere() {}
    sphere(vec3 cen, double r) : center(cen), radius(r){};
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    vec3 center; // point3 center;
    double radius;
};
bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    // 因为其实：float b = 2.0 * dot(oc, r.direction());
    // 两边除掉4，就得到下面的函数
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max > root)
            return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    return true;
}
#endif