#ifndef _UTILS_H
#define _UTILS_H

#include <string>

std::string load_file(const char *fname, size_t maxlen = 0);
int write_file(const char *fname, const std::string &buf);

#endif
