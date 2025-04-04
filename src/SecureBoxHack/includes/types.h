#ifndef types_h
#define types_h

#include "DynamicBitset.h"
#include <vector>

namespace SecureBoxHack
{
using BoolMatrix = std::vector<std::vector<bool>>;
using GaussMatrix = std::vector<DynamicBitset>;
} // namespace SecureBoxHack

#endif
