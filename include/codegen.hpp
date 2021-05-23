  
#include "../extern/codegen/include/GlobalNamespace/BeatmapDifficulty.hpp"
#include "../extern/codegen/include/GlobalNamespace/GameplayModifiers.hpp"
#include "../extern/codegen/include/GlobalNamespace/GameplayModifiersModelSO.hpp"
#include "../extern/codegen/include/GlobalNamespace/BeatmapDifficulty.hpp"
#include "../extern/codegen/include/GlobalNamespace/IDifficultyBeatmap.hpp"
#include "../extern/codegen/include/GlobalNamespace/IBeatmapLevel.hpp"
#include "../extern/codegen/include/GlobalNamespace/ScoreController.hpp"
#include "../extern/codegen/include/GlobalNamespace/ResultsViewController.hpp"
#include "../extern/codegen/include/GlobalNamespace/LevelCompletionResults.hpp"
#include "../extern/codegen/include/GlobalNamespace/Saber.hpp"
#include "../extern/codegen/include/GlobalNamespace/SaberManager.hpp"
#include "../extern/codegen/include/GlobalNamespace/MainMenuViewController.hpp"
#include "../extern/codegen/include/GlobalNamespace/MainFlowCoordinator.hpp"
#include "../extern/codegen/include/GlobalNamespace/EditAvatarFlowCoordinator.hpp"
#include "../extern/codegen/include/TMPro/TextMeshProUGUI.hpp"
#include "../extern/codegen/include/UnityEngine/Transform.hpp"
#include "../extern/codegen/include/UnityEngine/RectTransform.hpp"
#include "../extern/codegen/include/UnityEngine/Rect.hpp"
#include "../extern/codegen/include/UnityEngine/GameObject.hpp"
#include "../extern/codegen/include/UnityEngine/Object.hpp"
#include "../extern/codegen/include/UnityEngine/Resources.hpp"
#include "../extern/codegen/include/UnityEngine/Events/UnityAction.hpp"
#include "../extern/codegen/include/UnityEngine/Component.hpp"
#include "../extern/codegen/include/TMPro/TextMeshProUGUI.hpp"
#include "GlobalNamespace/PlayerTransforms.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/StandardLevelDetailViewController.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/AvatarVisualController.hpp"
#include "GlobalNamespace/AvatarPoseController.hpp"
#include "GlobalNamespace/AvatarData.hpp"
#include "GlobalNamespace/AvatarDataModel.hpp"
#include "GlobalNamespace/AvatarMeshPartSO.hpp"
#include "GlobalNamespace/AvatarSpritePartSO.hpp"
#include "GlobalNamespace/AvatarPartsModel.hpp"
#include "GlobalNamespace/FPSCounter.hpp"
#include "GlobalNamespace/AvatarPartCollection_1.hpp"
#include "GlobalNamespace/MulticolorAvatarPartPropertyBlockSetter.hpp"
#include "GlobalNamespace/AvatarPropertyBlockColorSetter.hpp"
#include "GlobalNamespace/SkinColorSO.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/BloomPrePass.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/BeatmapLineData.hpp"
#include "GlobalNamespace/BeatmapObjectData.hpp"
#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
#include "../extern/codegen/include/GlobalNamespace/OVRInput.hpp"
#include "../extern/codegen/include/GlobalNamespace/OVRInput_Button.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/Space.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/LineRenderer.hpp"
#include "UnityEngine/UI/CanvasScaler.hpp"
#include "UnityEngine/UI/GraphicRaycaster.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "GlobalNamespace/SongProgressUIController.hpp"
#include "GlobalNamespace/CoreGameHUDController.hpp"
#include "GlobalNamespace/MainCamera.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/VisualEffectsController.hpp"
#include "GlobalNamespace/MainEffectController.hpp"
#include "GlobalNamespace/PlayerSettingsPanelController.hpp"
#include "GlobalNamespace/MainCameraCullingMask.hpp"
#include "GlobalNamespace/MainCameraCullingMask_InitData.hpp"
#include "GlobalNamespace/LayerMasks.hpp"
#include "GlobalNamespace/ComboUIController.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/GameSongController.hpp"
#include "GlobalNamespace/AudioManagerSO.hpp"
#include "GlobalNamespace/BeatmapEventData.hpp"
#include "GlobalNamespace/BeatmapObjectCallbackController.hpp"
#include "GlobalNamespace/BeatmapObjectCallbackController_InitData.hpp"
#include "GlobalNamespace/BeatmapEventCallbackData.hpp"
#include "GlobalNamespace/BeatmapObjectCallbackData.hpp"
#include "GlobalNamespace/BeatmapObjectData.hpp"
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/IReadonlyBeatmapLineData.hpp"
#include "GlobalNamespace/NoteCutSoundEffectManager.hpp"
#include "GlobalNamespace/BeatmapObjectType.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/OVRManager.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/OVRPose.hpp"
#include "GlobalNamespace/MultiplayerController.hpp"
#include "GlobalNamespace/OVRExtensions.hpp"
#include "GlobalNamespace/OVRPlugin_Node.hpp"
#include "GlobalNamespace/OVRPlugin_Step.hpp"
#include "GlobalNamespace/OVRPlugin_PoseStatef.hpp"
#include "HMUI/TimeSlider.hpp"
#include "UnityEngine/UI/Slider.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/CameraClearFlags.hpp"
#include "UnityEngine/RenderTexture.hpp"
#include "UnityEngine/RenderTextureDescriptor.hpp"
#include "UnityEngine/RenderTextureFormat.hpp"
#include "UnityEngine/Graphics.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/AudioSettings.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/UI/Selectable.hpp"
#include "UnityEngine/UI/Selectable_SelectionState.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/RenderTextureReadWrite.hpp"
#include "UnityEngine/SpriteRenderer.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "UnityEngine/Bounds.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/StereoTargetEyeMask.hpp"
#include "UnityEngine/Matrix4x4.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Rigidbody.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Mathf.hpp"
#include "System/Nullable_1.hpp"
#include "System/Text/StringBuilder.hpp"
#include "System/Collections/Generic/IReadOnlyList_1.hpp"
#include "System/Collections/Generic/IReadOnlyCollection_1.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/XR/XRDevice.hpp"