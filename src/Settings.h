#ifndef SETTING_H
#define SETTING_H
#include <algorithm>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <vector>

struct Element {
  std::string name;
  double mass;
  double position[3];
  double velocity[3];
};

struct RandomSettings {
  bool active;
  int nElements;
  double minMass;
  double maxMass;
  double minPosition[3];
  double maxPosition[3];
  double minVelocity[3];
  double maxVelocity[3];
};

enum OutputFormat { CSV_FORMAT };

class Settings {
public:
  Settings();
  int load(const char *file_path);
  std::string getOutPath();
  double getWidth();
  double getHeight();
  double getDepth();
  double getDeltaT();
  bool getVerbose();
  RandomSettings getRandomSettings();
  int getWriteSteps();
  int getNElem();
  int getSteps();
  std::vector<Element> getElements();
  ~Settings() = default;

private:
  std::string output;
  bool verbose;
  OutputFormat outputFormat;
  double width;
  double height;
  double depth;
  double deltaT;
  RandomSettings randomSettings;
  int writeSteps;
  int nElem;
  int steps;
  std::vector<Element> elements;
};

#endif
