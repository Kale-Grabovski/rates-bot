#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <locale>
#include <postgresql/libpq-fe.h>
#include "src/dotenv/Dotenv.h"
#include "src/helpers/String.h"
#include "src/models/News.hpp"
#include "src/db/Pg.hpp"
#include "src/json.hpp"
#include <curl/curl.h>
#include <codecvt>

using namespace std;
using json = nlohmann::json;

static string buffer;
static int writer(char *data, size_t size, size_t nmemb, string *buffer) {
    int result = 0;

    if (buffer != NULL) {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }

    return result;
}

int main () {
    setlocale(LC_ALL, "");

    CURL *curl;
    CURLcode res;
	Dotenv dotenv;
	string host     = dotenv.get("DB_HOST");
	string username = dotenv.get("DB_USERNAME");
	string database = dotenv.get("DB_DATABASE");
	string password = dotenv.get("DB_PASSWORD");

	Pg<PGconn> conn(host, username, database, password);
    News<Pg<PGconn>> news(conn);

    string url = "http://ru.investing.com/economic-calendar/Service/getCalendarFilteredData";

    curl = curl_easy_init();
    if (!curl) {
        return 1;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36");
    headers = curl_slist_append(headers, "Referer: http://ru.investing.com/economic-calendar/");
    headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

    string post_data = "country[]=4&country[]=17&country[]=72&country[]=26&country[]=10&country[]=5&country[]=22&country[]=12&importance[]=3&timeZone=18&timeFilter=timeRemain&currentTab=today&submitFilters=1&limit_from=0&dateFrom=2016-07-07&dateTo=2016-07-07";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str() );
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.length() );

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cout << "curl_easy_perform() failed: %s\n", curl_easy_strerror(res);
    }

    //cout << buffer << endl; 
    //buffer = "fwoefjo datetime=\"eijfewij iefjewf";
    //vector<string> news = String::split(buffer, "datetime=\"");

/*    auto j3 = json::parse(buffer);
    string s = j3.dump();
*/
    cout << buffer;

/*    cout << buffer.substr(81, 83);

    news.create(buffer.substr(81, 83), "EUR", "2016-10-10 00:01:01+0");
*/    
    curl_easy_cleanup(curl);

	return 0;
}

// curl 'http://ru.investing.com/economic-calendar/Service/getCalendarFilteredData' 
// -H 'Origin: http://ru.investing.com' 
// -H 'Accept-Encoding: gzip, deflate' 
// -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36' 
// -H 'Accept-Language: en-US,en;q=0.8,ru;q=0.6,de;q=0.4' 
// -H 'Content-Type: application/x-www-form-urlencoded' 
// -H 'Accept: */*' 
// -H 'Referer: http://ru.investing.com/economic-calendar/' 
// -H 'X-Requested-With: XMLHttpRequest' 
// --data 'country[]=4&country[]=17&country[]=72&country[]=26&country[]=10&country[]=5&country[]=22&country[]=12&importance[]=3&timeZone=18&timeFilter=timeRemain&currentTab=today&submitFilters=1&limit_from=0&dateFrom=2016-07-07&dateTo=2016-07-07'
// --compressed
