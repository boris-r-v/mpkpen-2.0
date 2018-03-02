#include <client.h>
#include <version.h>
#include <iostream>

void MpkPen::Public::Client::get()
{
    std::cout <<"MpkPen::Client version:" << MpkPen::Define::version() << std::endl;
}
