/*
 * cargo_train.h
 *
 *  Created on: Dec 5, 2015
 *      Author: pavel
 */

#ifndef CARGO_TRAIN_H_
#define CARGO_TRAIN_H_

#include "train.h"

class CCargoTrain : public CTrain
{
public:
    CCargoTrain(const CTrainGenerator& generator);
    virtual ~CCargoTrain();
};

#endif /* CARGO_TRAIN_H_ */
