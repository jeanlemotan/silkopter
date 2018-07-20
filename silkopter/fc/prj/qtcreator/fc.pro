TEMPLATE = app
CONFIG += console
#CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

TARGET = fc
target.path = /root
INSTALLS = target

PRECOMPILED_HEADER = ../../src/FCStdAfx.h
CONFIG *= precompile_header

DEFINES += BOOST_ERROR_CODE_HEADER_ONLY
DEFINES += ASIO_STANDALONE
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
DESTDIR = ../../bin/$${DEST_FOLDER}

QMAKE_CXXFLAGS += -isystem =/opt/vc/include -isystem =/opt/vc/include/interface/vcos/pthreads -isystem =/opt/vc/include/interface/vmcs_host/linux
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -ffunction-sections -fdata-sections
QMAKE_CFLAGS += -Wno-unused-variable -Wno-unused-parameter -ffunction-sections -fdata-sections
QMAKE_LFLAGS += -rdynamic -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -Wl,--gc-sections


INCLUDEPATH += =/usr/local/include
INCLUDEPATH += ../../../../asio/include
INCLUDEPATH += ../../src
INCLUDEPATH += ../../def
INCLUDEPATH += ../../autogen
INCLUDEPATH += ../../../libs
INCLUDEPATH += ../../../libs/common/comms/def
INCLUDEPATH += ../../../libs/utils/hw
INCLUDEPATH += ../../../../def_lang/include
INCLUDEPATH += ../../../../qbase/include
INCLUDEPATH += ../../../../qmath/include
INCLUDEPATH += ../../../../eigen
INCLUDEPATH += ../../../../bullet

LIBS += -L=/opt/vc/lib/

ROOT_LIBS_PATH = ../../../..

LIBS += -L$${ROOT_LIBS_PATH}/def_lang/lib/$${DEST_FOLDER} -ldef_lang
LIBS += -L$${ROOT_LIBS_PATH}/qmath/lib/$${DEST_FOLDER} -lqmath
LIBS += -L$${ROOT_LIBS_PATH}/qbase/lib/$${DEST_FOLDER} -lqbase

LIBS += -lpthread
LIBS += -lz
rpi {
    LIBS += -lmmal_core
    LIBS += -lmmal_util
    LIBS += -lmmal_vc_client
    LIBS += -lvcos
    LIBS += -lbcm_host
} else {
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui
    LIBS += -lopencv_ml
}

