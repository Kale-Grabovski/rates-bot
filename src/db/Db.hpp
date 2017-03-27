#ifndef DOBBI_H
#define DOBBI_H

#include <string>

template <typename T>
class Db 
{
public:
    Db(std::string h, std::string u, std::string d, std::string p) 
        : m_host(h), m_username(u), m_database(d), m_password(p) {}
    Db() {}
    virtual ~Db() {}
    std::string getHost() const { return m_host; }
    std::string getUsername() const { return m_username; }
    std::string getDatabase() const { return m_database; }
    std::string getPassword() const { return m_password; }
    virtual std::string getConnectionString() const = 0;
    virtual void query(std::string) const = 0;

protected: 
    T* m_conn;
    std::string m_host;
    std::string m_username;
    std::string m_database;
    std::string m_password;
};

#endif // DOBBI_H
