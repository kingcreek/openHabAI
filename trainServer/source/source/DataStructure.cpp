/*
 * File:   DataStructure.h
 * Author: Joshua Johannson
 *
  */

#include <json.hpp>
#include <DataStructure.h>
#include <NeuralNetwork.h>

int DataStructure::idIncrement = 0;

DataStructure::DataStructure()
: JsonListItem::JsonListItem(idIncrement),
  networks([&](Json params){
             return new NeuralNetwork(this, params);
           },
           "dataStructures/" + to_string(id) + "/networks", "network.json")
{}

DataStructure::DataStructure(string path, string filename) : DataStructure()
{
  load(path, filename);
}


DataStructure::DataStructure(Json params)
    : DataStructure()
{
  fromJson(params);
}


DataStructure *DataStructure::create(Json params)
{
  if (params == NULL)
  {
    return NULL;
  }

  if (params["type"] == "function")
  {
    return new FunctionStructure(params);
  }
}

// -- GET / SET ----------------------
Json DataStructure::toJson()
{
  Json json = {
      {"name", name},
      {"type", type()}
  };
  return json.merge(JsonListItem::toJson());
}

void DataStructure::fromJson(Json params)
{
  // cout << "DataStructure::fromJson(Json params)" << endl;
  JsonListItem::fromJson(params);
  name = params["name"];
  type(params["type"]);
}

void DataStructure::loadChilds()
{
  // load networks
  networks.folder = "dataStructures/" + to_string(id) + "/networks";
  networks.loadAll();
}


string DataStructure::type()
{
  switch (type_)
  {
    case TYPE_FUNCTION:
      return "function";
    case TYPE_OPENHAB:
      return "openhab";
  }
}

void DataStructure::type(string type)
{
  if (type == "function")
  {
    this->type_ = TYPE_FUNCTION;
  }
  if (type == "openhab")
  {
    this->type_ = TYPE_OPENHAB;
  }
}
bool DataStructure::operator==(int other) const
{
  return this->id == other;
}
bool DataStructure::operator==(const DataStructure &other) const
{
  return this->id == other.id;
}


// -- FUCTION -------------------------------
Json FunctionStructure::toJson()
{
  return DataStructure::toJson().merge(Json {
      {"function", function}
  });
}
void FunctionStructure::fromJson(Json params)
{
  //cout << "FunctionStructure::fromJson(Json params)" << endl;
  DataStructure::fromJson(params);
  function = params["function"];
}

vector<float> FunctionStructure::getDataBatch(vector<float> input)
{
  return vector<float>();
}

FunctionStructure::FunctionStructure(Json params)
{
  fromJson(params);
}
