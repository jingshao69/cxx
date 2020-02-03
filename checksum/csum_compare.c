#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

unsigned short checksum1(const char *buf, unsigned int size)
{
	unsigned sum = 0;
	int i;

	/* Accumulate checksum */
	for (i = 0; i < size - 1; i += 2)
	{
		unsigned short word16 = *(unsigned short *) &buf[i];
		sum += word16;
	}

	/* Handle odd-sized case */
	if (size & 1)
	{
		unsigned short word16 = (unsigned char) buf[i];
		sum += word16;
	}

	/* Fold to get the ones-complement result */
	while (sum >> 16) sum = (sum & 0xFFFF)+(sum >> 16);

	/* Invert to get the negative in ones-complement arithmetic */
	return ~sum;
}

unsigned short checksum2(const char *buf, unsigned int size)
{
	unsigned long long sum = 0;
	const unsigned long long *b = (unsigned long long *) buf;

	unsigned t1, t2;
	unsigned short t3, t4;

	/* Main loop - 8 bytes at a time */
	while (size >= sizeof(unsigned long long))
	{
		unsigned long long s = *b++;
		sum += s;
		if (sum < s) sum++;
		size -= 8;
	}

	/* Handle tail less than 8-bytes long */
	buf = (const char *) b;
	if (size & 4)
	{
		unsigned s = *(unsigned *)buf;
		sum += s;
		if (sum < s) sum++;
		buf += 4;
	}

	if (size & 2)
	{
		unsigned short s = *(unsigned short *) buf;
		sum += s;
		if (sum < s) sum++;
		buf += 2;
	}

	if (size)
	{
		unsigned char s = *(unsigned char *) buf;
		sum += s;
		if (sum < s) sum++;
	}

	/* Fold down to 16 bits */
	t1 = sum;
	t2 = sum >> 32;
	t1 += t2;
	if (t1 < t2) t1++;
	t3 = t1;
	t4 = t1 >> 16;
	t3 += t4;
	if (t3 < t4) t3++;

	return ~t3;
}

static inline uint32_t __rte_raw_cksum(const void *buf, size_t len, uint32_t sum)
{
        /* workaround gcc strict-aliasing warning */
        const unsigned short *u16 = (const unsigned short *) buf;

        while (len >= (sizeof(*u16) * 4)) {
                sum += u16[0];
                sum += u16[1];
                sum += u16[2];
                sum += u16[3];
                len -= sizeof(*u16) * 4;
                u16 += 4;
        }
        while (len >= sizeof(*u16)) {
                sum += *u16;
                len -= sizeof(*u16);
                u16 += 1;
        }

        /* if length is in odd bytes */
        if (len == 1)
                sum += *((const uint8_t *)u16);

        return sum;
}

static inline uint16_t
__rte_raw_cksum_reduce(uint32_t sum)
{
        sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
        sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
        return (uint16_t)sum;
}

/**
 * Process the non-complemented checksum of a buffer.
 *
 * @param buf
 *   Pointer to the buffer.
 * @param len
 *   Length of the buffer.
 * @return
 *   The non-complemented checksum.
 */
static inline uint16_t
rte_raw_cksum(const void *buf, size_t len)
{
        uint32_t sum;

        sum = __rte_raw_cksum(buf, len, 0);
        return ~(__rte_raw_cksum_reduce(sum));
}




typedef unsigned short (*CSUM_FUNC)(const char *buf, unsigned int size);

extern unsigned short checksum5(const char *buf, unsigned int size);
extern unsigned short checksum6(const char *buf, unsigned int size);
extern unsigned short checksum14(const char *buf, unsigned int size);

typedef struct 
{
	const char *desc;
	CSUM_FUNC func;
} algo_desc_t;

algo_desc_t CSUM_FUNC_TABLE[] = {
	{"checksum1", checksum1},
	{"checksum2", checksum2},
	{"rte_raw_cksum", rte_raw_cksum},
	{"checksum5", checksum5},
	{"checksum6", checksum6},
	{"checksum14", checksum14},
};

const int NUM_CSUM_FUNC = sizeof(CSUM_FUNC_TABLE)/sizeof(algo_desc_t);


char buf1[64];
char buf2[1023];
char buf3[1024];

void time_func(CSUM_FUNC func, const char *buf, unsigned int size)
{
	clock_t begin = clock();
	for (int i=0; i< 0x1000000; i++)
	{
		func(buf, size);
	}
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf(" %d: %.2fs\n", size, time_spent);
}


int main()
{
        for (int i=0; i< 1024; i++)
        {
                 buf3[i] = (char) i*i;
        }

	for (int i=0; i< NUM_CSUM_FUNC; i++)
	{
                unsigned short cksum = CSUM_FUNC_TABLE[i].func(buf3, 1024);
                printf("%s: 0x%04x\n", CSUM_FUNC_TABLE[i].desc, cksum);
        }

	for (int i=0; i< NUM_CSUM_FUNC; i++)
	{
		printf("%s:\n", CSUM_FUNC_TABLE[i].desc);
		time_func(CSUM_FUNC_TABLE[i].func, buf1, 64);
		time_func(CSUM_FUNC_TABLE[i].func, buf2, 1023);
		time_func(CSUM_FUNC_TABLE[i].func, buf3, 1024);
	}
}

	
		

