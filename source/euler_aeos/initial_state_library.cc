//
// SPDX-License-Identifier: MIT
// Copyright (C) 2020 - 2023 by the ryujin authors
//

#include "initial_state_library.template.h"

namespace ryujin
{
  template class InitialStateLibrary<Description, 1, NUMBER>;
  template class InitialStateLibrary<Description, 2, NUMBER>;
  template class InitialStateLibrary<Description, 3, NUMBER>;
} // namespace ryujin
