#include "Common/MPU9250.h"
#include "Navio2/LSM9DS1.h"
#include <Common/MS5611.h>
#include <Navio2/ADC_Navio2.h>
#include <Navio+/ADC_Navio.h>
#include "Common/Util.h"
#include <unistd.h>
#include <string>
#include <memory>
#include <iostream>
#include "../include/constantes.hpp"
#include "../include/keys.hpp"

#define READ_FAILED -1

std::string cast(float f[3]);