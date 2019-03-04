/** @file ArmadilloExample.h
 *  @brief A very simple test C++ class, that will be wrapped by MexIFace
 */

#ifndef MEXIFACE_ARMADILLO_EXAMPLE_H
#define MEXIFACE_ARMADILLO_EXAMPLE_H

#include <exception>
#include <armadillo>

/** Primary namespace for ArmadilloExample library
 */
namespace armadillo_example {

/** Standard exception type used for all errors types.
 */
class ArmadilloExampleError : public std::exception { };

/** Example C++ numerical library using armadillo
 *
 * This example library is a simplified example of a potentially useful numerical library that uses Armadillo with BLAS and LAPACK
 * support and OpenMP for parallelization.
 *
 * An ArmadilloExample object maintains an internal set of K equal sized positive definite square symmetric matrices.
 *
 * It has methods to:
 *  - Generate random square symmetric positive definite matrices
 *  - Get and Set the internal matrices.
 *  - Compute the Cholesky decomposition of the internal matrices.
 *  - Solve a set of linear systems using the computed Cholesky decompositions.
 *
 * All computational operations are performed in parallel using OpenMP.  We use the OMPExceptionCatcher class to
 * prevent any exceptions leaving OpenMP code.
 *
 */
class ArmadilloExample
{
public:
    using IdxT = arma::uword; //!< Index type.
    using VecT = arma::Col<double>;  //!< Vector type.
    using MatT = arma::Mat<double>; //!< Matrix type.
    using CubeT = arma::Cube<double>; //!< Cube [3D matrix] type.

    /** Construct with new set of K generated NxN matrices
     * Throws ArmadilloExampleError if N or K are 0
     * @param[in] N Size of each square matrix. (>=1)
     * @param[in] K Number of matrices to generate. (>=1)
     */
    ArmadilloExample(IdxT N, IdxT K);

    IdxT getN() const; //!< get the square matrix size, N
    IdxT getK() const; //!< get the number of matrices stored, K

    /** Generate K random NxN positive definite symmetric matrices
     * Throws ArmadilloExampleError if N or K are 0
     * @param[in] N Size of each square matrix. (>=1)
     * @param[in] K Number of matrices to generate. (>=1)
     */
    void generate_mats(IdxT N, IdxT K);

    /** Set matrices to stack of square positive definite symmetric matrices
     *
     * Throws ArmadilloExampleError if matrices are not square, positive definite, or symmetric
     *
     * @param[in] mats A 3d cube size [N,N,K] where N,K>=1, and each matrix (slice) is positive definite symmetric.
     */
    void set_mats(const CubeT &mats);

    /** Return internal set of matrices
     * @return Stack of square positive definite symmetric matrices, size:[N,N,K]
     */
    const CubeT& get_mats() const;

    /** Get the \f$LL'\f$ Cholesky decomposition as returned by armadillo
     *
     * The matrices are stored in lower-triangular format so that \f$LL'=M\f$.
     *
     * These are computed on demand and stored for future use by the armadillo solver.
     *
     * @return Stack of lower-triangular Cholesky decomposition matrices, size:[N,N,K]
     */
    const CubeT& get_chol_armadillo() const;


    /** Get the \f$LDL'\f$ Cholesky decomposition as computed in inplace format.
     *
     * Internal Cholesky format represents an \f$LDL'\f$ decomposition with \f$L\f$ stored in the lower triangle,
     * and D stored on the diagonal, the upper triangle values are unspecified.  This format is superior
     * to the LL' format as it eliminated the need to take square roots.
     *
     * These are computed on demand and stored for future use by the internal Cholesky solver.
     *
     * @return Stack of lower-triangular Cholesky decomposition matrices, size:[N,N,K]
     */
    const CubeT& get_chol_direct() const;

    /** Use the Armadillo arma::solve() optimized for lower-triangular matrices
     *
     * This uses the \f$LL'\f$ Cholesky decomposition from armadillo.
     *
     * @param[in] v Vector to solve for in each of the K systems.  Size=N.
     * @return Matrix of solution to each of the K systems, Size:[N,K]
     */
    MatT solve_chol_armadillo(const VecT &v) const;

    /** Use the direct Cholesky solver algorithm optimized for \f$LDL'\f$ format
     *
     * @param[in] v Vector to solve for in each of the K systems.  Size=N.
     * @return Matrix of solution to each of the K systems, Size:[N,K]
     */
    MatT solve_chol_direct(const VecT &v) const;

    /** Compute the \f$LL'\f$ Cholesky decomposition as returned by armadillo for given matrices
     * Throws ArmadilloExampleError if matrices are not square, positive definite, or symmetric
     * @param[in] mats A 3d cube size [N,N,K] where N,K>=1, and each matrix (slice) is positive definite symmetric.
     */
    static CubeT compute_chol_armadillo(const CubeT &mats);

    /** Compute the \f$LDL'\f$ Cholesky decomposition as computed in in-place format.
     * Throws ArmadilloExampleError if matrices are not square, positive definite, or symmetric
     * @param[in] mats A 3d cube size [N,N,K] where N,K>=1, and each matrix (slice) is positive definite symmetric.
     */
    static void compute_chol_inplace(CubeT &mats);

private:
    IdxT N;
    IdxT K;
    CubeT mats;
    mutable bool _chol_computed=false;
    mutable CubeT chol_armadillo;
    mutable CubeT chol_internal;


    /** Compute the solution of a \f$Mx=b\f$ using the \f$LDL'\f$ Cholesky decomposition format.
     *
     * Throws ArmadilloExampleError if C or b are incorrect sizes.
     * @param[in] C An armadillo matrix-like type, representing a \f$LDL'\f$ Cholesky decomposition format.
     * @param[in] b The right-hand-side of the linear system.
     * @param[out] result The solution \f$x\f$
     */
    template<class Mat>
    void cholesky_solve(const Mat &C,const arma::vec &b, arma::vec &result);

    /** Compute the \f$LDL'\f$ Cholesky decomposition in-place on a single Matrix-like type.
     *
     * Result is \f$LDL'\f$ format with L stored on lower triangle and D on diagonal.  Upper triangle values are unspecified.
     *
     * Throws ArmadilloExampleError if matrix is not square, positive definite, or symmetric.
     * @param[in,out] C An armadillo matrix-like type, that square positive definite symmetric.  Modified in-place to Cholesky format.
     */
    template<class Mat>
    void cholesky_inplace(Mat &C);

};

} /* namespace armadillo_example */

#endif /* MEXIFACE_ARMADILLO_EXAMPLE_H */
