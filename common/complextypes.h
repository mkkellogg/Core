#include <vector>

#include "../util/ValueIterator.h"
#include "../util/PersistentWeakPointer.h"

namespace Core {

  template <typename N>
  using GameObjectIterator = ValueIterator<typename std::vector<PersistentWeakPointer<N>>::iterator, WeakPointer<N>>;

}