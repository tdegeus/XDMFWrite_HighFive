
#define CATCH_CONFIG_MAIN  // tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <GooseFEM/GooseFEM.h>
#include <highfive/H5Easy.hpp>

#define XDMFWRITE_HIGHFIVE_INDENT 0
#include <XDMFWrite_HighFive.hpp>

namespace xh = XDMFWrite_HighFive;

TEST_CASE("XDMFWrite_HighFive", "Basic")
{

    SECTION("version")
    {
        std::cout << xh::version() << std::endl;
    }

    SECTION("GooseFEM - detail")
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

    SECTION("GooseFEM - mesh")
    {
        H5Easy::File file("tmp.h5", H5Easy::File::Overwrite);

        GooseFEM::Mesh::Quad4::Regular mesh(3, 3);

        H5Easy::dump(file, "/coor", mesh.coor());
        H5Easy::dump(file, "/conn", mesh.conn());

        std::vector<std::string> t = {
            "<Topology NumberOfElements=\"9\" TopologyType=\"Quadrilateral\">",
            "<DataItem Dimensions=\"9 4\" Format=\"HDF\">tmp.h5:/conn</DataItem>",
            "</Topology>"};

        std::vector<std::string> g = {
            "<Geometry GeometryType=\"XY\">",
            "<DataItem Dimensions=\"16 2\" Format=\"HDF\">tmp.h5:/coor</DataItem>",
            "</Geometry>"};

        REQUIRE(t == xh::Topology(file, "/conn", mesh.getElementType()));
        REQUIRE(g == xh::Geometry(file, "/coor"));
    }
}


