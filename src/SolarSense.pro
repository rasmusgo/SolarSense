TEMPLATE = app
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL
INCLUDEPATH += $$PWD/
QMAKE_CXXFLAGS += -std=c++11
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/SolarSenseApp.cpp \
    $$PWD/tools.cpp \
    $$PWD/Scene.cpp \
    $$PWD/graphics/Texture.cpp \
    $$PWD/graphics/TextureManager.cpp \
    $$PWD/graphics/Shader.cpp \
	$$PWD/graphics/ShaderProgram.cpp \
	$$PWD/audio/AudioManager.cpp \
    $$PWD/input/InputManager.cpp \
    $$PWD/SceneSolarSystem/SceneSolarSystem.cpp \
	$$PWD/GameObject.cpp \
        $$PWD/SceneSolarSystem/TriangleObject.cpp \
	$$PWD/graphics/Mesh.cpp \
	$$PWD/graphics/Vertex.cpp \
	$$PWD/graphics/Model.cpp \
	$$PWD/graphics/Uniform.cpp \
	$$PWD/graphics/ShaderBinding.cpp \
        $$PWD/SceneSolarSystem/RegularPolygonObject.cpp \
	$$PWD/graphics/MeshManager.cpp \
    $$PWD/graphics/ShaderManager.cpp \
    SceneSolarSystem/SphereObject.cpp \
    Entity.cpp \
    SceneSolarSystem/Camera.cpp \
    SceneSolarSystem/OrbitingObject.cpp

HEADERS += \
    $$PWD/Scene.hpp \
    $$PWD/SolarSenseApp.hpp \
    $$PWD/tools.hpp \
    $$PWD/graphics/Texture.hpp \
    $$PWD/graphics/TextureManager.hpp \
    $$PWD/graphics/Shader.hpp \
    $$PWD/graphics/ShaderProgram.hpp \
    $$PWD/audio/AudioManager.hpp \
    $$PWD/input/InputManager.hpp \
    $$PWD/SceneSolarSystem/SceneSolarSystem.hpp \
    $$PWD/GameObject.hpp \
    $$PWD/glm/*.hpp \
    $$PWD/SceneSolarSystem/TriangleObject.hpp \
    $$PWD/graphics/Mesh.hpp \
    $$PWD/graphics/Vertex.hpp \
    $$PWD/graphics/Model.hpp \
    $$PWD/graphics/Uniform.hpp \
    $$PWD/graphics/ShaderBinding.hpp \
    $$PWD/SceneSolarSystem/RegularPolygonObject.hpp \
    $$PWD/graphics/MeshManager.hpp \
    $$PWD/graphics/ShaderManager.hpp \
    $$PWD/SceneSolarSystem/SphereObject.hpp \
    $$PWD/Entity.hpp \
    $$PWD/SceneSolarSystem/Camera.hpp \
    SceneSolarSystem/OrbitingObject.hpp

OTHER_FILES += \
	$$PWD/data/shaders/sample.vert \
	$$PWD/data/shaders/sample.frag \
	$$PWD/data/shaders/sample2.vert \
	$$PWD/data/shaders/sample2.frag
