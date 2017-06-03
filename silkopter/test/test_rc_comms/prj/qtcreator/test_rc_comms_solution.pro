TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}


SUBDIRS += qmath qbase test_rc_comms

qbase.file = ../../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}

qmath.file = ../../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

test_rc_comms.file = test_rc_comms.pro
test_rc_comms.depends = qbase qmath

