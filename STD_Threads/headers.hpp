#pragma once

#include "HelperClass.hpp"

#include <iostream>
#include <thread>
#include <memory>
#include <functional>
#include <mutex>
#include <tuple>

void mutexInsights();
void atomicInsights();
void promise_futureInsights();

void joinThread(std::thread& threadInst);
