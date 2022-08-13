#include <cfg.h>

#include <stdio.h>

// #define CFG_DEBUG

config_node::config_node(char* key, char* value) {
	int key_len = strlen(key);
	this->key = (char*) malloc(key_len + 1);
	memset(this->key, 0, key_len + 1);
	strcpy(this->key, key);

	int value_len = strlen(value);
	this->value = (char*) malloc(value_len + 1);
	memset(this->value, 0, value_len + 1);
	strcpy(this->value, value);
}

config_node::~config_node() {
	free(this->key);
	free(this->value);
}

config_loader::config_loader(char* cfg) : nodes(10) {
	int cfg_len = strlen(cfg);
	char* cfg_copy = (char*) malloc(cfg_len + 1);
	memset(cfg_copy, 0, cfg_len + 1);
	strcpy(cfg_copy, cfg);

	char* line = cfg_copy;
	for (int i = 0; i < cfg_len; i++) {
		if (cfg_copy[i] == '\n') {
			cfg_copy[i] = '\0';
		#ifdef CFG_DEBUG
			printf("got line: %s\n", line);
		#endif
			load_line(line);
			line = cfg_copy + i + 1;
		}
	}

#ifdef CFG_DEBUG
	printf("got line: %s\n", line);
#endif
	load_line(line);

	free(cfg_copy);
}

config_loader::~config_loader() {
	nodes.foreach<void*>([](void* _, list<config_node*>::node* n) {
		delete n->data;
	}, nullptr);
}


void config_loader::load_line(char* line) {
	int len = strlen(line);

	char* key = line;

	for (int i = 0; i < len; i++) {
		if (line[i] == '=') {
			line[i] = '\0';
			char* value = line + i + 1;
		#ifdef CFG_DEBUG
			printf("got key: %s, value: %s\n", key, value);
		#endif
			nodes.add(new config_node(key, value));
			break;
		}
	}
}

char* config_loader::get_key(char* key) {
	auto n = nodes.find<char*>([](char* key, list<config_node*>::node* n) {
		return strcmp(n->data->key, key) == 0;
	}, key);

	if (n != nullptr) {
		return n->data->value;
	} else {
		return nullptr;
	}
}