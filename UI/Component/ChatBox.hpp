#ifndef CHATBOX_HPP
#define CHATBOX_HPP
#include <string>
#include "Engine/IControl.hpp"
#include "Image.hpp"

namespace Engine {
    class ChatBox : public Image, public IControl {
    protected:
        std::string chatText;
        std::string currentDisplayText;
        float textTimer;
        float textSpeed;  // characters per second
        size_t currentCharIndex;
    public:
        ChatBox(float screenW, float screenH, const std::string& chatText);
        void Draw() const;
        void Update(float deltaTime);
        void OnKeyDown(int keycode) override;
    };
}
#endif