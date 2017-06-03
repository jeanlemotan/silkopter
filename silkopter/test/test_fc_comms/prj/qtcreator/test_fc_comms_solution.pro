TEMPLATE = subdirs

rpi {
    MKFL = "Makefile.rpi"
} else {
    MKFL = "Makefile"
}


SUBDIRS += qmath qbase test_fc_comms

qbase.file = ../../../../../qbase/prj/qtcreator/qbase.pro
qbase.makefile = $${MKFL}

qmath.file = ../../../../../qmath/prj/qtcreator/qmath.pro
qmath.makefile = $${MKFL}

test_fc_comms.file = test_fc_comms.pro
test_fc_comms.depends = qbase qmath

