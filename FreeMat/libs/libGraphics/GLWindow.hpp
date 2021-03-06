/*
 * Copyright (c) 2009 Samit Basu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
  Q_OBJECT

public:
  QString name;
  double scale;

  GLWidget(QWidget *parent = 0);
  ~GLWidget();
  
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
			
protected:
  void initializeGL();
  void paintGL();
  void paintAssembly(QString aname);
  void paintNode(QString aname);
  void paintClump(QString aname);
  void paintLines(QString aname);
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  
private:
  QPoint lastPos;
  float curquat[4];
  float lastquat[4];
  int W, H;
};

 #endif
