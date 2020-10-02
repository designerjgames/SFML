////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/JoystickImpl.hpp>
#include <nn/hid.h>
#include <nn/hid/hid_Npad.h>
#include <cassert>

const nn::hid::NpadIdType NpadIds[] = {
    nn::hid::NpadId::No1,
    nn::hid::NpadId::No2,
    nn::hid::NpadId::No3,
    nn::hid::NpadId::No4,
    nn::hid::NpadId::No5,
    nn::hid::NpadId::No6,
    nn::hid::NpadId::No7,
    nn::hid::NpadId::No8,
    nn::hid::NpadId::Handheld
};

enum NX_CONTROLLER_KEY
{
    NX_CONTROLLER_KEYS_A,
    NX_CONTROLLER_KEYS_B,
    NX_CONTROLLER_KEYS_X,
    NX_CONTROLLER_KEYS_Y,
    NX_CONTROLLER_KEYS_LSTICK,
    NX_CONTROLLER_KEYS_RSTICK,
    NX_CONTROLLER_KEYS_L,
    NX_CONTROLLER_KEYS_R,
    NX_CONTROLLER_KEYS_ZL,
    NX_CONTROLLER_KEYS_ZR,
    NX_CONTROLLER_KEYS_PLUS,
    NX_CONTROLLER_KEYS_MINUS,
    NX_CONTROLLER_KEYS_DLEFT,
    NX_CONTROLLER_KEYS_DUP,
    NX_CONTROLLER_KEYS_DRIGHT,
    NX_CONTROLLER_KEYS_DDOWN,

    NX_CONTROLLER_KEY_TOTAL
};

namespace sf
{
namespace priv
{
JoystickImpl::JoystickImpl()
: m_uControllerID(0)
{
}

////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
    // Initializes the Npad.
    nn::hid::InitializeNpad();

    // Sets the style of operation to use.
    nn::hid::SetSupportedNpadStyleSet(nn::hid::NpadStyleFullKey::Mask | nn::hid::NpadStyleHandheld::Mask);

    // Sets the Npad to use.
    nn::hid::SetSupportedNpadIdType(NpadIds, NN_ARRAY_SIZE(NpadIds));
}

////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
}

////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    assert(index < NN_ARRAY_SIZE(NpadIds));

    nn::hid::NpadFullKeyState state;
    nn::hid::GetNpadState(&state, static_cast<nn::hid::NpadIdType>(index));

    return state.attributes.Test<nn::hid::NpadAttribute::IsConnected>();
}

////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
    return true;
}

////////////////////////////////////////////////////////////
void JoystickImpl::close()
{

}

////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    // To implement
    JoystickCaps caps      = JoystickCaps();
    caps.buttonCount       = NX_CONTROLLER_KEY_TOTAL;
    caps.axes[Joystick::X] = true;
    caps.axes[Joystick::Y] = true;
    return caps;
}

////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    Joystick::Identification ident = Joystick::Identification();
    ident.name = sf::String("Generic Switch controller");
    ident.productId = ident.vendorId = 0;
    return ident;
}

////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    auto sfmlState = JoystickState();

    nn::hid::NpadFullKeyState state;
    nn::hid::GetNpadState(&state, m_uControllerID);

    sfmlState.buttons[NX_CONTROLLER_KEYS_A]      = state.buttons.Test<nn::hid::NpadButton::A>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_B]      = state.buttons.Test<nn::hid::NpadButton::B>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_X]      = state.buttons.Test<nn::hid::NpadButton::X>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_Y]      = state.buttons.Test<nn::hid::NpadButton::Y>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_LSTICK] = state.buttons.Test<nn::hid::NpadButton::StickL>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_RSTICK] = state.buttons.Test<nn::hid::NpadButton::StickR>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_L]      = state.buttons.Test<nn::hid::NpadButton::L>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_R]      = state.buttons.Test<nn::hid::NpadButton::R>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_ZL]     = state.buttons.Test<nn::hid::NpadButton::ZL>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_ZR]     = state.buttons.Test<nn::hid::NpadButton::ZR>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_PLUS]   = state.buttons.Test<nn::hid::NpadButton:: Plus>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_MINUS]  = state.buttons.Test<nn::hid::NpadButton:: Minus>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_DLEFT]  = state.buttons.Test<nn::hid::NpadButton:: Left>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_DUP]    = state.buttons.Test<nn::hid::NpadButton:: Up>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_DRIGHT] = state.buttons.Test<nn::hid::NpadButton:: Right>();
    sfmlState.buttons[NX_CONTROLLER_KEYS_DDOWN]  = state.buttons.Test<nn::hid::NpadButton:: Down>();

    // Correlates with the input coordinates of the Left Stick.
    sfmlState.axes[Joystick::X] =  GetStickPosition(state.analogStickL.x);
    sfmlState.axes[Joystick::Y] = -GetStickPosition(state.analogStickL.y);

    sfmlState.axes[Joystick::U] = GetStickPosition(state.analogStickR.x);
    sfmlState.axes[Joystick::V] = GetStickPosition(state.analogStickR.y);

    sfmlState.connected = state.attributes.Test<nn::hid::NpadAttribute::IsConnected>();

    return sfmlState;
}

float JoystickImpl::GetStickPosition(int x)
{
    assert(x >= -nn::hid::AnalogStickMax && x <= nn::hid::AnalogStickMax);

    // Boundary values are returned immediately, so there are no rounding errors.
    if (x == 0)
    {
        return 0.0f;
    }

    if (x == ::nn::hid::AnalogStickMax)
    {
        return 100.0f;
    }

    if (x == -::nn::hid::AnalogStickMax)
    {
        return -100.0f;
    }

    // Returns the normalized position.
    return (static_cast<float>(x) / nn::hid::AnalogStickMax) * 100.0f;
}

} // namespace priv

} // namespace sf
