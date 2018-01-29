#include "package.h"

void *package(char *version, char *protocol, int type, int length, void *data)
{
	package_t *head = malloc(sizeof(package_t)+length);
	assert(head != NULL);

	strcpy(head->version, version);
	strcpy(head->protocol, protocol);
	head->type = type;
	head->length = length;

	memcpy(head->data, data, length);

	return (void*)head;
}

void *packageHead(package_t pack, void *data)
{
	package_t *head = malloc(sizeof(package_t)+pack.length);
	assert(head != NULL);

	memcpy(head, &pack, sizeof(pack));
	memcpy(head->data, data, pack.length);

	return (void*)head;
}

int packageSend(int sockfd, char *version, char *protocol, int type, int length, void *data)
{
	void *p = package(version, protocol, type, length, data);

	int ret = write(sockfd, p, sizeof(package_t)+length);
	if (ret < 0)
		ret = -1;
	else
		ret = 0;

	free(p);
	return ret;
}

int packageHeadSend(int sockfd, package_t pack, void *data)
{
	void *p = packageHead(pack, data);

	int ret = write(sockfd, p, sizeof(package_t)+pack.length);
	if (ret < 0)
		ret = -1;
	else
		ret = 0;

	free(p);
	return ret;
}
