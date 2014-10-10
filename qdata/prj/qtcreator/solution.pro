TEMPLATE = subdirs


SUBDIRS += qdata qbase

qbase.file = ../../../qbase/prj/qtcreator/qbase.pro
qdata.file = qdata.pro
qdata.depends = qbase

