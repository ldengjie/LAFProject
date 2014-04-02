#include "LafKernel/LafException.h"

LafException::LafException(const std::string& msg)
    : m_msg(msg)
{
}

LafException::~LafException() throw()
{
}

const char* LafException::what() const throw()
{
    return m_msg.c_str();
}
