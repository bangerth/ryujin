//
// SPDX-License-Identifier: MIT
// Copyright (C) 2020 - 2022 by the ryujin authors
//

#pragma once

#include <compile_time_options.h>

#include <deal.II/base/config.h>

#include <atomic>
#include <omp.h>

/**
 * @name OpenMP parallel for macros
 *
 * Intended use:
 * ```
 * // serial work
 *
 * RYUJIN_PARALLEL_REGION_BEGIN
 *
 * // per thread work and thread-local storage declarations
 *
 * RYUJIN_OMP_FOR
 * for (unsigned int i = 0; i < size_internal; i += simd_length) {
 *   // parallel for loop that is statically distributed on all available
 *   // worker threads by slicing the interval [0,size_internal)
 * }
 *
 * RYUJIN_PARALLEL_REGION_END
 * ```
 */
//@{

/**
 * Macro expanding to a `#pragma` directive that can be used in other
 * preprocessor macro definitions.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_PRAGMA(x) _Pragma(#x)

/**
 * Begin an openmp parallel region.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_PARALLEL_REGION_BEGIN                                           \
  RYUJIN_PRAGMA(omp parallel default(shared))                                  \
  {

/**
 * End an openmp parallel region.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_PARALLEL_REGION_END }

/**
 * Enter a parallel for loop.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_OMP_FOR RYUJIN_PRAGMA(omp for)

/**
 * Enter a parallel for loop with "nowait" declaration, i.e., the end of
 * the for loop does not include an implicit thread synchronization
 * barrier.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_OMP_FOR_NOWAIT RYUJIN_PRAGMA(omp for nowait)

/**
 * Declare an explicit Thread synchronization barrier.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_OMP_BARRIER RYUJIN_PRAGMA(omp barrier)

/**
 * Annotate a critical section that has to be accessed sequentially.
 *
 * @ingroup Miscellaneous
 */
#define RYUJIN_OMP_CRITICAL RYUJIN_PRAGMA(omp critical)

/**
 * Compiler hint annotating a boolean to be likely true.
 *
 * Intended use:
 * ```
 * if (RYUJIN_LIKELY(thread_ready == true)) {
 *   // likely branch
 * }
 * ```
 *
 * @note The performance penalty of incorrectly marking a condition as
 * likely is severe. Use only if the condition is almost always true.
 * @ingroup Miscellaneous
 */
#define RYUJIN_LIKELY(x) (__builtin_expect(!!(x), 1))

/**
 * Compiler hint annotating a boolean expression to be likely false.
 *
 * Intended use:
 * ```
 * if (RYUJIN_UNLIKELY(thread_ready == false)) {
 *   // unlikely branch
 * }
 * ```
 *
 * @note The performance penalty of incorrectly marking a condition as
 * unlikely is severe. Use only if the condition is almost always false.
 * @ingroup Miscellaneous
 */
#define RYUJIN_UNLIKELY(x) (__builtin_expect(!!(x), 0))

namespace ryujin
{
  /**
   * @todo write documentation
   *
   * @ingroup Miscellaneous
   */
  template <typename Payload>
  class SynchronizationDispatch
  {
  public:
    SynchronizationDispatch(const Payload &payload)
        : payload_(payload)
        , executed_payload_(false)
        , n_threads_ready_(0)
    {
    }

    ~SynchronizationDispatch()
    {
      if (!executed_payload_)
        payload_();
    }

    DEAL_II_ALWAYS_INLINE inline void check(bool &thread_ready,
                                            const bool condition)
    {
#ifdef USE_COMMUNICATION_HIDING
      if (RYUJIN_UNLIKELY(thread_ready == false && condition)) {
#else
      (void)thread_ready;
      (void)condition;
      if constexpr (false) {
#endif
        thread_ready = true;
        if (++n_threads_ready_ == omp_get_num_threads()) {
          executed_payload_ = true;
          payload_();
        }
      }
    }

  private:
    const Payload payload_;
    bool executed_payload_;
    std::atomic_int n_threads_ready_;
  };
} // namespace ryujin

//@}
