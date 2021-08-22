#include "../include/main.hpp"
#include "../include/codegen.hpp"
#include "../include/UI.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "../extern/questui/shared/QuestUI.hpp"
#include "../extern/questui/shared/BeatSaberUI.hpp"
#include "../extern/custom-types/shared/register.hpp"

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <limits>
#include <math.h>
#include <fstream>
#include <string.h>

using namespace UnityEngine::UI;
using namespace il2cpp_utils;
using namespace GlobalNamespace;

static float clamp(float num, float min, float max) {
    if(num < min) {
        return min;
    } else if(num > max) {
        return max;
    } else {
        return num;
    }
}

Logger& logger() {
    static auto logger = new Logger(modInfo, LoggerOptions(false, false));
    return *logger;
}

#define log(...) logger().info(__VA_ARGS__)

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

struct GraphData {
    UnityEngine::Vector3 position = {0, 0, 0};
    UnityEngine::Vector3 rotation = {15, 0, 0};
    UnityEngine::Vector2 size = {300, 100};
};

GraphData graphData;
float pointMultiplier = 1.5f;
float npsLineThickness = 2.0f;//Maybe dont include for user use, too much options
float graphLineThickness = 3.0f;//Maybe dont include for user use, too much options
float timeIndicatorSize = 8.0f;//Maybe dont include for user use, too much options
float npsFontSize = 15.0f;//Maybe dont include for user use, too much options
bool npsLinesEnabled = true;
bool backgroundEnabled = true;
bool enabled = true;
UnityEngine::Vector3 lineColor = {38.0f, 203.0f, 0.0f};
UnityEngine::Vector3 timeIndicatorColor = {220.0f, 0.0f, 30.0f};
UnityEngine::Vector3 npsLineColor = {32.0f, 100.0f, 168.0f};
UnityEngine::Vector3 backgroundColor = {0.0f, 0.0f, 0.0f};

static void SaveConfig() {
    if(!getConfig().config.HasMember("Enabled")) {
        // log("Regenerating config!");
        getConfig().config.SetObject();
        auto& allocator = getConfig().config.GetAllocator();

        getConfig().config.AddMember("Point Frequency", pointMultiplier, allocator);
        getConfig().config.AddMember("NPS Lines Enabled", npsLinesEnabled, allocator);
        getConfig().config.AddMember("Background Enabled", backgroundEnabled, allocator);
        getConfig().config.AddMember("Enabled", enabled, allocator);
        
        rapidjson::Value graph(rapidjson::kObjectType);
        rapidjson::Value graphSize(rapidjson::kObjectType);
        graphSize.AddMember("Width", graphData.size.x, allocator);
        graphSize.AddMember("Height", graphData.size.y, allocator);
        graph.AddMember("Size", graphSize, allocator);
        rapidjson::Value graphPos(rapidjson::kObjectType);
        graphPos.AddMember("X", graphData.position.x, allocator);
        graphPos.AddMember("Y", graphData.position.y, allocator);
        graphPos.AddMember("Z", graphData.position.z, allocator);
        graph.AddMember("Position", graphPos, allocator);
        rapidjson::Value graphRot(rapidjson::kObjectType);
        graphRot.AddMember("X", graphData.rotation.x, allocator);
        graphRot.AddMember("Y", graphData.rotation.y, allocator);
        graphRot.AddMember("Z", graphData.rotation.z, allocator);
        graph.AddMember("Rotation", graphRot, allocator);
        getConfig().config.AddMember("Graph", graph, allocator);

        getConfig().Write();
        // log("Config regeneration complete!");
    } else {
        // log("Not regnerating config.");
    }
}

static void LoadConfig() {
    getConfig().Load();
    pointMultiplier = getConfig().config["Point Frequency"].GetFloat();
    graphData.position = UnityEngine::Vector3{getConfig().config["Graph"]["Position"]["X"].GetFloat(), getConfig().config["Graph"]["Position"]["Y"].GetFloat(), getConfig().config["Graph"]["Position"]["Z"].GetFloat()};
    graphData.rotation = UnityEngine::Vector3{getConfig().config["Graph"]["Rotation"]["X"].GetFloat(), getConfig().config["Graph"]["Rotation"]["Y"].GetFloat(), getConfig().config["Graph"]["Rotation"]["Z"].GetFloat()};
    graphData.size = UnityEngine::Vector2{getConfig().config["Graph"]["Size"]["Width"].GetFloat(), getConfig().config["Graph"]["Size"]["Height"].GetFloat()};
    npsLinesEnabled = getConfig().config["NPS Lines Enabled"].GetBool();
    backgroundEnabled = getConfig().config["Background Enabled"].GetBool();
    enabled = getConfig().config["Enabled"].GetBool();
}

