#include "Input.h"
#include <map>
#include <vector>

enum InputType
{
    keyboard,   // キーボード
    gamepad,    // ゲームパッド(Xbox)
    mouse,      // マウス
};

struct InputInfo
{
    InputType type; // 入力の種類
    int buttonID;   // ボタンの番号
};

using InputActionMap_t = std::map<std::string, std::vector<InputInfo>>;

namespace Input
{
    std::map<std::string, bool> currentInput;  // 現在の状態
    std::map<std::string, bool> previousInput; // 前回の状態
    InputActionMap_t inputActionMap;           // 各機種からの入力処理のマップ

    const int KEY_MAX = 256;    // キー入力の最大数
    char keyState[KEY_MAX];     // keyboard入力
    int padState;               // gamepad入力
    int mouseState;             // mouse入力
}

void Input::InitActionMap()
{
    inputActionMap["changeCamera"] = { {InputType::keyboard, KEY_INPUT_C} }; // カメラの切り替え
}

void Input::StateUpdate()
{
    GetHitKeyStateAll(keyState);
    padState = GetJoypadInputState(DX_INPUT_PAD1);
    mouseState = GetMouseInput();
    previousInput = currentInput;

    for (const auto& mapInfo : inputActionMap)
    {
        bool isDown = false;
        for (const auto& inputInfo : mapInfo.second)
        {
            isDown = (inputInfo.type == InputType::keyboard && keyState[inputInfo.buttonID]) ||
                ((inputInfo.type == InputType::gamepad) && (padState & inputInfo.buttonID)) ||
                ((inputInfo.type == InputType::mouse) && (mouseState & inputInfo.buttonID));
            if (isDown == true)
            {
                break;
            }
        }
        currentInput[mapInfo.first] = isDown;
    }

}

bool Input::IsKeyDown(const std::string& action)
{
    auto it = currentInput.find(action);
    if (it == currentInput.end()) // 見つからないならfalse
    {
        return false;
    }
    auto prevIt = previousInput.find(action);
    // 今は押してて、前回は押してない → 押した瞬間
    if (it->second == true && prevIt->second == false)
    {
        return true;
    }
    return false;
}

bool Input::IsKeyKeepDown(const std::string& action)
{
    auto it = currentInput.find(action);
    if (it == currentInput.end())
    {
        return false;
    }
    return it->second;
}

bool Input::IsKeyUp(const std::string& action)
{
    auto it = currentInput.find(action);
    if (it == currentInput.end())
    {
        return false;
    }
    auto prevIt = previousInput.find(action);
    // 現在は離してて、前回は押している → 離された瞬間
    if (it->second == false && prevIt->second == true)
    {
        return true;
    }
    return false;
}
