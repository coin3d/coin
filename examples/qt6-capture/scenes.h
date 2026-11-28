#pragma once

#include <QString>

class SoSeparator;

struct ExampleScene {
  const char *name;
  const char *title;
  const char *description;
  SoSeparator *(*load)(const QString &);
};

SoSeparator *readScene(const QString &path);
const ExampleScene *findExample(const QString &name);
