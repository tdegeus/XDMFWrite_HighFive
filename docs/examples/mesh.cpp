#include <xtensor/xtensor.hpp>
#include <highfive/H5Easy.hpp>
#include <XDMFWrite_HighFive.hpp>

namespace xh = XDMFWrite_HighFive;

int main()
{
    xt::xtensor<double,2> coor = {
        {0, 0},
        {0, 1},
        {0, 2},
        {1, 0},
        {1, 1},
        {1, 2}};

    xt::xtensor<size_t,2> conn = {
        {0, 1, 4, 3},
        {1, 2, 5, 4}};

    H5Easy::File file("mesh.h5", H5Easy::File::Overwrite);

    H5Easy::dump(file, "/coor", coor);
    H5Easy::dump(file, "/conn", conn);

    auto grid = xh::Grid({
        xh::Geometry(file, "/coor"),
        xh::Topology(file, "/conn", xh::ElementType::Quadrilateral)});

    xh::write("test.xdmf", grid);

    return 0;
}
