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
#include <SFML/System/switch/ThreadImpl.hpp>
#include <SFML/System/Thread.hpp>
#include <nn/os.h>
#include <iostream>
#include <cassert>

#define THREAD_STACK_SIZE  (4194304) // 4MB

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ThreadImpl::ThreadImpl(Thread* owner)
: m_ThreadStack(nullptr)
, m_thread()
, m_isActive(true)
{
#if defined( NN_BUILD_TARGET_PLATFORM_OS_NN ) && defined( NN_BUILD_APISET_NX )
    m_ThreadStack = aligned_alloc(nn::os::ThreadStackAlignment, THREAD_STACK_SIZE);
#else
    m_ThreadStack = _aligned_malloc(THREAD_STACK_SIZE, nn::os::ThreadStackAlignment);
#endif
    if (!m_ThreadStack)
        std::cerr << "Failed to create thread (out of memory)" << std::endl;
    else
    {
        auto result = nn::os::CreateThread(&m_thread, ThreadImpl::entryPoint, owner, m_ThreadStack, THREAD_STACK_SIZE, nn::os::DefaultThreadPriority);
        m_isActive  = result.IsSuccess();

        if (!m_isActive)
            std::cerr << "Failed to create thread" << std::endl;

        nn::os::StartThread(&m_thread);
    }
}

////////////////////////////////////////////////////////////
void ThreadImpl::wait()
{
    if (m_isActive)
    {
        assert(nn::os::GetCurrentThread() != m_thread); // A thread cannot wait for itself!
        nn::os::WaitThread(&m_thread);
    }
}

////////////////////////////////////////////////////////////
void ThreadImpl::terminate()
{
    if (m_isActive)
        nn::os::DestroyThread(&m_thread);

#if defined( NN_BUILD_TARGET_PLATFORM_OS_NN ) && defined( NN_BUILD_APISET_NX )
    free(m_ThreadStack);
#else
    _aligned_free(m_ThreadStack);
#endif

    m_ThreadStack = nullptr;
}

////////////////////////////////////////////////////////////
void ThreadImpl::entryPoint(void* userData)
{
    // The Thread instance is stored in the user data
    Thread* owner = static_cast<Thread*>(userData);

    // Forward to the owner
    owner->run();
}

} // namespace priv

} // namespace sf
