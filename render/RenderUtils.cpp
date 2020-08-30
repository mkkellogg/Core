#include "RenderUtils.h"
#include "../light/PointLight.h"
#include "../geometry/Mesh.h"
#include "../math/Quaternion.h"

namespace Core {

    Bool RenderUtils::isPointLightInRangeOfMesh(WeakPointer<PointLight> pointLight, Point3r& pointLightPos, WeakPointer<Mesh> mesh, WeakPointer<Object3D> meshOwner) {
        Vector4r boundingSphere = mesh->getBoundingSphere();
        Point3r boundingSphereCenter(0.0f, 0.0f, 0.0f);
        Matrix4x4 meshWorldMatrix =  meshOwner->getTransform().getWorldMatrix();
        meshWorldMatrix.transform(boundingSphereCenter);
        pointLight->getOwner()->getTransform().applyTransformationTo(pointLightPos);
        Vector3r centerToLight = pointLightPos - boundingSphereCenter;
        Real distance = centerToLight.magnitude();

        Point3r pos;
        Quaternion rot;
        Point3r scale;
        meshWorldMatrix.decompose(pos, rot, scale);
        Real maxScale = Math::max(Math::max(scale.x, scale.y), scale.z);

        return distance <= boundingSphere.w * maxScale + pointLight->getRadius();
    }
}
