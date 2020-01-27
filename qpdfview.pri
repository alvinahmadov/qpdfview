isEmpty(APPLICATION_VERSION):APPLICATION_VERSION = 0.4.18
PREFIX = /usr/local
isEmpty(TARGET_INSTALL_PATH):TARGET_INSTALL_PATH = $${PREFIX}/bin
isEmpty(PLUGIN_INSTALL_PATH):PLUGIN_INSTALL_PATH = $${PREFIX}/lib/qpdfview
isEmpty(DATA_INSTALL_PATH):DATA_INSTALL_PATH = $${PREFIX}/share/qpdfview
isEmpty(MANUAL_INSTALL_PATH):MANUAL_INSTALL_PATH = $${PREFIX}/share/man/man1
isEmpty(ICON_INSTALL_PATH):ICON_INSTALL_PATH = $${PREFIX}/share/icons/hicolor/scalable/apps
isEmpty(LAUNCHER_INSTALL_PATH):LAUNCHER_INSTALL_PATH = $${PREFIX}/share/applications
isEmpty(APPDATA_INSTALL_PATH):APPDATA_INSTALL_PATH = $${PREFIX}/share/appdata

win32:include(qpdfview_win32.pri)
os2:include(qpdfview_os2.pri)
