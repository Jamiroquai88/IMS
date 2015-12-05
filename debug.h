/*
 * debug.h
 *
 *  Created on: Dec 1, 2015
 *      Author: pavel
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "time_interval.h"
#include <iostream>

/**
 * \brief Debug helper code
 */
//#define DEBUG

/**
 * \brief Debug log to stdout
 */
#ifdef DEBUG
#define DBG_LOG(x) (std::cout << x << std::endl)
#else
#define DBG_LOG(x)
#endif

/**
 * \brief Debug log with time
 */
#ifdef DEBUG
#define DBG_LOG_T(x) (DBG_LOG(x << " (" << CTimeInterval::MinutesToTime(Time) << ")"))
#else
#define DBG_LOG_T(x)
#endif

/**
 * \brief Debug condition
 */
#ifdef DEBUG
#define DBG_CONDITION(c,str) { if(!(c)){ std::cerr << "DEBUG_CHECK: " << str << std::endl; exit(1); }
#else
#define DBG_CONDITION(x)
#endif

#endif /* DEBUG_H_ */
