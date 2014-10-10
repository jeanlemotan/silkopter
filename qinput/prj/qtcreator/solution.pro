TEMPLATE = subdirs


SUBDIRS += qinput qdata qbase

qbase.file = ../../../qbase/prj/qtcreator/qbase.pro
qdata.file = ../../../qdata/prj/qtcreator/qdata.pro

qinput.file = qinput.pro
qinput.depends = qbase qdata

