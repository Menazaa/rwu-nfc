/*
 * Wrapper file to build aes_example.cpp from examples directory
 * This allows building the example without overwriting main.cpp
 */

#ifdef BUILD_EXAMPLE_AES
#include "../examples/aes_example.cpp"
#endif
