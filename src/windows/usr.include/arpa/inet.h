#pragma once

#define inet_aton(cp, inp) inet_pton(AF_INET, cp, inp)
