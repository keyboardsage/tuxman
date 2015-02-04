#ifndef SCREENOBJECT_HPP
#define SCREENOBJECT_HPP

#include "OpenGL.hpp"

#include <map>

typedef struct {
    float x;                /**< The top left point's x coordinate (OpenGL style). */
    float y;                /**< The top left point's y coordinate (OpenGL style). */
    float width;            /**< Width of the object on screen. */
    float height;           /**< Height of the object on screen. */
    char type;              /**< A character that corresponds to the type of object. */
} ScreenObjectInfo;

/**
 * This is the basis for all renderable images on the screen.
 * If a class does not inherit this class then it is not renderable to the screen.
 */
class ScreenObject
{
    public:
        ScreenObject(float x, float y, float width, float height, char typeParam)
        {
            info = new ScreenObjectInfo();
            info->x = x;
            info->y = y;
            info->width = width;
            info->height = height;
            info->type = typeParam;
        }
        virtual ~ScreenObject() { delete info; }

        virtual void draw() = 0;

        void setPosition(float x, float y) {
            info->x = x;
            info->y = y;
        }

        void setX(float x) {
            info->x = x;
        }

        void setY(float y) {
            info->y = y;
        }

        float getX() const {
            return info->x;
        }

        float getY() const {
            return info->y;
        }

        ScreenObjectInfo* getInfo() const {
            return info;
        }

        char getType() const {
            return info->type;
        }

    protected:
        /**
         * Ensures that the Component's x, y coordinate is considered the
         * top left of the Component by moving to correct spot in OpenGL matrix.
         */
        void position()
        {
            glTranslatef(info->x, info->y, 0.0f);
        }

    protected:
        ScreenObjectInfo* info;
};

typedef std::map<int, ScreenObject*> ObjectMap;

#endif
