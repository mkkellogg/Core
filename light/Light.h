#pragma once

#include "../scene/Object3DComponent.h"
#include "../color/Color.h"
#include "../geometry/Vector3.h"
#include "../base/BitMask.h"

namespace Core {

    // forward declarations
    class Engine;

    enum class LightType {
        Directional = 1,
        Point = 2,
        Spot = 3,
        Ambient = 4,
        Planar = 5
    };

    enum class LightCullType {
        None,
        BoundingSphere,
        BoundingBox,
        Tiled
    };

    class Light : public Object3DComponent {
        friend class Engine;

    public:
       
        const Color& getColor() const;
        void setColor(const Color& color);
        void setColor(Real r, Real g, Real b, Real a);

        void setIntensity(Real intensity);
        Real getIntensity() const;

        void setShadowsEnabled(Bool enabled);
        Bool getShadowsEnabled() const;

        void setCullingMask(IntMask mask);
        IntMask getCullingMask() const;

    protected:
        Light(WeakPointer<Object3D> owner);
        virtual ~Light() = 0;

        Color color;
        Real intensity;
        Bool shadowsEnabled;
        IntMask cullingMask;
    };
}
