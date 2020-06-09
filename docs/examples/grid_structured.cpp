#include <xtensor/xtensor.hpp>
#include <xtensor/xrandom.hpp>
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

    xt::xtensor<size_t,1> conn = xt::arange<size_t>(coor.shape(0));

    xt::xtensor<double,1> radius = xt::random::rand<double>({coor.shape(0)});

    H5Easy::File file("grid_structured.h5", H5Easy::File::Overwrite);

    H5Easy::dump(file, "/coor", coor);
    H5Easy::dump(file, "/conn", conn);
    H5Easy::dump(file, "/radius", radius);

    auto grid = xh::Grid({
        xh::Structured(file, "/coor", "/conn"),
        xh::Attribute(file, "/radius", xh::AttributeCenter::Node)});

    xh::write("grid_structured.xdmf", grid);

    return 0;
}
