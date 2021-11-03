#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>
#include <errno.h>

#define NAME "rfc"
#define VERSION "0.0.1"

#define RFC_BASE_URL "https://www.rfc-editor.org/rfc/rfc"

long rfc_number = 0;

// Functions

size_t writefunc(void *contents, size_t size, size_t nmemb) {
	// print all the bytes stored in 'content'
	printf("%.*s\n", size * nmemb, contents);
	return size * nmemb;
}

void print_version() {
	printf("%s %s - %s\n", NAME, VERSION, curl_version());
}

void print_usage() {
	printf("Usage: %s [-h] [-V] [RFC NUMBER]\n", NAME);
}

void print_rfc() {
	CURL *curl_handle;
	CURLcode res;

	char *url = malloc(sizeof(RFC_BASE_URL) + 10);
	sprintf(url, "%s%lu.txt", RFC_BASE_URL, rfc_number);

	curl_global_init(CURL_GLOBAL_ALL);

	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl_handle, CURLOPT_FAILONERROR, 1L);

	res = curl_easy_perform(curl_handle);
	
	if (res != CURLE_OK) {
		if (res == CURLE_HTTP_RETURNED_ERROR) {
			long status;
			curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &status);

			if (status == 404) printf("error: invalid rfc number '%lu'\n", rfc_number);
			else printf("error: server returned status code %lu", status);

			exit(EXIT_FAILURE);
		} else fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
	}

	curl_easy_cleanup(curl_handle);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		char *end;

		// Iterate over all args and handle them accordingly
		for (argc--, argv++; *argv; argc--, argv++) {
			if (strncasecmp(*argv, "-V", 2) == 0 || strncasecmp(*argv, "--version", 10) == 0) {
				print_version();
				exit(EXIT_SUCCESS);
			}
			else if (strncasecmp(*argv, "-h", 2) == 0 || strncasecmp(*argv, "--help", 7) == 0) {
				print_usage();
				exit(EXIT_SUCCESS);
			} else {
				rfc_number = strtol(*argv, &end, 10);
				if (end == *argv || *end != '\0' || errno == ERANGE) {
					printf("error: invalid number '%s'\n", *argv);
					continue;
				}
				print_rfc();
			}
		}
	} else print_usage();

	return 0;
}
