//
// Created by Rhys on 5/16/2023.
//

#pragma once

#define GAME_TITLE "Sandboxington"

#define NAMESPACE namespace Sandbox

template <class Type, class BaseClass>
concept CheckType = std::is_base_of<BaseClass, Type>::value;