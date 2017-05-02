TEMPLATE = subdirs

android {
    MKFL = "Makefile.android"
} else {
    MKFL = "Makefile"
}

SUBDIRS += qmath qbase android_viewer

qbase.file = ../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}
qmath.file = ../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

android_viewer.file = android-viewer.pro
android_viewer.depends = qbase qmath

