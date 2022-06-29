#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include "utils.hpp"
#include <iostream>
#include <cmath>

class Ray;
class Hit;
class texture;
class solid_color;
class noise_texture;
class image_texture;

// TODO: Implement Shade function that computes Phong introduced in class.
class Material {
public:

    Material() {}
    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseVector3f() const {
        return diffuseColor;
    }


    Vector3f Shade(const Ray &Ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor);
                   
    // added function
    virtual Vector3f emitted(double u, double v, const Vector3f& p) const {
        return Vector3f(0,0,0);
    }

    virtual bool scatter(
        const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
    ) const ;

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


// added materials

class lambertian : public Material {
    public:
        lambertian(const Vector3f& a);
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
        ) const;

    public:
        shared_ptr<texture> albedo;
};

class metal : public Material {
    public:
        metal(const Vector3f& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
        ) const;

    public:
        Vector3f albedo;
        double fuzz;
};

class dielectric : public Material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
        ) const;

    public:
        double ir; // Index of Refraction

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

class diffuse_light : public Material  {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(Vector3f c);

        virtual bool scatter(
            const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
        ) const {
            return false;
        }

        virtual Vector3f emitted(double u, double v, const Vector3f& p) const override;

    public:
        shared_ptr<texture> emit;
};

class isotropic : public Material {
    public:
        isotropic(Vector3f c);
        isotropic(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const Ray& r_in, const Hit& rec, Vector3f& attenuation, Ray& scattered
        ) const;

    public:
        shared_ptr<texture> albedo;
};


#endif // MATERIAL_H
