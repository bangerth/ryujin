//
// SPDX-License-Identifier: MIT
// Copyright (C) 2020 - 2023 by the ryujin authors
//

#pragma once

#include "geometry_airfoil.h"
#include "geometry_annulus.h"
#include "geometry_cylinder.h"
#include "geometry_rectangular_domain.h"
#include "geometry_step.h"
#include "geometry_wall.h"

namespace ryujin
{
  /**
   * A namespace for a number of benchmark geometries and dealii::GridIn
   * wrappers.
   *
   * @ingroup Mesh
   */
  namespace Geometries
  {
    /**
     * Populate a given container with all initial state defined in this
     * namespace
     *
     * @ingroup Mesh
     */
    template <int dim, typename T>
    void populate_geometry_list(T &geometry_list, const std::string &subsection)
    {
      auto add = [&](auto &&object) {
        geometry_list.emplace(std::move(object));
      };

      add(std::make_unique<Cylinder<dim>>(subsection));
      add(std::make_unique<Step<dim>>(subsection));
      add(std::make_unique<Wall<dim>>(subsection));
      add(std::make_unique<RectangularDomain<dim>>(subsection));
      add(std::make_unique<Airfoil<dim>>(subsection));
      add(std::make_unique<Annulus<dim>>(subsection));
    }
  } /* namespace Geometries */
} /* namespace ryujin */
