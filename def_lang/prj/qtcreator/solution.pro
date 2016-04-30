TEMPLATE = subdirs

rpi {
    MKFL1 = "Makefile1.rpi"
    MKFL2 = "Makefile2.rpi"
} else {
    MKFL1 = "Makefile1"
    MKFL2 = "Makefile2"
}

SUBDIRS += def_lang generator

def_lang.file = def_lang.pro
def_lang.makefile = $${MKFL1}

generator.file = generator.pro
generator.makefile = $${MKFL2}
generator.depends = def_lang

