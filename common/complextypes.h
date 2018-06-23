#include <vector>

#include "../util/PersistentWeakPointer.h"
#include "../util/ValueIterator.h"

namespace Core {

    template <typename N>
    using GameObjectIterator = ValueIterator<typename std::vector<PersistentWeakPointer<N>>::iterator, WeakPointer<N>>;
}