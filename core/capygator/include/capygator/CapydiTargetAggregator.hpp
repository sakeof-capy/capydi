#ifndef CAPYDI_TARGET_AGGREGATOR_HPP_
#define CAPYDI_TARGET_AGGREGATOR_HPP_

#include "IDataAggregator.hpp"
#include "DelimFileReader.hpp"
#include "CapydiTarget.hpp"

class CapydiTargetAggregator : public IDataAggregator
{
public:
    ~CapydiTargetAggregator() override = default;

public:

    void process_single_unit(const ProcessedUnitType& file_path) override
    {
        this->aggregee_.include(file_path);
    }

    std::string get_aggregated_data() const noexcept override
    {
        return this->aggregee_.to_file_content();
    }

private:
    CapydiTarget aggregee_;
};

#endif // !CAPYDI_TARGET_AGGREGATOR_HPP_