template<class T>
UnityEngine::GameObject* FindObject(std::string name, bool byParent = true, bool getLastIndex = false) {
    // log("Finding GameObject of name "+name);
    Array<T>* trs = UnityEngine::Resources::FindObjectsOfTypeAll<T>();
    // log("There are "+std::to_string(trs->Length())+" GameObjects");
    for(int i = 0; i < trs->Length(); i++) {
        if(i != trs->Length()-1 && getLastIndex) continue;
        UnityEngine::GameObject* go = trs->values[i]->get_gameObject();
        if(byParent) {
            go = go->get_transform()->GetParent()->get_gameObject();
        }
        // log(to_utf8(csstrtostr(UnityEngine::Transform::GetName(trs->values[i]))));
        if(to_utf8(csstrtostr(UnityEngine::Transform::GetName(go))) == name){
            // log("Found GameObject");
            return go;
        }
    }
    // log("Could not find GameObject");
    return nullptr;
}

void SetGlobalScale(UnityEngine::Transform* transform, UnityEngine::Vector3 globalScale) {
    transform->set_localScale(UnityEngine::Vector3::get_one());
    transform->set_localScale(UnityEngine::Vector3{globalScale.x/transform->get_lossyScale().x, globalScale.y/transform->get_lossyScale().y, globalScale.z/transform->get_lossyScale().z});
}

float songLength;
float songTime;

int maxNoteCount;

std::vector<float> notes;
std::vector<std::pair<float, int>> graphPoints;

int songTimeXIndex;
int songTimeYIndex;

UnityEngine::GameObject* graphGO;
UnityEngine::GameObject* playerTime;

bool hasNotes;
bool createdGraph;

float pointFrequency;

UnityEngine::Material* uiMat;

UnityEngine::Vector2 graphOffset = {10, 0};

float convertToGraphSizeX(float value) {
    return value * (graphData.size.x / 400.0f);
}

float convertToGraphSizeY(float value) {
    return value * (graphData.size.y / 100.0f);
}

float getXPos(int i) {
    int offset = 9;
    if(!npsLinesEnabled) offset = 20;
    float pointDistance = (graphData.size.x - convertToGraphSizeX(offset)) / float(graphPoints.size()-1);
    return i*pointDistance + convertToGraphSizeX(graphOffset.x);
}

float getXPosForSongTime(float time) {
    int offset = 9;
    if(!npsLinesEnabled) offset = 20;
    float pointDistance = (graphData.size.x - convertToGraphSizeX(offset)) / float(graphPoints.size()-1);
    if(time > graphPoints[songTimeXIndex+1].first) {
        while(time > graphPoints[songTimeXIndex+1].first) songTimeXIndex++;
    } else if(time < graphPoints[songTimeXIndex].first) {
        while(time < graphPoints[songTimeXIndex].first) songTimeXIndex--;
    }
    float lerpAmount = (time - graphPoints[songTimeXIndex].first) / (graphPoints[songTimeXIndex+1].first - graphPoints[songTimeXIndex].first);
    return std::lerp(songTimeXIndex, songTimeXIndex+1, lerpAmount)*pointDistance + convertToGraphSizeX(graphOffset.x);
}

float getYPos(int i) {
    return (float(graphPoints[i].second) / float(maxNoteCount)) * graphData.size.y + convertToGraphSizeY(graphOffset.y);
}

