#include "scenes.h"

#include <Inventor/SoPath.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Quarter/Quarter.h>
#include <Quarter/QuarterWidget.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include <QVBoxLayout>

using namespace SIM::Coin3D::Quarter;

namespace {

class PickViewer : public QuarterWidget {
public:
  PickViewer(QLabel *resultLabel, QWidget *parent)
    : QuarterWidget(parent), result(resultLabel), marker(new QWidget(this))
  {
    marker->setFixedSize(18, 18);
    marker->setAttribute(Qt::WA_TransparentForMouseEvents);
    marker->setStyleSheet("border: 2px solid white; border-radius: 9px;");
    marker->hide();
  }

  bool pick(const SbVec2s &position)
  {
    const auto *manager = getSoRenderManager();
    SoRayPickAction action(manager->getViewportRegion());
    action.setPoint(position);
    action.setRadius(8.0f);
    action.apply(manager->getSceneGraph());
    const auto *point = action.getPickedPoint();
    if (!point) {
      result->setText("No object at this point");
      marker->hide();
      return false;
    }

    const auto *path = point->getPath();
    result->setText(QString("Picked %1 · %2 nodes in path")
                      .arg(path->getTail()->getTypeId().getName().getString())
                      .arg(path->getLength()));
    const auto scale = devicePixelRatioF();
    marker->move(qRound(position[0] / scale) - marker->width() / 2,
                 height() - 1 - qRound(position[1] / scale) - marker->height() / 2);
    marker->show();
    return true;
  }

  bool processSoEvent(const SoEvent *event) override
  {
    if (SO_MOUSE_PRESS_EVENT(event, BUTTON1)) {
      pick(event->getPosition(getSoRenderManager()->getViewportRegion()));
      return true;
    }
    return QuarterWidget::processSoEvent(event);
  }

private:
  QLabel *result;
  QWidget *marker;
};

int showExample(QApplication &app, const ExampleScene &example, SoSeparator *scene,
                const QString &output, const QString &pickCoordinates)
{
  QWidget window;
  window.setWindowTitle(example.title);
  window.setFixedSize(640, 480);
  auto *layout = new QVBoxLayout(&window);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  auto *header = new QWidget(&window);
  header->setFixedHeight(72);
  header->setObjectName("sceneHeader");
  header->setStyleSheet(
      "#sceneHeader { background: #f8fafc; }"
      "QLabel { color: #14253a; }"
      "QLabel#subtitle { color: #4c5a6b; }"
      "QPushButton { color: #14253a; background: white; border: 1px solid #b9c6d3;"
      " border-radius: 5px; padding: 6px 12px; }"
      "QPushButton:hover { background: #edf3fa; }");
  auto *headerLayout = new QHBoxLayout(header);
  headerLayout->setContentsMargins(20, 12, 20, 12);
  auto *labels = new QVBoxLayout();
  labels->setSpacing(3);
  auto *title = new QLabel(example.title, header);
  title->setFont(QFont("DejaVu Sans", 15, QFont::DemiBold));
  labels->addWidget(title);
  auto *subtitle = new QLabel(example.description, header);
  subtitle->setObjectName("subtitle");
  labels->addWidget(subtitle);
  headerLayout->addLayout(labels, 1);
  auto *fitView = new QPushButton("Fit view", header);
  headerLayout->addWidget(fitView);
  layout->addWidget(header);

  const QLatin1String name(example.name);
  auto *pickViewer = name == "pick-action" ? new PickViewer(subtitle, &window) : nullptr;
  auto *viewer = pickViewer ? pickViewer : new QuarterWidget(&window);
  viewer->setNavigationModeFile();
  if (name == "toolmaker-nodes") {
    viewer->setTransparencyType(QuarterWidget::SORTED_OBJECT_BLEND);
  }
  viewer->setBackgroundColor(QColor("#172334"));
  viewer->setSceneGraph(scene);
  layout->addWidget(viewer, 1);
  QObject::connect(fitView, &QPushButton::clicked, viewer, &QuarterWidget::viewAll);

  window.show();
  viewer->setFocus();
  if (!output.isEmpty()) {
    QTimer::singleShot(10000, &window, [&app] {
      qCritical("Timed out waiting for the OpenGL frame");
      app.exit(1);
    });
  }
  QTimer::singleShot(0, &window, [&] {
    viewer->viewAll();
    if (!pickCoordinates.isEmpty()) {
      const auto coordinates = pickCoordinates.split(',');
      bool validX = false;
      bool validY = false;
      const int x = coordinates.value(0).toInt(&validX);
      const int y = coordinates.value(1).toInt(&validY);
      if (!pickViewer || coordinates.size() != 2 || !validX || !validY ||
          x < 0 || x >= viewer->width() || y < 0 || y >= viewer->height() ||
          !pickViewer->pick(SbVec2s(qRound(x * viewer->devicePixelRatioF()),
                                   qRound((viewer->height() - 1 - y) * viewer->devicePixelRatioF())))) {
        qCritical("The requested viewport position did not pick an object");
        app.exit(1);
        return;
      }
    }
    if (!output.isEmpty()) {
      QObject::connect(viewer, &QOpenGLWidget::frameSwapped, &window, [&] {
        const QPixmap capture = window.screen()->grabWindow(window.winId());
        if (capture.size() != window.size() || !capture.save(output, "PNG")) {
          qCritical("Could not save a 640 × 480 PNG");
          app.exit(1);
        } else {
          app.quit();
        }
      }, Qt::SingleShotConnection);
      viewer->update();
    }
  });
  return app.exec();
}

}

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QApplication::setStyle("Fusion");
  app.setFont(QFont("DejaVu Sans", 10));

  QCommandLineParser parser;
  parser.setApplicationDescription("View and capture classic Coin examples");
  parser.addHelpOption();
  parser.addPositionalArgument("example", "Example name");
  parser.addPositionalArgument("ivexamples-dir", "Path to the ivexamples checkout");
  QCommandLineOption output("output", "Save the window to a PNG and exit", "path");
  QCommandLineOption pick("pick", "Pick at x,y measured from the top left of the viewport", "x,y");
  parser.addOptions({output, pick});
  parser.process(app);
  const auto arguments = parser.positionalArguments();
  if (arguments.size() != 2) parser.showHelp(1);
  const auto *example = findExample(arguments.at(0));
  if (!example) {
    qCritical("Unknown example: %s", qPrintable(arguments.at(0)));
    return 1;
  }

  Quarter::init();
  auto *scene = example->load(arguments.at(1));
  int result = 1;
  if (scene) {
    scene->ref();
    result = showExample(app, *example, scene, parser.value(output), parser.value(pick));
    scene->unref();
  } else {
    qCritical("Could not read the example data");
  }
  Quarter::clean();
  return result;
}
