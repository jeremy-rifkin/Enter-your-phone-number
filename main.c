// Jeremy Rifkin 2019
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef int bool;
#define true 1
#define false 0

#define BUFFER_SIZE 131072
// seems to be optimal on my system. tested many sizes as well with/without posix_fadvise

int main(int argc, char* argv[]) {
	if(argc == 1) {
		printf("Error: must specify path to input file\n");
		return 1;
	}
	
	char* filename = argv[1];
	int fd = open(filename, O_RDONLY);
	if(fd == -1) {
		printf("Error: failed to open file\n");
		return 1;
	}

	uint64_t maxPhoneNumbers = 10000000000;
	uint64_t bitmapSize = maxPhoneNumbers / 8 + (maxPhoneNumbers % 8 > 0 ? 1 : 0); // ceil division
	char *bitmap = malloc(bitmapSize);

	uint64_t phoneNumbersFound = 0; // how many phone numbers we've found
	uint64_t duplicates = 0; // how many duplicated phone numbers were there
	
	// bitmap should be all zeros, but just to make sure...
	printf("Verifying bitmap...\n");
	for(int i = 0; i < bitmapSize; i++)
		if(bitmap[i] != 0)
			bitmap[i] = 0;
	printf("Verified bitmap\n");

	printf("Processing digits...\n");
	// Advise the kernel of our access pattern.
	posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);  // FDADVICE_SEQUENTIAL
	
	char buf[BUFFER_SIZE];
	size_t bytes_read;
	lseek(fd, 0, SEEK_END);
	double filebytes = (double)lseek(fd, 0, SEEK_CUR);
	lseek(fd, 0, SEEK_SET);

	uint64_t phoneNumber = 0;

	// Load the first 9 digits after the decimal point to get us going
	if(read(fd, buf, 11) != 11) {
		printf("Error: unable to read first 11 bytes of file??\n");
		return 1;
	}
	for(int i = 2; i < 11; i++) {
		phoneNumber *= 10;
		phoneNumber += buf[i] - '0';
	}

	int l = 0;
	while(bytes_read = read(fd, buf, BUFFER_SIZE)) {
		//if(l++ % 100 == 0)
		//	printf("\r%05.2f%%", (double)lseek(fd, 0, SEEK_CUR) / filebytes * 100.);
		if(bytes_read == -1) {
			printf("Error: read failed\n");
			return 1;
		}
		for(int i = 0; i < bytes_read; i++) {
			// Load new phone number based off of past 10 digits
			phoneNumber = (phoneNumber * 10) % 10000000000 + buf[i] - '0';
			// Handle book keeping
			int b = 0x1 << (phoneNumber % 8);
			if(bitmap[phoneNumber / 8] & b)
				duplicates++;
			else {
				bitmap[phoneNumber / 8] |= b;
				phoneNumbersFound++;
				if(phoneNumbersFound == maxPhoneNumbers)
					goto breakread;
			}
		}
	}
	breakread:
	printf("\n");
	printf("Done!\n");
	
	printf("Found %011.8f%% of phone numbers\n",
		(double)phoneNumbersFound / (double)maxPhoneNumbers * 100.);
	printf("%lu duplicate phone numbers ocurred\n", duplicates);

	if(phoneNumbersFound == maxPhoneNumbers) {
		printf("Program reports it found all phone numbers!\n");
		printf("Verifying...\n");
		bool good = true;
		for(int i = 0; i < bitmapSize; i++)
			if(bitmap[i] != 0xff) {
				printf("Verification failed %d\n", bitmap[i]);
				good = false;
				break;
			}
		if(good) {
			printf("Verification success! All phone numbers found\n");
		}
	}

	free(bitmap);

	return 0;
}
