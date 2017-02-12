TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}


SUBDIRS += qmath qbase qdata q gs def_lang

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}

qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qdata.makefile = $${MKFL}

q.file = ../../../../q/prj/qtcreator/q.pro
q.makefile = $${MKFL}

qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

def_lang.file = ../../../../def_lang/prj/qtcreator/def_lang.pro
def_lang.makefile = $${MKFL}

gs.file = gs.pro
gs.depends = qbase qdata q qmath def_lang

