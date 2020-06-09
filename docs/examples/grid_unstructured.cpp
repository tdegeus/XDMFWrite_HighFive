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

    xt::xtensor<double,1> stress = {1.0, 2.0};

    H5Easy::File file("grid_unstructured.h5", H5Easy::File::Overwrite);

    H5Easy::dump(file, "/coor", coor);
    H5Easy::dump(file, "/conn", conn);
    H5Easy::dump(file, "/stress", stress);

    auto grid = xh::Grid({
        xh::Unstructured(file, "/coor", "/conn", xh::ElementType::Quadrilateral),
        xh::Attribute(file, "/stress", xh::AttributeCenter::Cell)});

    xh::write("grid_unstructured.xdmf", grid);

    return 0;
}
