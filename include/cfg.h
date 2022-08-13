#pragma once

#include <buildin/list.h>

class config_node {
	public:
		config_node(char* key, char* value);
		~config_node();

		char* key;
		char* value;
};

class config_loader {
	public:
		config_loader(char* cfg);
		~config_loader();

		char* get_key(char* key);

	private:
		list<config_node*> nodes;

		void load_line(char* line);
};