//
// Created by Rhys on 5/16/2023.
//

#pragma once

#define GAME_TITLE "Sandboxington"

#define UPS 25

#include <iostream>

#define LOGGER std::cout << "DEBUG: "

template <class Type, class BaseClass>
concept CheckType = std::is_base_of<BaseClass, Type>::value;