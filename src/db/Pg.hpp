#ifndef DBPG_H
#define DBPG_H

#include "Db.hpp"
#include <string>
#include <postgresql/libpq-fe.h>

template <typename T>
class Pg : public Db<T> {
public:
	Pg(std::string, std::string, std::string, std::string);
	Pg() {}
	virtual ~Pg() {}
	virtual std::string getConnectionString() const;
    virtual void query(std::string) const;
};

template <typename T>
Pg<T>::Pg(std::string host, std::string username, std::string database, std::string password)
    : Db<T>(host, username, database, password) {
    Pg::m_conn = PQconnectdb(getConnectionString().c_str());

	if (PQstatus(Pg::m_conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection failed");
        exit(0);
	}
}

template <typename T>
std::string Pg<T>::getConnectionString() const {
    return "host=" + this->getHost() + " user=" + this->getUsername()
        + " password=" + this->getPassword() + " dbname=" + this->getDatabase();
}

template <typename T>
void Pg<T>::query(std::string query) const {
	PGresult *res;
	res = PQexec(Pg::m_conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Unable to perform query: %s", query.c_str());
        PQclear(res);
        exit(0);
    }

    PQclear(res);
}

#endif // DBPG_H
