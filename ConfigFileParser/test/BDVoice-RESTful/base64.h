/*
对文件进行base64编码
*/

#ifndef  __ABASE64_H_
#define  __ABASE64_H_

#include <string.h>
#include <iostream>


std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);


#endif  //__BASE64_H_
