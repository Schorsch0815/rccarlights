/*
 * LightGroup.h
 *
 *  Created on: Nov 20, 2015
 *      Author: ejocsch
 */

#ifndef LIGHTGROUP_H_
#define LIGHTGROUP_H_

#include <stdio.h>

class LightGroup
{
public:
    LightGroup();
    virtual ~LightGroup();

    virtual void refresh() = 0;
};

#endif /* LIGHTGROUP_H_ */
