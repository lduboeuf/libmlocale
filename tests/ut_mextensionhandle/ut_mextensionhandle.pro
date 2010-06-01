include(../common_top.pri)
include(../common_mextensions.pri)

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/corelib/style
QT += xml

win32 {
    QMAKE_MOC = $${OUT_PWD}\..\..\mmoc\mmoc
    contains(DEFINES, IS_ARMEL) {
        QMAKE_MOC = perl.exe $${OUT_PWD}\..\..\mmoc\mmoc.pl
    }
} else {
    PRE_TARGETDEPS += ../../mgen/mgen
    QMAKE_MOC = PATH=../../mgen:$$(PATH) $${OUT_PWD}/../../mmoc/mmoc
    contains(DEFINES, IS_ARMEL) {
        QMAKE_MOC = PATH=../../mgen:$$(PATH) perl $${OUT_PWD}/../../mmoc/mmoc.pl
    }
}

MGEN_OUTDIR = .
mgenerator_model.name = mgenerator model
mgenerator_model.input = MODEL_HEADERS
mgenerator_model.depends = ../../mgen/mgen
mgenerator_model.output = $$MGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
mgenerator_model.commands += ../../mgen/mgen --model ${QMAKE_FILE_NAME} $$MGEN_OUTDIR
mgenerator_model.clean += $$MGEN_OUTDIR/gen_*
mgenerator_model.CONFIG = target_predeps no_link
mgenerator_model.variable_out = GENERATED_SOURCES
mgenerator_style.name = mgenerator style
mgenerator_style.input = STYLE_HEADERS
mgenerator_style.depends = ../../mgen/mgen
mgenerator_style.output = $$MGEN_OUTDIR/gen_${QMAKE_FILE_BASE}data.cpp
mgenerator_style.commands += ../../mgen/mgen --style ${QMAKE_FILE_NAME} $$MGEN_OUTDIR
mgenerator_style.clean += $$MGEN_OUTDIR/gen_*
mgenerator_style.CONFIG = target_predeps no_link
mgenerator_style.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += mgenerator_model mgenerator_style

MODEL_HEADERS += $$MSRCDIR/corelib/widgets/mwidgetmodel.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h

STYLE_HEADERS += $$MSRCDIR/extensions/style/mextensionhandlestyle.h

SOURCES += ut_mextensionhandle.cpp \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.cpp \
    $$MSRCDIR/corelib/widgets/mwidgetmodel.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp \

HEADERS += ut_mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandle_p.h \
    $$MSRCDIR/extensions/applicationextension/mextensionhandlemodel.h \
    $$MSRCDIR/corelib/widgets/core/mwidgetcontroller_p.h \
    $$MSRCDIR/corelib/widgets/core/mwidget_p.h \
    $$MSRCDIR/corelib/widgets/mwidgetmodel_p.h \
    $$MSRCDIR/corelib/widgets/mobjectmenu.h \
    $$MSRCDIR/extensions/style/mextensionhandlestyle.h

include(../common_bot.pri)
