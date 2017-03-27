#include <iostream>
#include <curl/curl.h>

using namespace std;

int main(void)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Authorization: Bearer 07a520224af64c09723ee3b9e0632519-848ca2687c1bb1005f73a84069401ca7");

        curl_easy_setopt(curl, CURLOPT_URL, "https://api-fxpractice.oanda.com/v3/accounts/101-004-4580004-001");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
 
        if (res != CURLE_OK) {
            cout << "curl_easy_perform() failed: %s\n", curl_easy_strerror(res);
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
