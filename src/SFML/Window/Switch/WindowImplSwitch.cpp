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
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Switch/WindowImplSwitch.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <nn/vi.h>
#include <nn/nn_Assert.h>

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace sf
{
namespace priv
{
WindowImplSwitch* WindowImplSwitch::singleInstance = NULL;

#if defined( NN_BUILD_TARGET_PLATFORM_OS_NN ) && defined( NN_BUILD_APISET_NX )
void* NvAllocateFunction(size_t size, size_t alignment, void* userPtr)
{
    NN_UNUSED(userPtr);
    // According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
    return aligned_alloc(alignment, nn::util::align_up(size, alignment));
}
void NvFreeFunction(void* addr, void* userPtr)
{
    NN_UNUSED(userPtr);
    free(addr);
}
void* NvReallocateFunction(void* addr, size_t newSize, void* userPtr)
{
    NN_UNUSED(userPtr);
    return realloc(addr, newSize);
}

void* NvDevtoolsAllocateFunction(size_t size, size_t alignment, void* userPtr)
{
    NN_UNUSED(userPtr);
    // According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
    return aligned_alloc(alignment, nn::util::align_up(size, alignment));
}
void NvDevtoolsFreeFunction(void* addr, void* userPtr)
{
    NN_UNUSED(userPtr);
    free(addr);
}
void* NvDevtoolsReallocateFunction(void* addr, size_t newSize, void* userPtr)
{
    NN_UNUSED(userPtr);
    return realloc(addr, newSize);
}
#endif

////////////////////////////////////////////////////////////
WindowImplSwitch::WindowImplSwitch(WindowHandle handle)
: m_window(nullptr)
, m_pDisplay(nullptr)
, m_pLayer(nullptr)
{
    singleInstance = this;
}


////////////////////////////////////////////////////////////
WindowImplSwitch::WindowImplSwitch(VideoMode mode, const String& title, unsigned long style, const ContextSettings* settings /*=nullptr*/)
: m_window(nullptr)
, m_pDisplay(nullptr)
, m_pLayer(nullptr)
{
    singleInstance = this;

    nn::vi::LayerCreationSettings xLCS{};
    xLCS.SetWidth(mode.width);
    xLCS.SetHeight(mode.height);
    xLCS.SetFullscreen(style & Style::Fullscreen);

#if defined( NN_BUILD_TARGET_PLATFORM_OS_NN ) && defined( NN_BUILD_APISET_NX )
    /*
 * Set memory allocator for graphics subsystem.
 * This function must be called before using any graphics API's.
 */
    nv::SetGraphicsAllocator(NvAllocateFunction, NvFreeFunction, NvReallocateFunction, NULL);

    /*
     * Set memory allocator for graphics developer tools and NVN debug layer.
     * This function must be called before using any graphics developer features.
     */
    nv::SetGraphicsDevtoolsAllocator(NvDevtoolsAllocateFunction, NvDevtoolsFreeFunction, NvDevtoolsReallocateFunction, NULL);
    /*
     * Donate memory for graphics driver to work in.
     * This function must be called before using any graphics API's.
     */
    size_t graphicsSystemMemorySize = 8 * 1024 * 1024;
    void* graphicsHeap = malloc(graphicsSystemMemorySize);
    nv::InitializeGraphics(graphicsHeap, graphicsSystemMemorySize);

    /*
     * Initialize Video Interface (VI) system to display
     * to the target's screen
     */
    nn::vi::Initialize();

    nn::Result result = nn::vi::OpenDefaultDisplay(&s_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::CreateLayer(&s_pLayer, s_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::GetNativeWindow(&m_window, s_pLayer);
    NN_ASSERT(result.IsSuccess());
#else
    nn::vi::Initialize();

    nn::Result result = nn::vi::OpenDefaultDisplay(&m_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::CreateLayer(&m_pLayer, m_pDisplay, &xLCS);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::GetNativeWindow(&m_window, m_pLayer);
    NN_ASSERT(result.IsSuccess());
#endif
}


////////////////////////////////////////////////////////////
WindowImplSwitch::~WindowImplSwitch()
{
    nn::vi::DestroyLayer(m_pLayer);
    nn::vi::CloseDisplay(m_pDisplay);
    nn::vi::Finalize();
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplSwitch::getSystemHandle() const
{
    return (WindowHandle) m_window;
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::processEvents()
{
    //appletMainLoop();
    //hidScanInput();
}


////////////////////////////////////////////////////////////
Vector2i WindowImplSwitch::getPosition() const
{
    // Not applicable
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setPosition(const Vector2i& position)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
Vector2u WindowImplSwitch::getSize() const
{
    // TODO: Map to display size?
    return Vector2u(1280, 720);
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setSize(const Vector2u& size)
{
    // N/A
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setTitle(const String& title)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setMouseCursorVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setMouseCursorGrabbed(bool grabbed)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setMouseCursor(const CursorImpl& cursor)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::setKeyRepeatEnabled(bool enabled)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplSwitch::requestFocus()
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool WindowImplSwitch::hasFocus() const
{
    // TODO: use libnx somehow?
    return true;
}



} // namespace priv
} // namespace sf
