#include <mpi.h>
#include <iostream>
#include <libpq-fe.h>
#include <cstdlib>
#include <ctime>

void checkConn(PGconn *conn) {
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    const char *dbHost = std::getenv("DB_HOST");
    const char *dbPort = std::getenv("DB_PORT");
    const char *dbUser = std::getenv("DB_USER");
    const char *dbPass = std::getenv("DB_PASSWORD");
    const char *dbName = std::getenv("DB_NAME");

    // Формируем строку подключения
    std::string connStr = "host=" + std::string(dbHost) + 
                          " port=" + std::string(dbPort) + 
                          " user=" + std::string(dbUser) + 
                          " password=" + std::string(dbPass) + 
                          " dbname=" + std::string(dbName);

    PGconn *conn = PQconnectdb(connStr.c_str());
    checkConn(conn);

    if (ProcRank == 0) {
        // Выполним простой запрос
        PGresult *res = PQexec(conn, "SELECT NOW()");
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        } else {
            std::cout << "Current time from DB: " << PQgetvalue(res, 0, 0) << std::endl;
        }
        PQclear(res);
    }

    PQfinish(conn);

    MPI_Finalize();
    return 0;
}
