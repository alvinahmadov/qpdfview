/*
Copyright 2020 vit9696
This file is part of qpdfview.
qpdfview is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
qpdfview is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with qpdfview.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QFileOpenEvent>

#include "application.h"
#include "mainwindow.h"

namespace qpdfview {

Application::Application(int& argc, char** argv) : QApplication(argc, argv),
      m_window(0)
{
    setOrganizationDomain("local.qpdfview");
    setOrganizationName("qpdfview");
    setApplicationName("qpdfview");

    setApplicationVersion(APPLICATION_VERSION);

#ifdef Q_OS_MAC

    // On macOS menu icons should not be shown, and app icons are determined by .app bundle.
    setAttribute(Qt::AA_DontShowIconsInMenus);

#else

    setWindowIcon(QIcon(":icons/qpdfview"));

#endif // Q_OS_MAC

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    setAttribute(Qt::AA_UseHighDpiPixmaps);

#endif // QT_VERSION
}

void Application::setMainWindow(MainWindow* window)
{
    m_window = window;
}

bool Application::event(QEvent *event)
{
    if(event->type() == QEvent::FileOpen)
    {
        // On macOS and potentially other systems this handles "Open With" and "Drag & Drop" after exec().
        auto openEvent = dynamic_cast< QFileOpenEvent * >(event);
        m_window->jumpToPageOrOpenInNewTab(openEvent->file(), -1);
    }

    return QApplication::event(event);
}

} // qpdfview