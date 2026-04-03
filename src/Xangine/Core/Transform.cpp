#include <Xangine/Core/Transform.hpp>

namespace Xangine {

Mat4 Transform::getWorldMatrix() const {
    if (matrixDirty) {
        if (parent) {
            cachedMatrix = parent->getWorldMatrix() * getLocalMatrix();
        } else {
            cachedMatrix = getLocalMatrix();
        }
        matrixDirty = false;
    }
    return cachedMatrix;
}

} // namespace Xangine