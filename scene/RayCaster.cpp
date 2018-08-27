#include <algorithm>
#include <functional>

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

        std::sort(hits.begin(), hits.end(), [](const Hit& a, const Hit& b){
            return a.Distance <= b.Distance;
        });

        return hitFound;
    }

    Bool RayCaster::castRay(const Ray& ray, WeakPointer<Mesh> mesh, const Matrix4x4& transform, std::vector<Hit>& hits) {
        Matrix4x4 inverse = transform;
        inverse.invert();
        
        Matrix4x4 inverseTranspose = inverse;
        inverseTranspose.transpose();

        Matrix4x4 inverseInverseTranspose = inverseTranspose;
        inverseInverseTranspose.invert();

        Ray localRay(ray.Origin, ray.Direction);
        inverse.transform(localRay.Origin);
        inverse.transform(localRay.Direction);

        Hit bbHit;
        Bool bbIntersect = localRay.intersectBox(mesh->getBoundingBox(), bbHit);

        UInt32 startIndex = hits.size();
        if (bbIntersect) {
            localRay.intersectMesh(mesh, hits);
        }

        for(UInt32 i = startIndex; i< hits.size(); i++) {
            Hit& hit = hits[i];
            transform.transform(hit.Origin);
            inverseTranspose.transform(hit.Normal);
            Vector3r distanceVec = hit.Origin - ray.Origin;
            hit.Distance = distanceVec.magnitude();
        }

        return hits.size() > 0;
    }
}