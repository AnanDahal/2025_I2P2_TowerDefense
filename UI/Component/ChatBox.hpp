#ifndef CHATBOX_HPP
#define CHATBOX_HPP

#include <string>
#include <allegro5/allegro_font.h>
#include "Engine/IControl.hpp"
#include "Image.hpp"

namespace Engine {
    class ChatBox : public Image, public IControl {
    protected:
        std::string speakerName;
        std::string currentDisplayText;
        float textTimer;
        float textSpeed;  // characters per second
        size_t currentCharIndex;
        ALLEGRO_FONT* nameFont;
        ALLEGRO_FONT* chatFont;
        ALLEGRO_FONT* smallFont;

    public:
        std::string chatText;  // Make this public so BeforeScene can access it
        bool finished = false;
        ChatBox(float screenW, float screenH, const std::string& speakerName = "", const std::string& chatText = "");
        ~ChatBox();
        void Draw() const override;
        void Update(float deltaTime) override;
        void OnKeyDown(int keycode) override;
        void Terminate();
        
        void SetSpeaker(const std::string& name);
        void SetText(const std::string& text);
    };
}

#endif
