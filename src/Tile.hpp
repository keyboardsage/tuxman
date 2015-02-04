#ifndef TILE_HPP
#define TILE_HPP

#include "ScreenObject.hpp"
#include "GraphicStreamOut.hpp"

class Tile: public ScreenObject {
        public:
            Tile(ScreenObjectInfo& objInfo):ScreenObject(objInfo.x, objInfo.y, objInfo.width, objInfo.height, objInfo.type) { hasFood = (info->type == 'a') ? true : false; }
            virtual ~Tile() { }

            virtual void draw() {
                switch (info->type) {
                    case 'A':
                    case 'a': gout.drawColor(GraphicStreamOut::RED); break;
                    case 'b': gout.drawColor(GraphicStreamOut::GREEN); break;
                    case 'c': gout.drawColor(GraphicStreamOut::BLUE); break;
                    case 'd': gout.drawColor(GraphicStreamOut::PURPLE); break;
                    case 'e': gout.drawColor(GraphicStreamOut::LIGHTBLUE); break;
                    case 'f': gout.drawColor(GraphicStreamOut::YELLOW); break;
                }

                // draw tile
                glBegin(GL_QUADS);
                    glVertex2f(0.0, 0.0);
                    glVertex2f(info->width, 0.0);
                    glVertex2f(info->width, -info->height);
                    glVertex2f(0.0, -info->height);
                glEnd();

                // draw food on top of tile if necessary
                if (info->type == 'a' && hasFood) {
                    gout.drawColor(GraphicStreamOut::WHITE);
                    glBegin(GL_QUADS);
                        glVertex2f(0.25 * info->width, 0.25 * -info->height);
                        glVertex2f(0.75 * info->width, 0.25 * -info->height);
                        glVertex2f(0.75 * info->width, 0.75 * -info->height);
                        glVertex2f(0.25 * info->width, 0.75 * -info->height);
                    glEnd();
                }
            }

            bool getHasFood() { return hasFood; }
            void setHasFood(bool hasFoodParam) { hasFood = hasFoodParam; }

        private:
            bool hasFood;
};

#endif
