#include "RenderUtils.h"
#include "../light/PointLight.h"
#include "../geometry/Mesh.h"
#include "../math/Quaternion.h"

namespace Core {

    Bool RenderUtils::isPointLightInRangeOfMesh(WeakPointer<PointLight> pointLight, WeakPointer<Mesh> mesh, WeakPointer<Object3D> meshOwner) {
        static Point3r pointLightPos;
        pointLightPos.set(0.0f, 0.0f, 0.0f);
        pointLight->getOwner()->getTransform().applyTransformationTo(pointLightPos);
        return RenderUtils::isPointLightInRangeOfMesh(pointLightPos, pointLight->getRadius(), mesh, meshOwner);
    }

    Bool RenderUtils::isPointLightInRangeOfMesh(const Point3r& pointLightPosition, Real radius, WeakPointer<Mesh> mesh, WeakPointer<Object3D> meshOwner) {
        Vector4r boundingSphere = mesh->getBoundingSphere();
        Point3r boundingSphereCenter(boundingSphere.x, boundingSphere.y, boundingSphere.z);
        meshOwner->getTransform().applyTransformationTo(boundingSphereCenter);
        Vector3r centerToLight = pointLightPosition - boundingSphereCenter;
        Real distance = centerToLight.magnitude();

        static Point3r pos;
        static Quaternion rot;
        static Point3r scale;
        Matrix4x4 meshWorldMatrix = meshOwner->getTransform().getWorldMatrix();
        meshWorldMatrix.decompose(pos, rot, scale);
        Real maxScale = Math::max(Math::max(scale.x, scale.y), scale.z);

        return distance <= boundingSphere.w * maxScale + radius;
    }
}
