#ifndef SDOCOMMUNICATION_DATA_H
#define SDOCOMMUNICATION_DATA_H

#include "sdocommunication.h"

struct SDOCommunication_data {
    SDOCommunication::Type m_type;
    SDOCommunication::NodeId m_nodeId;
    SDOCommunication::Index m_index;
    SDOCommunication::SubIndex m_subIndex;
    void* m_data;
    size_t m_dataSize;
    int m_timeout;
    SDOCommunication::State m_state;
    SDOCommunication::Error m_error;
    bool m_cancel;
    size_t m_transferSize;
    size_t m_dataTransfered;
    size_t m_dataBuffered;
};

#endif // SDOCOMMUNICATION_DATA_H
