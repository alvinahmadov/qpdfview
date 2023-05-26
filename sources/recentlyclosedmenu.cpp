/*

Copyright 2013 Adam Reichold

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

#include "recentlyclosedmenu.h"

#include "documentview.h"

namespace qpdfview
{

RecentlyClosedMenu::RecentlyClosedMenu(int count, QWidget* parent) : ToolTipMenu(parent),
    m_count(count)
{
    menuAction()->setText(tr("&Recently closed"));

    m_tabActionGroup = new QActionGroup(this);
    connect(m_tabActionGroup, SIGNAL(triggered(QAction*)), SLOT(onTabActionTriggered(QAction*)));

    m_separatorAction = addSeparator();

    m_clearListAction = addAction(tr("&Clear list"));
    connect(m_clearListAction, SIGNAL(triggered()), SLOT(onClearListTriggered()));
}

void RecentlyClosedMenu::addTabAction(QAction* tabAction)
{
    if(m_tabActionGroup->actions().count() >= m_count)
    {
        QAction* first = m_tabActionGroup->actions().at(0);

        removeAction(first);
        m_tabActionGroup->removeAction(first);

        first->parent()->deleteLater();
    }

    insertAction(actions().at(0), tabAction);
    m_tabActionGroup->addAction(tabAction);
}

DECL_UNUSED
void RecentlyClosedMenu::triggerFirstTabAction()
{
    const QList< QAction* >& actions = m_tabActionGroup->actions();

    if(!actions.isEmpty())
    {
        onTabActionTriggered(actions.first());
    }
}

void RecentlyClosedMenu::triggerLastTabAction()
{
    const QList< QAction* >& actions = m_tabActionGroup->actions();

    if(!actions.isEmpty())
    {
        onTabActionTriggered(actions.last());
    }
}

void RecentlyClosedMenu::onTabActionTriggered(QAction* tabAction)
{
    removeAction(tabAction);
    m_tabActionGroup->removeAction(tabAction);

    emit tabActionTriggered(tabAction);
}

void RecentlyClosedMenu::onClearListTriggered()
{
    foreach(QAction* action, m_tabActionGroup->actions())
    {
        action->parent()->deleteLater();
    }
}

} // qpdfview
