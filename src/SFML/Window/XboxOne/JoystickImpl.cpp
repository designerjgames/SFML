#include <SFML/Window/JoystickImpl.hpp>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void JoystickImpl::initialize()
{
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanup()
{
}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnected(unsigned int index)
{
    return false;
}

////////////////////////////////////////////////////////////
void JoystickImpl::setLazyUpdates(bool status)
{

}

////////////////////////////////////////////////////////////
void JoystickImpl::updateConnections()
{
}

////////////////////////////////////////////////////////////
bool JoystickImpl::open(unsigned int index)
{
	return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::close()
{

}

////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilities() const
{
    JoystickCaps caps;

    return caps;
}


////////////////////////////////////////////////////////////
Joystick::Identification JoystickImpl::getIdentification() const
{
    return Joystick::Identification();
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::update()
{
    JoystickState state;

    state.connected = true;

    return state;
}


////////////////////////////////////////////////////////////
void JoystickImpl::initializeDInput()
{
  
}


////////////////////////////////////////////////////////////
void JoystickImpl::cleanupDInput()
{

}


////////////////////////////////////////////////////////////
bool JoystickImpl::isConnectedDInput(unsigned int index)
{
    return false;
}


////////////////////////////////////////////////////////////
void JoystickImpl::updateConnectionsDInput()
{
}


////////////////////////////////////////////////////////////
bool JoystickImpl::openDInput(unsigned int index)
{
}


////////////////////////////////////////////////////////////
void JoystickImpl::closeDInput()
{
}


////////////////////////////////////////////////////////////
JoystickCaps JoystickImpl::getCapabilitiesDInput() const
{
}


////////////////////////////////////////////////////////////
JoystickState JoystickImpl::updateDInput()
{
}

} // namespace priv

} // namespace sf
