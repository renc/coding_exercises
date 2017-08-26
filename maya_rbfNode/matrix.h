#ifndef TERPOLATOR_VECTOR_H
#define TERPOLATOR_VECTOR_H

#include <vector>
#include <iostream>
#include "Eigen/Eigen/Core"
#include "Eigen/Eigen/Dense"
#include "Eigen/Eigen/LU"
#include "Eigen/Eigen/SVD"

typedef Eigen::VectorXd Vector;
typedef Eigen::MatrixXd Matrix;
typedef Eigen::FullPivLU<Matrix> FullPivLUMatrix;
typedef Eigen::JacobiSVD<Matrix> JacobiSVD;


#endif
