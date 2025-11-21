#ifndef SRC_CONFIGURATION_TEST_TEST_STRUCTURE_TRAIT_H
#define SRC_CONFIGURATION_TEST_TEST_STRUCTURE_TRAIT_H

#include "test_structure.h"
#include "test_structure_builder.h"
#include "test_structure_Writer.h"
#include "include/configuration/module/traits.h"


template<>
struct O::Configuration::Module::Traits<Various_Data>
{
    using Builder = Various_Data_Builder;
    using Writer = Various_Data_Writer;
};

template<>
struct O::Configuration::Module::Traits<Numeric>
{
    using Builder = Numeric_Builder;
     using Writer = Numeric_Writer;
};

#endif //SRC_CONFIGURATION_TEST_TEST_STRUCTURE_TRAIT_H