#ifndef LAF_PHY_EVENT_BUF_H
#define LAF_PHY_EVENT_BUF_H

#include "LafKernel/DataBuffer.h"
#include "LafKernel/PhyEvent.h"

typedef DataBuffer<PhyEvent> PhyEventBuf;

template<> DataBuffer<PhyEvent>::~DataBuffer();
template<> void DataBuffer<PhyEvent>::clear();

#endif
