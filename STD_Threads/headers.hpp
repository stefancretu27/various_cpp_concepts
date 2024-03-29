#pragma once

#include "HelperClass.hpp"

#include <iostream>
#include <thread>
#include <memory>
#include <functional>
#include <mutex>
#include <tuple>
#include <algorithm>

void mutexInsights();
void atomicInsights();
void promise_futureInsights();
void conditionVariableInsights();

void joinThread(std::thread& threadInst);
