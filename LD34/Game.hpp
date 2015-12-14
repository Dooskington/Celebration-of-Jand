#pragma once
#include "PCH.hpp"
#include "Peon.hpp"
#include "Tree.hpp"
#include "Stone.hpp"
#include "Bonfire.hpp"

class Game
{
    public:
        Game();
        ~Game();

        void Start();
        void Update();
        void ProcessInput();
        void Render();
        void LeftClick();
        void LeftClickUp();
        void RightClick();
        void RightClickUp();

        Bonfire* FindBonfire(Peon* peon);
        Tree* FindTree(Peon* peon);
        void SpawnPeons(bool initial);
        void SacrificePeon(Peon* peon);
        void CommandPeons(GameObject* target);
        void DepositResources(int amount);
        int GetResources() const;

        bool CheckCollision(SDL_Rect a, SDL_Rect b);

        // Textures
        bool LoadTexture(const std::string& path, const std::string& id);
        void RenderTexture(const std::string& id, const int& x, const int& y, const int& width, const int& height);

        // Fonts
        bool LoadFont(const std::string& path, const std::string& id);
        void RenderText(const std::string& fontID, const int& x, const int& y, const std::string& text, SDL_Color color = {0, 0, 0, 255});

        // Sounds
        bool LoadSound(const std::string& path, const std::string& id);
        void PlaySound(const std::string& id);

    public:
        double m_deltaTime;

        int mouseX;
        int mouseY;

        bool m_selecting = false;
        SDL_Rect m_selectionRect;

    private:
        const std::string WINDOW_TITLE = "LD34 - Celebration Of Jand";
        const int WINDOW_WIDTH = 640;
        const int WINDOW_HEIGHT = 480;
        bool m_isRunning;

        SDL_Window* m_window;
        SDL_Renderer* m_renderer;

        // Input
        bool m_buttonsDown[5];
        bool m_buttonsUp[5];
        bool m_buttonsCurrent[5];

        // Textures
        std::map<std::string, SDL_Texture*> m_textureMap;

        // Fonts
        std::map<std::string, TTF_Font*> m_fontMap;

        // Sounds
        std::map<std::string, Mix_Chunk*> m_soundMap;

        // GameObjects
        std::vector<GameObject*> m_gameObjects;
        GameObject* m_bonfire;

        std::vector<Peon*> m_selectedPeons;

        std::stringstream sstream;
        int m_resources;
        int m_peons;

        int m_peonsToSpawn = 10;
};