float getYPosForSongTime(float time) {
    if(time > graphPoints[songTimeYIndex+1].first) {
        while(time > graphPoints[songTimeYIndex+1].first) songTimeYIndex++;
    } else if(time < graphPoints[songTimeYIndex].first) {
        while(time < graphPoints[songTimeYIndex].first) songTimeYIndex--;
    }
    float lerpAmount = (time - graphPoints[songTimeYIndex].first) / (graphPoints[songTimeYIndex+1].first - graphPoints[songTimeYIndex].first);
    return (float(std::lerp(graphPoints[songTimeYIndex].second, graphPoints[songTimeYIndex+1].second, lerpAmount)) / float(maxNoteCount)) * graphData.size.y + convertToGraphSizeY(graphOffset.y);
}

void createBackground(UnityEngine::GameObject* go, UnityEngine::Vector3 pos, UnityEngine::Vector2 size) {
    UnityEngine::GameObject* background = UnityEngine::GameObject::New_ctor(createcsstr("Background"));
    background->get_transform()->SetParent(go->get_transform(), false);
    background->get_transform()->set_localPosition(UnityEngine::Vector3{pos.x, pos.y, 0});
    UnityEngine::UI::Image* image = background->AddComponent<UnityEngine::UI::Image*>();
    image->set_color(UnityEngine::Color{backgroundColor.x, backgroundColor.y, backgroundColor.z, 0.5f});
    image->set_material(uiMat);
    UnityEngine::RectTransform* backgroundRT = background->GetComponent<UnityEngine::RectTransform*>();
    backgroundRT->set_sizeDelta(UnityEngine::Vector2{size.x, size.y});
}

void createNPSLine(UnityEngine::GameObject* go, UnityEngine::Vector3 pos, int num) {
    auto text = QuestUI::BeatSaberUI::CreateText(go->get_transform(), std::to_string(num), UnityEngine::Vector2{pos.x, pos.y});
    text->set_fontSize(std::min(npsFontSize * (graphData.size.x/400.0f), npsFontSize * (graphData.size.y/100.0f)));

    text->set_alignment(TMPro::TextAlignmentOptions::Center);
    UnityEngine::Vector3 currentPosition = text->get_transform()->get_localPosition();
    text->get_transform()->set_localPosition(UnityEngine::Vector3{currentPosition.x-convertToGraphSizeX(37), currentPosition.y-convertToGraphSizeY(3), currentPosition.z});

    UnityEngine::GameObject* lineGO = UnityEngine::GameObject::New_ctor(createcsstr("NPSLine"));
    lineGO->get_transform()->SetParent(go->get_transform(), false);
    lineGO->get_transform()->set_localPosition(UnityEngine::Vector3{pos.x+graphData.size.x/2-convertToGraphSizeX(30.5f), pos.y-convertToGraphSizeY(3), 0});
    UnityEngine::UI::Image* image = lineGO->AddComponent<UnityEngine::UI::Image*>();
    image->set_color(UnityEngine::Color{npsLineColor.x/255.0f, npsLineColor.y/255.0f, npsLineColor.z/255.0f, 0.3f});
    image->set_material(uiMat);

    UnityEngine::RectTransform* rt = lineGO->GetComponent<UnityEngine::RectTransform*>();
    rt->set_sizeDelta(UnityEngine::Vector2{graphData.size.x-convertToGraphSizeX(7.0f), std::min(npsLineThickness * (graphData.size.x/400.0f), npsLineThickness * (graphData.size.y/100.0f))});
}

void createLine(UnityEngine::Vector2 a, UnityEngine::Vector2 b, UnityEngine::Transform* parent) {
    UnityEngine::GameObject* go = UnityEngine::GameObject::New_ctor(createcsstr("line"));
    go->get_transform()->SetParent(parent, false);
    UnityEngine::UI::Image* image = go->AddComponent<UnityEngine::UI::Image*>();
    image->set_color(UnityEngine::Color{lineColor.x/255.0f, lineColor.y/255.0f, lineColor.z/255.0f, 1});
    image->set_material(uiMat);
    UnityEngine::RectTransform* rt = go->GetComponent<UnityEngine::RectTransform*>();
    UnityEngine::Vector2 dir = (b - a).get_normalized();
    float dist = UnityEngine::Vector2::Distance(a, b);
    rt->set_sizeDelta(UnityEngine::Vector2{dist+std::min(convertToGraphSizeX(1.5f), convertToGraphSizeY(1)), std::min(graphLineThickness * (graphData.size.x/400.0f), graphLineThickness * (graphData.size.y/100.0f))});
    rt->set_anchoredPosition(a + dir * dist * 0.5f);
    rt->set_localEulerAngles(UnityEngine::Vector3{0, 0, atan2(dir.y, dir.x) * 57.29578f});
}

