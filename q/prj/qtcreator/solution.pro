TEMPLATE = subdirs


SUBDIRS += qinput qdata qbase qmath q

qbase.file = ../../../qbase/prj/qtcreator/qbase.pro
qdata.file = ../../../qdata/prj/qtcreator/qdata.pro
qmath.file = ../../../qmath/prj/qtcreator/qmath.pro
qinput.file = ../../../qinput/prj/qtcreator/qinput.pro

q.file = q.pro
q.depends = qbase qdata qmath qinput

