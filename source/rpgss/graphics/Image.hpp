#ifndef RPGSS_GRAPHICS_IMAGE_HPP_INCLUDED
#define RPGSS_GRAPHICS_IMAGE_HPP_INCLUDED

#include <string>

#include "../common/RefCountedObject.hpp"
#include "../common/RefCountedObjectPtr.hpp"
#include "../core/Dim2.hpp"
#include "../core/Vec2.hpp"
#include "../core/Rect.hpp"
#include "RGBA.hpp"


namespace rpgss {
    namespace graphics {

        // forward declarations
        class Font;
        class WindowSkin;

        struct BlendMode {
            enum {
                Set,
                Mix,
                Add,
                Subtract,
                Multiply,
            };
        };

        class Image : public RefCountedObject {
        public:
            typedef RefCountedObjectPtr<Image> Ptr;

        public:
            static Image::Ptr New(int width, int height);
            static Image::Ptr New(int width, int height, RGBA color);
            static Image::Ptr New(int width, int height, const RGBA* pixels);

        public:
            int getWidth() const;
            int getHeight() const;
            core::Dim2i getDimensions() const;

            int getPitch() const;
            int getSizeInBytes() const;
            int getSizeInPixels() const;

            const RGBA* getPixels() const;
            RGBA* getPixels();

            RGBA getPixel(int x, int y) const;
            void setPixel(int x, int y, RGBA color);

            const core::Recti& getClipRect() const;
            void  setClipRect(const core::Recti& clipRect);

            int  getBlendMode() const;
            void setBlendMode(int blendMode);

            Image::Ptr copyRect(const core::Recti& rect, Image* destination = 0);
            void resize(int new_width, int new_height);
            void setAlpha(u8 alpha);
            void clear(RGBA color = RGBA(0, 0, 0));
            void grey();

            void flipHorizontal();
            void flipVertical();

            void rotateClockwise();
            void rotateCounterClockwise();

            void drawPoint(const core::Vec2i& pos, RGBA color);

            void drawLine(const core::Vec2i& startPos, const core::Vec2i& endPos, RGBA color);
            void drawLine(const core::Vec2i& startPos, const core::Vec2i& endPos, RGBA startColor, RGBA endColor);

            void drawRectangle(bool fill, const core::Recti& rect, RGBA color);
            void drawRectangle(bool fill, const core::Recti& rect, RGBA ulColor, RGBA urColor, RGBA lrColor, RGBA llColor);

            void drawCircle(bool fill, const core::Vec2i& center, int radius, RGBA color);
            void drawCircle(bool fill, const core::Vec2i& center, int radius, RGBA innerColor, RGBA outerColor);

            void drawTriangle(bool fill, const core::Vec2i& p1, const core::Vec2i& p2, const core::Vec2i& p3, RGBA color);
            void drawTriangle(bool fill, const core::Vec2i& p1, const core::Vec2i& p2, const core::Vec2i& p3, RGBA c1, RGBA c2, RGBA c3);

            void draw(const Image* image, const core::Vec2i& pos, float angle = 0.0, float scale = 1.0, RGBA color = RGBA(255, 255, 255, 255));
            void draw(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos, float angle = 0.0, float scale = 1.0, RGBA color = RGBA(255, 255, 255, 255));

            void drawq(const Image* image, const core::Vec2i& ul, const core::Vec2i& ur, const core::Vec2i& lr, const core::Vec2i& ll, RGBA color = RGBA(255, 255, 255, 255));
            void drawq(const Image* image, const core::Recti& image_rect, const core::Vec2i& ul, const core::Vec2i& ur, const core::Vec2i& lr, const core::Vec2i& ll, RGBA color = RGBA(255, 255, 255, 255));

            void drawWindow(const WindowSkin* windowSkin, core::Recti windowRect, int opacity = 255);

            void drawText(const Font* font, core::Vec2i pos, const char* text, int len = -1, float scale = 1.0, RGBA color = RGBA(255, 255, 255, 255));

        private:
            // use New()
            Image(int width, int height);
            Image(int width, int height, RGBA color);
            Image(int width, int height, const RGBA* pixels);
            ~Image();

            RGBA* allocatePixels(int width, int height);
            void  deletePixels(RGBA* pixels);
            void  reset(int new_width, int new_height, RGBA* new_pixels);

            Image::Ptr copyRect_generic(const core::Recti& rect, Image* destination = 0);
            Image::Ptr copyRect_sse2(const core::Recti& rect, Image* destination = 0);

            void clear_generic(RGBA color);
            void clear_sse2(RGBA color);

            void draw_sse2_set(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos);
            void draw_sse2_add(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos);
            void draw_sse2_sub(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos);
            void draw_sse2_mul(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos);

            void draw_sse2_set(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos, RGBA color);
            void draw_sse2_add(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos, RGBA color);
            void draw_sse2_sub(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos, RGBA color);
            void draw_sse2_mul(const Image* image, const core::Recti& image_rect, const core::Vec2i& pos, RGBA color);

        private:
            int   _width;
            int   _height;
            RGBA* _pixels;
            core::Recti _bbox;
            core::Recti _clipRect;
            int   _blendMode;
        };

        //-----------------------------------------------------------------
        inline int
        Image::getWidth() const
        {
            return _width;
        }

        //-----------------------------------------------------------------
        inline int
        Image::getHeight() const
        {
            return _height;
        }

        //-----------------------------------------------------------------
        inline core::Dim2i
        Image::getDimensions() const
        {
            return core::Dim2i(_width, _height);
        }

        //-----------------------------------------------------------------
        inline int
        Image::getPitch() const
        {
            return _width * sizeof(RGBA);
        }

        //-----------------------------------------------------------------
        inline int
        Image::getSizeInBytes() const
        {
            return _width * _height * sizeof(RGBA);
        }

        //-----------------------------------------------------------------
        inline int
        Image::getSizeInPixels() const
        {
            return _width * _height;
        }

        //-----------------------------------------------------------------
        inline const RGBA*
        Image::getPixels() const
        {
            return _pixels;
        }

        //-----------------------------------------------------------------
        inline RGBA*
        Image::getPixels()
        {
            return _pixels;
        }

        //-----------------------------------------------------------------
        inline const core::Recti&
        Image::getClipRect() const
        {
            return _clipRect;
        }

        //-----------------------------------------------------------------
        inline int
        Image::getBlendMode() const
        {
            return _blendMode;
        }

        //-----------------------------------------------------------------
        inline RGBA
        Image::getPixel(int x, int y) const
        {
            return _pixels[_width * y + x];
        }

        //-----------------------------------------------------------------
        inline void
        Image::setPixel(int x, int y, RGBA color)
        {
            _pixels[_width * y + x] = color;
        }

    } // namespace graphics
} // namespace rpgss


#endif // RPGSS_GRAPHICS_IMAGE_HPP_INCLUDED
