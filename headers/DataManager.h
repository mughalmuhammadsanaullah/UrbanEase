#pragma once

#include "Store.h"

const int MIN_STORES = 100;
const char DATA_FILE[] = "urbaneaseData.txt";

const double LAT_MIN = 23.5, LAT_MAX = 37.0;
const double LON_MIN = 60.5, LON_MAX = 77.5;

Store generateStore(int index);

void saveAllStores(const Store* stores, int count, const char* file);

Store* loadAllStores(const char* file, int& outCount);

