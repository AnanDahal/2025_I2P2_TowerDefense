#include "ChatBox.hpp"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <allegro5/allegro_primitives.h>

namespace Engine {

    ChatBox::ChatBox(float screenW, float screenH, const std::string& speakerName, const std::string& chatText)
        : Image("stage-select/chat_box.png", (screenW-1052)/2, (screenH-237)-20, 1052, 237, 0, 0),
          speakerName(speakerName), chatText(chatText) {

        currentDisplayText = "";
        textTimer = 0;
        textSpeed = 10.0f;  // characters per second
        currentCharIndex = 0;

        // Load fonts once
        chatFont = al_load_ttf_font("Resource/fonts/pirulen.ttf", 40, 0);
        nameFont = al_load_ttf_font("Resource/fonts/pirulen.ttf", 48, 0); // slightly larger for name
        smallFont = al_load_ttf_font("Resource/fonts/pirulen.ttf", 24, 0);

        if (!chatFont || !nameFont || !smallFont)
            std::cout << "[ChatBox] Failed to load one or more fonts.\n";
    }

    ChatBox::~ChatBox() {
        if (chatFont) al_destroy_font(chatFont);
        if (nameFont) al_destroy_font(nameFont);
        if (smallFont) al_destroy_font(smallFont);
    }

    void ChatBox::Update(float deltaTime) {
        textTimer += deltaTime;
        if (textTimer >= 1.0f / textSpeed && currentCharIndex < chatText.length()) {
            currentDisplayText += chatText[currentCharIndex];
            currentCharIndex++;
            textTimer -= 1.0f / textSpeed;
            if (textTimer > 1.0f / textSpeed)
                textTimer = 0;
        }
    }

    void ChatBox::Draw() const {
        // Draw the main chat box background
        al_draw_filled_rounded_rectangle(
            Position.x, Position.y,
            Position.x + Size.x, Position.y + Size.y,
            15, 15,  // Corner radius
            al_map_rgb(240, 240, 240)  // Light gray background
        );
        
        // Draw border for chat box
        al_draw_rounded_rectangle(
            Position.x, Position.y,
            Position.x + Size.x, Position.y + Size.y,
            15, 15,  // Corner radius
            al_map_rgb(100, 100, 100),  // Gray border
            2.0f  // Border thickness
        );

        if (nameFont && chatFont && smallFont) {
            // Create a background for the speaker name
            float nameBoxWidth = 200;  // Fixed width for name box
            float nameBoxHeight = 40;  // Fixed height for name box
            float nameBoxX = Position.x + 120;  // Positioned to the right
            float nameBoxY = Position.y - nameBoxHeight/2;  // Position it to overlap the top edge
            
            // Draw the name background box
            al_draw_filled_rounded_rectangle(
                nameBoxX - nameBoxWidth/2, nameBoxY,
                nameBoxX + nameBoxWidth/2, nameBoxY + nameBoxHeight,
                10, 10,  // Corner radius
                al_map_rgb(0, 0, 128)  // Dark blue background
            );
            
            // Draw border for name box
            al_draw_rounded_rectangle(
                nameBoxX - nameBoxWidth/2, nameBoxY,
                nameBoxX + nameBoxWidth/2, nameBoxY + nameBoxHeight,
                10, 10,  // Corner radius
                al_map_rgb(64, 64, 255),  // Light blue border
                2.0f  // Border thickness
            );

            // Speaker name (centered in the name box)
            al_draw_text(nameFont, al_map_rgb(255, 255, 255), 
                        nameBoxX, nameBoxY + (nameBoxHeight - al_get_font_line_height(nameFont))/2, 
                        ALLEGRO_ALIGN_CENTER, speakerName.c_str());

            // Dialogue text
            float textX = Position.x + 30;
            float textY = Position.y + 30;  // Moved up since we don't have the bulge
            float textW = Size.x - 60;
            al_draw_multiline_text(chatFont, al_map_rgb(0,0,0), textX, textY, textW, 40,
                                   ALLEGRO_ALIGN_LEFT, currentDisplayText.c_str());

            if (currentCharIndex >= chatText.length()) {
                float promptX = Position.x + Size.x - 30;
                float promptY = Position.y + Size.y - 30;
                al_draw_text(smallFont, al_map_rgb(128,128,128), promptX, promptY,
                             ALLEGRO_ALIGN_RIGHT, "press [ENTER] to continue");
            }
        }
    }

    void Engine::ChatBox::SetSpeaker(const std::string& name) {
        speakerName = name;
    }

    void Engine::ChatBox::SetText(const std::string& text) {
        chatText = text;
        currentDisplayText = "";
        currentCharIndex = 0;
        textTimer = 0;
        finished = false;
    }

    void Engine::ChatBox::OnKeyDown(int keycode) {
        if (keycode == ALLEGRO_KEY_ENTER || keycode == ALLEGRO_KEY_SPACE) {
            if (currentCharIndex >= chatText.length()) {
                // If text is fully displayed, signal to advance to next dialogue
                // but only if Enter was pressed
                if (keycode == ALLEGRO_KEY_ENTER) {
                    finished = true;
                }
            } else {
                // If text is still being displayed, show it all immediately
                currentCharIndex = chatText.length();
                currentDisplayText = chatText;
            }
        }
    }

    void ChatBox::Terminate() {}
}
