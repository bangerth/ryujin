//
// SPDX-License-Identifier: MIT
// Copyright (C) 2020 - 2023 by the ryujin authors
//

#pragma once

#include <initial_state_library.h>

namespace ryujin
{
  namespace EulerInitialStates
  {
    /**
     * A 2D extension of the "contrast" initial state consisting of 4 different
     * states separated at x = 0 and y = 0. Visually, this looks like:
     *
     *        state 1  | state 2
     *        ---------|-----------
     *        state 3  | state 4
     *
     * @note The @p t argument is ignored. This class always returns the
     * initial configuration.
     *
     * @ingroup EulerEquations
     */
    template <typename Description, int dim, typename Number>
    class FourStateContrast : public InitialState<Description, dim, Number>
    {
    public:
      using HyperbolicSystem = typename Description::HyperbolicSystem;
      using HyperbolicSystemView =
          typename HyperbolicSystem::template View<dim, Number>;
      using state_type = typename HyperbolicSystemView::state_type;

      FourStateContrast(const HyperbolicSystem &hyperbolic_system,
                        const std::string &subsection)
          : InitialState<Description, dim, Number>("four state contrast",
                                                   subsection)
          , hyperbolic_system_(hyperbolic_system)
      {
        primitive_bottom_left_[0] = 1.4;
        primitive_bottom_left_[1] = 0.;
        primitive_bottom_left_[2] = 0.;
        primitive_bottom_left_[3] = 1.;
        this->add_parameter(
            "primitive state bottom left",
            primitive_bottom_left_,
            "Initial primitive state (rho, u, v, p) on bottom left");

        primitive_bottom_right_[0] = 1.4;
        primitive_bottom_right_[1] = 0.;
        primitive_bottom_right_[2] = 0.;
        primitive_bottom_right_[3] = 1.;
        this->add_parameter(
            "primitive state bottom right",
            primitive_bottom_right_,
            "Initial primitive state (rho, u, v, p) on bottom right");

        primitive_top_left_[0] = 1.4;
        primitive_top_left_[1] = 0.;
        primitive_top_left_[2] = 0.;
        primitive_top_left_[3] = 1.;

        this->add_parameter(
            "primitive state top left",
            primitive_top_left_,
            "Initial primitive state (rho, u, v, p) on top left");

        primitive_top_right_[0] = 1.4;
        primitive_top_right_[1] = 0.;
        primitive_top_right_[2] = 0.;
        primitive_top_right_[3] = 1.;
        this->add_parameter(
            "primitive state top right",
            primitive_top_right_,
            "Initial primitive state (rho, u, v, p) on top right");

        const auto convert_states = [&]() {
          if constexpr (dim != 1) {
            state_bottom_left_ =
                hyperbolic_system_.from_initial_state(primitive_bottom_left_);
            state_bottom_right_ =
                hyperbolic_system_.from_initial_state(primitive_bottom_right_);
            state_top_left_ =
                hyperbolic_system_.from_initial_state(primitive_top_left_);
            state_top_right_ =
                hyperbolic_system_.from_initial_state(primitive_top_right_);
          }
        };
        this->parse_parameters_call_back.connect(convert_states);
        convert_states();
      }

      state_type compute(const dealii::Point<dim> &point, Number /*t*/) final
      {
        if constexpr (dim == 1) {
          AssertThrow(false, dealii::ExcNotImplemented());
          __builtin_trap();

        } else {

          const auto top = point[0] >= 0. ? state_top_right_ : state_top_left_;
          const auto bottom =
              point[0] >= 0. ? state_bottom_right_ : state_bottom_left_;
          return (point[1] >= 0. ? top : bottom);
        }
      }

    private:
      const HyperbolicSystemView hyperbolic_system_;

      dealii::Tensor<1, 4, Number> primitive_bottom_left_;
      dealii::Tensor<1, 4, Number> primitive_bottom_right_;
      dealii::Tensor<1, 4, Number> primitive_top_left_;
      dealii::Tensor<1, 4, Number> primitive_top_right_;

      state_type state_bottom_left_;
      state_type state_bottom_right_;
      state_type state_top_left_;
      state_type state_top_right_;
    };
  } // namespace EulerInitialStates
} // namespace ryujin
