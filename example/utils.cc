#include "utils.h"
#include <stdio.h>

std::string load_file(const char *fname, size_t maxlen)
{
	FILE *f = fopen(fname, "rb");
	if (f == NULL)
		return "";
	if (maxlen == 0) {
		fseek(f, 0, SEEK_END);
		maxlen = ftell(f);
		fseek(f, 0, SEEK_SET);
	}

	std::string buf(maxlen, 0);
	ssize_t ret = fread(&buf[0], 1, maxlen, f);
	fclose(f);
	if (ret < 0)
		return "";
	buf.resize(ret);
	return buf;
}

int write_file(const char *fname, const std::string &buf)
{
	FILE *f = fopen(fname, "wb");
	if (f == NULL)
		return -1;
	if (fwrite(buf.data(), 1, buf.size(), f) < buf.size()) {
		fclose(f);
		return -1;
	}
	fclose(f);
	return 0;
}
