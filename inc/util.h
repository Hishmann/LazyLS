#pragma once

#include "pch.h"

#include <termios.h>

void set_raw_mode(bool enable);

void signalHandler(int signum);