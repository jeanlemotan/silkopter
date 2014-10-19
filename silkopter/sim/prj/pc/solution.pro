TEMPLATE = subdirs


SUBDIRS += qmath qdata qbase q sim

qbase.file = ../../../../qbase/prj/qtcreator/qbase.pro
qdata.file = ../../../../qdata/prj/qtcreator/qdata.pro
qmath.file = ../../../../qmath/prj/qtcreator/qmath.pro
q.file = ../../../../q/prj/qtcreator/q.pro

sim.file = sim.pro
sim.depends = qbase qdata qmath q

