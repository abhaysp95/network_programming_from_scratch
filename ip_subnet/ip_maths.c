#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PREFIX_LEN 16

// #define DEBUG

void get_broadcast_address(const char *, char, char *);
void print_ip_bits(const char*);

int main(void) {
	char ipaddr_buffer[PREFIX_LEN];
	memset(ipaddr_buffer, '\0', PREFIX_LEN);
	char *ip_addr = "192.168.2.1";
	char mask = 20;
	get_broadcast_address(ip_addr, mask, ipaddr_buffer);
	printf("Broadcast addr = %s\n", ipaddr_buffer);

	return 0;
}

// don't use int(4-byte or less) for storing ipv4, cause it'll work upto 2^31 not, 2^32
void get_broadcast_address(const char *ip_addr, char mask, char *ipaddr_buffer) {
	char *temp_addr = (char *)malloc(strlen(ip_addr));
	strncpy(temp_addr, ip_addr, strlen(ip_addr));
	char *quad = strtok(temp_addr, ".");

	char ip[32] = { 0 };
	int counter = 24;

	while (quad != NULL) {
		int i = 0;
		int q = atoi(quad);
		while (q != 0) {
			ip[counter + i++] |= (q & 1);
			q >>= 1;
		}
		counter -= 8;
		quad = strtok(NULL, ".");
	}
#ifdef DEBUG
	print_ip_bits(ip);
#endif

	for (int i = 0; i < 32 - mask; i++) {
		ip[i] = 1;
	}
#ifdef DEBUG
	print_ip_bits(ip);
#endif

	int num = 0;
	int clen = 0;
	counter = 0;
	for (int i = 31; i >= 0; i--) {
		num |= ip[i];
		if (i % 8 == 0) {
			clen += sprintf(ipaddr_buffer + clen, "%d", num);
			if(counter++ < 3) ipaddr_buffer[clen++] = '.';
			num = 0;
		} else num <<= 1;  // shift only 7 times, not 8
	}
}

void print_ip_bits(const char* ip) {
	for (int i = 31; i >= 0 ; i--) {
		printf("%d", ip[i]);
	}
	printf("\n");
}
