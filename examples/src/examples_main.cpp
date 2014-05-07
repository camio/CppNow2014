#include <boost/optional.hpp>
#include <QApplication>
#include <QBrush>
#include <QPen>
#include <QRect>
#include <sani/drawing.hpp>
#include <sani/interactiveanimation.hpp>
#include <sani/interactiveanimationview.hpp>
#include <sani/userinput.hpp>
#include <sfrp/behaviormap.hpp>
#include <sfrp/behaviorutil.hpp>
#include <cstdlib>  // std::atoi
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

static void example1() {
  const sani::Animation circleAtCenter =
      sfrp::BehaviorUtil::always(circleAt(QPointF(0.0, 0.0)));

  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setAnimation(*view, circleAtCenter);
  view->show();
}

sfrp::Behavior<sani::Drawing> circleFollowsMouse(
    sfrp::Behavior<QPointF> mousePosition) {
  return sfrp::BehaviorMap()(circleAt, mousePosition);
}

static void example2() {
  sani::InteractiveAnimationView* const view =
      new sani::InteractiveAnimationView();
  setMousePosAnimation(*view, circleFollowsMouse);
  view->show();
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
