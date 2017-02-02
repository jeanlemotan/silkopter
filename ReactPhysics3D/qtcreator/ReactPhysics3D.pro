#-------------------------------------------------
#
# Project created by QtCreator 2017-02-02T09:32:10
#
#-------------------------------------------------

TARGET = ReactPhysics3D
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CFLAGS += -Wno-unused-variable

rpi {
    DEFINES+=RASPBERRY_PI
    QMAKE_MAKEFILE = "Makefile.rpi"
    MAKEFILE = "Makefile.rpi"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = rpi/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = rpi/release
        DEFINES += NDEBUG
    }
} else {
    QMAKE_MAKEFILE = "Makefile"
    CONFIG(debug, debug|release) {
        DEST_FOLDER = pc/debug
    }
    CONFIG(release, debug|release) {
        DEST_FOLDER = pc/release
        DEFINES += NDEBUG
    }
}

OBJECTS_DIR = ./.obj/$${DEST_FOLDER}
MOC_DIR = ./.moc/$${DEST_FOLDER}
RCC_DIR = ./.rcc/$${DEST_FOLDER}
UI_DIR = ./.ui/$${DEST_FOLDER}
DESTDIR = ../../lib/$${DEST_FOLDER}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../src

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    ../src/body/Body.h \
    ../src/body/CollisionBody.h \
    ../src/body/RigidBody.h \
    ../src/collision/CollisionDetection.h \
    ../src/collision/CollisionShapeInfo.h \
    ../src/collision/ContactManifold.h \
    ../src/collision/ContactManifoldSet.h \
    ../src/collision/ProxyShape.h \
    ../src/collision/RaycastInfo.h \
    ../src/collision/TriangleMesh.h \
    ../src/collision/TriangleVertexArray.h \
    ../src/collision/broadphase/BroadPhaseAlgorithm.h \
    ../src/collision/broadphase/DynamicAABBTree.h \
    ../src/collision/narrowphase/CollisionDispatch.h \
    ../src/collision/narrowphase/ConcaveVsConvexAlgorithm.h \
    ../src/collision/narrowphase/DefaultCollisionDispatch.h \
    ../src/collision/narrowphase/NarrowPhaseAlgorithm.h \
    ../src/collision/narrowphase/SphereVsSphereAlgorithm.h \
    ../src/collision/narrowphase/EPA/EdgeEPA.h \
    ../src/collision/narrowphase/EPA/EPAAlgorithm.h \
    ../src/collision/narrowphase/EPA/TriangleEPA.h \
    ../src/collision/narrowphase/EPA/TrianglesStore.h \
    ../src/collision/narrowphase/GJK/GJKAlgorithm.h \
    ../src/collision/narrowphase/GJK/Simplex.h \
    ../src/collision/shapes/AABB.h \
    ../src/collision/shapes/BoxShape.h \
    ../src/collision/shapes/CapsuleShape.h \
    ../src/collision/shapes/CollisionShape.h \
    ../src/collision/shapes/ConcaveMeshShape.h \
    ../src/collision/shapes/ConcaveShape.h \
    ../src/collision/shapes/ConeShape.h \
    ../src/collision/shapes/ConvexMeshShape.h \
    ../src/collision/shapes/ConvexShape.h \
    ../src/collision/shapes/CylinderShape.h \
    ../src/collision/shapes/HeightFieldShape.h \
    ../src/collision/shapes/SphereShape.h \
    ../src/collision/shapes/TriangleShape.h \
    ../src/constraint/BallAndSocketJoint.h \
    ../src/constraint/ContactPoint.h \
    ../src/constraint/FixedJoint.h \
    ../src/constraint/HingeJoint.h \
    ../src/constraint/Joint.h \
    ../src/constraint/SliderJoint.h \
    ../src/engine/CollisionWorld.h \
    ../src/engine/ConstraintSolver.h \
    ../src/engine/ContactSolver.h \
    ../src/engine/DynamicsWorld.h \
    ../src/engine/EventListener.h \
    ../src/engine/Impulse.h \
    ../src/engine/Island.h \
    ../src/engine/Material.h \
    ../src/engine/OverlappingPair.h \
    ../src/engine/Profiler.h \
    ../src/engine/Timer.h \
    ../src/mathematics/mathematics.h \
    ../src/mathematics/mathematics_functions.h \
    ../src/mathematics/Matrix2x2.h \
    ../src/mathematics/Matrix3x3.h \
    ../src/mathematics/Quaternion.h \
    ../src/mathematics/Ray.h \
    ../src/mathematics/Transform.h \
    ../src/mathematics/Vector2.h \
    ../src/mathematics/Vector3.h \
    ../src/memory/MemoryAllocator.h \
    ../src/memory/Stack.h \
    ../src/configuration.h \
    ../src/decimal.h \
    ../src/reactphysics3d.h

