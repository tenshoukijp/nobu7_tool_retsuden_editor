#pragma once

#include <string>

using namespace System;
using namespace System::Text;


// .NETタイプのString^ → マルチバイトのC++型std::stringへ
std::string DotNetStringToMultiByte(String^ dotnettext);

