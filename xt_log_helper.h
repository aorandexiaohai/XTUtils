#pragma once
#include "xt_asyc_cout.h"
#define XTLOGE(message) ACOUT<<__FILE__<<","<<__FUNCTION__<<","<<__LINE__<<":"<<message<<std::endl;