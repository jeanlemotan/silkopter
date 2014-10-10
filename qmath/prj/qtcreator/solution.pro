TEMPLATE = subdirs


SUBDIRS += qmath qbase

qbase.file = ../../../qbase/prj/qtcreator/qbase.pro
qmath.file = qmath.pro
qmath.depends = qbase

