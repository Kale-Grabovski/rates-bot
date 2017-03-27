#ifndef RATES_H
#define RATES_H

#include <string>
#include <vector>

template <typename T>
class Rates
{
public:
	Rates(T);
    Rates();

    void addRate(int, float, float, float, float, std::string);
    void addRates(int, std::vector<float>, std::vector<float>, std::vector<float>, 
    	std::vector<float>, std::vector<std::string>);

private:
	std::string getInsertQuery(int, float, float, float, float, std::string);
	T m_conn;
};

template <typename T>
Rates<T>::Rates(const T conn) : m_conn(conn) {}

template <typename T>
std::string Rates<T>::getInsertQuery(int pair, float open, float high, float low, 
	float close, std::string ts) {
	return "INSERT INTO rates "
        "(pair, open, close, low, high, created_at) "
        "VALUES (" 
	        + std::to_string(pair) + ", " 
	        + std::to_string(open) + ", "
	        + std::to_string(close) + ", "
	        + std::to_string(low) + ", "
	        + std::to_string(high) + ", "
	        "'" + ts + "') "
	    "ON CONFLICT DO NOTHING"; // todo injection
}

template <typename T>
void Rates<T>::addRate(int pair, float open, float high, float low, 
	float close, std::string ts) {
    m_conn.query(getInsertQuery(pair, open, high, low, close));
}

template <typename T>
void Rates<T>::addRates(int pair, std::vector<float> openList, 
	std::vector<float> highList, std::vector<float> lowList, 
	std::vector<float> closeList, std::vector<std::string> tsList) {
	std::string inserts = "";

	for (int i = 0; i < openList.size(); i++) {
	    inserts += getInsertQuery(pair, openList[i], highList[i], 
	    	lowList[i], closeList[i], tsList[i]) + "; ";
	}

	m_conn.query(inserts);
}

#endif // RATES_H
