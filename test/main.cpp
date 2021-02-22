
#define CATCH_CONFIG_MAIN  // tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <XDMFWrite_HighFive.hpp>

TEST_CASE("XDMFWrite_HighFive", "Basic")
{

    SECTION("version")
    {
        std::cout << XDMFWrite_HighFive::version() << std::endl;
    }

}


