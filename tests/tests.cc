#include "bencode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

template<class T>
void test_decode(const T &expected, const std::string &buf)
{
	/* test decode */
	ben::variant data;
	if (ben::decoder::decode_all(&data, buf)) {
		printf("WARNING: unable to decode\n");
		printf("input: %s\n", buf.c_str());
		return;
	}

	T out;
	if (data.get(&out)) {
		printf("WARNING: unable to convert from variant\n");
		printf("input: %s\n", buf.c_str());
		return;
	}

	if (out != expected) {
		printf("WARNING: decoded data differs from expected\n");
		printf("input: %s\n", buf.c_str());
		return;
	}

	/* test encode */
	std::string encoded;
	if (ben::encode(&encoded, expected)) {
		printf("WARNING: unable to encode\n");
		return;
	}
	if (encoded != buf) {
		printf("WARNING: encoded data differs\n");
		printf("encoded: %s\n", encoded.c_str());
		printf("buf: %s\n", buf.c_str());
		return;
	}
}

void test_invalid_bencode(const std::string &buf)
{
	ben::variant data;
	if (ben::decoder::decode_all(&data, buf) == 0) {
		printf("WARNING: unexpected: able to decode\n");
		printf("input: %s\n", buf.c_str());
		return;
	}
}

template<class T>
void test_invalid_type(const std::string &buf)
{
	ben::variant data;
	if (ben::decoder::decode_all(&data, buf)) {
		printf("WARNING: unable to decode\n");
		printf("input: %s\n", buf.c_str());
		return;
	}

	T out;
	if (data.get(&out) == 0) {
		printf("WARNING: unexpected: able to convert from variant\n");
		printf("input: %s\n", buf.c_str());
		return;
	}
}

int main(int argc, char **argv)
{
	test_decode(1234, "i1234e");
	test_decode(-1234, "i-1234e");
	test_invalid_bencode("i 1e");
	test_invalid_bencode("i-0e");
	test_invalid_bencode("i");
	test_invalid_bencode("i1234");
	test_invalid_type<int>("3:foo");
	test_invalid_type<int>("de");

	test_decode(std::string("foo"), "3:foo");
	test_invalid_bencode("3:");
	test_invalid_bencode("3:fooz");
	test_invalid_bencode("1foo");
	test_invalid_type<std::string>("i1234e");
	test_invalid_type<std::string>("de");

	ben::map<std::string, std::string> sd;
	test_decode(sd, "de");
	sd.insert("foo", "bar");
	sd.insert("foz", "baz");
	test_decode(sd, "d3:foo3:bar3:foz3:baze");
	test_invalid_bencode("d3:foo3:bar3:foz3:baz");
	test_invalid_bencode("d3:foo3:bar1234");
	test_invalid_bencode("d3:foo3:bari1234e");
	test_invalid_type<ben::map<std::string, std::string> >
		("d3:foo3:bar3:fozi1234ee");

	std::list<std::string> sl;
	test_decode(sl, "le");
	sl.push_back("foo");
	sl.push_back("bar");
	test_decode(sl, "l3:foo3:bare");
	test_invalid_bencode("l3:foo3:bar");
	test_invalid_type<std::list<std::string> >("l3:fooi1234ee");

	printf("tests done.\n");
	return 0;
}
