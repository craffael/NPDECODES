#include "simple_linear_finite_elements.h"

namespace SimpleLinearFiniteElements {

/* Explanations for ElementMatrix_Mass_LFE
// we compute compute area using the cross product
//   /|
// b/ |
// /  |
// ----
// for a 3D flat triangle (not relevant here)
//  a
// set up vectors a and b in 3D a << a , 0; b << b, 0
// a1     b1     a2*0 - 0*b2
// a2  x  b2  =  0*b1 - a1*0    = cr
// 0      0      a1*b2 - a2*b1
//
// area = cr.norm() =  sqrt((a2*0 - 0*b2)^2 + (0*b1 - a1*0)^2 + (a1*b2 -
// a2*b1)^2)/2.
//                  = sqrt((a1*b2 - a2*b1)^2)/2. = |a1*b2 - a2*b1|/2.
*/
/** @brief Computation of element mass matrix on planar triangle
 *  @param vertices 2x3 matrix of vertex coordinates
 */
/* SAM_LISTING_BEGIN_2 */
Eigen::Matrix3d ElementMatrix_Mass_LFE(const TriGeo_t& vertices) {
  Eigen::Matrix3d result;
  //====================
  // Your code goes here
  //====================
  return result;
}
/* SAM_LISTING_END_2 */

/**
 * @brief L2Error Computes the L2 error between the approximate solution and
 *                the exact solution
 * @param mesh the mesh to use
 * @param uFEM the solution approximated through FEM
 * @param exact the exact solution
 * @return the L2 difference
 */
/* SAM_LISTING_BEGIN_1 */
double L2Error(const TriaMesh2D& mesh, const Eigen::VectorXd& uFEM,
               const std::function<double(double, double)> exact) {
  double error;
  const auto& triangles = mesh.Elements;
  const auto& vertices = mesh.Coordinates;

  //====================
  // Your code goes here
  //====================
  return std::sqrt(error);
}
/* SAM_LISTING_END_1 */

/**
 * @brief H1Serror Computes the H^1 error between the approximate solution and
 *                the exact solution
 * @param mesh the mesh to use
 * @param uFEM the solution approximated through FEM
 * @param exact the exact gradient of the solution
 * @return the H^1 difference
 *
 * @note This implementation seems to be flawed!
 */
/* SAM_LISTING_BEGIN_3 */
double H1Serror(const TriaMesh2D& mesh, const Eigen::VectorXd& uFEM,
                const std::function<Eigen::Vector2d(double, double)> exact) {
  double error = 0;
  //====================
  // Your code goes here
  //====================
  return std::sqrt(error);
}
/* SAM_LISTING_END_3 */

/**
 * @brief Element oriented assembly of right hand side vector for Galerkin
 * finite element discretization with piecewise linear Lagrangian finite
 * elements
 * @param Mesh An object describing the triangulation in the form of the
 * Coordinates and Elements matrices
 * @param getElementVector A handle to a function expecting a 3\times 2-matrix
 * of vertex coordinates as input and returning an element load vector as a
 * vector of size 3
 * @param FHandle A handle to a function that provides f(x) for the
 * source function f and for any point x in Omega
 * @return A vector of size N, where N is the number
 * of vertices of the mesh
 */
/* SAM_LISTING_BEGIN_4 */
Eigen::VectorXd assemLoad_LFE(const TriaMesh2D& Mesh,
                              const LocalVectorHandle_t& getElementVector,
                              const FHandle_t& FHandle) {
  // obtain the number of vertices
  int N = Mesh.Coordinates.rows();
  // obtain the number of elements/cells
  int M = Mesh.Elements.rows();
  Eigen::VectorXd phi = Eigen::VectorXd::Zero(N);
  // loop over elements and add local contributions
  for (int i = 0; i < M; i++) {
    // get local$\to$global index mapping for current element
    Eigen::Vector3i element = Mesh.Elements.row(i);
    TriGeo_t Vertices;
    // extract vertices of current element
    for (int j = 0; j < 3; j++) {
      Vertices.col(j) = Mesh.Coordinates.row(element(j)).transpose();
    }
    // compute element right hand side vector
    Eigen::Vector3d philoc = getElementVector(Vertices, FHandle);
    // add contributions to global load vector
    for (int j = 0; j < 3; j++) {
      phi(element(j)) += philoc(j);
    }
  }
  return phi;
}

/* SAM_LISTING_END_4 */

/**
 * @brief Efficient assembly of global Galerkin matrix for piecewise linear
 * Lagrangian finite elements on a triangular mesh without special treatment of
 * boundaries and/or interfaces.
 * @param Mesh An object describing the triangulation in the form of the
 * Coordinates and Elements matrices
 * @param getElementMatrix A function handle to a function that expects a
 * $3\times 2$-matrix of vertex coordinates and returns a $3\times 3$ element
 * matrix.
 * @return A sparse $N\times N$-matrix, where $N$ is the
 * number of vertices of the mesh
 */
/* SAM_LISTING_BEGIN_5 */
Eigen::SparseMatrix<double> GalerkinAssembly(
    const TriaMesh2D& Mesh, const LocalMatrixHandle_t& getElementMatrix) {
  // obtain the number of vertices
  int N = Mesh.Coordinates.rows();
  // obtain the number of elements/cells
  int M = Mesh.Elements.rows();
  std::vector<Eigen::Triplet<double> > triplets;
  // loop over elements and add local contributions
  for (int i = 0; i < M; i++) {
    // get local$\to$global index mapping for current element, \emph{cf.}
    // \lref{eq:idxdef}
    Eigen::Vector3i element = Mesh.Elements.row(i);
    TriGeo_t Vertices;
    // extract vertices of current element
    for (int j = 0; j < 3; j++) {
      Vertices.col(j) = Mesh.Coordinates.row(element(j)).transpose();
    }
    // compute element contributions
    Eigen::Matrix3d Ak = getElementMatrix(Vertices);
    // build triplets from contributions
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        triplets.push_back({element(j), element(k), Ak(j, k)});
      }
    }
  }
  // build sparse matrix from triplets
  Eigen::SparseMatrix<double> A(N, N);
  A.setFromTriplets(triplets.begin(), triplets.end());
  A.makeCompressed();
  return A;
}
/* SAM_LISTING_END_5 */

}  // namespace SimpleLinearFiniteElements
