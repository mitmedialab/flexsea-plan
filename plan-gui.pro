#-------------------------------------------------
#
# Project created by QtCreator 2016-08-23T15:35:07

QT += core gui charts serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = plan-gui
#CONFIG += console
TEMPLATE = app
DEFINES += BOARD_TYPE_FLEXSEA_PLAN INCLUDE_UPROJ_RICNU_KNEE_V1
DEFINES += INCLUDE_UPROJ_MIT_A2DOF INCLUDE_UPROJ_CYCLE_TESTER
DEFINES += INCLUDE_UPROJ_DPEB42 INCLUDE_UPROJ_ACTPACK
DEFINES += DEPHY

RC_ICONS = misc/icons/d_logo_outlined.ico

INCLUDEPATH += inc \
				inc/MDIWinObjects \
				inc/FlexSEADevice \
				flexsea-comm/inc \
				flexsea-system/inc \
				flexsea-shared/unity \
				flexsea-shared/inc \
				flexsea-projects/inc \
				flexsea-dephy/inc \
				flexsea-projects/MIT_2DoF_Ankle_v1/inc \
				flexsea-projects/RICNU_Knee_v1/inc \
				flexsea-projects/Rigid/inc \
				flexsea-dephy/DpEb42/inc \
				flexsea-dephy/CycleTester/inc \
                                biomech_flexsea-projects/ActPack/inc

SOURCES += src/main.cpp \
				src/mainwindow.cpp \
				src/datalogger.cpp \
				src/flexsea_generic.cpp \
				src/serialdriver.cpp \
				src/flexsea_board.c \
				src/trapez.c \
				src/peripherals.c \
				src/chartcontroller.cpp \
				src/chartdata.cpp \
				src/customchartview.cpp \
				src/dataprovider.cpp \
				src/passthroughprovider.cpp \
				src/sineprovider3.cpp \
				src/quantdata.cpp \
				src/dynamicuserdatamanager.cpp \
				src/FlexSEADevice/executeDevice.cpp \
				src/FlexSEADevice/flexseaDevice.cpp \
				src/FlexSEADevice/batteryDevice.cpp \
				src/FlexSEADevice/gossipDevice.cpp \
				src/FlexSEADevice/manageDevice.cpp \
				src/FlexSEADevice/strainDevice.cpp \
				src/FlexSEADevice/ricnuProject.cpp \
				src/FlexSEADevice/ankle2DofProject.cpp \
				src/MDIWinObjects/w_2dplot.cpp \
				src/MDIWinObjects/w_anycommand.cpp \
				src/MDIWinObjects/w_battery.cpp \
				src/MDIWinObjects/w_calibration.cpp \
				src/MDIWinObjects/w_control.cpp \
				src/MDIWinObjects/w_converter.cpp \
				src/MDIWinObjects/w_execute.cpp \
				src/MDIWinObjects/w_gossip.cpp \
				src/MDIWinObjects/w_manage.cpp \
				src/MDIWinObjects/w_ricnu.cpp \
				src/MDIWinObjects/w_slavecomm.cpp \
				src/MDIWinObjects/w_strain.cpp \
				src/MDIWinObjects/w_config.cpp \
				src/MDIWinObjects/w_logkeypad.cpp \
				src/MDIWinObjects/w_userrw.cpp \
				src/MDIWinObjects/w_incontrol.cpp \
				src/MDIWinObjects/w_event.cpp \
				src/MDIWinObjects/w_commtest.cpp \
				src/MDIWinObjects/w_glplot.cpp \
				src/MDIWinObjects/w_ankleTorque.cpp \
				src/MDIWinObjects/w_ankleAnglePlot.cpp \
				src/MDIWinObjects/w_cycletester.cpp \
				src/MDIWinObjects/w_usertesting.cpp \
				src/MDIWinObjects/scribblearea.cpp \
				flexsea-shared/unity/unity.c \
                                biomech_flexsea-projects/src/flexsea_cmd_user.c \
                                biomech_flexsea-projects/src/dynamic_user_structs_plan.c \
                                biomech_flexsea-projects/src/flexsea_user_structs.c \
                                biomech_flexsea-projects/MIT_2DoF_Ankle_v1/src/cmd-MIT_2DoF_Ankle_v1.c \
                                biomech_flexsea-projects/RICNU_Knee_v1/src/cmd-RICNU_Knee_v1.c \
				flexsea-comm/src/flexsea.c \
				flexsea-comm/src/flexsea_buffers.c \
				flexsea-comm/src/flexsea_circular_buffer.c \
				flexsea-comm/src/flexsea_comm.c \
				flexsea-comm/src/flexsea_payload.c \
				flexsea-system/src/flexsea_system.c \
				flexsea-system/src/flexsea_global_structs.c \
				flexsea-system/src/flexsea_cmd_data.c \
				flexsea-system/src/flexsea_cmd_external.c \
				flexsea-system/src/flexsea_cmd_sensors.c \
				flexsea-system/src/flexsea_cmd_calibration.c \
				flexsea-system/src/flexsea_cmd_control_1.c \
				flexsea-system/src/flexsea_cmd_control_2.c \
				flexsea-system/src/flexsea_cmd_tools.c \
				flexsea-system/src/flexsea_cmd_in_control.c \
				flexsea-system/src/flexsea_cmd_stream.c \
                                biomech_flexsea-projects/MIT_2DoF_Ankle_v1/src/user-ex-MIT_2DoF_Ankle_v1.c \
                                biomech_flexsea-projects/MIT_2DoF_Ankle_v1/src/user-mn-MIT_2DoF_Ankle_v1.c \
                                biomech_flexsea-projects/RICNU_Knee_v1/src/user-ex-RICNU_Knee_v1.c \
                                biomech_flexsea-projects/RICNU_Knee_v1/src/user-mn-RICNU_Knee_v1.c \
                                biomech_flexsea-projects/src/user-ex.c \
                                biomech_flexsea-projects/src/user-mn.c \
                                biomech_flexsea-projects/src/dynamic_user_structs_common.c \
				flexsea-dephy/CycleTester/src/cmd-CycleTester.c \
				flexsea-dephy/src/flexsea_cmd_angle_torque_profile.c \
				src/FlexSEADevice/rigidDevice.cpp \
				src/MDIWinObjects/w_rigid.cpp \
                                biomech_flexsea-projects/Rigid/src/cmd-Rigid.c \
				flexsea-dephy/src/flexsea_cmd_dephy.c \
				flexsea-dephy/DpEb42/src/cmd-DpEb42.c \
				flexsea-dephy/DpEb42/src/cmd-UTT.c \
				flexsea-dephy/DpEb42/src/cmd-GaitStats.c \
                                biomech_flexsea-projects/ActPack/src/cmd-ActPack.c \
				src/commanager.cpp \
				src/MDIWinObjects/w_gaitstats.cpp \
				src/MDIWinObjects/w_status.cpp \
    src/MDIWinObjects/w_pocket.cpp \
    src/FlexSEADevice/pocketDevice.cpp \
    src/betterchartview.cpp \
    src/betterchart.cpp

