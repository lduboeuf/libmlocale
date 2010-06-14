###############################################################################
# Internationalization module
############################################################################### 
I18N_SRC_DIR=./i18n
INCLUDEPATH+=./i18n


PUBLIC_HEADERS += \
    $$I18N_SRC_DIR/mbreakiterator.h \
    $$I18N_SRC_DIR/mlocale.h \
    $$I18N_SRC_DIR/mcountry.h \
    $$I18N_SRC_DIR/mcity.h \
    $$I18N_SRC_DIR/mlocationdatabase.h \

SOURCES += \
    $$I18N_SRC_DIR/mbreakiterator.cpp \
    $$I18N_SRC_DIR/mlocale.cpp \
    $$I18N_SRC_DIR/mcountry.cpp \
    $$I18N_SRC_DIR/mcity.cpp \
    $$I18N_SRC_DIR/mlocationdatabase.cpp \

contains(DEFINES, HAVE_ICU) {
    PUBLIC_HEADERS += \
        $$I18N_SRC_DIR/mcalendar.h \
        $$I18N_SRC_DIR/mcollator.h \

    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/micubreakiterator.h \
        $$I18N_SRC_DIR/micuconversions.h

    SOURCES += \
        $$I18N_SRC_DIR/mcalendar.cpp \
        $$I18N_SRC_DIR/mcollator.cpp \
        $$I18N_SRC_DIR/micubreakiterator.cpp \
        $$I18N_SRC_DIR/micuconversions.cpp
} else {
    PRIVATE_HEADERS += \
        $$I18N_SRC_DIR/mnullbreakiterator.h \

    SOURCES += \
        $$I18N_SRC_DIR/mnullbreakiterator.cpp
}
