#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>

#define NAME "rfc"
#define VERSION "0.0.1"

#define RFC_BASE_URL "https://www.rfc-editor.org/rfc/rfc"

// Data Structs

struct string {
	char *data;
	size_t length;
};

// Functions

size_t writefunc(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct string *str = (struct string *)userp;

	char *ptr = realloc(str->data, str->length + realsize + 1);
	if (!ptr) {
		printf("realloc() failed: not enough memory\n");
		return 0;
	}

	str->data = ptr;
	memcpy(&(str->data[str->length]), contents, realsize);
	str->length += realsize;
	str->data[str->length] = '\0';

	return realsize;
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

		struct string str;
		str.data = malloc(1);
		str.length = 0;

		char *url = malloc(sizeof(RFC_BASE_URL) + (int)(rfc_number == 0 ? 1 : log10(rfc_number) + 1));
		sprintf(url, "%s%u.txt", RFC_BASE_URL, rfc_number);

		curl_global_init(CURL_GLOBAL_ALL);

		curl_handle = curl_easy_init();

		curl_easy_setopt(curl_handle, CURLOPT_URL, url);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&str);

		res = curl_easy_perform(curl_handle);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		} else {
			printf("%s", str.data);
		}

		curl_easy_cleanup(curl_handle);

	} else print_usage();

	return 0;
}
