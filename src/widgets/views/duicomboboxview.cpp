/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "duicomboboxview.h"
#include "duicomboboxview_p.h"
#include "duicombobox.h"

#include "duitheme.h"
#include "duiimagewidget.h"
#include "duilabel.h"
#include "duiscalableimage.h"
#include "duiviewcreator.h"
#include "duipopuplist.h"
#include "duiapplicationwindow.h"
#include "duiscenemanager.h"
#include "duilocale.h"
#include "duicontentitem.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>

DuiComboBoxViewPrivate::DuiComboBoxViewPrivate()
    : controller(0), contentItem(0), popuplist(0)
{
}

DuiComboBoxViewPrivate::~DuiComboBoxViewPrivate()
{
    if (popuplist != 0)
        delete popuplist;
}

void DuiComboBoxViewPrivate::init()
{
    Q_Q(DuiComboBoxView);

    layout = new QGraphicsLinearLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    contentItem = new DuiContentItem(DuiContentItem::TwoTextLabels);
    layout->addItem(contentItem);
    updateSubtitle(controller->currentIndex());

    QObject::connect(contentItem, SIGNAL(clicked()), controller, SLOT(click()));
    QObject::connect(controller, SIGNAL(currentIndexChanged(int)), q, SLOT(_q_itemModelCurrentIndexChanged(int)));
    QObject::connect(controller, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
    QObject::connect(controller, SIGNAL(itemModelChanged(QAbstractItemModel *)),
                     q, SLOT(setItemModel(QAbstractItemModel *)));
    QObject::connect(controller, SIGNAL(selectionModelChanged(QItemSelectionModel *)),
                     q, SLOT(setSelectionModel(QItemSelectionModel *)));
}

void DuiComboBoxViewPrivate::initLayout()
{
    Q_Q(DuiComboBoxView);

    DuiContentItem::ContentItemStyle newStyle;;

    if (controller->isIconVisible() && !controller->iconID().isEmpty())
        newStyle = DuiContentItem::IconAndTwoTextLabels;
    else
        newStyle = DuiContentItem::TwoTextLabels;

    if (contentItem->itemStyle() != newStyle) {
        delete contentItem;

        contentItem = new DuiContentItem(newStyle);
        contentItem->setObjectName(q->style()->contentItemObjectName());
        contentItem->setItemMode((DuiContentItem::ContentItemMode)q->style()->itemMode());
        layout->addItem(contentItem);
        updateSubtitle(controller->currentIndex());
        contentItem->setTitle(controller->title());
        if (newStyle == DuiContentItem::IconAndTwoTextLabels)
            contentItem->setPixmap(*DuiTheme::pixmap(controller->iconID()));

        QObject::connect(contentItem, SIGNAL(clicked()), controller, SLOT(click()));
    }
}

void DuiComboBoxViewPrivate::updateSubtitle(int currentIndex) {
    if (currentIndex != -1)  {
        contentItem->setSubtitle(controller->itemText(currentIndex));
    } else {
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.doc
        //: default value for Popup List button sublabel when nothing has been selected yet
        //% "Tap to Select"
        contentItem->setSubtitle(qtTrId("xx_ComboBoxSubtitle"));
    }
}

void DuiComboBoxViewPrivate::_q_itemModelCurrentIndexChanged(int currentIndex)
{
    updateSubtitle(currentIndex);
}

void DuiComboBoxViewPrivate::_q_showPopup()
{
    if (controller->count() == 0) return;

    if (controller->sceneManager()) {
        if (!popuplist) {
            popuplist = new DuiPopupList();
            popuplist->setItemModel(controller->itemModel());
            popuplist->setSelectionModel(controller->selectionModel());
        }

        popuplist->setTitle(controller->title());
        controller->sceneManager()->showWindow(popuplist);
    }
}

DuiComboBoxView::DuiComboBoxView(DuiComboBox *controller) :
    DuiWidgetView(* new DuiComboBoxViewPrivate, controller)
{
    Q_D(DuiComboBoxView);
    d->controller = controller;
    d->init();

}

DuiComboBoxView::DuiComboBoxView(DuiComboBoxViewPrivate &dd, DuiComboBox *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiComboBoxView);
    d->controller = controller;
    d->init();
}

DuiComboBoxView::~DuiComboBoxView()
{
}

void DuiComboBoxView::applyStyle()
{
    Q_D(DuiComboBoxView);

    DuiWidgetView::applyStyle();

    d->contentItem->setObjectName(style()->contentItemObjectName());
    d->contentItem->setItemMode((DuiContentItem::ContentItemMode)style()->itemMode());
}

void DuiComboBoxView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiComboBoxView);

    if (d->controller->isDown())
        return;

    d->controller->setDown(true);
}

void DuiComboBoxView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiComboBoxView);

    if (!d->controller->isDown())
        return;

    d->controller->setDown(false);

    QPointF touch = event->scenePos();
    QRectF rect  = d->controller->sceneBoundingRect();
    if (rect.contains(touch)) {
        d->controller->click();
    }
}

void DuiComboBoxView::cancelEvent(DuiCancelEvent *event)
{
    Q_D(DuiComboBoxView);
    Q_UNUSED(event);

    if (!d->controller->isDown())
        return;

    d->controller->setDown(false);

    update();
}

void DuiComboBoxView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiComboBoxView);

    DuiWidgetView::updateData(modifications);

    const char *member;
    const int count = modifications.count();
    for (int i = 0; i < count; ++i) {
        member = modifications[i];

        if (member == DuiComboBoxModel::IconID || member == DuiComboBoxModel::IconVisible) {
            d->initLayout();
        } else if (member == DuiComboBoxModel::Title) {
            QString text = model()->title();
            d->contentItem->setTitle(text);
            if (d->popuplist)
                d->popuplist->setTitle(text);
        } else if (member == DuiComboBoxModel::Down) {
            if (model()->down())
                style().setModePressed();
            else
                style().setModeDefault();
            update();
        }
    }
}

void DuiComboBoxView::setupModel()
{
    Q_D(DuiComboBoxView);
    DuiWidgetView::setupModel();

    d->initLayout();

    d->contentItem->setTitle(model()->title());
}

void DuiComboBoxView::setItemModel(QAbstractItemModel *itemModel)
{
    Q_D(DuiComboBoxView);
    if(d->popuplist)
        d->popuplist->setItemModel(itemModel);
}

void DuiComboBoxView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    Q_D(DuiComboBoxView);
    if(d->popuplist)
        d->popuplist->setSelectionModel(selectionModel);
}


DUI_REGISTER_VIEW_NEW(DuiComboBoxView, DuiComboBox)

#include "moc_duicomboboxview.cpp"
