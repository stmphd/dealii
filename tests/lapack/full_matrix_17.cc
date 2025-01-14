// ---------------------------------------------------------------------
//
// Copyright (C) 2005 - 2020 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------


// test LAPACKFullMatrix::reciprocal_condition_number() by comparing estimated
// value to 1 / ( ||A||_1 * ||A^{-1}||_1 )

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/lapack_full_matrix.h>
#include <deal.II/lac/vector.h>

#include <iostream>

#include "../tests.h"

#include "create_matrix.h"


template <typename NumberType>
void
test(const unsigned int size)
{
  // Full matrix:
  FullMatrix<NumberType> F(size), invF(size);
  create_spd(F);
  invF.invert(F);
  const double l1     = F.l1_norm();
  const double inv_l1 = invF.l1_norm();

  // Lapack:
  LAPACKFullMatrix<NumberType> M(size);
  M = F;
  M.set_property(LAPACKSupport::symmetric);
  const double la_l1 = M.l1_norm();
  M.compute_cholesky_factorization();
  const double rcond = M.reciprocal_condition_number(la_l1);

  deallog << 1. / (l1 * inv_l1) << ' ' << rcond << std::endl;
}


int
main()
{
  initlog();
  deallog.get_file_stream().precision(3);

  const std::vector<unsigned int> sizes = {{1, 3, 11, 17, 32, 64, 200, 391}};
  for (const auto &s : sizes)
    {
      deallog << "size=" << s << std::endl;
      // test<float>(s);
      test<double>(s);
    }
}
