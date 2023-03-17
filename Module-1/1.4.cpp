#include <iostream>

uint32_t inverse_byte( uint32_t n, int k )
{
    return n ^ (1 << k);
}


int main()
{
    uint32_t n = 0;
    int k = 0;
    std::cin >> n >> k;

    uint32_t result = inverse_byte( n, k );

    std::cout << result;
    return 0;
}