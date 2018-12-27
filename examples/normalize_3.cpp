#include <iostream>
#include <cstring>

#include <pushkin/math/vector.hpp>
#include <pushkin/math/vector_io.hpp>

/// Read vectors of size 3 from standard input and output their dot product

using vec3f = psst::math::vector<float, 3>;

int
main(int argc, char* argv[])
{
    if (argc > 1) {
      if (std::strcmp(argv[1], "--pretty") == 0) {
          std::cout << psst::math::io::pretty << psst::math::io::set_col_width(8);
      }
    }
    while(std::cin) {
        vec3f v1;
        if (!(std::cin >> v1))
            break;
        std::cout << "normalize " << v1 << " = " << normalize(v1) << "\n";
    }
    return 0;
}
