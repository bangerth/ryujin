//
// SPDX-License-Identifier: MIT
// Copyright (C) 2020 - 2023 by the ryujin authors
//

#pragma once

#include "hyperbolic_system.h"
#include <initial_state_library.h>

namespace ryujin
{
  namespace ShallowWater
  {
    struct Description;

    /**
     * Solitary wave over flat bottom. See TODO
     *
     * @ingroup ShallowWaterEquations
     */
    template <int dim, typename Number>
    class SolitaryWave : public InitialState<Description, dim, Number>
    {
    public:
      using HyperbolicSystemView = HyperbolicSystem::View<dim, Number>;
      using state_type = typename HyperbolicSystemView::state_type;
      using primitive_state_type =
          typename HyperbolicSystemView::primitive_state_type;

      SolitaryWave(const HyperbolicSystem &hyperbolic_system,
                   const std::string subsec)
          : InitialState<Description, dim, Number>("solitary wave", subsec)
          , hyperbolic_system(hyperbolic_system)
      {
        depth_ = 0.5;
        this->add_parameter("water depth", depth_, "Depth of still water");

        amplitude_ = 0.2;
        this->add_parameter(
            "amplitude", amplitude_, "Solitary wave amplitude ");
      }

      state_type compute(const dealii::Point<dim> &point, Number t) final
      {
        const auto g = hyperbolic_system.gravity();
        const Number x = point[0];

        const Number celerity = std::sqrt(g * (amplitude_ + depth_));

        const Number width =
            std::sqrt(3. * amplitude_ /
                      (4. * std::pow(depth_, 2) * (amplitude_ + depth_)));

        const Number sechSqd =
            1. / std::pow(cosh(width * (x - celerity * t)), 2);

        const Number wave = depth_ + amplitude_ * sechSqd;

        const Number h = std::max(wave, Number(0.));
        const Number v = celerity * (wave - depth_) / wave;

        return state_type{{h, h * v}};

#if 0
        // FIXME
        if (this->hyperbolic_system.description() == "Hyperbolic Serre") {
          const Number h_prime =
              -2. * amplitude_ * width * tanh(width * (x - celerity * t)) * sechSqd;

          final_state[3] = h * h;
          final_state[4] =
              -std::pow(h, 2) * (celerity * depth_ * h_prime) / std::pow(wave, 2);
          final_state[5] = Number(0.);
        }
#endif
      }

      /* Default bathymetry of 0 */

    private:
      const HyperbolicSystemView hyperbolic_system;

      Number depth_;
      Number amplitude_;
    };

  } // namespace ShallowWater
} // namespace ryujin
