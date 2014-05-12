#include <boost/math/constants/constants.hpp>
#include <boost/optional.hpp>
#include <QApplication>
#include <QBrush>
#include <QPen>
#include <QRect>
#include <sani/drawing.hpp>
#include <sani/interactiveanimation.hpp>
#include <sani/interactiveanimationview.hpp>
#include <sani/userinput.hpp>
#include <sfrp/behavioroperators.hpp>
#include <sfrp/behaviortimeutil.hpp>
#include <sfrp/behaviorutil.hpp>
#include <sfrp/normedvectorspaceutil.hpp>
#include <sfrp/wormhole.hpp>
#include <smisc/point1dnormedvectorspace.hpp>
#include <cstdlib>  // std::atoi
#include <cmath>    // std::cos, std::sin
#include <iostream>

void setAnimation(sani::InteractiveAnimationView& animationView,
                  sani::Animation animation) {
  animationView.setInteractiveAnimation([animation](const sani::UserInput&) {
    return animation;
  });
}

void setMousePosAnimation(sani::InteractiveAnimationView& animationView,
                          boost::function<sfrp::Behavior<sani::Drawing>(
                              sfrp::Behavior<QPointF>)> animation) {
  animationView.setInteractiveAnimation([animation](const sani::UserInput &
                                                    userInput) {
    return animation(userInput.mousePos);
  });
}

sani::Drawing circleAt(QPointF pos) {
  return sani::drawEllipse(
      QPen(), QBrush(Qt::red), QRectF(-10 + pos.x(), -10 + pos.y(), 20, 20));
}

sani::Drawing circleWithRadiusAt(double radius, QPointF pos) {
  return sani::drawEllipse(
      QPen(),
      QBrush(Qt::red),
      QRectF(-radius + pos.x(), -radius + pos.y(), radius * 2, radius * 2));
}

sani::Drawing blueCircleWithRadiusAt(double radius, QPointF pos) {
  return sani::drawEllipse(
      QPen(),
      QBrush(Qt::blue),
      QRectF(-radius + pos.x(), -radius + pos.y(), radius * 2, radius * 2));
}

static void example1() {
  const sani::Animation circleAtCenter =
      sfrp::BehaviorUtil::always(circleAt(QPointF(0.0, 0.0)));

  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setAnimation(*view, circleAtCenter);
  view->showMaximized();
}

sfrp::Behavior<sani::Drawing> spinningCircle(
    sfrp::Behavior<QPointF> mousePosition) {
  sfrp::Behavior<QPointF> spinningPoint = sfrp::BehaviorUtil::pure([](
      double time) {
    return QPointF(
        200 * std::cos(time / 4.0 * 2 * boost::math::constants::pi<double>()),
        200 * std::sin(time / 4.0 * 2 * boost::math::constants::pi<double>()));
  });
  return sfrp::BehaviorUtil::map(circleAt, spinningPoint);
}

static void example2() {
  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, spinningCircle);
  view->showMaximized();
}

sfrp::Behavior<sani::Drawing> circleFollowsMouse(
    sfrp::Behavior<QPointF> mousePosition) {
  return sfrp::BehaviorUtil::map(circleAt, mousePosition);
}

static void example3() {
  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, circleFollowsMouse);
  view->showMaximized();
}

sfrp::Behavior<QPointF> spinningPoint =
    sfrp::BehaviorUtil::pure([](double time) {
      return QPointF(
          50 * std::cos(time * 2 * boost::math::constants::pi<double>()),
          50 * std::sin(time * 2 * boost::math::constants::pi<double>()));
    });

sfrp::Behavior<sani::Drawing> spinningBallFollowsMouse(
    sfrp::Behavior<QPointF> mousePosition) {
  return sfrp::BehaviorUtil::map(circleAt, mousePosition + spinningPoint);
}

static void example4() {
  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, spinningBallFollowsMouse);
  view->showMaximized();
}

