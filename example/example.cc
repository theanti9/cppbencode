#include "bencode.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class user {
public:
	user() {}
	user(const std::string &name, int age) :
		m_name(name), m_age(age)
	{
	}

	int load(const ben::variant &v);
	ben::variant save() const;

	void display() const;

	void add_contact(const std::string &type, const std::string &value)
	{
		m_contacts.insert(type, value);
	}

private:
	std::string m_name;
	ben::map<std::string, std::string> m_contacts;
	int m_age;
};

int user::load(const ben::variant &v)
{
	ben::variant_map d;
	if (v.get(&d) ||
	    d.get("name").get(&m_name) ||
	    d.get("age").get(&m_age) ||
	    d.get("contacts").get(&m_contacts) ||
	    m_name.empty() ||
	    m_age < 0 || m_age >= 100) {
		return -1;
	}

	return 0;
}

ben::variant user::save() const
{
	ben::variant_map d;
	d.insert("name", m_name);
	d.insert("age", m_age);
	d.insert("contacts", m_contacts);
	return d;
}

void user::display() const
{
	printf("%s, %d\n", m_name.c_str(), m_age);
	printf("----------------\n");
	for (ben::map_const_iter<std::string, std::string> i(m_contacts);
	     i.valid(); i.next()) {
		printf("%s = %s\n", i.key().c_str(), i->c_str());
	}
}

std::list<user> users;

int load_users(const char *fname)
{
	std::string buf = load_file(fname);
	if (buf.empty())
		return -1;
	ben::variant data;
	ben::variant_list list;
	if (ben::decoder::decode_all(&data, buf))
		return -1;

	if (data.get(&list))
		return -1;

	users.clear();
	for (ben::list_iter<ben::variant> i(list); i.valid(); i.next()) {
		user user;
		if (user.load(*i))
			return -1;
		users.push_back(user);
	}

	return 0;
}

int save_users(const char *fname)
{
	ben::variant_list list;
	for (ben::list_iter<user> i(users); i.valid(); i.next()) {
		list.push_back(i->save());
	}
	std::string buf;
	if (ben::encode(&buf, list))
		return -1;
	return write_file(fname, buf);
}

std::string prompt(const char *s)
{
	while (1) {
		printf("%s? ", s);
		fflush(stdout);
		char buf[1024];
		if (fgets(buf, sizeof buf - 1, stdin) == NULL)
			return "";
		char *endp = &buf[strlen(buf)];
		while (endp > buf && isspace(endp[-1]))
			endp--;
		if (endp > buf) {
			*endp = 0;
			return buf;
		}
	}
}

void help()
{
	printf("Available commands:\n");
	printf(" add\n");
	printf(" list\n");
	printf(" quit\n");
}

int main(int argc, char **argv)
{
	load_users("users.db");
	help();
	while (1) {
		std::string cmd = prompt("command");
		if (cmd == "add") {
			printf("Please enter user details.\n");
			std::string name = prompt("name");
			std::string age = prompt("age");
			user user(name, atoi(age.c_str()));

			while (prompt("Add a contact") == "yes") {
				std::string type = prompt("type");
				std::string value = prompt("value");
				user.add_contact(type, value);
			}

			users.push_back(user);

		} else if (cmd == "list") {
			for (ben::list_iter<user> i(users); i.valid(); i.next()) {
				i->display();
				printf("\n");
			}

		} else if (cmd.empty() || cmd == "quit") {
			break;

		} else
			help();
	}

	save_users("users.db");
	return 0;
}