SOURCES += \
    ../../src/main.cpp \
    ../../src/sink/PIGPIO.cpp \
    ../../src/source/MPU9250.cpp \
    ../../src/source/MS5611.cpp \
    ../../src/source/Raspicam.cpp \
    ../../src/source/RC5T619.cpp \
    ../../src/source/SRF02.cpp \
    ../../src/source/UBLOX.cpp \
    ../../src/processor/ADC_Ammeter.cpp \
    ../../src/processor/ADC_Voltmeter.cpp \
    ../../src/processor/Comp_AHRS.cpp \
    ../../src/processor/Gravity_Filter.cpp \
    ../../src/source/EHealth.cpp \
    ../../src/controller/Rate_Controller.cpp \
    ../../src/simulator/Multirotor_Simulation.cpp \
    ../../src/simulator/Multirotor_Simulator.cpp \
    ../../src/processor/Servo_Gimbal.cpp \
    ../../src/processor/Motor_Mixer.cpp \
    ../../src/sink/PCA9685.cpp \
    ../../src/generator/Oscillator.cpp \
    ../../../libs/lz4/lz4.c \
    ../../src/source/OpenCV_Capture.cpp \
    ../../src/source/SRF01.cpp \
    ../../src/brain/Multirotor_Brain.cpp \
    ../../src/source/MaxSonar.cpp \
    ../../src/processor/Throttle_To_PWM.cpp \
    ../../../libs/utils/Coordinates.cpp \
    ../../src/processor/Comp_ECEF.cpp \
    ../../src/processor/KF_ECEF.cpp \
    ../../src/processor/Proximity.cpp \
    ../../src/source/AVRADC.cpp \
    ../../src/processor/Pressure_Velocity.cpp \
    ../../src/processor/ENU_Frame_System.cpp \
    ../../src/source/RaspiCamControl.cpp \
    ../../src/brain/LiPo_Battery.cpp \
    ../../src/pilot/Multirotor_Pilot.cpp \
    ../../def/hal.def.cpp \
    ../../../libs/common/comms/def/gs_comms.def.cpp \
    ../../src/uav_properties/IMultirotor_Properties.cpp \
    ../../src/HAL.cpp \
    ../../src/RC_Comms.cpp \
    ../../src/GS_Comms.cpp \
    ../../src/uav_properties/Hexa_Multirotor_Properties.cpp \
    ../../src/uav_properties/Hexatri_Multirotor_Properties.cpp \
    ../../src/uav_properties/Octo_Multirotor_Properties.cpp \
    ../../src/uav_properties/Octoquad_Multirotor_Properties.cpp \
    ../../src/uav_properties/Quad_Multirotor_Properties.cpp \
    ../../src/uav_properties/Tri_Multirotor_Properties.cpp \
    ../../src/source/CPPM_Receiver.cpp \
    ../../../libs/utils/hw/bcm2835.c \
    ../../../libs/utils/hw/command.c \
    ../../../libs/utils/hw/pigpio.c \
    ../../../libs/utils/comms/UDP_Socket.cpp \
    ../../../libs/utils/hw/SPI_Dev.cpp \
    ../../../libs/common/comms/def/messages.def.cpp \
    ../../src/FCStdAfx.cpp \
    ../../../libs/utils/hw/ADS1115.cpp \
    ../../../libs/utils/hw/I2C_BCM.cpp \
    ../../../libs/utils/hw/I2C_Dev.cpp \
    ../../../libs/utils/hw/SPI_BCM.cpp \
    ../../../libs/utils/hw/UART_BB.cpp \
    ../../../libs/utils/hw/UART_Dev.cpp \
    ../../src/source/ADS1115_Source.cpp \
    ../../src/bus/I2C_BCM_Bus.cpp \
    ../../src/bus/I2C_Linux_Bus.cpp \
    ../../src/bus/SPI_BCM_Bus.cpp \
    ../../src/bus/SPI_Linux_Bus.cpp \
    ../../src/bus/UART_Linux_Bus.cpp \
    ../../src/bus/UART_BB_Bus.cpp \
    ../../src/processor/Quad_Multirotor_Motor_Mixer.cpp \
    ../../../libs/utils/hw/SPI_PIGPIO.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btDbvt.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btGhostObject.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/btUnionFind.cpp \
    ../../../../bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btBox2dShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btBoxShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btCollisionShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btCompoundShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConcaveShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConeShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btCylinderShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btEmptyShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btShapeHull.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btSphereShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
    ../../../../bullet/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btContactProcessing.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactBvh.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactShape.cpp \
    ../../../../bullet/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
    ../../../../bullet/BulletCollision/Gimpact/gim_box_set.cpp \
    ../../../../bullet/BulletCollision/Gimpact/gim_contact.cpp \
    ../../../../bullet/BulletCollision/Gimpact/gim_memory.cpp \
    ../../../../bullet/BulletCollision/Gimpact/gim_tri_collision.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
    ../../../../bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
    ../../../../bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp \
    ../../../../bullet/BulletDynamics/Dynamics/btRigidBody.cpp \
    ../../../../bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
    ../../../../bullet/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp \
    ../../../../bullet/LinearMath/btAlignedAllocator.cpp \
    ../../../../bullet/LinearMath/btConvexHull.cpp \
    ../../../../bullet/LinearMath/btConvexHullComputer.cpp \
    ../../../../bullet/LinearMath/btGeometryUtil.cpp \
    ../../../../bullet/LinearMath/btPolarDecomposition.cpp \
    ../../../../bullet/LinearMath/btQuickprof.cpp \
    ../../../../bullet/LinearMath/btSerializer.cpp \
    ../../../../bullet/LinearMath/btSerializer64.cpp \
    ../../../../bullet/LinearMath/btThreads.cpp \
    ../../../../bullet/LinearMath/btVector3.cpp \
    ../../../libs/utils/comms/RCP.cpp \
    ../../src/source/UltimateSensorFusion.cpp \
    ../../../libs/utils/comms/esp32/Phy.cpp

