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
#include "render/MeshContainer.h"
#include "material/BasicCubeMaterial.h"
#include "material/BasicMaterial.h"
#include "material/StandardAttributes.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"

namespace Core {

  Demo::Demo() {
    //this->imageLoader = engine.getImageLoader();
    //this->assetLoader = engine.getAssetLoader();
  }

  void Demo::run() {

    WeakPointer<Core::Camera> camera;
    Engine::instance()->onUpdate([this, camera]() {

      static Core::Real rotationAngle = 0.0;
      if (WeakPointer<Camera>::isValid(camera)) {
        WeakPointer<Camera> cameraPtr(camera);
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

        WeakPointer<Object3D> cameraOwner(cameraPtr->getOwner());
        cameraOwner->getTransform().getLocalMatrix().copy(worldMatrix);
      }

    });

    WeakPointer<Core::Scene> scene = Engine::instance()->createScene();
    Engine::instance()->setActiveScene(scene);
    WeakPointer<Core::Scene> scenePtr(scene);

    WeakPointer<Core::Mesh> skyboxMesh(Engine::instance()->createMesh(36, 0));
    skyboxMesh->init();

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

    skyboxMesh->enableAttribute(Core::StandardAttribute::Position);
    Core::Bool positionInited = skyboxMesh->initVertexPositions();
    ASSERT(positionInited, "Unable to initialize skybox mesh vertex positions.");
    skyboxMesh->getVertexPositions()->store(vertexPositions);

    this->skyboxMaterial = Engine::instance()->createMaterial<Core::BasicMaterial>();
    WeakPointer<Core::BasicMaterial> skyboxMaterialPtr(this->skyboxMaterial);
    skyboxMaterialPtr->build();

    WeakPointer<Core::MeshContainer> skyboxObj = Engine::instance()->createObject3D<Core::MeshContainer>();
    WeakPointer<Core::MeshContainer> skyboxObjPtr = WeakPointer<Core::MeshContainer>(skyboxObj);

    WeakPointer<Core::MeshRenderer> skyboxRenderer = Engine::instance()->createRenderer<Core::MeshRenderer, Core::Mesh>(this->skyboxMaterial, skyboxObj);
    skyboxObjPtr->addRenderable(skyboxMesh);

    WeakPointer<Object3D> sceneRootPtr = WeakPointer<Object3D>(scenePtr->getRoot());
    sceneRootPtr->addChild(skyboxObj);

    WeakPointer<Core::Object3D> cameraObj = Engine::instance()->createObject3D<Core::Object3D>();
    camera = Engine::instance()->createPerspectiveCamera(cameraObj, Camera::DEFAULT_FOV, Camera::DEFAULT_ASPECT_RATIO, 0.1f, 100);
    sceneRootPtr->addChild(cameraObj);

  }

}
