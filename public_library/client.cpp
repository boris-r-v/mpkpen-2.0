#include <client.h>
#include <version.h>
#include <iostream>

void MpkPen::Client::get()
{
    std::cout <<"MpkPen::Client version:" << MpkPen::Define::version() << std::endl;
}
