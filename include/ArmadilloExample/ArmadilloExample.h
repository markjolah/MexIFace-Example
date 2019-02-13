/** @file ArmadilloExample.h
 *  @brief A very simple test C++ class, that will be wrapped by MexIFace
 */

#ifndef MEXIFACE_ARMADILLO_EXAMPLE_H
#define MEXIFACE_ARMADILLO_EXAMPLE_H

#include <armadillo>

namespace armadillo_example {

class ArmadilloExample
{
public:
    using VecT = arma::Col<double>;
    ArmadilloExample(VecT v);
    VecT add(const VecT &o) const;
    VecT ret() const;
    void inc(const VecT &o);
private:
    VecT v;
};

} /* namespace armadillo_example */

#endif /* MEXIFACE_ARMADILLO_EXAMPLE_H */
