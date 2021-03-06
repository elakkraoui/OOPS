/*
 * (C) Copyright 2009-2016 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef OOPS_ASSIMILATION_IPCGMINIMIZER_H_
#define OOPS_ASSIMILATION_IPCGMINIMIZER_H_

#include <string>

#include "oops/assimilation/BMatrix.h"
#include "oops/assimilation/ControlIncrement.h"
#include "oops/assimilation/CostFunction.h"
#include "oops/assimilation/HessianMatrix.h"
#include "oops/assimilation/IPCG.h"
#include "oops/assimilation/PrimalMinimizer.h"

namespace oops {

/// IPCG Minimizer
/*!
 * Implements the Golub-Ye Inexact-Preconditioned Conjugate Gradients algorithm.
 */

// -----------------------------------------------------------------------------

template<typename MODEL> class IPCGMinimizer : public PrimalMinimizer<MODEL> {
  typedef BMatrix<MODEL>             Bmat_;
  typedef CostFunction<MODEL>        CostFct_;
  typedef ControlIncrement<MODEL>    CtrlInc_;
  typedef HessianMatrix<MODEL>       Hessian_;

 public:
  const std::string classname() const override {return "IPCGMinimizer";}
  IPCGMinimizer(const eckit::Configuration &, const CostFct_ & J): PrimalMinimizer<MODEL>(J) {}
  ~IPCGMinimizer() {}

 private:
  double solve(CtrlInc_ &, const CtrlInc_ &,
               const Hessian_ &, const Bmat_ &,
               const int, const double) override;
};

// =============================================================================

template<typename MODEL>
double IPCGMinimizer<MODEL>::solve(CtrlInc_ & dx, const CtrlInc_ & rhs,
                                   const Hessian_ & hessian, const Bmat_ & B,
                                   const int ninner, const double gnreduc) {
// Solve the linear system
  double reduc = IPCG(dx, rhs, hessian, B, ninner, gnreduc);
  return reduc;
}

// -----------------------------------------------------------------------------

}  // namespace oops

#endif  // OOPS_ASSIMILATION_IPCGMINIMIZER_H_
