#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <glad/glad.h>

#include <radix/env/Util.hpp>
#include <radix/env/Environment.hpp>

#include <radix/core/math/Vector2i.hpp>
#include <radix/data/texture/SpritSheetLoader.hpp>
#include <radix/data/texture/TextureLoader.hpp>


#ifdef __cplusplus
extern "C" {
#endif  //! __cplusplus

  PFNGLGENTEXTURESPROC glad_glGenTextures;
  void glad_glGenTextures_(GLsizei n, GLuint *textures) { *textures = 1; }
  PFNGLBINDTEXTUREPROC glad_glBindTexture;
  void glad_glBindTexture_(GLenum target, GLuint texture) {}
  PFNGLPIXELSTOREIPROC glad_glPixelStorei;
  void glad_glPixelStorei_(GLenum pname, GLint param) {}
  PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
  void glad_glTexParameteri_(GLenum target, GLenum pname, GLint param) {}
  PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
  void glad_glTexImage2D_(GLenum target, GLint level, GLint internalformat,
      GLsizei width, GLsizei height, GLint border,
      GLenum format, GLenum type, const void *pixels) {
  }
  PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
  void glad_glGenerateMipmap_(GLenum target) {}

#ifdef __cplusplus
}
#endif  //! __cplusplus

struct TextureLoaderFixtires {
  TextureLoaderFixtires() {
    radix::Util::Init();
    radix::Environment::setDataDir(".");

    glGenTextures = glad_glGenTextures_;
    glBindTexture = glad_glBindTexture_;
    glPixelStorei = glad_glPixelStorei_;
    glTexParameteri = glad_glTexParameteri_;
    glTexImage2D = glad_glTexImage2D_;
    glGenerateMipmap = glad_glGenerateMipmap_;
  }
  ~TextureLoaderFixtires() {}
};


SCENARIO("Pass texture path to get full loaded spritSheet",
         "[sprit-sheet,texture-loader]") {
  TextureLoaderFixtires fix;
  radix::SpritSheetLoader loader;

  GIVEN("A texture not exist") {
    WHEN("pass empty string") {
      loader.load("");
      THEN("the full size is 0x0") {
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.fullWidth()  == 0);
        REQUIRE(spritSheet.fullHeight() == 0);
      }
    }
  }

  GIVEN("A texture/animation_512.png exist") {
    const std::string textureID = "animation_512.png";
    WHEN("pass texture/animation_512.png") {
      loader.load(textureID);
      THEN("the full size is 512x512") {
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.fullWidth()  == 512);
        REQUIRE(spritSheet.fullHeight() == 512);

        REQUIRE(spritSheet.width()  == 512);
        REQUIRE(spritSheet.height() == 512);
      }

      THEN("set sprit size 7x4") {
        loader.setSpritSize(radix::Vector2i{7, 4});
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.width()  == 73);
        REQUIRE(spritSheet.height() == 128);
      }
    }
  }

  GIVEN("A texture/animation_1024.png exist") {
    const std::string textureID = "animation_1024.png";
    WHEN("pass texture/animation_1024.png") {
      loader.load(textureID);
      THEN("the full size is 1024x1024") {
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.fullWidth()  == 1024);
        REQUIRE(spritSheet.fullHeight() == 1024);
      }

      WHEN("set sprit size 7x4") {
        const auto gridSize = radix::Vector2i{146, 256};
        loader.setSpritSize({7, 4});
        auto spritSheet = loader.create();
        REQUIRE(spritSheet.width()  == gridSize.width);
        REQUIRE(spritSheet.height() == gridSize.height);
      }
    }
  }
}