HEADERS += inc/main.h \
				inc/mainwindow.h \
				inc/counter.h \
				inc/datalogger.h \
				inc/trapez.h \
				inc/serialdriver.h \
				inc/flexsea_generic.h \
				inc/flexsea_board.h \
				inc/peripherals.h \
				inc/chartcontroller.h \
				inc/chartdata.h \
				inc/customchartview.h \
				inc/dataprovider.h \
				inc/passthroughprovider.h \
				inc/sineprovider3.h \
				inc/quantdata.h \
				inc/dynamicuserdatamanager.h \
				inc/define.h \
				inc/FlexSEADevice/executeDevice.h \
				inc/FlexSEADevice/flexseaDevice.h \
				inc/FlexSEADevice/batteryDevice.h \
				inc/FlexSEADevice/gossipDevice.h \
				inc/FlexSEADevice/manageDevice.h \
				inc/FlexSEADevice/strainDevice.h \
				inc/FlexSEADevice/ricnuProject.h \
				inc/FlexSEADevice/ankle2DofProject.h \
				inc/MDIWinObjects/w_2dplot.h \
				inc/MDIWinObjects/w_anycommand.h \
				inc/MDIWinObjects/w_battery.h \
				inc/MDIWinObjects/w_calibration.h \
				inc/MDIWinObjects/w_control.h \
				inc/MDIWinObjects/w_converter.h \
				inc/MDIWinObjects/w_execute.h \
				inc/MDIWinObjects/w_gossip.h \
				inc/MDIWinObjects/w_manage.h \
				inc/MDIWinObjects/w_ricnu.h \
				inc/MDIWinObjects/w_slavecomm.h \
				inc/MDIWinObjects/w_strain.h \
				inc/MDIWinObjects/w_incontrol.h \
				inc/MDIWinObjects/w_event.h \
				inc/MDIWinObjects/w_config.h \
				inc/MDIWinObjects/w_logkeypad.h \
				inc/MDIWinObjects/w_userrw.h \
				inc/MDIWinObjects/w_commtest.h \
				inc/MDIWinObjects/w_glplot.h \
				inc/MDIWinObjects/w_ankleTorque.h \
				inc/MDIWinObjects/w_ankleAnglePlot.h \
				inc/MDIWinObjects/w_cycletester.h \
				inc/MDIWinObjects/w_usertesting.h \
				inc/MDIWinObjects/scribblearea.h \
				flexsea-shared/unity/unity.h \
				flexsea-shared/unity/unity_internals.h \
                                biomech_flexsea-projects/inc/flexsea_cmd_user.h \
                                biomech_flexsea-projects/MIT_2DoF_Ankle_v1/inc/cmd-MIT_2DoF_Ankle_v1.h \
                                biomech_flexsea-projects/RICNU_Knee_v1/inc/cmd-RICNU_Knee_v1.h \
                                biomech_flexsea-projects/inc/flexsea_user_structs.h \
                                biomech_flexsea-projects/inc/user-plan.h \
                                biomech_flexsea-projects/inc/dynamic_user_structs.h \
				flexsea-comm/inc/flexsea.h \
				flexsea-comm/inc/flexsea_comm_def.h \
				flexsea-comm/inc/flexsea_buffers.h \
				flexsea-comm/inc/flexsea_circular_buffer.h \
				flexsea-comm/inc/flexsea_comm.h \
				flexsea-comm/inc/flexsea_payload.h \
				flexsea-system/inc/flexsea_system.h \
				flexsea-system/inc/flexsea_sys_def.h \
				flexsea-system/inc/flexsea_global_structs.h \
				flexsea-system/inc/flexsea_dataformats.h \
				flexsea-system/inc/flexsea_cmd_data.h \
				flexsea-system/inc/flexsea_cmd_external.h \
				flexsea-system/inc/flexsea_cmd_sensors.h \
				flexsea-system/inc/flexsea_cmd_calibration.h \
				flexsea-system/inc/flexsea_cmd_control.h \
				flexsea-system/inc/flexsea_cmd_tools.h \
				flexsea-system/inc/flexsea_cmd_in_control.h \
				flexsea-system/inc/flexsea_cmd_stream.h \
				flexsea-dephy/CycleTester/inc/cmd-CycleTester.h \
				inc/FlexSEADevice/rigidDevice.h \
				flexsea-dephy/inc/flexsea_cmd_angle_torque_profile.h \
				inc/MDIWinObjects/w_rigid.h \
                                biomech_flexsea-projects/Rigid/inc/cmd-Rigid.h \
				flexsea-dephy/inc/flexsea_cmd_dephy.h \
				flexsea-dephy/DpEb42/inc/cmd-DpEb42.h \
				flexsea-dephy/DpEb42/inc/cmd-UTT.h \
				flexsea-dephy/DpEb42/inc/cmd-GaitStats.h \
                                biomech_flexsea-projects/ActPack/inc/cmd-ActPack.h \
				inc/commanager.h \
				inc/MDIWinObjects/w_gaitstats.h \
				inc/MDIWinObjects/w_status.h \
    inc/MDIWinObjects/w_pocket.h \
    inc/FlexSEADevice/pocketDevice.h \
    inc/betterchartview.h \
    inc/betterchart.h

