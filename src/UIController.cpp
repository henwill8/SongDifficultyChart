#include "../include/UI.hpp"
#include "../include/codegen.hpp"
using namespace QuestUI;
using namespace UnityEngine::UI;
using namespace UnityEngine;

#define CreateIncrement(parent, floatConfigValue, name, decimals, increment, hasMin, hasMax, minValue, maxValue) QuestUI::BeatSaberUI::CreateIncrementSetting(parent, name, decimals, increment, floatConfigValue.GetFloat(), hasMin, hasMax, minValue, maxValue, UnityEngine::Vector2{}, il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(classof(UnityEngine::Events::UnityAction_1<float>*), (void*)nullptr, +[](float value) { floatConfigValue.SetFloat(value); }))

#define CreateToggle(parent, boolConfigValue, name) QuestUI::BeatSaberUI::CreateToggle(parent, name, boolConfigValue.GetBool(), il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(classof(UnityEngine::Events::UnityAction_1<bool>*), (void*)nullptr, +[](bool value) { boolConfigValue.SetBool(value); }))

DEFINE_CLASS(SongDifficultyChart::UIController);

VerticalLayoutGroup* layout1;

void createGraphSettings() {

}

void SongDifficultyChart::UIController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
	if(firstActivation) {
		HorizontalLayoutGroup* layout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(get_rectTransform());
		layout->set_spacing(1.5f);
        
        float spacing = 0.1f;
		layout1 = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(layout->get_rectTransform());
		layout1->set_spacing(spacing);
		// layout1->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));

		layout1->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 0, 0));
        // layout1->set_childForceExpandWidth(false);
        // layout1->set_childControlWidth(false);

        layout1->get_transform()->set_localScale(UnityEngine::Vector3{0.9f, 0.9f, 0.9f});

		Transform* Parent1 = layout1->get_transform();

        CreateToggle(Parent1, getConfig().config["Enabled"], "Enabled");
        CreateToggle(Parent1, getConfig().config["NPS Lines Enabled"], "NPS Lines Enabled");
        CreateToggle(Parent1, getConfig().config["Background Enabled"], "Background Enabled");
        CreateIncrement(Parent1, getConfig().config["Point Frequency"], "Point Frequency", 1, 0.1f, true, true, 0.1f, 5.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Size"]["Width"], "Graph Width", 0, 10, true, false, 10, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Size"]["Height"], "Graph Height", 0, 10, true, false, 10, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Position"]["X"], "Pos X", 1, 0.5f, false, false, 0, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Position"]["Y"], "Pos Y", 1, 0.5f, false, false, 0, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Position"]["Z"], "Pos Z", 1, 0.5, false, false, 0, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Rotation"]["X"], "Rot X", 1, 5, false, false, 0, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Rotation"]["Y"], "Rot Y", 1, 5, false, false, 0, 0.0f);
        CreateIncrement(Parent1, getConfig().config["Graph"]["Rotation"]["Z"], "Rot Z", 1, 5, false, false, 0, 0.0f);
    }
}

void SongDifficultyChart::UIController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling)  {
    getConfig().Write();
}