HEADERS += \
    ../../../libs/utils/chrono.h \
    ../../../libs/utils/PID.h \
    ../../../libs/utils/Timed_Scope.h \
    ../../../libs/physics/constants.h \
    ../../../libs/common/Comm_Data.h \
    ../../../libs/common/Manual_Clock.h \
    ../../../libs/common/node/source/IInertial.h \
    ../../src/sink/PIGPIO.h \
    ../../src/source/MPU9250.h \
    ../../src/source/MS5611.h \
    ../../src/source/Raspicam.h \
    ../../src/source/RC5T619.h \
    ../../src/source/SRF02.h \
    ../../src/source/UBLOX.h \
    ../../src/processor/ADC_Voltmeter.h \
    ../../src/processor/ADC_Ammeter.h \
    ../../src/processor/EKF_AHRS.h \
    ../../src/processor/Comp_AHRS.h \
    ../../../libs/utils/Butterworth.h \
    ../../../libs/common/node/INode.h \
    ../../../libs/common/node/ISink.h \
    ../../../libs/common/node/ISource.h \
    ../../src/processor/Gravity_Filter.h \
    ../../src/Globals.h \
    ../../../libs/common/node/data/IData.h \
    ../../../libs/common/node/IProcessor.h \
    ../../src/generator/Vec3_Generator.h \
    ../../src/generator/Scalar_Generator.h \
    ../../src/source/EHealth.h \
    ../../../libs/common/node/IController.h \
    ../../../libs/common/node/IGenerator.h \
    ../../../libs/common/node/ILPF.h \
    ../../../libs/common/node/IResampler.h \
    ../../../libs/common/node/ITransformer.h \
    ../../src/controller/Rate_Controller.h \
    ../../src/lpf/LPF.h \
    ../../src/resampler/Resampler.h \
    ../../src/transformer/Transformer.h \
    ../../src/transformer/Transformer_Inv.h \
    ../../src/simulator/Multirotor_Simulation.h \
    ../../src/simulator/Multirotor_Simulator.h \
    ../../../libs/common/node/IConfig.h \
    ../../src/processor/Servo_Gimbal.h \
    ../../src/processor/Motor_Mixer.h \
    ../../src/sink/PCA9685.h \
    ../../src/generator/Oscillator.h \
    ../../src/Sample_Accumulator.h \
    ../../src/MPL_Helper.h \
    ../../src/Basic_Output_Stream.h \
    ../../../libs/lz4/lz4.h \
    ../../src/source/OpenCV_Capture.h \
    ../../../libs/utils/Serialization.h \
    ../../src/source/SRF01.h \
    ../../src/brain/Multirotor_Brain.h \
    ../../../libs/common/node/IBrain.h \
    ../../../libs/common/node/IPilot.h \
    ../../src/source/MaxSonar.h \
    ../../src/processor/Throttle_To_PWM.h \
    ../../../libs/utils/Coordinates.h \
    ../../../libs/common/stream/IAcceleration.h \
    ../../../libs/common/stream/IADC.h \
    ../../../libs/common/stream/IAngular_Velocity.h \
    ../../../libs/common/stream/IBattery_State.h \
    ../../../libs/common/stream/IBool.h \
    ../../../libs/common/stream/ICurrent.h \
    ../../../libs/common/stream/IDistance.h \
    ../../../libs/common/stream/IFloat.h \
    ../../../libs/common/stream/IForce.h \
    ../../../libs/common/stream/IFrame.h \
    ../../../libs/common/stream/IGPS_Info.h \
    ../../../libs/common/stream/ILinear_Acceleration.h \
    ../../../libs/common/stream/IMagnetic_Field.h \
    ../../../libs/common/stream/IMultirotor_State.h \
    ../../../libs/common/stream/IPosition.h \
    ../../../libs/common/stream/IPressure.h \
    ../../../libs/common/stream/IProximity.h \
    ../../../libs/common/stream/IPWM.h \
    ../../../libs/common/stream/IStream.h \
    ../../../libs/common/stream/ITemperature.h \
    ../../../libs/common/stream/IThrottle.h \
    ../../../libs/common/stream/ITorque.h \
    ../../../libs/common/stream/IVelocity.h \
    ../../../libs/common/stream/IVideo.h \
    ../../../libs/common/stream/IVoltage.h \
    ../../../libs/common/stream/Stream_Base.h \
    ../../../libs/common/bus/IBus.h \
    ../../../libs/common/bus/II2C.h \
    ../../../libs/common/bus/ISPI.h \
    ../../../libs/common/bus/IUART.h \
    ../../../libs/kalman/ekfilter.hpp \
    ../../../libs/kalman/ekfilter_impl.hpp \
    ../../../libs/kalman/kfilter.hpp \
    ../../../libs/kalman/kfilter_impl.hpp \
    ../../../libs/kalman/kmatrix.hpp \
    ../../../libs/kalman/kmatrix_impl.hpp \
    ../../../libs/kalman/ktypes.hpp \
    ../../../libs/kalman/kvector.hpp \
    ../../../libs/kalman/kvector_impl.hpp \
    ../../src/processor/Comp_ECEF.h \
    ../../src/processor/KF_ECEF.h \
    ../../src/processor/Proximity.h \
    ../../../libs/common/stream/IMultirotor_Commands.h \
    ../../src/source/AVRADC.h \
    ../../../libs/common/node/ICombiner.h \
    ../../src/combiner/Combiner.h \
    ../../src/processor/Pressure_Velocity.h \
    ../../src/processor/ENU_Frame_System.h \
    ../../src/source/RaspiCamControl.h \
    ../../src/brain/LiPo_Battery.h \
    ../../src/pilot/Multirotor_Pilot.h \
    ../../../libs/common/node/IMultirotor_Simulator.h \
    ../../def/gen_support.h \
    ../../def/hal.def.h \
    ../../../libs/common/comms/def/gs_comms.def.h \
    ../../src/uav_properties/IMultirotor_Properties.h \
    ../../src/uav_properties/IUAV_Properties.h \
    ../../src/HAL.h \
    ../../src/RC_Comms.h \
    ../../src/GS_Comms.h \
    ../../src/uav_properties/Hexa_Multirotor_Properties.h \
    ../../src/uav_properties/Hexatri_Multirotor_Properties.h \
    ../../src/uav_properties/Octo_Multirotor_Properties.h \
    ../../src/uav_properties/Octoquad_Multirotor_Properties.h \
    ../../src/uav_properties/Quad_Multirotor_Properties.h \
    ../../src/uav_properties/Tri_Multirotor_Properties.h \
    ../../src/source/CPPM_Receiver.h \
    ../../../libs/utils/comms/Channel.h \
    ../../../libs/utils/comms/ISocket.h \
    ../../../libs/utils/comms/UDP_Socket.h \
    ../../../libs/utils/hw/bcm2835.h \
    ../../../libs/utils/hw/command.h \
    ../../../libs/utils/hw/pigpio.h \
    ../../../libs/utils/hw/SPI_Dev.h \
    ../../../libs/utils/Pool.h \
    ../../../libs/common/comms/def/messages.def.h \
    ../../../libs/common/stream/IMultirotor_Simulator_State.h \
    ../../src/FCStdAfx.h \
    ../../../libs/utils/hw/ADS1115.h \
    ../../../libs/utils/hw/I2C_BCM.h \
    ../../../libs/utils/hw/I2C_Dev.h \
    ../../../libs/utils/hw/II2C.h \
    ../../../libs/utils/hw/ISPI.h \
    ../../../libs/utils/hw/IUART.h \
    ../../../libs/utils/hw/SPI_BCM.h \
    ../../../libs/utils/hw/UART_BB.h \
    ../../../libs/utils/hw/UART_Dev.h \
    ../../src/source/ADS1115_Source.h \
    ../../src/bus/I2C_BCM_Bus.h \
    ../../src/bus/I2C_Linux_Bus.h \
    ../../src/bus/SPI_BCM_Bus.h \
    ../../src/bus/SPI_Linux_Bus.h \
    ../../src/bus/UART_Linux_Bus.h \
    ../../src/bus/UART_BB_Bus.h \
    ../../src/processor/Quad_Multirotor_Motor_Mixer.h \
    ../../../libs/common/stream/ICamera_Commands.h \
    ../../../libs/utils/hw/SPI_PIGPIO.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btAxisSweep3.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btAxisSweep3Internal.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btDbvt.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btDispatcher.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btQuantizedBvh.h \
    ../../../../bullet/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btBoxBoxDetector.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionConfiguration.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionDispatcher.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionObject.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCollisionWorldImporter.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btGhostObject.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btHashedSimplePairCache.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btManifoldResult.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/btUnionFind.h \
    ../../../../bullet/BulletCollision/CollisionDispatch/SphereTriangleDetector.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btBox2dShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btBoxShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btCapsuleShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btCollisionMargin.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btCollisionShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btCompoundShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConcaveShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConeShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvex2dShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexHullShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexInternalShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexPointCloudShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexPolyhedron.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btCylinderShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btEmptyShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btMaterial.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btMinkowskiSumShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btMultiSphereShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btOptimizedBvh.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btShapeHull.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btSphereShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btStaticPlaneShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btStridingMeshInterface.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTetrahedronShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleBuffer.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleCallback.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleInfoMap.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleMesh.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleMeshShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btTriangleShape.h \
    ../../../../bullet/BulletCollision/CollisionShapes/btUniformScalingShape.h \
    ../../../../bullet/BulletCollision/Gimpact/btBoxCollision.h \
    ../../../../bullet/BulletCollision/Gimpact/btClipPolygon.h \
    ../../../../bullet/BulletCollision/Gimpact/btCompoundFromGimpact.h \
    ../../../../bullet/BulletCollision/Gimpact/btContactProcessing.h \
    ../../../../bullet/BulletCollision/Gimpact/btContactProcessingStructs.h \
    ../../../../bullet/BulletCollision/Gimpact/btGenericPoolAllocator.h \
    ../../../../bullet/BulletCollision/Gimpact/btGeometryOperations.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactBvh.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactBvhStructs.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactMassUtil.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactQuantizedBvhStructs.h \
    ../../../../bullet/BulletCollision/Gimpact/btGImpactShape.h \
    ../../../../bullet/BulletCollision/Gimpact/btQuantization.h \
    ../../../../bullet/BulletCollision/Gimpact/btTriangleShapeEx.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_array.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_basic_geometry_operations.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_bitset.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_box_collision.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_box_set.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_clip_polygon.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_contact.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_geometry.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_geom_types.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_hash_table.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_linear_math.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_math.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_memory.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_radixsort.h \
    ../../../../bullet/BulletCollision/Gimpact/gim_tri_collision.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btComputeGjkEpaPenetration.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btConvexCast.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkCollisionDescription.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkEpa3.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btMprPenetration.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btPointCollector.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h \
    ../../../../bullet/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btConstraintSolver.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btContactConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btContactSolverInfo.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btFixedConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGearConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btHinge2Constraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btHingeConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btJacobianEntry.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSliderConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSolverBody.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btSolverConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h \
    ../../../../bullet/BulletDynamics/ConstraintSolver/btUniversalConstraint.h \
    ../../../../bullet/BulletDynamics/Dynamics/btActionInterface.h \
    ../../../../bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h \
    ../../../../bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h \
    ../../../../bullet/BulletDynamics/Dynamics/btDynamicsWorld.h \
    ../../../../bullet/BulletDynamics/Dynamics/btRigidBody.h \
    ../../../../bullet/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h \
    ../../../../bullet/BulletDynamics/Dynamics/btSimulationIslandManagerMt.h \
    ../../../../bullet/LinearMath/btAabbUtil2.h \
    ../../../../bullet/LinearMath/btAlignedAllocator.h \
    ../../../../bullet/LinearMath/btAlignedObjectArray.h \
    ../../../../bullet/LinearMath/btConvexHull.h \
    ../../../../bullet/LinearMath/btConvexHullComputer.h \
    ../../../../bullet/LinearMath/btCpuFeatureUtility.h \
    ../../../../bullet/LinearMath/btDefaultMotionState.h \
    ../../../../bullet/LinearMath/btGeometryUtil.h \
    ../../../../bullet/LinearMath/btGrahamScan2dConvexHull.h \
    ../../../../bullet/LinearMath/btHashMap.h \
    ../../../../bullet/LinearMath/btIDebugDraw.h \
    ../../../../bullet/LinearMath/btList.h \
    ../../../../bullet/LinearMath/btMatrix3x3.h \
    ../../../../bullet/LinearMath/btMatrixX.h \
    ../../../../bullet/LinearMath/btMinMax.h \
    ../../../../bullet/LinearMath/btMotionState.h \
    ../../../../bullet/LinearMath/btPolarDecomposition.h \
    ../../../../bullet/LinearMath/btPoolAllocator.h \
    ../../../../bullet/LinearMath/btQuadWord.h \
    ../../../../bullet/LinearMath/btQuaternion.h \
    ../../../../bullet/LinearMath/btQuickprof.h \
    ../../../../bullet/LinearMath/btRandom.h \
    ../../../../bullet/LinearMath/btScalar.h \
    ../../../../bullet/LinearMath/btSerializer.h \
    ../../../../bullet/LinearMath/btSpatialAlgebra.h \
    ../../../../bullet/LinearMath/btStackAlloc.h \
    ../../../../bullet/LinearMath/btThreads.h \
    ../../../../bullet/LinearMath/btTransform.h \
    ../../../../bullet/LinearMath/btTransformUtil.h \
    ../../../../bullet/LinearMath/btVector3.h \
    ../../../libs/utils/comms/RCP.h \
    ../../../libs/utils/comms/esp8266/Queue.h \
    ../../src/source/UltimateSensorFusion.h \
    ../../../libs/utils/comms/esp32/Phy.h \
    ../../../libs/utils/comms/esp32/spi_comms.h

DISTFILES += \
    ../../../../bullet/Bullet3Common/CMakeLists.txt \
    ../../../../bullet/Bullet3Common/premake4.lua

