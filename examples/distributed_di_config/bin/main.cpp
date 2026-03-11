#include "di_generated/DI.hpp"
#include "SomeType.hpp"

#include <iostream>

using capy::di::Resolution;

int main()
{
    auto container = capy::di::build_up_container();
    
    Resolution<SomeOtherType, capy::di::Error> auto 
        resolution = container.resolve<SomeOtherType>();

    SomeOtherType& some_other = resolution.value();

    std::cout << "Result: " << some_other.operation() << std::endl;

    return EXIT_SUCCESS;
}
