/*

Copyright 2013 Alexander Volkov
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

#ifndef PSMODEL_H
#define PSMODEL_H

#include <QCoreApplication>
#include <QMutex>

class QFormLayout;
class QSettings;
class QSpinBox;

struct SpectrePage;
struct SpectreDocument;
struct SpectreRenderContext;

#include "model.h"

namespace qpdfview
{

class PsPlugin;

namespace Model
{
    class PsPage final : public Page
    {
        friend class PsDocument;

    public:
        ~PsPage() final;

        QSizeF size() const final;

        QImage render(qreal horizontalResolution, qreal verticalResolution, Rotation rotation, QRect boundingRect) const final;

    private:
        Q_DISABLE_COPY(PsPage)

        PsPage(QMutex* mutex, SpectrePage* page, SpectreRenderContext* renderContext);

        mutable QMutex* m_mutex;
        SpectrePage* m_page;
        SpectreRenderContext* m_renderContext;

    };

    class PsDocument final : public Document
    {
        Q_DECLARE_TR_FUNCTIONS(Model::PsDocument)

        friend class qpdfview::PsPlugin;

    public:
        ~PsDocument() final;

        int numberOfPages() const final;

        Page* page(int index) const final;

        QStringList saveFilter() const final;

        bool canSave() const final;
        bool save(const QString& filePath, bool withChanges) const final;

        bool canBePrintedUsingCUPS() const final;

        Properties properties() const final;

    private:
        Q_DISABLE_COPY(PsDocument)

        PsDocument(SpectreDocument* document, SpectreRenderContext* renderContext);

        mutable QMutex m_mutex;
        SpectreDocument* m_document;
        SpectreRenderContext* m_renderContext;

    };
}

class PsSettingsWidget final : public SettingsWidget
{
    Q_OBJECT

public:
    explicit PsSettingsWidget(QSettings* settings, QWidget* parent = nullptr);

    void accept() final;
    void reset() final;

private:
    Q_DISABLE_COPY(PsSettingsWidget)

    QSettings* m_settings;

    QFormLayout* m_layout;

    QSpinBox* m_graphicsAntialiasBitsSpinBox;
    QSpinBox* m_textAntialiasBitsSpinBox;

};

class PsPlugin final : public QObject, Plugin
{
    Q_OBJECT
    Q_INTERFACES(qpdfview::Plugin)

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)

    Q_PLUGIN_METADATA(IID "local.qpdfview.Plugin")

#endif // QT_VERSION

public:
    explicit PsPlugin(QObject* parent = nullptr);

    DECL_NODISCARD
    Model::Document* loadDocument(const QString& filePath) const final;

    DECL_NODISCARD
    SettingsWidget* createSettingsWidget(QWidget* parent) const final;

private:
    Q_DISABLE_COPY(PsPlugin)

    QSettings* m_settings;

};

} // qpdfview

#endif // PSMODEL_H
