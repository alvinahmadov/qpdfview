//
// Created by alvin on 27/05/23.
//

#ifndef QPDFVIEW_COLORWIDGET_H
#define QPDFVIEW_COLORWIDGET_H

#include "macros.h"

#include <QWidget>

class QColorDialog;
class QComboBox;
class QFormLayout;
class QPushButton;

namespace qpdfview
{

class ColorWidget : public QWidget
{
    Q_OBJECT

private:
    explicit ColorWidget(QWidget* parent = nullptr);

public:
    explicit ColorWidget(QFormLayout* layout, const QString& label, const QColor& color,
                         const QString& toolTip = {}, QWidget* parent = nullptr);

    ~ColorWidget() override;

    DECL_NODISCARD
    QColor color(const QColor& defaultColor) const;
    void setColor(const QColor& color);

protected slots:
    void onColorSelected(const QColor& color);
    void onCurrentTextChanged(const QString& color);

protected:
    void setButtonColor(const QColor& color);

private:
    QComboBox* m_colorComboBox;
    QColorDialog* m_colorDialog;
    QPushButton* m_colorDialogButton;
};

}   // qpdfview

#endif   // QPDFVIEW_COLORWIDGET_H
