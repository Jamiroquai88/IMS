/*
 * debug.h
 *
 *  Created on: Dec 1, 2015
 *      Author: pavel
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

/**
 * \brief Debug helper code
 */
#define DEBUG

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
#define DBG_LOG_T(x) (DBG_LOG(x << "(" << Time << ")"))
#else
#define DBG_LOG_T(x)
#endif


#endif /* DEBUG_H_ */
