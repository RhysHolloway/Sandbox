//#include "text.h"
//#include "GL/glew.h"
//
//#include <stdexcept>
//
//#include <cmrc/cmrc.hpp>
//
//CMRC_DECLARE(client);
//
//void TextCtx::load(std::string &path) {
//    glPixelStorei(GL_UNPACK_ALIGNMENT, GL_TRUE);
//    auto fs = cmrc::client::get_filesystem();
//    auto fData = fs.open("fonts/" + path);
//    auto pair = fonts.emplace(path.substr(0, path.length() - 4), Font{});
//
//    auto &font = pair.first->second;
//
//    FT_New_Memory_Face(lib, reinterpret_cast<const unsigned char *>(fData.begin()), fData.size(), 0, &font.face);
//    glPixelStorei(GL_UNPACK_ALIGNMENT, GL_FALSE);
//}
//
//void TextCtx::init() {
//    if (FT_Init_FreeType(&lib)) {
//        throw std::runtime_error("Could not initialize FreeType library!");
//    }
//}
//
//void
//TextCtx::draw(const std::string &font, const std::string &text, glm::vec3 position, float height, TextDrawFlags flags,
//              float sx, float sy) {
//    if (auto f = fonts.find(font); f != fonts.end()) {
//        auto g = f->second.face->glyph;
//        const char *p;
//
//        for (p = text.c_str(); *p; p++) {
//            if (FT_Load_Char(f->second.face, *p, FT_LOAD_RENDER))
//                continue;
//
//            glTexImage2D(
//                    GL_TEXTURE_2D,
//                    0,
//                    GL_RED,
//                    g->bitmap.width,
//                    g->bitmap.rows,
//                    0,
//                    GL_RED,
//                    GL_UNSIGNED_BYTE,
//                    g->bitmap.buffer
//            );
//
//            float x2 = position.x + g->bitmap_left * sx;
//            float y2 = -position.y - g->bitmap_top * sy;
//            float w = g->bitmap.width * sx;
//            float h = g->bitmap.rows * sy;
//
//            GLfloat box[4][5] = {
//                    {x2,     -y2,     position.z, 0, 0},
//                    {x2 + w, -y2,     position.z, 1, 0},
//                    {x2,     -y2 - h, position.z, 0, 1},
//                    {x2 + w, -y2 - h, position.z, 1, 1},
//            };
//
//            glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
//            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//            position.x += (g->advance.x / 64) * sx;
//            position.y += (g->advance.y / 64) * sy;
//
//        }
//    } else {
//        throw std::runtime_error("Could not find font " + font + "!");
//    }
//}