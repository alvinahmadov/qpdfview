/*

Copyright 2018 S. Razi Alavizadeh
Copyright 2014, 2018 Adam Reichold

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

#ifndef FITZMODEL_H
#define FITZMODEL_H

#include <QMutex>

class QSpinBox;
class QComboBox;
class QFormLayout;
class QSettings;

extern "C"
{

#include <mupdf/fitz/context.h>

typedef struct fz_page_s fz_page;
typedef struct fz_document_s fz_document;

}

#include "model.h"

namespace qpdfview
{

class FitzPlugin;

namespace Model
{
    class FitzPage final : public Page
    {
        friend class FitzDocument;

    public:
        ~FitzPage() final;

        DECL_NODISCARD
        QSizeF size() const final;

        DECL_NODISCARD
        QImage render(qreal horizontalResolution, qreal verticalResolution, Rotation rotation, QRect boundingRect) const final;

        DECL_NODISCARD
        QList< Link* > links() const final;

        DECL_NODISCARD
        QString text(const QRectF& rect) const final;

        DECL_NODISCARD
        QList< QRectF > search(const QString& text, bool matchCase, bool wholeWords) const final;

    private:
        Q_DISABLE_COPY(FitzPage)

        FitzPage(const class FitzDocument* parent, fz_page* page);

        const class FitzDocument* m_parent;

        fz_page* m_page;
        const fz_rect m_boundingRect;

        struct DisplayList;
        mutable DisplayList* m_displayList;

    };

    class FitzDocument final : public Document
    {
        friend class FitzPage;
        friend class qpdfview::FitzPlugin;

    public:
        ~FitzDocument() final;

        DECL_NODISCARD
        int numberOfPages() const final;

        DECL_NODISCARD
        Page* page(int index) const final;

        DECL_NODISCARD
        bool canBePrintedUsingCUPS() const final;

        void setPaperColor(const QColor& paperColor) final;

        DECL_NODISCARD
        Outline outline() const final;

    private:
        Q_DISABLE_COPY(FitzDocument)

        FitzDocument(fz_context* context, fz_document* document);

        mutable QMutex m_mutex;
        fz_context* m_context;
        fz_document* m_document;

        QColor m_paperColor;

    };
}

class FitzSettingsWidget final : public SettingsWidget
{
Q_OBJECT

public:
	explicit FitzSettingsWidget(QSettings* settings, QWidget* parent = nullptr);

	void accept() final;

	void reset() final;

private:
	Q_DISABLE_COPY(FitzSettingsWidget)

	QSettings* m_settings;

	QFormLayout* m_layout;

	QComboBox* m_defaultFontComboBox;

	QSpinBox* m_defaultFontSizeSpinBox;
	QSpinBox* m_monospaceFontSizeSpinBox;
	QSpinBox* m_horizontalMarginSpinBox;
	QSpinBox* m_verticalMarginSpinBox;
};

class FitzPlugin final : public QObject, Plugin
{
    Q_OBJECT
    Q_INTERFACES(qpdfview::Plugin)

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    Q_PLUGIN_METADATA(IID "local.qpdfview.Plugin")

#endif // QT_VERSION

public:
    explicit FitzPlugin(QObject* parent = nullptr);
    ~FitzPlugin() final;

    DECL_NODISCARD
    Model::Document* loadDocument(const QString& filePath) const final;

    DECL_NODISCARD
    SettingsWidget* createSettingsWidget(QWidget* parent) const final;

private:
	Q_DISABLE_COPY(FitzPlugin)

	QSettings* m_settings;
    QMutex m_mutex[FZ_LOCK_MAX];
    fz_locks_context m_locksContext;
    fz_context* m_context;

    static void lock(void* user, int lock);
    static void unlock(void* user, int lock);

};

} // qpdfview

#endif // FITZMODEL_H
