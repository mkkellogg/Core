//
// Created by Mark Kellogg on 1/18/18.
//

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
#include "material/BasicCubeMaterial.h"
#include "material/BasicMaterial.h"
#include "material/StandardAttributes.h"
#include "image/RawImage.h"
#include "image/CubeTexture.h"

namespace MPM {

  Demo::Demo(Engine& engine): engine(engine) {
    this->imageLoader = engine.getImageLoader();
    this->assetLoader = engine.getAssetLoader();
  }

  void Demo::run() {

    this->engine.onUpdate([this](Engine& engine) {

      // TODO: Remove this camera rotation code, it's only for demo purposes
     /* static MPM::Real rotationAngle = 0.0;
      std::shared_ptr<MPM::Camera> camera = engine.getCamera();
      if (camera) {
        rotationAngle += 0.01;
        if (rotationAngle >= MPM::Math::TwoPI) rotationAngle -= MPM::Math::TwoPI;

        MPM::Quaternion qA;
        qA.fromAngleAxis(rotationAngle, 0, 1, 0);
        MPM::Matrix4x4 rotationMatrixA;
        qA.rotationMatrix(rotationMatrixA);

        MPM::Quaternion qB;
        qB.fromAngleAxis(-0.8, 1, 0, 0);
        MPM::Matrix4x4 rotationMatrixB;
        qB.rotationMatrix(rotationMatrixB);

        MPM::Matrix4x4 worldMatrix;
        worldMatrix.multiply(rotationMatrixA);

        if(demoMode == DemoMode::Dollhouse) {
          worldMatrix.translate(0, 12, 15);
          worldMatrix.multiply(rotationMatrixB);
        }

        camera->getLocalTransform().getMatrix().copy(worldMatrix);
      }*/

    });

   /* std::shared_ptr<MPM::Scene> scene = std::make_shared<MPM::Scene>();
    engine.setScene(scene);

    if (demoMode == DemoMode::Panorama) {
      std::shared_ptr<MPM::Mesh> skyboxMesh = std::make_shared<MPM::Mesh>(36, false);
      MPM::Real vertexPositions[] = {
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
      skyboxMesh->enableAttribute(MPM::StandardAttributes::Position);
      MPM::Bool positionInited = skyboxMesh->initVertexPositions(36);
      ASSERT(positionInited, "Unable to initialize skybox mesh vertex positions.");
      skyboxMesh->getVertexPositions()->store(vertexPositions);

      this->skyboxImages.push_back(imageLoader->loadJPEG("Pano/skybox1.jpg", false));
      this->skyboxImages.push_back(imageLoader->loadJPEG("Pano/skybox3.jpg", false));
      this->skyboxImages.push_back(imageLoader->loadJPEG("Pano/skybox0.jpg", false));
      this->skyboxImages.push_back(imageLoader->loadJPEG("Pano/skybox5.jpg", false));
      this->skyboxImages.push_back(imageLoader->loadJPEG("Pano/skybox4.jpg", false));
      this->skyboxImages.push_back(imageLoader->loadJPEG("Pano/skybox2.jpg", false));

      this->skyboxTexture = MPM::CubeTexture::createCubeTexture(this->skyboxImages[0].get(), this->skyboxImages[1].get(),
                                                                this->skyboxImages[2].get(), this->skyboxImages[3].get(),
                                                                this->skyboxImages[4].get(), this->skyboxImages[5].get());

      this->skyboxMaterial = std::make_shared<MPM::BasicCubeMaterial>();
      skyboxMaterial->build();
      skyboxMaterial->setSkyboxTexture(skyboxTexture);

      std::shared_ptr<MPM::Object3D> skyboxObj = std::make_shared<MPM::Object3D>();
      std::shared_ptr<MPM::MeshRenderer> skyboxRenderer = std::make_shared<MPM::MeshRenderer>(
          skyboxMaterial, skyboxObj);
      skyboxObj->addRenderable<MPM::Mesh>(skyboxMesh);
      skyboxObj->setCustomRenderer<MPM::Mesh>(skyboxRenderer);
      scene->getRoot()->addObject(skyboxObj);
    }
    else {
      MPM::DAMLoader damLoader(assetLoader, imageLoader);
      auto meshObj = damLoader.load("hT3RtfKyx9B","3f13ceb2d2494214ad7cce632c20b770", true, true);
      scene->getRoot()->addObject(meshObj);
    }

    std::shared_ptr<MPM::Camera> camera = std::make_shared<MPM::Camera>();
    engine.setCamera(camera);*/
  }

}
