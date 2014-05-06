#include <QApplication>
#include <QBrush>
#include <QPen>
#include <QRect>
#include <sani/drawing.hpp>
#include <sani/interactiveanimation.hpp>
#include <sani/interactiveanimationview.hpp>
#include <sani/userinput.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/util.hpp>
#include <iostream>

void setAnimation(sani::InteractiveAnimationView &animationView,
                  sani::Animation animation) {
  animationView.setInteractiveAnimation([animation](const sani::UserInput &) {
    return animation;
  });
}

void setMousePosAnimation(sani::InteractiveAnimationView &animationView,
                          boost::function<sfrp::Behavior<sani::Drawing>(
                              sfrp::Behavior<QPointF>)> animation) {
  animationView.setInteractiveAnimation([animation](const sani::UserInput &
                                                    userInput) {
    return animation(userInput.mousePos);
  });
}

sani::Drawing circleAt(QPointF pos) {
  return sani::drawEllipse(QPen(), QBrush(Qt::red),
                           QRectF(-10 + pos.x(), -10 + pos.y(), 20, 20));
}

static void example1() {
  const sani::Animation circleAtCenter =
      sfrp::pmConst(circleAt(QPointF(0.0, 0.0)));

  sani::InteractiveAnimationView * const view = new sani::InteractiveAnimationView();
  setAnimation(*view, circleAtCenter);
  view->show();
}

sfrp::Behavior<sani::Drawing>
circleFollowsMouse(sfrp::Behavior<QPointF> mousePosition) {
  return sfrp::BehaviorMap()(circleAt, mousePosition);
}

static void example2() {
  sani::InteractiveAnimationView * const view = new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, circleFollowsMouse);
  view->show();
}

int main(int argc, char **argv) {
  QApplication app(argc, argv);

//  example1();
  example2();
  return app.exec();
}
