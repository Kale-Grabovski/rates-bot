#!/bin/sh
g++-5 -o $1 -g src/dotenv/Dotenv.cpp src/db/Pg.hpp \
src/json.hpp \
src/models/News.hpp src/models/Rates.hpp \
src/helpers/String.cpp $1.cpp -std=c++11 -lcurl -lpq && ./$1 && rm $1
