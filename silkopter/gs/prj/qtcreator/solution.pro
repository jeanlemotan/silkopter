TEMPLATE = subdirs


SUBDIRS += qmath qdata qbase qinput q gs

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qinput.file = ../../../../qinput/prj/qtcreator/qinput.pro
q.file = ../../../../q/prj/qtcreator/q.pro

gs.file = gs.pro
gs.depends = qbase qdata qmath q qinput

