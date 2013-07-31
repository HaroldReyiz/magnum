#ifndef Magnum_DebugTools_ObjectRenderer_h
#define Magnum_DebugTools_ObjectRenderer_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class Magnum::DebugTools::ObjectRenderer, Magnum::DebugTools::ObjectRendererOptions, typedef Magnum::DebugTools::ObjectRenderer2D, Magnum::DebugTools::ObjectRenderer3D
 */

#include "Resource.h"
#include "SceneGraph/Drawable.h"
#include "Shaders/Shaders.h"

#include "magnumDebugToolsVisibility.h"

namespace Magnum { namespace DebugTools {

/**
@brief Object renderer options

See ObjectRenderer documentation for more information.
*/
class ObjectRendererOptions {
    public:
        constexpr ObjectRendererOptions(): _size(1.0f) {}

        /** @brief Size of the rendered axes */
        constexpr Float size() const { return _size; }

        /**
         * @brief Set size of the rendered axes
         * @return Reference to self (for method chaining)
         *
         * Default is `1.0f`.
         */
        ObjectRendererOptions& setSize(Float size) {
            _size = size;
            return *this;
        }

    private:
        Float _size;
};

/**
@brief Object renderer

Visualizes object position, rotation and scale using colored axes. See
@ref debug-tools-renderers for more information.

@section ObjectRenderer-usage Basic usage

Example code:
@code
// Create some options
DebugTools::ResourceManager::instance()->set("my", DebugTools::ObjectRendererOptions().setSize(0.3f));

// Create debug renderer for given object, use "my" options for it
Object3D* object;
new DebugTools::ObjectRenderer2D(object, "my", debugDrawables);
@endcode

@see ObjectRenderer2D, ObjectRenderer3D
*/
template<UnsignedInt dimensions> class MAGNUM_DEBUGTOOLS_EXPORT ObjectRenderer: public SceneGraph::Drawable<dimensions, Float> {
    public:
        /**
         * @brief Constructor
         * @param object    Object for which to create debug renderer
         * @param options   Options resource key. See
         *      @ref ObjectRenderer-usage "class documentation" for more
         *      information.
         * @param drawables Drawable group
         *
         * The renderer is automatically added to object's features.
         */
        explicit ObjectRenderer(SceneGraph::AbstractObject<dimensions, Float>* object, ResourceKey options = ResourceKey(), SceneGraph::DrawableGroup<dimensions, Float>* drawables = nullptr);

    protected:
        void draw(const typename DimensionTraits<dimensions, Float>::MatrixType& transformationMatrix, SceneGraph::AbstractCamera<dimensions, Float>* camera) override;

    private:
        Resource<ObjectRendererOptions> options;
        Resource<AbstractShaderProgram, Shaders::VertexColor<dimensions>> shader;
        Resource<Mesh> mesh;
        Resource<Buffer> vertexBuffer, indexBuffer;
};

/** @brief Two-dimensional object renderer */
typedef ObjectRenderer<2> ObjectRenderer2D;

/** @brief Three-dimensional object renderer */
typedef ObjectRenderer<3> ObjectRenderer3D;

}}

#endif
