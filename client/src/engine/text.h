//#pragma once
//
//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//#include <string>
//#include <unordered_map>
//
//#include <glm/vec3.hpp>
////#include <string>
//
//enum TextDrawFlags : uint8_t {
//    NONE = 0,
//    CENTERED_X,
//    CENTERED_Y,
//};
//
//struct Font {
//    FT_Face face = nullptr;
//    unsigned int texture, vbo;
//};
//
//class TextCtx {
//public:
//    void init();
//
//    //TODO: implement properly
//    void load(std::string &font);
//
//    void draw(const std::string& font, const std::string& text, glm::vec3 position, float height, TextDrawFlags flags, float sx, float sy);
//private:
//    FT_Library lib;
//    std::unordered_map<std::string, Font> fonts;
//};
//