FORMS += ui/mainwindow.ui \
				ui/w_execute.ui \
				ui/w_control.ui \
				ui/w_2dplot.ui \
				ui/w_slavecomm.ui \
				ui/w_anycommand.ui \
				ui/w_converter.ui \
				ui/w_ricnu.ui \
				ui/w_manage.ui \
				ui/w_calibration.ui \
				ui/w_battery.ui \
				ui/w_gossip.ui \
				ui/w_strain.ui \
				ui/w_config.ui \
				ui/w_logkeypad.ui \
				ui/w_userrw.ui \
				ui/w_commtest.ui \
				ui/w_incontrol.ui \
				ui/w_event.ui \
				ui/w_glplot.ui \
				ui/w_ankleTorque.ui \
				ui/w_ankleAnglePlot.ui \
				ui/w_rigid.ui \
				ui/w_cycletester.ui \
				ui/w_usertesting.ui \
				ui/w_gaitstats.ui \
				ui/w_status.ui \
    ui/w_pocket.ui

RESOURCES += \
		misc/icons.qrc

QMAKE_CFLAGS = $$QMAKE_CFLAGS -Wno-unused-but-set-variable

#Enable the next 2 lines to inspect the pre-processor output
#Linked will fail - debug only
#QMAKE_CFLAGS = $$QMAKE_CFLAGS -E
#QMAKE_CXXFLAGS = $$QMAKE_CXXFLAGS -E

DISTFILES += \
                biomech_flexsea-projects/flexsea.gitignore \
                biomech_flexsea-projects/GPL-3.0.txt \
                biomech_flexsea-projects/README.md \
		flexsea-dephy/flexsea.gitignore \
		flexsea-dephy/GPL-3.0.txt \
		flexsea-dephy/README.md \
		flexsea-comm/flexsea.gitignore \
		flexsea-comm/GPL-3.0.txt \
		flexsea-shared/unity/readme.txt \
		flexsea-shared/flexsea.gitignore \
		flexsea-shared/GPL-3.0.txt \
		flexsea-system/.gitignore \
		flexsea-system/GPL-3.0.txt \
		.gitignore \
		.gitmodules \
		GPL-3.0.txt \
		flexsea-comm/README.md \
		flexsea-shared/README.md \
		flexsea-system/README.md \
		README.md
