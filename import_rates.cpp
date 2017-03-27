#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <postgresql/libpq-fe.h>
#include "src/dotenv/Dotenv.h"
#include "src/helpers/String.h"
#include "src/db/Pg.hpp"
#include "src/models/Rates.hpp"

using namespace std;

const int EUR_USD_ID         = 1;
const int INSERTS_CHUNK_SIZE = 1000;

int main () {
	Dotenv dotenv;
	string host     = dotenv.get("DB_HOST");
	string username = dotenv.get("DB_USERNAME");
	string database = dotenv.get("DB_DATABASE");
	string password = dotenv.get("DB_PASSWORD");

	Pg<PGconn> conn(host, username, database, password);
    Rates<Pg<PGconn>> rates(conn);

    // Chunks with the rates
    vector<float> openList;
    vector<float> highList;
    vector<float> lowList;
    vector<float> closeList;
    vector<string> tsList;

    string line;
    int chunkCounter = 0;
    ifstream file("history/EURUSD.txt");

    while (getline(file, line)) {
        vector<string> cols = String::split(line, ",");

        // First row
        if (cols[0] == "<TICKER>") {
            continue;
        }

        openList.push_back(stof(cols[3]));
        highList.push_back(stof(cols[4]));
        lowList.push_back(stof(cols[5]));
        closeList.push_back(stof(cols[6]));
        tsList.push_back(cols[1] + " " + cols[2] + "+0"); // UTC+0

        // We should split on chunks to speed up inserts
        if (openList.size() == INSERTS_CHUNK_SIZE) {
            rates.addRates(EUR_USD_ID, openList, highList, lowList, closeList, tsList);

            openList.clear();
            highList.clear();
            lowList.clear();
            closeList.clear();
            tsList.clear();
        }

        cout << chunkCounter++ << endl;
    }

    // Insert the tail of the pack, cause we did not reach CHUNK_SIZE
    if (openList.size() > 0) {
        rates.addRates(EUR_USD_ID, openList, highList, lowList, closeList, tsList);
    }


	return 0;
}
