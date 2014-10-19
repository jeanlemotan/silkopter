TEMPLATE = subdirs


SUBDIRS += qmath qdata qbase brain

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro

brain.file = brain.pro
brain.depends = qbase qdata qmath

