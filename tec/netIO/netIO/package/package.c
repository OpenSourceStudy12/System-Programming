#include "package.h"

void *package(char *version, char *protocol, short type, int length, void *data)
{
	package_t *head = malloc(sizeof(package_t)+length);
	assert(head != NULL);

	strcpy(head->version, version);
	strcpy(head->protocol, protocol);
	head->type = type;
	head->length = length;

	memcpy(head->data, data, length);
	return head;
}

void *packageHead(package_t head, void *data)
{
	package_t *pack = malloc(sizeof(package_t)+head.length);
	assert(pack != NULL);

	memcpy(pack, &head, sizeof(head));
	memcpy(pack->data, data, head.length);
	return pack;
}
