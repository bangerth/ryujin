//
// SPDX-License-Identifier: MIT
// Copyright (C) 2020 - 2023 by the ryujin authors
//

#include "postprocessor.template.h"
#include <instantiate.h>

namespace ryujin
{
  /* instantiations */
  template class Postprocessor<Description, 1, NUMBER>;
  template class Postprocessor<Description, 2, NUMBER>;
  template class Postprocessor<Description, 3, NUMBER>;

} /* namespace ryujin */
