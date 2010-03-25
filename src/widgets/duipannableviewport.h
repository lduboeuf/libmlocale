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

#ifndef DUIPANNABLEVIEWPORT_H
#define DUIPANNABLEVIEWPORT_H

#include "duipannablewidget.h"
#include <duipannableviewportmodel.h>

class QGraphicsWidget;
class DuiPannableViewportPrivate;
class DuiPositionIndicator;

/*!
 * \class DuiPannableViewport
 * \brief DuiPannableViewport is a viewport widget which can be used
 * to make any widget pannable.
 *
 * DuiPannableViewport contains a widget (set with setWidget()) and
 * displays it inside the viewport. The area is monitored for panning
 * gestures and when one is detected, the widget is scrolled in the
 * viewport. Other mouse events are forwarded to the panned widget.
 *
 * sizePosChanged() signal is emitted when the size of the viewport,
 * the the panned range or the position of the panning changes. It can
 * be used to track the currently visible area of the widget.
 *
 * Viewport has an autoRange feature which sets the panning range
 * based on the preferred size of the panned widget. This can be
 * turned off after which the method setRange() can be used to set the
 * panning range.
 *
 * The viewport creates a position indicator widget on top of it which
 * shows the panning position.
 *
 * By default, the viewport is created with minimum size of 0x0 pixels
 * and preferred size of 1x1 pixels. In order to show the contents of
 * the viewport it needs to be put into layout which will extend it
 * or minimum size of it needs to be increased. This can be achived by
 * styling the widget or as in line 2 of the example.
 *
 * Example:
 *
 * \code
 *     DuiPannableViewport* viewport = new DuiPannableViewport(parent);
 *     viewport->setMinimumSize(100,100);
 *     DuiImage* image = new DuiImage("myimage.png");
 *     viewport->setWidget(image);
 * \endcode
 *
 * \sa DuiPannableWidget DuiPannableWidgetStyle
 *
 */
class DUI_EXPORT DuiPannableViewport : public DuiPannableWidget
{
    Q_OBJECT
    DUI_CONTROLLER(DuiPannableViewport)

    //! \brief Enable status of autoRange feature
    Q_PROPERTY(bool autoRange READ autoRange WRITE setAutoRange)
    //! \brief Current panning range
    Q_PROPERTY(QRectF range READ range WRITE setRange)
    //! \brief Current panned widget
    Q_PROPERTY(QGraphicsWidget *widget READ widget WRITE setWidget)
    //! \brief Clipping status of the panned widget
    Q_PROPERTY(bool clipping READ hasClipping WRITE setClipping)

public:
    /*!
     * \brief Constructs a pannable viewport with a \a parent.
     */
    DuiPannableViewport(QGraphicsItem *parent = 0);

    /*!
     * \brief Destroys the pannable widget.
     */
    virtual ~DuiPannableViewport();

    /*!
     * \brief Sets the state of the autoRange feature.
     *
     * When AutoRange is set on, the panning range is automatically
     * determined based on the size() of the panned widget.
     */
    void setAutoRange(bool enable);

    /*!
     * \brief Returns the state of the autoRange feature.
     */
    bool autoRange() const;

    /*!
     * \brief Sets the clipping state of the panned widget.
     *
     * Normally this needs to be true. It can be set to false if the
     * pannable viewport occupies the whole screen in which case the
     * clipping of the panned widget to viewport is redundant.
     */
    void setClipping(bool enabled);

    /*!
     * \brief Returns the clipping state.
     */
    bool hasClipping() const;

    /*!
     * \brief Sets the \a widget which will be displayed and panned in
     * the viewport.
     *
     * Ownership of the widget is transferred to pannable viewport.
     */
    void setWidget(QGraphicsWidget *widget);

    /*!
     * \brief Returns the widget which is currently panned.
     */
    QGraphicsWidget *widget() const;

    /*!
     * \brief If autoRange is disabled, manually sets the range of the
     * panning.
     *
     * This method overrides the method in DuiPannableWidget. Note
     * that in DuiPannableViewport the meaning of the range is
     * different from DuiPannableWidget (range is not the range of the
     * physics (i.e. allowed movement) but it is the range of the area
     * that can be panned over).
     */
    void setRange(const QRectF &range);

    /*!
     * \brief Returns the current panning range.
     */
    virtual QRectF range() const;

    //! \reimp
    void setPanDirection(const Qt::Orientations &panDirection);
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void updateGeometry();
    //! \reimp_end

Q_SIGNALS:

    /*!
     * \brief When viewport size changes this signal is emitted.
     */
    void viewportSizeChanged(const QSizeF &viewportSize);

public Q_SLOTS:

    /*!
     * \brief Receives the position updates from the physics and moves
     * the panned widget based on them.
     *
     * This slot should only be used by physics engine.
     */
    virtual void updatePosition(const QPointF &position);

private:
    Q_DISABLE_COPY(DuiPannableViewport)
    Q_DECLARE_PRIVATE(DuiPannableViewport)
    Q_PRIVATE_SLOT(d_func(), void _q_resolvePannedWidgetIsOnDisplay())

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_DuiPannableViewport;
#endif

};

#endif
