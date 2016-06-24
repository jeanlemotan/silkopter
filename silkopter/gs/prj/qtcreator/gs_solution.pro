TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}


SUBDIRS += qmath qdata qbase gs qmapcontrol def_lang

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}

qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qdata.makefile = $${MKFL}

qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

qmapcontrol.file = ../../../../QMapControl/src/qmapcontrol.pro
qmapcontrol.makefile = $${MKFL}

def_lang.file = ../../../../def_lang/prj/qtcreator/def_lang.pro
def_lang.makefile = $${MKFL}

gs.file = gs.pro
gs.depends = qbase qdata qmath qmapcontrol def_lang

