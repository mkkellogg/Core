#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"

namespace Core {

    template <typename N>
    using SceneObjectIterator = ValueIterator<typename std::vector<PersistentWeakPointer<N>>::iterator, WeakPointer<N>>;
}