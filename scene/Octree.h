#pragma once

#include <memory>
#include <vector>
#include "../common/types.h"
#include "../geometry/Box3.h"
#include "../geometry/Mesh.h"
#include "../geometry/Vector3.h"
#include "Object3D.h"

namespace Core {

    class Octree {
        class OctreeNode : public Object3D {
        public:
            static const UInt32 MaxLevel = 8;

            OctreeNode(const Box3& bounds, UInt32 level) {
                this->level = level;
                this->bounds = bounds;
            }

        private:
            Box3 bounds;
            std::vector<std::shared_ptr<OctreeNode>> childNodes;
            UInt32 level;

            void buildChildNodes() {
                const Vector3r& min = this->bounds.getMin();
                const Vector3r& max = this->bounds.getMax();

                Vector3r bounds = max - min;
                Vector3r halfBounds = bounds * 0.5;

                for (UInt32 y = 0; y < 2; y++) {
                    for (UInt32 z = 0; z < 2; z++) {
                        for (UInt32 x = 0; x < 2; x++) {
                            Real minX = min.x + halfBounds.x * x;
                            Real maxX = minX + halfBounds.x;

                            Real minY = min.y + halfBounds.y * y;
                            Real maxY = minY + halfBounds.y;

                            Real minZ = min.z + halfBounds.z * z;
                            Real maxZ = minZ + halfBounds.z;

                            Vector3r newMin(minX, minY, minZ);
                            Vector3r newMax(maxX, maxY, maxZ);
                            Box3 childBounds(newMin, newMax);

                            // std::shared_ptr<OctreeNode> newNode = std::make_shared<OctreeNode>(childBounds, this->level + 1);
                            // this->childNodes.push_back(newNode);
                            // this->addChild(newNode);
                        }
                    }
                }
            }

            void addMesh(std::shared_ptr<Mesh> mesh) {
                Bool meshAdded = false;

                if (this->level < OctreeNode::MaxLevel) {
                    if (this->childNodes.size() == 0) {
                        this->buildChildNodes();
                    }

                    for (auto childNode : this->childNodes) {
                        if (childNode->bounds.containsBox(mesh->getBoundingBox())) {
                            meshAdded = true;
                            childNode->addMesh(mesh);
                        }
                    }
                }

                if (!meshAdded) {
                    // this->addObject(mesh);
                }
            }
        };

    public:
        /*

        public addMeshes(obj: Object3D) {
              if (obj instanceof Mesh) {
                this.addMesh(<Mesh> obj);
              }
              for (const child of obj.children) {
                this.addMeshes(child);
              }
          }*/
    };
}
