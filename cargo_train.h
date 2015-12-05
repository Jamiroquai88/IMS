/*
 * cargo_train.h
 *
 *  Created on: Dec 5, 2015
 *      Author: pavel
 */

#ifndef CARGO_TRAIN_H_
#define CARGO_TRAIN_H_

#include <simlib.h>

class CCargoTrain: public simlib3::Process
{
public:
    CCargoTrain();
    virtual ~CCargoTrain();
};

#endif /* CARGO_TRAIN_H_ */
