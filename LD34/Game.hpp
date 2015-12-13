#pragma once
#include "PCH.hpp"
#include "Peon.hpp"
#include "Tree.hpp"
#include "Bonfire.hpp"

class Game
{
    public:
        Game();
        ~Game();

        void Start();
        void Stop();
        void Update();
        void ProcessInput();
        void Render();
        void LeftClick();
        void LeftClickUp();
        void RightClick();
        void RightClickUp();

        Bonfire* FindBonfire(Peon* peon);
        Tree* FindTree(Peon* peon);
        void CommandPeon(Peon* peon, GameObject* target);

        bool CheckCollision(SDL_Rect a, SDL_Rect b);

        // Textures
        bool LoadTexture(const std::string& path, const std::string& id);
        void DrawTexture(const std::string& id, const double& x, const double& y, const double& width, const double& height);

        // Sounds
        bool LoadSound(const std::string& path, const std::string& id);
        void PlaySound(const std::string& id);

    public:
        double m_deltaTime;

        int mouseX;
        int mouseY;

        bool m_selecting = false;
        int m_selectionStartX;
        int m_selectionStartY;
        int m_selectionEndX;
        int m_selectionEndY;
        SDL_Rect m_selectionRect;

    private:
        const std::string WINDOW_TITLE = "LD34";
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
        
        // Sounds
        std::map<std::string, Mix_Chunk*> m_soundMap;

        // GameObjects
        std::vector<GameObject*> m_gameObjects;

        GameObject* m_peon;
        GameObject* m_peon2;
        GameObject* m_peon3;
        GameObject* m_peon4;
        GameObject* m_peon5;
        GameObject* m_peon6;
        GameObject* m_peon7;
        GameObject* m_tree;
        GameObject* m_tree2;
        GameObject* m_tree3;
        GameObject* m_bonfire;

        std::vector<Peon*> m_selectedPeons;
};