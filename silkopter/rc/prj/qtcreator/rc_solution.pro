TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}


SUBDIRS += qmath qdata qbase qinput def_lang rc

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}

qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qdata.makefile = $${MKFL}

qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

qinput.file = ../../../../qinput/prj/qtcreator/qinput.pro
qinput.makefile = $${MKFL}

def_lang.file = ../../../../def_lang/prj/qtcreator/def_lang.pro
def_lang.makefile = $${MKFL}

rc.file = rc.pro
rc.depends = qbase qdata qmath qinput def_lang

