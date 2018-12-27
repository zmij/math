#include <pushkin/math/vector.hpp>
#include <pushkin/math/vector_io.hpp>

#include <iostream>

/// Read vectors of size 3 from standard input and output their dot product

using vec3f = psst::math::vector<float, 3>;

int
main(int argc, char* argv[])
{
    std::cout << psst::math::io::pretty << psst::math::io::set_col_width(8);
    while (std::cin) {
        vec3f v1, v2;
        if (!(std::cin >> v1))
            break;
        if (!(std::cin >> v2))
            break;
        std::cout << v1 << " dot " << v2 << " = " << dot_product(v1, v2) << "\n";
    }
    return 0;
}
