TEMPLATE = subdirs

rpi {
    QMAKE_MAKEFILE = "Makefile.rpi"
} else {
    QMAKE_MAKEFILE = "Makefile"
}

SUBDIRS += qdata qbase

qbase.file = ../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${QMAKE_MAKEFILE}

qdata.file = qdata.pro
qdata.depends = qbase


