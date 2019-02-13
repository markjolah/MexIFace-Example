/** @file ArmadilloExample.cpp
 *  @brief A very simple test C++ class, that will be wrapped by MexIFace
 */

#include "ArmadilloExample/ArmadilloExample.h"

ArmadilloExample::ArmadilloExample(VecT v) : v(v)
{
}

ArmadilloExample::VecT ArmadilloExample::add(const VecT &o) const
{
    return o+v;
}

ArmadilloExample::VecT ArmadilloExample::ret() const
{
    return v;
}

void ArmadilloExample::inc(const VecT &o)
{
    v+=o;
}
