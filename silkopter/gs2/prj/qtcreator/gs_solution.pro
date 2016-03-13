TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}


SUBDIRS += qmath qdata qbase qinput gs

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}

qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qdata.makefile = $${MKFL}

qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

qinput.file = ../../../../qinput/prj/qtcreator/qinput.pro
qinput.makefile = $${MKFL}

gs.file = gs.pro
gs.depends = qbase qdata qmath qinput

