#include "RayCaster.h"
#include "../geometry/Mesh.h"

namespace Core {

    void RayCaster::addObject(WeakPointer<Object3D> sceneObject, WeakPointer<Mesh> mesh) {
        this->objects.push_back(sceneObject);
        this->meshes.push_back(mesh);
    }

    Bool RayCaster::castRay(const Ray& ray, std::vector<Hit>& hits) {
        if (this->objects.size() != this->meshes.size()) {
            throw Exception("RayCaster::castRay() -> 'meshes' and 'objects' have different sizes.");
        }

        Bool hitFound = false;
        for (UInt32 i = 0; i < this->objects.size(); i++) {
            WeakPointer<Object3D> object = this->objects[i];
            WeakPointer<Mesh> mesh = this->meshes[i];
            Transform objTransform = object->getTransform();
            objTransform.updateWorldMatrix();
            Matrix4x4 transform = objTransform.getWorldMatrix();
            hitFound = this->castRay(ray, mesh, transform, hits) || hitFound;
        }

        return hitFound;
    }

    Bool RayCaster::castRay(const Ray& ray, WeakPointer<Mesh> mesh, const Matrix4x4& transform, std::vector<Hit>& hits) {
        Matrix4x4 inverse = transform;
        inverse.invert();
        Matrix4x4 inverseTranspose = inverse;
        inverseTranspose.transpose();
        Ray localRay(ray.Origin, ray.Direction);
        inverse.transform(localRay.Origin);
        Point3r rayEnd = ray.Origin + ray.Direction;
        inverse.transform(rayEnd);
        localRay.Direction = rayEnd - localRay.Origin;
        Hit bbHit;
        Bool bbIntersect = localRay.intersectBox(mesh->getBoundingBox(), bbHit);
        if (bbIntersect) {
            localRay.intersectMesh(mesh, hits);
        }
        return hits.size() > 0;
    }
}