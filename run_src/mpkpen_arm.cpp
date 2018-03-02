#include <iostream>
#include <version.h>
#include <client.h>
#include <sdg_arm.h>

int main(int argc, char** argv )
{

    MpkPen::Public::Client s;
    s.get();

    MpkPen::Private::Sdg_Arm::CommandPackHeader oph = MpkPen::Private::Sdg_Arm::create_pack();
    std::cout << oph.title << std::endl;
        
    return 0;
}