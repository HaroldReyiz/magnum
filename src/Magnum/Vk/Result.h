#ifndef Magnum_Vk_Result_h
#define Magnum_Vk_Result_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Enum @ref Magnum::Vk::Result, macro @ref MAGNUM_VK_INTERNAL_ASSERT_RESULT()
 * @m_since_latest
 */

#include <Corrade/Containers/EnumSet.h>
#include <Corrade/Utility/Assert.h>

#include "Magnum/Magnum.h"
#include "Magnum/Vk/visibility.h"
#include "Magnum/Vk/Vulkan.h"

namespace Magnum { namespace Vk {

/**
@brief Result
@m_since_latest

Wraps a @type_vk_keyword{VkResult}.
@m_enum_values_as_keywords
@see @ref MAGNUM_VK_INTERNAL_ASSERT_RESULT()
*/
enum class Result: Int {
    /** Command successfully completed */
    Success = VK_SUCCESS,

    /** A fence or query has not yet completed */
    NotReady = VK_NOT_READY,

    /** A wait operation has not completed in the specified time */
    Timeout = VK_TIMEOUT,

    /** An event is signaled */
    EventSet = VK_EVENT_SET,

    /** An event is unsignaled */
    EventReset = VK_EVENT_RESET,

    /** A return array was too small for the result */
    Incomplete = VK_INCOMPLETE,

    /** A host memory allocation has failed */
    ErrorOutOfHostMemory = VK_ERROR_OUT_OF_HOST_MEMORY,

    /** A device memory allocation has failed */
    ErrorOutOfDeviceMemory = VK_ERROR_OUT_OF_DEVICE_MEMORY,

    /**
     * Initialization of an object could not be completed for
     * implementation-specific reasons
     */
    ErrorInitializationFailed = VK_ERROR_INITIALIZATION_FAILED,

    /** The logical or physical device has been lost */
    ErrorDeviceLost = VK_ERROR_DEVICE_LOST,

    /** Mapping of a memory object has failed */
    ErrorMemoryMapFailed = VK_ERROR_MEMORY_MAP_FAILED,

    /** A requested layer is not present or could not be loaded */
    ErrorLayerNotPresent = VK_ERROR_LAYER_NOT_PRESENT,

    /** A requested extension is not supported */
    ErrorExtensionNotPresent = VK_ERROR_EXTENSION_NOT_PRESENT,

    /** A requested feature is not supported */
    ErrorFeatureNotPresent = VK_ERROR_FEATURE_NOT_PRESENT,

    /**
     * The requested version of Vulkan is not supported by the driver or is
     * otherwise incompatible for implementation-specific reasons
     */
    ErrorIncompatibleDriver = VK_ERROR_INCOMPATIBLE_DRIVER,

    /** Too many objects of the type have already been created */
    ErrorTooManyObjects = VK_ERROR_TOO_MANY_OBJECTS,

    /** A requested format is not supported on this device */
    ErrorFormatNotSupported = VK_ERROR_FORMAT_NOT_SUPPORTED,

    /**
     * A pool allocation has failed due to fragmentation of the pool's memory
     */
    ErrorFragmentedPool = VK_ERROR_FRAGMENTED_POOL,

    /**
     * An unknown error has occurred; either the application has provided
     * invalid input, or an implementation failure has occurred
     */
    ErrorUnknown = VK_ERROR_UNKNOWN
};

/**
@debugoperatorenum{Result}
@m_since_latest
*/
MAGNUM_VK_EXPORT Debug& operator<<(Debug& debug, Result value);

/**
@brief Assert that a Vulkan function call doesn't fail
@m_since_latest

Compared to using @ref CORRADE_INTERNAL_ASSERT_OUTPUT() to verify that
@cpp call == VK_SUCCESS @ce, this macro also prints the result value. Otherwise
the behavior is the same, including interactions with
@ref CORRADE_STANDARD_ASSERT and @ref CORRADE_NO_ASSERT. Works with both plain
Vulkan functions returning @type_vk{Result} and APIs returning @ref Result.

You can override this implementation by placing your own
@cpp #define MAGNUM_VK_INTERNAL_ASSERT_RESULT @ce before including the
@ref Magnum/Vk/Result.h header.
*/
#ifndef MAGNUM_VK_INTERNAL_ASSERT_RESULT
#if defined(CORRADE_NO_ASSERT) || (defined(CORRADE_STANDARD_ASSERT) && defined(NDEBUG))
#define MAGNUM_VK_INTERNAL_ASSERT_RESULT(call)                              \
    static_cast<void>(call)
#elif defined(CORRADE_STANDARD_ASSERT)
#define MAGNUM_VK_INTERNAL_ASSERT_RESULT(call)                              \
    assert(Magnum::Vk::Result(call) == Magnum::Vk::Result::Success)
#else
#define MAGNUM_VK_INTERNAL_ASSERT_RESULT(call)                              \
    do {                                                                    \
        const Magnum::Vk::Result _CORRADE_HELPER_PASTE(magnumVkResult, __LINE__) = Magnum::Vk::Result(call);         \
        if(_CORRADE_HELPER_PASTE(magnumVkResult, __LINE__) != Magnum::Vk::Result::Success) {                                                       \
            Corrade::Utility::Error{Corrade::Utility::Error::defaultOutput()} << "Call " #call " failed with" << _CORRADE_HELPER_PASTE(magnumVkResult, __LINE__) << "at " __FILE__ ":" CORRADE_LINE_STRING; \
            std::abort();                                                   \
        }                                                                   \
    } while(false)
#endif
#endif

}}

#endif
