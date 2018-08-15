#include "Ray.h"
#include "Mesh.h"
#include "AttributeArray.h"
#include "IndexBuffer.h"
#include "Vector4.h"
#include "Vector3.h"

namespace Core {

    Ray::Ray(const Vector3Components<Real>& origin, const Vector3Components<Real>& direction) {
        this->Origin.copy(origin);
        this->Direction.copy(direction);
    }

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
            Point3rs * v1 = nullptr;
            Point3rs * v2 = nullptr;
            Point3rs * v3 = nullptr;
            if (mesh->isIndexed()) {
                v1 = vertices + indices->getIndex(i);
                v2 = vertices + indices->getIndex(i + 1);
                v3 = vertices + indices->getIndex(i + 2);
            }
            else {
                v1 = vertices + i;
                v2 = vertices + (i + 1);
                v3 = vertices + (i + 2);
            }
            Bool wasHit = this->intersectTriangle(*v1, *v2, *v3, hit);
            if (wasHit)hits.push_back(hit);
        }

        return hits.size() > 0;
    }

    Bool Ray::intersectBox(const Box3& box, Hit& hit) const {

        const Vector3r& max = box.getMax();
        const Vector3r& min = box.getMin();

        std::cerr << "]] testing origin: " << this->Origin.x << ", " << this->Origin.y << ", " << this->Origin.z << std::endl;
         std::cerr << "]] testing dir: " << this->Direction.x << ", " << this->Direction.y << ", " << this->Direction.z << std::endl;

        for (UInt32 i = 0; i < 3; i++) {
            Real origin = i == 0 ? this->Origin.x : i == 1 ? this->Origin.y : this->Origin.z;
            Real originA = i == 0 ? this->Origin.y : i == 1 ? this->Origin.z : this->Origin.x;
            Real originB = i == 0 ? this->Origin.z : i == 1 ? this->Origin.x : this->Origin.y;
            Real dir = i == 0 ? this->Direction.x : i == 1 ? this->Direction.y : this->Direction.z;
            Real dirA = i == 0 ? this->Direction.y : i == 1 ? this->Direction.z : this->Direction.x;
            Real dirB = i == 0 ? this->Direction.z : i == 1 ? this->Direction.x : this->Direction.y;
            Vector3r hitNormal = i == 0 ? Vector3r(1.0, 0.0, 0.0) : i == 1 ? Vector3r(0.0, 1.0, 0.0) : Vector3r(0.0, 0.0, 1.0);

            Real extreme = 0.0f, aAtExtreme = 0.0f, bAtExtreme = 0.0f;
            Bool potentialIntersect = false;
            if (dir > 0) {
                extreme = i == 0 ? min.x : i == 1 ? min.y : min.z;
                Real toMin = extreme - origin;
                if (toMin > 0) {
                    aAtExtreme = dirA / dir * toMin + originA;
                    bAtExtreme = dirB / dir * toMin + originB;
                    potentialIntersect = true;
                    hitNormal = hitNormal * -1.0f;
                }
            }
            else if(dir < 0) {
                extreme = i == 0 ? max.x : i == 1 ? max.y : max.z;
                Real toMax = extreme - origin;
                if (toMax < 0) {
                    aAtExtreme = dirA / dir * toMax + originA;
                    bAtExtreme = dirB / dir * toMax + originB;
                    potentialIntersect = true;
                    if ( i == 1)std::cerr << "dats: " << dirA << ", " << dirB << ", " << dir << ", " << toMax << std::endl;
                }
            }

            if (potentialIntersect) {
                Real x = i == 0 ? extreme : i == 1 ? bAtExtreme : aAtExtreme;
                Real y = i == 0 ? aAtExtreme : i == 1 ? extreme : bAtExtreme;
                Real z = i == 0 ? bAtExtreme : i == 1 ? aAtExtreme : extreme;
                if ( i == 1)std::cerr << "potential: " << x << ", " << y << ", " << z << std::endl;
                Real epsilon = 0.0001f;
                if (x >= min.x - epsilon && x <= max.x + epsilon &&
                    y >= min.y - epsilon && y <= max.y + epsilon &&
                    z >= min.z - epsilon && z <= max.z + epsilon) {
                       hit.Origin.set(x, y, z); 
                       hit.Normal = hitNormal;
                       return true ;
                    }
            }
        }

        return false;
    }

    Bool Ray::intersectTriangle(const Vector3Components<Real>& p0, const Vector3Components<Real>& p1,
                                const Vector3Components<Real>& p2, Hit& hit, const Vector3Components<Real>* normal) const {
        Point3r _p0, _p1, _p2;
        _p0.copy(p0); 
        _p1.copy(p1); 
        _p2.copy(p2);

        Vector3r q1 = _p1 - _p0;
        Vector3r q2 = _p2 - _p0;
        Vector3r _normal;

        if (normal != nullptr) {
            _normal.copy(*normal);
        }
        else {
            Vector3r::cross(q1, q2, _normal);
            _normal.normalize();
        }

        if (Vector3r::dot(_normal, this->Direction) >= 0) return false;

        Real d = Vector3r::dot(p0, _normal);
        Vector4r planeEq(_normal.x, _normal.y, _normal.z, d);
        Vector4r rayOrigin(this->Origin.x, this->Origin.y, this->Origin.z, 1.0f);
        Vector4r rayDir(this->Direction.x, this->Direction.y, this->Direction.z, 0.0f);
        Real t = -(Vector4r::dot(planeEq, rayOrigin) / Vector4r::dot(planeEq, rayDir));
        Point3r intersection = this->Origin + this->Direction * t;

        Vector3r r = intersection - _p0;
        Real rDotQ1 = Vector3r::dot(r, q1);
        Real rDotQ2 = Vector3r::dot(r, q2);

        Real q1Sq = Vector3r::dot(q1, q1);
        Real q2Sq = Vector3r::dot(q2, q2);
        Real q1Dotq2 = Vector3r::dot(q1, q2);

        Real qF = -(q2Sq / q1Dotq2);
        Real w1 = (qF * rDotQ1 + rDotQ2) / (q1Sq * qF + q1Dotq2);
        Real w2 = (rDotQ2 - (w1 * q1Dotq2)) / q2Sq;

        if (w1 < 0 || w2 < 0 || w1 > 1.0 || w2 > 1.0) return false;

        hit.Origin = intersection;
        hit.Normal = _normal;
        
        return true;
    }

}