MAKE_HOOK_MATCH(SongUpdate, &AudioTimeSyncController::Update, void, AudioTimeSyncController* self) {
    
    SongUpdate(self);

    songTime = self->songTime;

    if(enabled) {
        if(!createdGraph && hasNotes) {
            hasNotes = false;
            
            songLength = *il2cpp_utils::RunMethod<float>(self, "get_songLength");
            pointFrequency = songLength / 60 / pointMultiplier;
            // log("Point Frequency is %f / 60 / %f = %f", songLength, pointMultiplier, pointFrequency);

            // log("Getting all notes");
            auto bocc = UnityEngine::Object::FindObjectOfType<GlobalNamespace::BeatmapObjectCallbackController*>();
            auto beatmapLinesData = reinterpret_cast<GlobalNamespace::BeatmapData*>(bocc->initData->beatmapData)->beatmapLinesData;
            for (int i = 0; i < beatmapLinesData->Length(); i++) {
                auto beatmapLineData = beatmapLinesData->values[i];
                auto beatmapObjectsData = beatmapLineData->beatmapObjectsData->items;
                for (int j = 0; j < beatmapObjectsData->Length(); j++) {
                    auto beatmapObjectData = beatmapObjectsData->values[j];
                    if (!beatmapObjectData) continue;
                    if (*RunMethod<int>(beatmapObjectData, "get_beatmapObjectType") == 0) {
                        auto note = reinterpret_cast<GlobalNamespace::NoteData*>(beatmapObjectData);
                        if(note->get_colorType() != ColorType::None) {
                            hasNotes = true;
                            notes.push_back(note->get_time());
                        }
                    }
                }
            }

            if(hasNotes) {
                // log("Sorting notes");
                std::sort(notes.begin(), notes.end());
                
                // log("Getting keypoints");
                float time = 0;
                std::vector<float> keyPoints;
                while(time < songLength) {
                    keyPoints.push_back(time);
                    time += pointFrequency;
                }
                keyPoints.push_back(songLength);
                // log("There are %i keypoints", keyPoints.size());

                // log("Getting note counts");
                maxNoteCount = 0;
                int index = 0;
                int start;
                while(index < keyPoints.size()) {
                    int noteCount = 0;
                    if(index == 0) {
                        int noteIndex = 0;
                        while(notes[noteIndex] < keyPoints[index+1]) {
                            noteCount++;
                            noteIndex++;
                        }
                        start = 0;
                    } else if(index == keyPoints.size()-1) {
                        int noteIndex = start;
                        while(notes[noteIndex] < keyPoints[index] && noteIndex < notes.size()) {
                            noteCount++;
                            noteIndex++;
                        }
                    } else {
                        int noteIndex = start;
                        while(notes[noteIndex] < keyPoints[index+1] && noteIndex < notes.size()) {
                            if(notes[noteIndex] < keyPoints[index]) {
                                start = noteIndex+1;
                            }
                            noteCount++;
                            noteIndex++;
                        }
                    }
                    if(noteCount > maxNoteCount) {
                        maxNoteCount = noteCount;
                    }

                    graphPoints.push_back(std::make_pair(keyPoints[index], noteCount));
                    index++;
                }
                // log("Max note count is %i", maxNoteCount);
                float maxNPS = maxNoteCount / pointFrequency / 2;
                // log("Max NPS is %f", maxNPS);
                
                // log("Creating chart game object");
                graphGO = UnityEngine::GameObject::New_ctor(createcsstr("GraphGO"));
                bool getLastIndex = false;
                if(FindObject<MultiplayerController*>("MultiplayerController", false) == nullptr) {
                    getLastIndex = true;
                }
                // log("Get last index is %i", getLastIndex);
                graphGO->get_transform()->SetParent(FindObject<ComboUIController*>("ComboPanel", false, getLastIndex)->get_transform());
                graphGO->get_transform()->set_position(UnityEngine::Vector3{-graphData.size.x/200 + graphData.position.x, -2.8f + graphData.position.y, 6 + graphData.position.z});
                graphGO->get_transform()->set_eulerAngles(graphData.rotation);
                SetGlobalScale(graphGO->get_transform(), UnityEngine::Vector3{0.01f, 0.01f, 0.01f});

                if(backgroundEnabled) {
                    createBackground(graphGO, UnityEngine::Vector3{graphData.size.x/2, graphData.size.y/2, 0}, UnityEngine::Vector2{graphData.size.x+convertToGraphSizeX(20), graphData.size.y+convertToGraphSizeY(20)});
                }

                if(npsLinesEnabled) {
                    for(int i = 0; i < 5; i++) {
                        createNPSLine(
                            graphGO,
                            UnityEngine::Vector3{convertToGraphSizeX(36), convertToGraphSizeY(3)+float(i*graphData.size.y/4), 0},
                            int(maxNPS/4*i)
                        );
                    }
                }

                // log("Creating lines");
                for(int i = 0; i < graphPoints.size()-1; i++) {
                    createLine(
                        UnityEngine::Vector2{getXPos(i), getYPos(i)},
                        UnityEngine::Vector2{getXPos(i+1), getYPos(i+1)},
                        graphGO->get_transform()
                    );
                }

                // log("Creating player time indicator");
                playerTime = UnityEngine::GameObject::New_ctor(createcsstr("PlayerTime"));
                playerTime->get_transform()->SetParent(graphGO->get_transform(), false);
                playerTime->get_transform()->set_localPosition(UnityEngine::Vector3{getXPos(0), getYPos(0), 0});
                UnityEngine::UI::Image* timeImage = playerTime->AddComponent<UnityEngine::UI::Image*>();
                timeImage->set_color(UnityEngine::Color{timeIndicatorColor.x/255.0f, timeIndicatorColor.y/255.0f, timeIndicatorColor.z/255.0f, 1});
                timeImage->set_material(uiMat);
                UnityEngine::RectTransform* playerTimeRT = playerTime->GetComponent<UnityEngine::RectTransform*>();
                float size = std::min(timeIndicatorSize * (graphData.size.x/400.0f), timeIndicatorSize * (graphData.size.y/100.0f));
                playerTimeRT->set_sizeDelta(UnityEngine::Vector2{size, size});

                songTimeXIndex = 0;
                songTimeYIndex = 0;
                createdGraph = true;
                // log("Finished creating song difficulty chart!");
            }
        } else if(hasNotes && createdGraph) {
            if(songTime < 3) {
                graphGO->get_transform()->set_position(UnityEngine::Vector3{-graphData.size.x/200 + graphData.position.x, -2.8f + graphData.position.y, 6 + graphData.position.z});
                graphGO->get_transform()->set_eulerAngles(graphData.rotation);
            }
            UnityEngine::Vector3 newPosition = UnityEngine::Vector3{getXPosForSongTime(std::max(0.0f, songTime)), getYPosForSongTime(std::max(0.0f, songTime)), 0};
            playerTime->get_transform()->set_localPosition(newPosition);
        }
    }
}

MAKE_HOOK_MATCH(SongStart, &AudioTimeSyncController::Awake, void, AudioTimeSyncController* self) {

    // log("SongStart");

    uiMat = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>(), [](UnityEngine::Material* x) { return to_utf8(csstrtostr(x->get_name())) == "UINoGlow"; });

    songTime = 0;
    notes.clear();
    graphPoints.clear();
    hasNotes = true;
    createdGraph = false;

    LoadConfig();

    SongStart(self);

    // log("Finished Song Start Setup");
}

extern "C" void setup(ModInfo& info) {
    info.id = "SongDifficultyChart";
    info.version = "0.1.0";
    modInfo = info;
    
    getConfig().Load();
    SaveConfig();

    LoadConfig();
}

extern "C" void load() {
    QuestUI::Init();

    custom_types::Register::AutoRegister();
    QuestUI::Register::RegisterModSettingsViewController<SongDifficultyChart::UIController*>(modInfo, "Song Difficulty Chart");

    // log("Installing hooks...");
    INSTALL_HOOK(logger().get(), SongUpdate);
    INSTALL_HOOK(logger().get(), SongStart);
    // log("Installed all hooks!");
}