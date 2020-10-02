////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/switch/ThreadLocalImpl.hpp>
#include <nn/os.h>
#include <iostream>
#include <cassert>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadLocalImpl::ThreadLocalImpl() :
m_key()
{
    nn::Result      result = nn::os::AllocateTlsSlot(&m_key, nullptr);
    if (!result.IsSuccess())
        std::cerr << "Failed to create Thread Local Storage" << std::endl;
}

////////////////////////////////////////////////////////////
ThreadLocalImpl::~ThreadLocalImpl()
{
    nn::os::FreeTlsSlot(m_key);
}

////////////////////////////////////////////////////////////
void ThreadLocalImpl::setValue(void* value)
{
    nn::os::SetTlsValue(m_key, reinterpret_cast<uintptr_t>(value));
}

////////////////////////////////////////////////////////////
void* ThreadLocalImpl::getValue() const
{
    return  reinterpret_cast<char*>(nn::os::GetTlsValue(m_key));
}

} // namespace priv

} // namespace sf
