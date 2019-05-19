#include "Bone.h"

namespace Core {
    /*
    * Default constructor.
    */
    Bone::Bone() {
        Node = nullptr;
        this->Name = std::string("");
        this->ID = -1;
    }

    /*
     * Parameterized constructor only with [name] parameter.
     */
    Bone::Bone(const std::string& name) : Bone(name, (UInt32)-1) {}

    /*
     * Fully parameterized constructor.
     */
    Bone::Bone(const std::string& name, UInt32 id) {
        this->Name = name;
        this->ID = id;
        Node = nullptr;
    }

    /*
     * Destructor
     */
    Bone::~Bone() {}

    /*
     * Copy member data from [bone].
     */
    void Bone::copy(const Bone * bone) {
        this->Name = bone->Name;
        this->ID = bone->ID;
        this->OffsetMatrix.copy(bone->OffsetMatrix);
        this->Node = bone->Node;
    }

}


