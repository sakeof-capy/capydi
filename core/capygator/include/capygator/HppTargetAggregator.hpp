#ifndef HPPTARGETAGGREGATOR_HPP_
#define HPPTARGETAGGREGATOR_HPP_

#include "IDataAggregator.hpp"
#include "DelimFileReader.hpp"
#include "HppTarget.hpp"

class HppTargetAggregator : public IDataAggregator
{
public:
    ~HppTargetAggregator() override = default;

public:

    void process_single_unit(const ProcessedUnitType& hppdi_file_path) override
    {
        const FileSystem::FileContentType DI_SUBSCRIBE_MARK = "DI_SUBSCRIBER(";
        DelimFileReader tokenizer(hppdi_file_path, "\n", 20u);

        aggregee_.include(hppdi_file_path);

        for
        (
            auto token_opt = tokenizer.next_token();
            token_opt.has_value();
            token_opt = tokenizer.next_token()
        ) {
            auto& token = *token_opt;

            auto found_mark = token.find(DI_SUBSCRIBE_MARK);

            if (found_mark != std::string::npos)
            {
                std::size_t first_ndx = found_mark + DI_SUBSCRIBE_MARK.size();
                std::size_t i = found_mark + DI_SUBSCRIBE_MARK.size();
                for (; i < token.size() && token[i] != ')'; ++i);
                auto subscriber_type = token.substr(first_ndx, i - first_ndx);
                aggregee_.add_subscriber_type(std::move(subscriber_type));
            }
        }
    }

    std::string get_aggregated_data() const noexcept override
    {
        return aggregee_.to_file_content();
    }

private:
    HppTarget aggregee_;
};

#endif //HPPTARGETAGGREGATOR_HPP_
