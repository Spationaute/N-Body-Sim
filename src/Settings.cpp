#include "Settings.h"

Settings::Settings() {
  // Set default Values
  this->width = 300;
  this->height = 300;
  this->depth = 50;
  this->nElem = 0;
  this->steps = 2000;
  this->writeSteps = 10;
  this->deltaT = 1;
  this->elements.clear();
  this->output = "results";
  this->randomSettings.active = 0;
  this->outputFormat = CSV_FORMAT;
}

int Settings::load(const char *file_path) {
  std::ifstream file(file_path);
  if (file.is_open()) {
    Json::Value root;
    file >> root;
    try {
      Json::Value settings = root["settings"];
      this->width = settings["width"].asDouble();
      this->height = settings["height"].asDouble();
      this->depth = settings["depth"].asDouble();
      this->steps = settings["steps"].asInt();
      this->writeSteps = settings["writeSteps"].asInt();
      this->deltaT = settings["deltaT"].asDouble();
      this->verbose = settings["verbose"].asBool();

      Json::Value rl = root["random-elements"];
      this->randomSettings.active = rl["active"].asBool();
      this->randomSettings.nElements = rl["nElements"].asInt();
      this->randomSettings.minMass = rl["mass"][0].asDouble();
      this->randomSettings.maxMass = rl["mass"][1].asDouble();
      this->randomSettings.minPosition[0] = rl["position"][0][0].asDouble();
      this->randomSettings.minPosition[1] = rl["position"][1][0].asDouble();
      this->randomSettings.minPosition[2] = rl["position"][2][0].asDouble();
      this->randomSettings.minVelocity[0] = rl["velocity"][0][0].asDouble();
      this->randomSettings.minVelocity[1] = rl["velocity"][1][0].asDouble();
      this->randomSettings.minVelocity[2] = rl["velocity"][2][0].asDouble();
      this->randomSettings.maxPosition[0] = rl["position"][0][1].asDouble();
      this->randomSettings.maxPosition[1] = rl["position"][1][1].asDouble(),
      this->randomSettings.maxPosition[2] = rl["position"][2][1].asDouble();
      this->randomSettings.maxVelocity[0] = rl["velocity"][0][1].asDouble();
      this->randomSettings.maxVelocity[1] = rl["velocity"][1][1].asDouble();
      this->randomSettings.maxVelocity[2] = rl["velocity"][2][1].asDouble();
      Json::Value elementsList = root["elements"];
      std::for_each(elementsList.begin(), elementsList.end(),
                    [&](const Json::Value &e) {
                      Element temp;
                      temp.name = e["name"].asString();
                      temp.mass = e["mass"].asDouble();
                      for (int jj = 0; jj < 3; jj++) {
                        temp.position[jj] = e["position"][jj].asDouble();
                        temp.velocity[jj] = e["velocity"][jj].asDouble();
                      }
                      this->elements.push_back(temp);
                    });
      this->nElem = this->elements.size();
    } catch (Json::RuntimeError &e) {
      std::cout << "Case Reading Error: " << e.what() << std::endl;
    }
    file.close();
    return 0;
  }
  return -1;
}

double Settings::getWidth() { return width; }
double Settings::getHeight() { return height; }
double Settings::getDepth() { return depth; }
double Settings::getDeltaT() { return deltaT; }
bool Settings::getVerbose() { return verbose; }
RandomSettings Settings::getRandomSettings() { return randomSettings; }
std::string Settings::getOutPath() { return output; }
int Settings::getNElem() { return nElem; }
int Settings::getSteps() { return steps; }
int Settings::getWriteSteps() { return writeSteps; }
std::vector<Element> Settings::getElements() { return elements; }