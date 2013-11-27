include(VBE/VBE.pro)

DEFINES += __DEBUG __LOG

TEMPLATE = app
LD_LIBRARY_PATH += libs:
CONFIG -= qt

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch


LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lOpenNI2 -lNiTE2
unix:{
    linux-g++:contains(QMAKE_TARGET.arch, i686):{
        message( "Building for 32 bit")
        LIBS += -lGL \
            -L$$PWD/OpenNIx86/ \
            -L$$PWD/NiTEx86/ \
            -L$$PWD/GrabDetector/
        INCLUDEPATH += $$PWD/ \
                   $$PWD/OpenNIx86/Include \
                   $$PWD/OpenNIx86/Include/Linux-x86 \
                   $$PWD/NiTEx86/Include
    } else {
        message( "Building for 64 bit")
        LIBS += -lGL \
            -L$$PWD/OpenNI/ \
            -L$$PWD/NiTE/ \
            -L$$PWD/GrabDetector/
        INCLUDEPATH += $$PWD/ \
                   $$PWD/OpenNI/Include \
                   $$PWD/OpenNI/Include/Linux-x86 \
                   $$PWD/NiTE/Include
    }
} else:win32:{
    message( "Building for Windows" )
    LIBS -= -lOpenNI2 -lNiTE2
    LIBS += -lopengl32 -lglu32 -lglut32 -lglew32d \
        -L"D:/Workspace/SFML-2.1/lib/" \
        #-L"C:/Program Files/OpenNI2/Lib/" \
        #-L"C:/Program Files/PrimeSense/NiTE2/Lib/" \
        -L"D:/Workspace/glew/"
    INCLUDEPATH += $$PWD/ \
        "D:/Workspace/SFML-2.1/include/" \
        #"C:/Program Files/OpenNI2/Include/" \
        #"C:/Program Files/PrimeSense/NiTE2/Include/" \
        "D:/Workspace/OpenGL/include/"
    DEPENDPATH += $$PWD/ \
        "D:/Workspace/SFML-2.1/include/" \
        #"C:/Program Files/OpenNI2/Include/" \
        #"C:/Program Files/PrimeSense/NiTE2/Include/" \
        "D:/Workspace/OpenGL/include/"
    DEFINES += SFML_DYNAMIC \
               GLEW_STATIC
    SOURCES += $$PWD/glew.c
    QMAKE_CXXFLAGS += -static-libgcc -static-libstdc++
}


QMAKE_CXXFLAGS += -std=c++11
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/SceneSolarSystem/SceneSolarSystem.cpp \
    $$PWD/SceneSolarSystem/StandardPlanet.cpp \
    $$PWD/SceneSolarSystem/inputreader.cpp \
    $$PWD/SceneSolarSystem/SphereObject.cpp \
    $$PWD/SceneSolarSystem/Camera.cpp \
    $$PWD/SceneSolarSystem/Sun.cpp \
    $$PWD/SceneSolarSystem/Earth.cpp \
    $$PWD/SceneSolarSystem/SunHalo.cpp \
    $$PWD/SceneSolarSystem/Planet.cpp \
    $$PWD/input/InteractionListener.cpp \
    $$PWD/input/InteractionManager.cpp \
    #$$PWD/input/SensorManager.cpp \
    SceneSolarSystem/WorldObject.cpp \
    SceneSolarSystem/Rock.cpp \
    SceneSolarSystem/RingPlanet.cpp \
    SceneSolarSystem/Orbit.cpp


HEADERS += \
    $$PWD/SceneSolarSystem/SceneSolarSystem.hpp \
    $$PWD/SceneSolarSystem/SphereObject.hpp \
    $$PWD/SceneSolarSystem/Camera.hpp \
    $$PWD/SceneSolarSystem/StandardPlanet.hpp \
    $$PWD/SceneSolarSystem/Sun.hpp \
    $$PWD/SceneSolarSystem/Planet.hpp \
    $$PWD/SceneSolarSystem/Earth.hpp \
    $$PWD/SceneSolarSystem/SunHalo.hpp \
    $$PWD/input/InteractionListener.hpp \
    $$PWD/input/InteractionManager.hpp \
    $$PWD/SceneSolarSystem/inputreader.h \
    #$$PWD/input/SensorManager.hpp \
    Commons.hpp \
    SceneSolarSystem/WorldObject.hpp \
    SceneSolarSystem/Rock.hpp \
    SceneSolarSystem/RingPlanet.hpp \
    SceneSolarSystem/Orbit.hpp

OTHER_FILES += \
    $$PWD/data/shaders/sample.vert \
    $$PWD/data/shaders/sample.frag \
    $$PWD/data/shaders/orbit.vert \
    $$PWD/data/shaders/orbit.frag \
    $$PWD/data/shaders/sample2.vert \
    $$PWD/data/shaders/sample2.frag \
    $$PWD/data/shaders/earthShader.vert \
    $$PWD/data/shaders/earthShader.frag \
    $$PWD/data/shaders/planetShader.vert \
    $$PWD/data/shaders/planetShader.frag \
    $$PWD/data/shaders/planetShaderBump.vert \
    $$PWD/data/shaders/planetShaderBump.frag \
    $$PWD/data/shaders/sun.vert \
    $$PWD/data/shaders/sun.frag \
    $$PWD/data/shaders/hand.frag \
    $$PWD/data/shaders/hand.vert \
    $$PWD/data/shaders/sun3d.vert \
    $$PWD/data/shaders/sun3d.frag \
    data/shaders/objectShader.vert \
    data/shaders/objectShader.frag \
    data/shaders/ring.vert \
    data/shaders/ring.frag
