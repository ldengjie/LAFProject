#ifndef LAF_EXCEPTION_H
#define LAF_EXCEPTION_H

#include <exception>
#include <string>

class LafException : public std::exception
{
    public :

	LafException(const std::string& msg);
	virtual ~LafException() throw();

	const char* what() const throw();

    private :

	std::string m_msg;
};

#endif
