/*
 * Profiler.h
 *
 *  Created on: Aug 7, 2025
 *      Author: MaximilianLell
 */

#ifndef PROFILER_PROFILER_H_
#define PROFILER_PROFILER_H_

#include "../../Utils/CommonTypes.h"

/**
 * @brief Profiler configuration structure for measuring code execution time.
 *
 * Holds the start and stop cycle counts for a profiling session.
 */
typedef struct
{
    u32 u32StartCycles;
    u32 u32StopCycles;
} Profiler_Config_t;

/**
 * @brief Initializes the DWT cycle counter for profiling (must be called once
 * before use).
 */
void Profiler_Init( void );

/**
 * @brief Starts the profiler by capturing the current cycle count.
 * @param inout_ptProfiler Pointer to Profiler_Config_t structure
 */
void Profiler_Start( Profiler_Config_t *const inout_ptProfiler );

/**
 * @brief Stops the profiler by capturing the current cycle count.
 * @param inout_ptProfiler Pointer to Profiler_Config_t structure
 */
void Profiler_Stop( Profiler_Config_t *const inout_ptProfiler );

/**
 * @brief Returns the number of CPU cycles between start and stop.
 * @param inout_ptProfiler Pointer to Profiler_Config_t structure
 * @return Number of cycles elapsed
 */
u32 Profiler_GetCycles( const Profiler_Config_t *const inout_ptProfiler );

/**
 * @brief Returns the elapsed time in microseconds.
 * @param inout_ptProfiler Pointer to Profiler_Config_t structure
 * @return Elapsed time in microseconds
 */
f32 Profiler_GetUsec( const Profiler_Config_t *const inout_ptProfiler );

#endif /* PROFILER_PROFILER_H_ */
