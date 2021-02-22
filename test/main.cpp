
#define CATCH_CONFIG_MAIN  // tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <GooseFEM/GooseFEM.h>
#include <XDMFWrite_HighFive.hpp>

namespace xh = XDMFWrite_HighFive;

TEST_CASE("XDMFWrite_HighFive", "Basic")
{

    SECTION("version")
    {
        std::cout << xh::version() << std::endl;
    }

    SECTION("GooseFEM")
    {
        {
            auto t = GooseFEM::Mesh::ElementType::Tri3;
            REQUIRE(xh::detail::to<decltype(t)>::str(t) == "Triangle");
        }
        {
            auto t = GooseFEM::Mesh::ElementType::Quad4;
            REQUIRE(xh::detail::to<decltype(t)>::str(t) == "Quadrilateral");
        }
        {
            auto t = GooseFEM::Mesh::ElementType::Hex8;
            REQUIRE(xh::detail::to<decltype(t)>::str(t) == "Hexahedron");
        }
    }
}


