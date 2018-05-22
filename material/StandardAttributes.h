//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_STANDARDATTRIBUTES_H
#define MPM_STANDARDATTRIBUTES_H

namespace MPM {

  enum class StandardAttributes {
    Position = 0,
    Color = 1,
    UV = 2,
    _Count = 3, // Must always be last in the list;
  };

}

#endif //MPM_STANDARDATTRIBUTES_H
