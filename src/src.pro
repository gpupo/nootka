
QT += multimedia androidextras core widgets quick quickcontrols2 printsupport


TARGET = Nootka2
TEMPLATE = app

SOURCES +=  main.cpp \
            main/tnameitem.cpp \
          \
            main/texamexecutor.cpp \
            main/texecutorsupply.cpp \
            main/tglobalexamstore.cpp \
            main/texercises.cpp \
            main/tequalrand.cpp \
            main/texammelody.cpp \
            main/trandmelody.cpp \
            main/texamview.cpp \
            main/tpenalty.cpp \
            main/ttiphandler.cpp \
            main/tmainscoreobject.cpp \
            main/texamsummary.cpp \
            main/tstartexamitem.cpp \
            main/tnootkacertificate.cpp \
            main/tpiechartitem.cpp \
          \
            dialogs/tdialogloaderobject.cpp \
            dialogs/tlevelcreatoritem.cpp \
            dialogs/tlevelpreviewitem.cpp \
            dialogs/tlevelsdefs.cpp \
            dialogs/tlevelselector.cpp \
            dialogs/tlevelheaderwdg.cpp \
            dialogs/tmelodylistview.cpp \
            dialogs/ttunerdialogitem.cpp \
            dialogs/trtmselectoritem.cpp \
            dialogs/tmelgenitem.cpp \
          \
            help/thelpdialogbase.cpp \
            help/texamhelp.cpp \
            help/tmainhelp.cpp \
          \
            updater/tupdatechecker.cpp \
            updater/updatefunctions.cpp \
            updater/tupdateruleswdg.cpp \
            updater/tupdatesummary.cpp \
          \

HEADERS  += main/tnameitem.h \
          \
            main/texamexecutor.h \
            main/texecutorsupply.h \
            main/tglobalexamstore.h \
            main/texercises.h \
            main/tequalrand.h \
            main/texammelody.h \
            main/trandmelody.h \
            main/texamview.h \
            main/tpenalty.h \
            main/ttiphandler.h \
            main/tmainscoreobject.h \
            main/texamsummary.h \
            main/tstartexamitem.h \
            main/tnootkacertificate.h \
            main/tpiechartitem.h \
          \
            dialogs/tdialogloaderobject.h \
            dialogs/tlevelcreatoritem.h \
            dialogs/tlevelpreviewitem.h \
            dialogs/tlevelsdefs.h \
            dialogs/tlevelselector.h \
            dialogs/tlevelheaderwdg.h \
            dialogs/tmelodylistview.h \
            dialogs/ttunerdialogitem.h \
            dialogs/trtmselectoritem.h \
            dialogs/tmelgenitem.h \
          \
            help/thelpdialogbase.h \
            help/texamhelp.h \
            help/tmainhelp.h \
          \
            updater/tupdatechecker.h \
            updater/updatefunctions.h \
            updater/tupdateruleswdg.h \
            updater/tupdatesummary.h \
          \



CONFIG += mobility warn_off
MOBILITY = 

android {
  QMAKE_CXXFLAGS_RELEASE += -fsigned-char
  QMAKE_CXXFLAGS_DEBUG += -fsigned-char -O1
  ANDROID_EXTRA_LIBS += $$OUT_PWD/libs/core/libNootkaCore.so\
#                         $$OUT_PWD/libs/mobile/libNootkaMobile.so\
                        $$OUT_PWD/libs/sound/libNootkaSound.so\

}

INCLUDEPATH += libs/core libs/mobile libs/sound libs/main

LIBS += -Llibs/core/ -lNootkaCore \
#         -Llibs/mobile/ -lNootkaMobile \
        -Llibs/sound -lNootkaSound \

RESOURCES += nootka.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/net/sf/nootka/TshareExam.java\
    android/net/sf/nootka/ToutVolume.java\


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# append Qt base translations from current Qt installation
TR_DIR = "$$system(dirname $$QMAKESPEC)/../translations"

lang.path = /assets/lang
lang.files += lang/nootka_cs.qm
lang.files += lang/nootka_de.qm
lang.files += lang/nootka_es.qm
lang.files += lang/nootka_fr.qm
lang.files += lang/nootka_hu.qm
lang.files += lang/nootka_it.qm
lang.files += lang/nootka_pl.qm
lang.files += lang/nootka_ru.qm
lang.files += lang/nootka_sl.qm
lang.files += lang/qtbase_sl.qm
lang.files += $$system(ls $$TR_DIR/qtbase_cs.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_de.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_es.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_fr.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_hu.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_it.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_pl.qm)
lang.files += $$system(ls $$TR_DIR/qtbase_ru.qm)

lang.depends += FORCE

INSTALLS += lang


