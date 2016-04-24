TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}

SUBDIRS += qmath qdata qbase def_lang brain

def_lang.file = ../../../../def_lang/prj/qtcreator/def_lang.pro
def_lang.makefile = $${MKFL}
qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}
qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qdata.makefile = $${MKFL}
qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

brain.file = brain.pro
brain.depends = def_lang qbase qdata qmath