sfrp::Behavior<sani::Drawing> circleGrow(
    sfrp::Behavior<QPointF> mousePosition) {
  sfrp::Wormhole<double> circleRadiusWormhole(10);
  sfrp::Behavior<bool> inCircle =
      sfrp::BehaviorUtil::map([](QPointF pos, double radius)->bool {
                                double distToCenter = std::sqrt(
                                    pos.x() * pos.x() + pos.y() * pos.y());
                                return distToCenter < radius;
                              },
                              mousePosition,
                              circleRadiusWormhole.outputBehavior());
  sfrp::Behavior<bool> inCircleChange = sfrp::BehaviorUtil::map(
      [](std::pair<bool, bool> pair) { return pair.first != pair.second; },
      sfrp::BehaviorTimeUtil::withPriorPull(false, inCircle));
  sfrp::Behavior<double> circleRadius = circleRadiusWormhole.setInputBehavior(
      sfrp::BehaviorUtil::map([](double oldRadius, bool inCircleChange) {
                                return inCircleChange ? oldRadius + 10
                                                      : oldRadius;
                              },
                              circleRadiusWormhole.outputBehavior(),
                              inCircleChange));
  return sfrp::BehaviorUtil::map(circleWithRadiusAt,
                                 circleRadius,
                                 sfrp::BehaviorUtil::always(QPointF(0.0, 0.0)));
}

static void example5() {
  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, circleGrow);
  view->showMaximized();
}

sfrp::Behavior<sani::Drawing> circleGrow2(
    sfrp::Behavior<QPointF> mousePosition) {
  sfrp::Wormhole<double> targetRadiusWormhole(10);
  sfrp::Wormhole<double> circleRadiusWormhole(10);
  sfrp::Behavior<bool> inCircle =
      sfrp::BehaviorUtil::map([](QPointF pos, double radius)->bool {
                                double distToCenter = std::sqrt(
                                    pos.x() * pos.x() + pos.y() * pos.y());
                                return distToCenter < radius;
                              },
                              mousePosition,
                              circleRadiusWormhole.outputBehavior());
  sfrp::Behavior<bool> inCircleChange = sfrp::BehaviorUtil::map(
      [](std::pair<bool, bool> pair) { return pair.first != pair.second; },
      sfrp::BehaviorTimeUtil::withPriorPull(false, inCircle));
  sfrp::Behavior<double> targetRadius = targetRadiusWormhole.setInputBehavior(
      sfrp::BehaviorUtil::map([](double oldRadius, bool inCircleChange) {
                                return inCircleChange ? oldRadius + 10
                                                      : oldRadius;
                              },
                              targetRadiusWormhole.outputBehavior(),
                              inCircleChange));
  sfrp::Behavior<double> circleRadius =
      circleRadiusWormhole.setInputBehavior(sfrp::NormedVectorSpaceUtil::smooth(
          sfrp::BehaviorUtil::always(20.0), targetRadius));

  sfrp::Behavior<sani::Drawing> circleDrawing =
      sfrp::BehaviorUtil::map(circleWithRadiusAt,
                              circleRadius,
                              sfrp::BehaviorUtil::always(QPointF(0.0, 0.0)));

  sfrp::Behavior<sani::Drawing> targetRadiusDrawing =
      sfrp::BehaviorUtil::map(blueCircleWithRadiusAt,
                              targetRadius,
                              sfrp::BehaviorUtil::always(QPointF(0.0, 0.0)));
  return sfrp::BehaviorUtil::map(
      sani::drawOver, circleDrawing, targetRadiusDrawing);
}
static void example6() {
  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, circleGrow2);
  view->showMaximized();
}

// Parse the specified arguments (as passed to main) for a selected example
// number. Return the example number (within [1..]) parsed or 'boost::none' if
// there was a problem parsing the arguments.
boost::optional<int> parseArgs(int argc, char** argv) {
  if (argc != 2)
    return boost::none;
  else {
    int parsedIntArg = std::atoi(argv[1]);
    if (parsedIntArg >= 1)
      return boost::make_optional(parsedIntArg);
    else
      return boost::none;
  }
}

int main(int argc, char** argv) {

  boost::optional<int> chosenExample = parseArgs(argc, argv);

  if (chosenExample) {
    QApplication app(argc, argv);

    switch (*chosenExample) {
      case 1:
        example1();
        break;
      case 2:
        example2();
        break;
      case 3:
        example3();
        break;
      case 4:
        example4();
        break;
      case 5:
        example5();
        break;
      case 6:
        example6();
        break;
      default:
        example1();
        break;
    }
    return app.exec();
  } else {
    std::cerr << "Usage: examples <ExampleNum>\n"
              << "  where <ExampleNum> is a number 1 or greater.\n";
  }
}
