
#include <iostream>
#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

int main(int argc, char const *argv[])
{
    std::cout << "Starting ..." << std::endl;
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if (all_platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    std::cout << "Found Platform !" << std::endl;
    return 0;
}
