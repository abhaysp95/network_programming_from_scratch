#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PREFIX_LEN 16

// #define DEBUG

void get_broadcast_address(const char *, char, char *);
void get_broadcast_address2(const char *, const char, char *);
unsigned int get_ip_integral_equivalent2(const char *);
void print_ip_bits(const char*);
unsigned int get_ip_integral_equivalent(char*);

int main(int argc, char **argv) {
	char ipaddr_buffer[PREFIX_LEN];
	memset(ipaddr_buffer, '\0', PREFIX_LEN);
	char *ip_addr = "192.168.2.1";
	char mask = 20;

	/* if (argc != 2) {
		fprintf(stderr, "usage: ./a.out [1|2] [[ipv4 mask]|[ipv4]]");
		exit(1);
	} */

	/* get_broadcast_address(ip_addr, mask, ipaddr_buffer);
	printf("Broadcast addr = %s\n", ipaddr_buffer); */

	// printf("ip integral equivalent: %u\n", get_ip_integral_equivalent(ip_addr));
	// get_ip_integral_equivalent2(ip_addr);

	get_broadcast_address2(ip_addr, mask, ipaddr_buffer);
	printf("Broadcast addr = %s\n", ipaddr_buffer);

	return 0;
}

// ip_buff should be of size 32
void get_ip_bits(const char *ip_addr, char *ip_buff) {
	char *temp_addr = (char *)malloc(strlen(ip_addr));
	strncpy(temp_addr, ip_addr, strlen(ip_addr));
	char *quad = strtok(temp_addr, ".");

	int counter = 24;

	while (quad != NULL) {
		int i = 0;
		int q = atoi(quad);
		while (q != 0) {
			ip_buff[counter + i++] |= (q & 1);
			q >>= 1;
		}
		counter -= 8;
		quad = strtok(NULL, ".");
	}
	free(temp_addr);
}

// don't use int(4-byte or less) for storing ipv4, cause it'll work upto 2^31 not, 2^32
void get_broadcast_address(const char *ip_addr, char mask, char *ipaddr_buffer) {
	char ip_buf[32] = { 0 };
	get_ip_bits(ip_addr, ip_buf);

#ifdef DEBUG
	print_ip_bits(ip);
#endif

	for (int i = 0; i < 32 - mask; i++) {
		ip_buf[i] = 1;
	}
#ifdef DEBUG
	print_ip_bits(ip);
#endif

	int num = 0;
	int clen = 0;
	int counter = 0;
	for (int i = 31; i >= 0; i--) {
		num |= ip_buf[i];
		if (i % 8 == 0) {
			clen += sprintf(ipaddr_buffer + clen, "%d", num);
			if(counter++ < 3) ipaddr_buffer[clen++] = '.';
			num = 0;
		} else num <<= 1;  // shift only 7 times, not 8
	}
}

void get_broadcast_address2(const char *ip_addr, const char mask, char *ipaddr_buffer) {
	unsigned int ip = get_ip_integral_equivalent2(ip_addr);

	for (int i = 0; i < 32 - mask; i++) {
		ip |= 1 << i;
	}

	int num = 0, clen = 0, counter = 0;
	for (unsigned int i = 32; i > 0; i--) {
		int k =  (ip & (1 << (i - 1))) ? 1 : 0;
		num |= k;
		if ((i - 1) % 8 == 0) {
			clen += sprintf(ipaddr_buffer + clen, "%d", num);
			if (counter++ < 3) ipaddr_buffer[clen++] = '.';
			num = 0;
		} else num <<= 1;
	}
}

// use "unsigned int" as it doesn't have signed bit, so it can work for full 32 bit of ipv4 address
unsigned int get_ip_integral_equivalent2(const char *ip_addr) {
	char *temp_addr = (char *)malloc(PREFIX_LEN);
	strncpy(temp_addr, ip_addr, PREFIX_LEN);
	char *quad = strtok(temp_addr, ".");
	int counter = 0;
	unsigned int ip = 0;
	while (quad != NULL) {
		int q = atoi(quad);
		int k = 7;
		while (k >= 0) {
			ip |= q & (1 << k--);  // check the kth bit and set it in ip
		}
		quad = strtok(NULL, ".");
		if (quad != NULL) {
			ip <<= 8;  // get 8 zeroes on right side
		}
	}

	return ip;
}

unsigned int get_ip_integral_equivalent(char *ip_addr) {
	char ip_buf[32] = { 0 };
	get_ip_bits(ip_addr, ip_buf);

	unsigned int ip = 0;
	for (int i = 31; i >= 0; i--) {
		ip |= ip_buf[i];
		if(i != 0) ip <<= 1;
	}

	return ip;
}

void print_ip_bits(const char* ip) {
	for (int i = 31; i >= 0 ; i--) {
		printf("%d", ip[i]);
	}
	printf("\n");
}
