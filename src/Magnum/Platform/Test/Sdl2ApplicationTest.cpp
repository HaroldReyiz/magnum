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

#include <Corrade/Containers/Optional.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Resource.h>

#include "Magnum/ImageView.h"
#include "Magnum/Platform/Sdl2Application.h"
#include "Magnum/Trade/AbstractImporter.h"
#include "Magnum/Trade/ImageData.h"

#ifdef CORRADE_TARGET_CLANG_CL
/* SDL does #pragma pack(push,8) and #pragma pack(pop,8) in different headers
   (begin_code.h and end_code.h) and clang-cl doesn't like that, even though it
   is completely fine. Silence the warning. */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpragma-pack"
#endif
#include <SDL_events.h>
#ifdef CORRADE_TARGET_CLANG_CL
#pragma clang diagnostic pop
#endif

namespace Magnum { namespace Platform { namespace Test { namespace {

struct Sdl2ApplicationTest: Platform::Application {
    explicit Sdl2ApplicationTest(const Arguments& arguments);

    void exitEvent(ExitEvent& event) override {
        Debug{} << "application exiting";
        event.setAccepted(); /* Comment-out to test app exit suppression */
    }

    void drawEvent() override {}

    /* For testing HiDPI resize events */
    void viewportEvent(ViewportEvent& event) override {
        Debug{} << "viewport event" << event.windowSize()
            #ifdef MAGNUM_TARGET_GL
            << event.framebufferSize()
            #endif
            << event.dpiScaling();
    }

    /* For testing event coordinates */
    void mousePressEvent(MouseEvent& event) override {
        Debug{} << "mouse press event:" << event.position() << Int(event.button());
    }

    void mouseReleaseEvent(MouseEvent& event) override {
        Debug{} << "mouse release event:" << event.position() << Int(event.button());
    }

    void mouseMoveEvent(MouseMoveEvent& event) override {
        Debug{} << "mouse move event:" << event.position() << event.relativePosition() << Uint32(event.buttons());
    }

    void mouseScrollEvent(MouseScrollEvent& event) override {
        Debug{} << "mouse scroll event:" << event.offset() << event.position();
    }

    void keyPressEvent(KeyEvent& event) override {
        Debug{} << "key press event:" << SDL_Keycode(event.key()) << event.keyName();

        if(event.key() == KeyEvent::Key::F1) {
            Debug{} << "starting text input";
            startTextInput();
        } else if(event.key() == KeyEvent::Key::Esc) {
            Debug{} << "stopping text input";
            stopTextInput();
        } else if(event.key() == KeyEvent::Key::T) {
            Debug{} << "setting window title";
            setWindowTitle("This is a UTF-8 Window Title™!");
        }
        #ifndef CORRADE_TARGET_EMSCRIPTEN
        else if(event.key() == KeyEvent::Key::C) {
            Debug{} << "toggling mouse capture to" << !isMouseCaptured();
            setMouseCaptured(!isMouseCaptured());
        } else if(event.key() == KeyEvent::Key::S) {
            Debug{} << "setting window size, which should trigger a viewport event";
            setWindowSize(Vector2i{300, 200});
        } else if(event.key() == KeyEvent::Key::W) {
            Debug{} << "setting max window size, which should trigger a viewport event";
            setMaxWindowSize(Vector2i{700, 500});
        }
        #endif
        else if(event.key() == KeyEvent::Key::H) {
            Debug{} << "toggling hand cursor";
            setCursor(cursor() == Cursor::Arrow ? Cursor::Hand : Cursor::Arrow);
        }
        #ifndef CORRADE_TARGET_EMSCRIPTEN
        else if(event.key() == KeyEvent::Key::L) {
            Debug{} << "toggling locked mouse";
            setCursor(cursor() == Cursor::Arrow ? Cursor::HiddenLocked : Cursor::Arrow);
        }
        #else
        else if(event.key() == KeyEvent::Key::F) {
            Debug{} << "toggling fullscreen";
            setContainerCssClass((_fullscreen ^= true) ? "fullsize" : "");
        }
        #endif
    }

    void textInputEvent(TextInputEvent& event) override {
        Debug{} << "text input event:" << std::string{event.text(), event.text().size()};
    }

    /* Should fire on currently not handled events, such as minimize/maximize
       or window focus/blur. Comment out to verify correct behavior with the
       override not present. */
    void anyEvent(SDL_Event& event) override {
        Debug d;
        d << "any event:" << event.type;
        if(event.type == SDL_WINDOWEVENT) d << event.window.event;
    }

    #ifdef CORRADE_TARGET_EMSCRIPTEN
    private:
        bool _fullscreen = false;
    #endif
};

Sdl2ApplicationTest::Sdl2ApplicationTest(const Arguments& arguments): Platform::Application{arguments, Configuration{}.setWindowFlags(Configuration::WindowFlag::Resizable)} {
    /* For testing resize events */
    Debug{} << "window size" << windowSize()
        #ifdef MAGNUM_TARGET_GL
        << framebufferSize()
        #endif
        << dpiScaling();

    #ifndef CORRADE_TARGET_EMSCRIPTEN
    #if SDL_MAJOR_VERSION*1000 + SDL_MINOR_VERSION*100 + SDL_PATCHLEVEL >= 2005
    Utility::Resource rs{"icons"};
    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer;
    Containers::Optional<Trade::ImageData2D> image;
    if((importer = manager.loadAndInstantiate("AnyImageImporter")) &&
       importer->openData(rs.getRaw("icon-64.tga")) &&
       (image = importer->image2D(0))) setWindowIcon(*image);
    else Warning{} << "Can't load the plugin / file, not setting window icon";
    #else
    Debug{} << "SDL too old, can't set window icon";
    #endif
    #endif
}

}}}}

MAGNUM_APPLICATION_MAIN(Magnum::Platform::Test::Sdl2ApplicationTest)
