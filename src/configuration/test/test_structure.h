#ifndef SRC_CONFIGURATION_TEST_TEST_STRUCTURE_H
#define SRC_CONFIGURATION_TEST_TEST_STRUCTURE_H

#include <variant>
struct Numeric
{
    double tolerance = 1e-6;
};

struct Null
{

};

struct Double 
{
    double value = 0;
};

struct Int 
{
    int value = 0;
};

struct Various_Data
{
    std::variant<Int, Double, Null> type;
};

#endif //SRC_CONFIGURATION_TEST_TEST_STRUCTURE_H