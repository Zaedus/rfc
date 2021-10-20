#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>

#define NAME "rfc"
#define VERSION "0.0.1"

#define RFC_BASE_URL "https://www.rfc-editor.org/rfc/rfc"

// Functions

size_t writefunc(void *contents, size_t size, size_t nmemb) {
	printf("%.*s\n", size * nmemb, contents);
	return size * nmemb;
}

void print_version() {
	printf("%s %s - %s\n", NAME, VERSION, curl_version());
}

void print_usage() {
	printf("Usage: %s [-h] [-V] [RFC NUMBER]\n", NAME);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		int rfc_number = 0;
		for (argc--, argv++; *argv; argc--, argv++) {
			if (strncasecmp(*argv, "-V", 2) == 0) {
				print_version();
				exit(EXIT_FAILURE);
			}
			else if (strncasecmp(*argv, "-h", 2) == 0) {
				print_usage();
				exit(EXIT_FAILURE);
			} else {
				rfc_number = atoi(*argv);
			}
		}

		CURL *curl_handle;
		CURLcode res;

		char *url = malloc(sizeof(RFC_BASE_URL) + (int)(rfc_number < 10 ? 2 : log10(rfc_number) + 1));
		sprintf(url, "%s%u.txt", RFC_BASE_URL, rfc_number);

		curl_global_init(CURL_GLOBAL_ALL);

		curl_handle = curl_easy_init();

		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writefunc);

		res = curl_easy_perform(curl_handle);
		
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl_handle);

	} else print_usage();

	return 0;
}
