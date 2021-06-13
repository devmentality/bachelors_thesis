#include <iostream>
#include "tcp_client.h"

int main(int argc, char *argv[])
{
    run_push("127.0.0.1", 12345);
    return 0;
}