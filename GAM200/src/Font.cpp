#include <Debug.h>
#include "Font.h"

Font* font = NULL;
Font::Font() {
}

FT_Library ft;
namespace
{
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };
    struct outline
    {
        std::map<char, Character> Characters;
        FT_Face face;
        void set_pixel_size(int size);
        void load_ascii_chars();
    };
    unsigned int VAO, VBO;
    GLSLShader shdr_pgm;
    outline fontOutlines[total];
    outline* fontTracker = nullptr;
    FT_Error error;
}

void init_shaders();

void SetFont(FONT f)
{
    fontTracker = &fontOutlines[f];
}

void Font::Initialize()
{
    init_shaders();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window->width), 0.0f, static_cast<float>(window->height));
    shdr_pgm.Use();
    glUniformMatrix4fv(glGetUniformLocation(shdr_pgm.GetHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // FreeType
        // --------
        // All functions return a value different than 0 whenever an error occurred
    
    error = FT_Init_FreeType(&ft);
    if(error)
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        }

    error = (FT_New_Face(ft, "Asset/Fonts/Aldrich-Regular.ttf", 0, &fontOutlines[AldrichRegular].face));
        if(error)
        {
            std::cout << "ERROR::FREETYPE: Failed to load font!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        }
        error = (FT_New_Face(ft, "Asset/Fonts/Geo-Regular.ttf", 0, &fontOutlines[GeoRegular].face));
        if (error)
        {
            std::cout << "ERROR::FREETYPE: Failed to load font!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        }

        for (int i = 0; i < total; ++i)
        {
            fontOutlines[i].set_pixel_size(48);
            fontOutlines[i].load_ascii_chars();
        }
          
        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        shdr_pgm.UnUse();
}

void normalise_coord(float& x, float& y)
{
    x += window->width / 2.0f;
    y += window->height / 2.0f;
}

void RenderText(std::string text, float x, float y, float scale, glm::ivec3 color)
{
    normalise_coord(x, y);
    shdr_pgm.Use();
    // activate corresponding render 
    glUniform3f(glGetUniformLocation(shdr_pgm.GetHandle(), "textColor"), (float)color.x, (float)color.y, (float)color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    for (char c : text)
    {
        Character ch = fontTracker->Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shdr_pgm.UnUse();
}

bool DrawText(std::string const& text, float posX, float posY, float scale , float red , float green , float blue)
{
    RenderText(text, posX, posY, scale, glm::vec3(red,green,blue));
    return true;
}

void Font::Update()
{
}

Font::~Font()
{
    for (int i = 0; i < total; ++i)
    {
        FT_Done_Face(fontOutlines[i].face);
    }
    FT_Done_FreeType(ft);
}


int find_width(std::string const& str , FONT f)
{
    int width{};
    for (size_t i = 0; i < str.size(); ++i)
    {
        FT_Load_Char(fontOutlines[f].face, str.at(i), FT_LOAD_RENDER);
        width += fontOutlines[f].face->glyph->advance.x;
    }
    return width>>6;
}

void init_shaders()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files
    {
        std::make_pair(GL_VERTEX_SHADER, "shaders/Font.vert"),
        std::make_pair(GL_FRAGMENT_SHADER, "shaders/Font.frag")
    };

    shdr_pgm.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr_pgm.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        std::cout << shdr_pgm.GetLog() << "\n";
        std::exit(EXIT_FAILURE);
    }
}

void outline::load_ascii_chars()
{
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            error = (FT_Load_Char(face, c, FT_LOAD_RENDER));
            if (error)
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            }
            // generate texture 
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character charac = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, charac));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
}

void outline::set_pixel_size(int size)
{
    FT_Set_Pixel_Sizes(face, 0, size);
}