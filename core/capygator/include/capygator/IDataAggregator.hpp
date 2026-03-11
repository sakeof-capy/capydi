#ifndef I_DATA_AGGREGATOR_HPP_
#define I_DATA_AGGREGATOR_HPP_

#include "FileSystem.hpp"

class IDataAggregator
{
public:
    using ProcessedUnitType = FileSystem::PathType;

public:
    virtual ~IDataAggregator() = default;

public:
    virtual void process_single_unit(const ProcessedUnitType& unit_to_parse) = 0;
    virtual std::string get_aggregated_data() const noexcept = 0;
};

#endif // !I_DATA_AGGREGATOR_HPP_