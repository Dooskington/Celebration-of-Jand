#include "PCH.hpp"
#include "Game.hpp"
#include "Vector2D.hpp"

Game::Game() :
    m_isRunning(true),
    m_deltaTime(0.0)
{
}

Game::~Game()
{
    std::vector<GameObject*>::const_iterator objIt;
    for (objIt = m_gameObjects.begin(); objIt != m_gameObjects.end(); objIt++)
    {
        delete *objIt;
    }

    std::map<std::string, SDL_Texture*>::const_iterator texIt;
    for (texIt = m_textureMap.begin(); texIt != m_textureMap.end(); texIt++)
    {
        SDL_DestroyTexture(texIt->second);
    }

    std::map<std::string, Mix_Chunk*>::const_iterator soundIt;
    for (soundIt = m_soundMap.begin(); soundIt != m_soundMap.end(); soundIt++)
    {
        Mix_FreeChunk(soundIt->second);
    }
}

void Game::Start()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    //Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
        std::cerr << "SDL_ttf could not be initialized! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    // Create window
    m_window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        std::cerr << "Window could not be created! SDL error: " << SDL_GetError() << std::endl;
    }

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL error: " << SDL_GetError() << std::endl;
    }

    // Load Textures
    LoadTexture("res/textures/man.png", "man");
    LoadTexture("res/textures/tree.png", "tree");
    LoadTexture("res/textures/selection.png", "selection");
    LoadTexture("res/textures/pile.png", "pile");

    // Load Sounds
    LoadSound("res/sounds/chop.wav", "chop");
    LoadSound("res/sounds/drop.wav", "drop");

    // Load GameObjects
    m_peon = new Peon(this);
    m_peon2 = new Peon(this);
    m_peon3 = new Peon(this);
    m_peon4 = new Peon(this);
    m_peon5 = new Peon(this);
    m_peon6 = new Peon(this);
    m_peon7 = new Peon(this);
    m_tree = new Tree(this);
    m_tree2 = new Tree(this);
    m_tree3 = new Tree(this);
    m_bonfire = new Bonfire(this);

    m_peon->Load(Vector2D(128, 128), 32, 32, "man");
    m_peon2->Load(Vector2D(192, 64), 32, 32, "man");
    m_peon3->Load(Vector2D(256, 64), 32, 32, "man");
    m_peon4->Load(Vector2D(400, 300), 32, 32, "man");
    m_peon5->Load(Vector2D(300, 300), 32, 32, "man");
    m_peon6->Load(Vector2D(128, 32), 32, 32, "man");
    m_peon7->Load(Vector2D(32, 328), 32, 32, "man");
    m_tree->Load(Vector2D(90, 200), 32, 32, "tree");
    m_tree2->Load(Vector2D(200, 140), 32, 32, "tree");
    m_tree3->Load(Vector2D(320, 300), 32, 32, "tree");
    m_bonfire->Load(Vector2D(304, 224), 32, 32, "pile");

    m_gameObjects.push_back(m_tree);
    m_gameObjects.push_back(m_tree2);
    m_gameObjects.push_back(m_tree3);
    m_gameObjects.push_back(m_bonfire);
    m_gameObjects.push_back(m_peon);
    m_gameObjects.push_back(m_peon2);
    m_gameObjects.push_back(m_peon3);
    m_gameObjects.push_back(m_peon4);
    m_gameObjects.push_back(m_peon5);
    m_gameObjects.push_back(m_peon6);
    m_gameObjects.push_back(m_peon7);

    // Game loop
    double frameStartTime = 0.0;
    double frameEndTime = 0.0;
    SDL_Event event;
    while (m_isRunning)
    {
        frameStartTime = SDL_GetTicks();
        m_deltaTime = (frameStartTime - frameEndTime) / 1000;
        frameEndTime = frameStartTime;

        for (int i = 0; i < 5; i++)
        {
            m_buttonsDown[i] = false;
            m_buttonsUp[i] = false;
        }

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                m_isRunning = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (!m_buttonsCurrent[SDL_BUTTON_LEFT])
                    {
                        m_buttonsDown[SDL_BUTTON_LEFT] = true;
                    }

                    m_buttonsCurrent[SDL_BUTTON_LEFT] = true;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    if (!m_buttonsCurrent[SDL_BUTTON_RIGHT])
                    {
                        m_buttonsDown[SDL_BUTTON_RIGHT] = true;
                    }

                    m_buttonsCurrent[SDL_BUTTON_RIGHT] = true;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    m_buttonsUp[SDL_BUTTON_LEFT] = true;
                    m_buttonsCurrent[SDL_BUTTON_LEFT] = false;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    m_buttonsUp[SDL_BUTTON_RIGHT] = true;
                    m_buttonsCurrent[SDL_BUTTON_RIGHT] = false;
                }
            }
        }

        if (!m_isRunning)
        {
            break;
        }

        ProcessInput();
        Update();
        Render();
    }

    Stop();
}

