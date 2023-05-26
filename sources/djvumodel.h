/*

Copyright 2013 Adam Reichold
Copyright 2013 Alexander Volkov

This file is part of qpdfview.

The implementation is based on KDjVu by Pino Toscano.

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

#ifndef DJVUMODEL_H
#define DJVUMODEL_H

#include <QHash>
#include <QMutex>

typedef struct ddjvu_context_s ddjvu_context_t;
typedef struct ddjvu_format_s ddjvu_format_t;
typedef struct ddjvu_document_s ddjvu_document_t;
typedef struct ddjvu_pageinfo_s ddjvu_pageinfo_t;

#include "model.h"

namespace qpdfview
{

class DjVuPlugin;

namespace Model
{
    class DjVuPage final : public Page
    {
        friend class DjVuDocument;

    public:
        ~DjVuPage() final = default;

        DECL_NODISCARD
        QSizeF size() const final;

        DECL_NODISCARD
        QImage render(qreal horizontalResolution, qreal verticalResolution, Rotation rotation, QRect boundingRect) const final;

        DECL_NODISCARD
        QString label() const final;

        DECL_NODISCARD
        QList< Link* > links() const final;

        DECL_NODISCARD
        QString text(const QRectF& rect) const final;

        DECL_NODISCARD
        QList< QRectF > search(const QString& text, bool matchCase, bool wholeWords) const final;

    private:
        Q_DISABLE_COPY(DjVuPage)

        DjVuPage(const class DjVuDocument* parent, int index, const ddjvu_pageinfo_t& pageinfo);

        const class DjVuDocument* m_parent;

        int m_index;
        QSizeF m_size;
        int m_resolution;

    };

    class DjVuDocument final : public Document
    {
        friend class DjVuPage;
        friend class qpdfview::DjVuPlugin;

    public:
        ~DjVuDocument() final;

        int numberOfPages() const final;

        Page* page(int index) const final;

        QStringList saveFilter() const final;

        bool canSave() const final;
        bool save(const QString& filePath, bool withChanges) const final;

        Outline outline() const final;
        Properties properties() const final;

    private:
        Q_DISABLE_COPY(DjVuDocument)

        DjVuDocument(QMutex* globalMutex, ddjvu_context_t* context, ddjvu_document_t* document);

        mutable QMutex m_mutex;
        mutable QMutex* m_globalMutex;

        ddjvu_context_t* m_context;
        ddjvu_document_t* m_document;
        ddjvu_format_t* m_format;

        QHash< QString, int > m_pageByName;
        QHash< int, QString > m_titleByIndex;

        void prepareFileInfo();

    };
}

class DjVuPlugin final : public QObject, Plugin
{
    Q_OBJECT
    Q_INTERFACES(qpdfview::Plugin)

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    Q_PLUGIN_METADATA(IID "local.qpdfview.Plugin")

#endif // QT_VERSION

public:
    explicit DjVuPlugin(QObject* parent = nullptr);

    Model::Document* loadDocument(const QString& filePath) const final;

private:
    mutable QMutex m_globalMutex;

};

} // qpdfview

#endif // DJVUMODEL_H
