#ifndef NEWS_H
#define NEWS_H

#include <string>
#include <vector>

template <typename T>
class News
{
public:
	News(T);
    News();
    void create(std::string, std::string, std::string);

private:
	T m_conn;
};

template <typename T>
News<T>::News(const T conn) : m_conn(conn) {}

template <typename T>
void News<T>::create(std::string title, std::string currency, std::string ts) {
    m_conn.query("INSERT INTO news (title, currency, created_at) "
        "VALUES (""'" + title + "', ""'" + currency + "', ""'" + ts + "')");
}

#endif // NEWS_H