SOURCES += \
    ../src/body/Body.cpp \
    ../src/body/CollisionBody.cpp \
    ../src/body/RigidBody.cpp \
    ../src/collision/CollisionDetection.cpp \
    ../src/collision/ContactManifold.cpp \
    ../src/collision/ContactManifoldSet.cpp \
    ../src/collision/ProxyShape.cpp \
    ../src/collision/RaycastInfo.cpp \
    ../src/collision/TriangleMesh.cpp \
    ../src/collision/TriangleVertexArray.cpp \
    ../src/collision/broadphase/BroadPhaseAlgorithm.cpp \
    ../src/collision/broadphase/DynamicAABBTree.cpp \
    ../src/collision/narrowphase/ConcaveVsConvexAlgorithm.cpp \
    ../src/collision/narrowphase/DefaultCollisionDispatch.cpp \
    ../src/collision/narrowphase/NarrowPhaseAlgorithm.cpp \
    ../src/collision/narrowphase/SphereVsSphereAlgorithm.cpp \
    ../src/collision/narrowphase/EPA/EdgeEPA.cpp \
    ../src/collision/narrowphase/EPA/EPAAlgorithm.cpp \
    ../src/collision/narrowphase/EPA/TriangleEPA.cpp \
    ../src/collision/narrowphase/EPA/TrianglesStore.cpp \
    ../src/collision/narrowphase/GJK/GJKAlgorithm.cpp \
    ../src/collision/narrowphase/GJK/Simplex.cpp \
    ../src/collision/shapes/AABB.cpp \
    ../src/collision/shapes/BoxShape.cpp \
    ../src/collision/shapes/CapsuleShape.cpp \
    ../src/collision/shapes/CollisionShape.cpp \
    ../src/collision/shapes/ConcaveMeshShape.cpp \
    ../src/collision/shapes/ConcaveShape.cpp \
    ../src/collision/shapes/ConeShape.cpp \
    ../src/collision/shapes/ConvexMeshShape.cpp \
    ../src/collision/shapes/ConvexShape.cpp \
    ../src/collision/shapes/CylinderShape.cpp \
    ../src/collision/shapes/HeightFieldShape.cpp \
    ../src/collision/shapes/SphereShape.cpp \
    ../src/collision/shapes/TriangleShape.cpp \
    ../src/constraint/BallAndSocketJoint.cpp \
    ../src/constraint/ContactPoint.cpp \
    ../src/constraint/FixedJoint.cpp \
    ../src/constraint/HingeJoint.cpp \
    ../src/constraint/Joint.cpp \
    ../src/constraint/SliderJoint.cpp \
    ../src/engine/CollisionWorld.cpp \
    ../src/engine/ConstraintSolver.cpp \
    ../src/engine/ContactSolver.cpp \
    ../src/engine/DynamicsWorld.cpp \
    ../src/engine/Island.cpp \
    ../src/engine/Material.cpp \
    ../src/engine/OverlappingPair.cpp \
    ../src/engine/Profiler.cpp \
    ../src/engine/Timer.cpp \
    ../src/mathematics/mathematics_functions.cpp \
    ../src/mathematics/Matrix2x2.cpp \
    ../src/mathematics/Matrix3x3.cpp \
    ../src/mathematics/Quaternion.cpp \
    ../src/mathematics/Transform.cpp \
    ../src/mathematics/Vector2.cpp \
    ../src/mathematics/Vector3.cpp \
    ../src/memory/MemoryAllocator.cpp
