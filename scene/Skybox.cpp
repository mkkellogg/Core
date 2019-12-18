#include "Skybox.h"
#include "../material/StandardAttributes.h"
#include "../material/SkyboxMaterial.h"
#include "../material/PhysicalSkyboxMaterial.h"
#include "../geometry/Mesh.h"
#include "../render/MeshRenderer.h"

namespace Core {

    Skybox::Skybox() {
    }

    Skybox::~Skybox() {
        if(this->skyboxObj.isValid()) Engine::safeReleaseObject(this->skyboxObj);
    }

    void Skybox::build(WeakPointer<CubeTexture> skyboxTexture, Bool isPhysical, Real exposure) {
        WeakPointer<Mesh> skyboxMesh(Engine::instance()->createMesh(36, 0));
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
            1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
            // bottom
            -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
            -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
            // front
            1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0
        };

        skyboxMesh->enableAttribute(Core::StandardAttribute::Position);
        Bool positionInited = skyboxMesh->initVertexPositions();
        if (!positionInited) {
            throw Exception("Skybox::build -> Unable to initialize skybox mesh vertex positions.");
        }
        skyboxMesh->getVertexPositions()->store(vertexPositions);

        WeakPointer<SkyboxMaterial> skyboxMaterial;
        if (isPhysical) {
            WeakPointer<PhysicalSkyboxMaterial> physicalSkyboxMaterial = Engine::instance()->createMaterial<PhysicalSkyboxMaterial>();
            physicalSkyboxMaterial->setExposure(exposure);
            skyboxMaterial = physicalSkyboxMaterial;
        }
        else {
            skyboxMaterial = Engine::instance()->createMaterial<SkyboxMaterial>();
        }
        skyboxMaterial->setTexture(skyboxTexture);
        skyboxMaterial->setCullFace(RenderState::CullFace::Front);

        this->skyboxObj = Engine::instance()->createObject3D<MeshContainer>();

        WeakPointer<MeshRenderer> skyboxRenderer = Engine::instance()->createRenderer<MeshRenderer, Mesh>(skyboxMaterial, skyboxObj);
        this->skyboxObj->addRenderable(skyboxMesh);
    }

     WeakPointer<MeshContainer> Skybox::getSkyboxObject() {
         return this->skyboxObj;
     }
}
