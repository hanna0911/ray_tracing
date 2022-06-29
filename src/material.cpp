#include "material.hpp"
#include "texture.hpp"
#include "hit.hpp"
#include "ray.hpp"


Vector3f Material::Shade(const Ray &Ray, const Hit &hit,
                const Vector3f &dirToLight, const Vector3f &lightColor) {
    Vector3f shaded = Vector3f::ZERO;
    // 
    Vector3f N = hit.getNormal(); N.normalize(); // bug solved: 单位化!
    Vector3f L = dirToLight; L.normalize();
    Vector3f V = -Ray.getDirection(); V.normalize();
    Vector3f R = 2 * N.Vector3f::dot(N, L) * N - L; R.normalize();
    float diffuse = L.Vector3f::dot(L, N), specular = V.Vector3f::dot(V, R);
    if(diffuse < 0) diffuse = 0;
    if(specular < 0) specular = 0;
    else specular = pow(specular, shininess);
    shaded = lightColor * (diffuseColor * diffuse + specularColor * specular);
    return shaded;
}

bool Material::scatter(
    const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
) const {
    auto scatter_direction = rec.normal + random_unit_vector();
    
    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction, r_in.time());
    attenuation = Vector3f(1.0f);
    return true;
}

bool metal::scatter(
    const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
) const {
    Vector3f reflected = reflect((r_in.getDirection()).normalized(), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
    attenuation = albedo;
    return (Vector3f::dot(scattered.getDirection(), rec.normal) > 0);
}

bool dielectric::scatter(
        const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
    ) const {
    attenuation = Vector3f(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

    Vector3f unit_direction = (r_in.getDirection()).normalized();
    
    double cos_theta = fmin(Vector3f::Vector3f::dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3f direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = Ray(rec.p, direction, r_in.time());

    return true;
}

lambertian::lambertian(const Vector3f& a) : albedo(make_shared<solid_color>(a)) {}

Vector3f diffuse_light::emitted(double u, double v, const Vector3f& p) const {
    return emit->value(u, v, p);
}

bool lambertian::scatter(
            const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
        ) const {
    auto scatter_direction = rec.normal + random_unit_vector();
    
    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
        scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}

diffuse_light::diffuse_light(Vector3f c) : emit(make_shared<solid_color>(c)) {}

isotropic::isotropic(Vector3f c) : albedo(make_shared<solid_color>(c)) {}

bool isotropic::scatter(
    const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
) const {
    scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}