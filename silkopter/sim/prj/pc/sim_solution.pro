TEMPLATE = subdirs


SUBDIRS += qmath qdata qbase qinput q sim

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
qinput.file = ../../../../qinput/prj/qtcreator/qinput.pro
q.file = ../../../../q/prj/qtcreator/q.pro

sim.file = sim.pro
sim.depends = qbase qdata qmath qinput q

