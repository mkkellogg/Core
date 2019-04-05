#include "Ray.h"
#include "Mesh.h"
#include "AttributeArray.h"
#include "IndexBuffer.h"
#include "Vector4.h"
#include "Vector3.h"

namespace Core {

    Bool Ray::intersectMesh(WeakPointer<Mesh> mesh, std::vector<Hit>& hits) const {
        WeakPointer<AttributeArray<Point3rs>> vertexArray = mesh->getVertexPositions();
        Point3rs * vertices = vertexArray->getAttributes();

        UInt32 tCount = vertexArray->getAttributeCount();
        WeakPointer<IndexBuffer> indices;
        if (mesh->isIndexed()) {
            indices = mesh->getIndexBuffer();
            tCount = indices->getSize();
        }

        Hit hit;
        for (UInt32 i = 0; i < tCount; i+=3) {
            Bool wasHit = false;
            Point3r a = mesh->isIndexed() ? *(vertices + indices->getIndex(i)) : *(vertices + i);
            Point3r b = mesh->isIndexed() ? *(vertices + indices->getIndex(i + 1)) : *(vertices + i + 1);
            Point3r c = mesh->isIndexed() ? *(vertices + indices->getIndex(i + 2)) : *(vertices + i + 2);
            wasHit = this->intersectTriangle(a, b, c, hit);
            if (wasHit) {
                hit.Object = mesh;
                hits.push_back(hit);
            }
        }

        return hits.size() > 0;
    }

    Bool Ray::intersectBox(const Box3& box, Hit& hit) const {

        Real _origin[] = {this->Origin.x, this->Origin.y, this->Origin.z};
        Real _dir[] = {this->Direction.x, this->Direction.y, this->Direction.z};
        Real extremes[] = {0.0f, 0.0f, 0.0f};

        for (UInt32 i = 0; i < 3; i++) {
            if (_dir[i] == 0.0f) continue;

            const Vector3r& hitNormal = i == 0 ? Vector3r::Right : i == 1 ? Vector3r::Up : Vector3r::Backward;
            const Vector3r& extremeVec = _dir[i] < 0 ? box.getMax() : box.getMin();
            Real multiplier = -Math::sign(_dir[i]);
            extremes[0] = i == 0 ? extremeVec.x : i == 1 ? extremeVec.y : extremeVec.z;
            Real toExtreme = extremes[0] - _origin[i];

            if (toExtreme * multiplier < 0) {
                UInt32 idx1 = (i + 1) % 3;
                UInt32 idx2 = (i + 2) % 3;
                extremes[2] = _dir[idx1] / _dir[i] * toExtreme + _origin[idx1];
                extremes[1] = _dir[idx2] / _dir[i] * toExtreme + _origin[idx2];
                Point3r extreme(extremes[i], extremes[idx2], extremes[idx1]);
                if (box.containsPoint(extreme, 0.0001f)) {
                       hit.Origin = extreme;
                       hit.Normal = hitNormal * multiplier;
                       return true ;
                    }
            }
        }

        return false;
    }

    Bool Ray::intersectTriangle(const Point3r& p0, const Point3r& p1,
                                const Point3r& p2, Hit& hit) const {
        Vector3r q1 = p2 - p0;
        Vector3r q2 = p1 - p0;
        Vector3r normal = q1.cross(q2);

        if (Vector3r::dot(normal, this->Direction) >= 0) return false;

        Real d = -Vector3r::dot(p0, normal);
        Vector4r planeEq(normal.x, normal.y, normal.z, d);
        
        Hit planeHit;
        Bool intersectsPlane = this->intersectPlane(planeEq, planeHit);
        if (!intersectsPlane) return false;

        Vector3r r = planeHit.Origin - p0;
        Real rDotQ1 = Vector3r::dot(r, q1);
        Real rDotQ2 = Vector3r::dot(r, q2);

        Real q1Sq = Vector3r::dot(q1, q1);
        Real q2Sq = Vector3r::dot(q2, q2);
        Real q1Dotq2 = Vector3r::dot(q1, q2);

        Real qF = -q1Dotq2 / q2Sq;
        Real w1 = (qF * rDotQ2 + rDotQ1) / (qF * q1Dotq2 + q1Sq);
        Real w2 = (rDotQ2 - (w1 * q1Dotq2)) / q2Sq;
        
        Real w0 = 1.0 - w2 - w1;

        if (w0 < 0 || w1 < 0 || w2 < 0 || w0 > 1.0 || w1 > 1.0 || w2 > 1.0) {
            return false;
        }

        hit.Origin = planeHit.Origin;
        hit.Normal = normal;

        return true;
    }

    Bool Ray::intersectPlane(const Plane& plane, Hit& hit) const {
        return this->intersectPlane(plane.getPlaneEquation(), hit);
    }

    Bool Ray::intersectPlane(const Vector4r& plane, Hit& hit) const {
        Vector4r rayOrigin(this->Origin.x, this->Origin.y, this->Origin.z, 1.0f);
        Vector4r rayDir(this->Direction.x, this->Direction.y, this->Direction.z, 0.0f);
        Real planeRayDot = Vector4r::dot(plane, rayDir);
        if (planeRayDot == 0.0f) return false;
        Real t = -(Vector4r::dot(plane, rayOrigin) / planeRayDot);
        Point3r intersection = this->Origin + this->Direction * t;

        hit.Origin = intersection;
        hit.Normal.set(plane.x, plane.y, plane.z);
        return true;
    }

}