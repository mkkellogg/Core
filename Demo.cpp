#include <memory>
#include "Demo.h"
#include "common/types.h"
#include "math/Math.h"
#include "math/Matrix4x4.h"
#include "math/Quaternion.h"
#include "scene/Scene.h"
#include "render/Camera.h"
#include "material/StandardAttributes.h"
#include "geometry/Mesh.h"
#include "render/MeshRenderer.h"
#include "render/RenderableContainer.h"
#include "material/BasicCubeMaterial.h"
#include "material/BasicMaterial.h"
#include "material/StandardAttributes.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"
#include "util/ValidWeakPointer.h"

namespace Core {

  Demo::Demo(Engine& engine): engine(engine) {
    //this->imageLoader = engine.getImageLoader();
    //this->assetLoader = engine.getAssetLoader();
  }

  void Demo::run() {

    std::weak_ptr<Core::Camera> camera;
    this->engine.onUpdate([this, camera](Engine& engine) {

      static Core::Real rotationAngle = 0.0;
      if (!camera.expired()) {
        ValidWeakPointer<Camera> cameraPtr(camera);
        rotationAngle += 0.01;
        if (rotationAngle >= Core::Math::TwoPI) rotationAngle -= Core::Math::TwoPI;

        Core::Quaternion qA;
        qA.fromAngleAxis(rotationAngle, 0, 1, 0);
        Core::Matrix4x4 rotationMatrixA;
        qA.rotationMatrix(rotationMatrixA);

        Core::Quaternion qB;
        qB.fromAngleAxis(-0.8, 1, 0, 0);
        Core::Matrix4x4 rotationMatrixB;
        qB.rotationMatrix(rotationMatrixB);

        Core::Matrix4x4 worldMatrix;
        worldMatrix.multiply(rotationMatrixA);

        worldMatrix.translate(0, 12, 15);
        worldMatrix.multiply(rotationMatrixB);

        cameraPtr->getTransform().getLocalMatrix().copy(worldMatrix);
      }

    });

    std::weak_ptr<Core::Scene> scene = engine.createScene();
    engine.setActiveScene(scene);
    ValidWeakPointer<Core::Scene> scenePtr(scene);

    ValidWeakPointer<Core::Mesh> skyboxMesh(engine.createMesh<Core::Mesh>(36, false));

    Core::Real vertexPositions[] = {
        // back
        -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
        // left
        -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0,
        // right
        1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
        1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        // top
        -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
        // bottom
        -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
        -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
        // front
        1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0
    };

    skyboxMesh->enableAttribute(Core::StandardAttributes::Position);
    Core::Bool positionInited = skyboxMesh->initVertexPositions(36);
    ASSERT(positionInited, "Unable to initialize skybox mesh vertex positions.");
    skyboxMesh->getVertexPositions()->store(vertexPositions);

    this->skyboxMaterial = engine.createMaterial<Core::BasicMaterial>();
    ValidWeakPointer<Core::BasicMaterial> skyboxMaterialPtr(this->skyboxMaterial);
    skyboxMaterialPtr->build();

    std::weak_ptr<Core::RenderableContainer<Mesh>> skyboxObj = engine.createObject3D<Core::RenderableContainer<Mesh>>();
    ValidWeakPointer<Core::RenderableContainer<Mesh>> skyboxObjPtr = ValidWeakPointer<Core::RenderableContainer<Mesh>>(skyboxObj);

    std::weak_ptr<Core::MeshRenderer> skyboxRenderer = engine.createRenderer<Core::MeshRenderer>(this->skyboxMaterial, skyboxObj);
    skyboxObjPtr->addRenderable(skyboxMesh);

    ValidWeakPointer<Object3D> sceneRootPtr = ValidWeakPointer<Object3D>(scenePtr->getRoot());
    sceneRootPtr->addObject(skyboxObj);

    camera = engine.createCamera();
    sceneRootPtr->addObject(camera);

  }

}
