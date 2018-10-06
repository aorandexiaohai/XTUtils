#pragma once
#include "xt_asyc_cout.h"
#define CODE_LOCATION __FILE__<<","<<__FUNCTION__<<","<<__LINE__
#define XTLOGE(message) ACOUT<< CODE_LOCATION <<":"<<message<<std::endl;