void Game::Stop()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
    Mix_Quit();
}

void Game::Update()
{
    for (std::vector<GameObject*>::const_iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
    {
        (*it)->Update();
    }
}

void Game::ProcessInput()
{
    SDL_GetMouseState(&mouseX, &mouseY);

    if (m_buttonsDown[SDL_BUTTON_LEFT])
    {
        LeftClick();
    }
    else if (m_buttonsDown[SDL_BUTTON_RIGHT])
    {
        RightClick();
    }
    
    if (m_buttonsUp[SDL_BUTTON_LEFT])
    {
        LeftClickUp();
    }
    if (m_buttonsUp[SDL_BUTTON_LEFT])
    {
        RightClickUp();
    }

    // Box selection
    if (m_buttonsCurrent[SDL_BUTTON_LEFT])
    {
        if (!m_selecting)
        {
            m_selecting = true;
            m_selectionStartX = mouseX;
            m_selectionStartY = mouseY;
        }
        
        if (m_selecting)
        {
            m_selectionEndX = mouseX - m_selectionStartX;
            m_selectionEndY = mouseY - m_selectionStartY;

            // Construct selection box
            m_selectionRect.x = m_selectionStartX;
            m_selectionRect.y = m_selectionStartY;
            m_selectionRect.w = m_selectionEndX;
            m_selectionRect.h = m_selectionEndY;
        }
    }
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer, 133, 222, 80, 255);
    SDL_RenderClear(m_renderer);

    for (std::vector<GameObject*>::const_iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
    {
        (*it)->Render();
        //SDL_Rect rect = (*it)->GetHitBox();
        //SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
        //SDL_RenderDrawRect(m_renderer, &rect);
    }

    for (std::vector<Peon*>::const_iterator it = m_selectedPeons.begin(); it != m_selectedPeons.end(); it++)
    {
        //SDL_Rect rect = { (*it)->GetX(), (*it)->GetY() , (*it)->GetWidth(), (*it)->GetHeight() };
        //SDL_Rect rect = (*it)->GetHitBox();
        //SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        //SDL_RenderDrawRect(m_renderer, &rect);
        DrawTexture("selection", (*it)->GetPosition().GetX(), (*it)->GetPosition().GetY(), (*it)->GetWidth(), (*it)->GetHeight());
    }

    if (m_selecting)
    {
        //SDL_Rect rect = { m_selectionStartX, m_selectionStartY , m_selectionEndX - m_selectionStartX, m_selectionEndY - m_selectionStartY };
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(m_renderer, &m_selectionRect);
    }

    SDL_RenderPresent(m_renderer);
}

void Game::LeftClick()
{
    /*
    // Look for a peon that we clicked on, and select it
    for (std::vector<GameObject*>::const_iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
    {
        GameObject* obj = *it;
        if (obj->m_ID == "peon")
        {
            double x = obj->GetX();
            double y = obj->GetY();
            double w = obj->GetWidth();
            double h = obj->GetHeight();

            // Check if the selection contains that unit
            if (mouseX > x && mouseY > y && mouseX < (x + w) && mouseY < (y + w))
            {
                m_selectedPeons.push_back(dynamic_cast<Peon*>(obj));
            }
        }
    }
    */

    m_selectedPeons.clear();
}

void Game::LeftClickUp()
{
    if (m_selecting)
    {
        // Look for the peons in our selection box, and select them
        for (std::vector<GameObject*>::const_iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
        {
            GameObject* obj = *it;
            if (obj->m_ID == "peon")
            {
                double x = obj->GetPosition().GetX();
                double y = obj->GetPosition().GetY();
                double w = obj->GetWidth();
                double h = obj->GetHeight();

                // Check if the selection contains that unit
                if (CheckCollision(m_selectionRect, obj->GetHitBox()))
                {
                    m_selectedPeons.push_back(dynamic_cast<Peon*>(obj));
                }
            }
        }

        m_selecting = false;
    }
}

void Game::RightClick()
{
    if (m_selectedPeons.size() > 0)
    {
        for (std::vector<GameObject*>::const_iterator objIt = m_gameObjects.begin(); objIt != m_gameObjects.end(); objIt++)
        {
            GameObject* obj = *objIt;
            if (obj->m_ID == "tree")
            {
                double x = obj->GetPosition().GetX();
                double y = obj->GetPosition().GetY();
                double w = obj->GetWidth();
                double h = obj->GetHeight();

                SDL_Rect mouseRect = { mouseX, mouseY, 5, 5 };

                if (CheckCollision(mouseRect, obj->GetHitBox()))
                {
                    for (std::vector<Peon*>::const_iterator it = m_selectedPeons.begin(); it != m_selectedPeons.end(); it++)
                    {
                        //CommandPeon(*it, *objIt);
                        //(*it)->m_job = (*it)->LUMBERJACK;
                    }
                }
            }
        }
    }
}

void Game::RightClickUp()
{

}

bool Game::CheckCollision(SDL_Rect a, SDL_Rect b)
{
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    if (rightA < leftA)
    {
        int temp = leftA;
        leftA = rightA;
        rightA = temp;
    }

    if (rightB < leftB)
    {
        int temp = leftB;
        leftB = rightB;
        rightB = temp;
    }

    if (bottomA < topA)
    {
        int temp = topA;
        topA = bottomA;
        bottomA = temp;
    }

    if (bottomB < topB)
    {
        int temp = topB;
        topB = bottomB;
        bottomB = temp;
    }
    

    if (rightA > leftB && leftA < rightB)
    {
        if (bottomA > topB && topA < bottomB)
        {
            return true;
        }
    }

    return false;
}

Bonfire* Game::FindBonfire(Peon* peon)
{
    Bonfire* bonfire = nullptr;

    for (std::vector<GameObject*>::const_iterator objIt = m_gameObjects.begin(); objIt != m_gameObjects.end(); objIt++)
    {
        GameObject* obj = *objIt;
        if (obj->m_ID == "bonfire")
        {
            bonfire = dynamic_cast<Bonfire*>(obj);
        }
    }

    return bonfire;
}

Tree* Game::FindTree(Peon* peon)
{
    Tree* tree = nullptr;

    for (std::vector<GameObject*>::const_iterator objIt = m_gameObjects.begin(); objIt != m_gameObjects.end(); objIt++)
    {
        GameObject* obj = *objIt;
        if (obj->m_ID == "tree")
        {
            if (tree == nullptr)
            {
                tree = dynamic_cast<Tree*>(obj);
            }
            else
            {
                if (Vector2D::Distance(peon->GetPosition(), obj->GetPosition()) < Vector2D::Distance(peon->GetPosition(), tree->GetPosition()))
                {
                    tree = dynamic_cast<Tree*>(obj);
                }
            }
        }
    }

    return tree;
}

void Game::CommandPeon(Peon* peon, GameObject* target)
{
    peon->dest = Vector2D(mouseX, mouseY);
}

bool Game::LoadTexture(const std::string& path, const std::string& id)
{
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    if (tempSurface == nullptr)
    {
        std::cerr << "Unable to load image " << path << "! SDL_image error: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (texture == nullptr)
    {
        std::cerr << "Unable to create texture from " << path << "! SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Texture " << id << " loaded." << std::endl;
    m_textureMap[id] = texture;
    return true;
}

void Game::DrawTexture(const std::string& id, const double& x, const double& y, const double& width, const double& height)
{
    SDL_Rect srcRect = { 0, 0, width, height };
    SDL_Rect destRect = { x, y, width, height };

    SDL_RenderCopyEx(m_renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

bool Game::LoadSound(const std::string& path, const std::string& id)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr)
    {
        std::cerr << "Failed to load WAV from " << path << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    std::cout << "Sound " << id << " loaded." << std::endl;
    m_soundMap[id] = sound;
    return true;
}

void Game::PlaySound(const std::string& id)
{
    Mix_PlayChannel(-1, m_soundMap[id], 0);
}