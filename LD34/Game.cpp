#include "PCH.hpp"
#include "Game.hpp"
#include "Vector2D.hpp"

Game::Game() :
    m_isRunning(true),
    m_deltaTime(0.0),
    m_resources(0)
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

    std::map<std::string, TTF_Font*>::const_iterator fontIt;
    for (fontIt = m_fontMap.begin(); fontIt != m_fontMap.end(); fontIt++)
    {
        TTF_CloseFont(fontIt->second);
    }

    std::map<std::string, Mix_Chunk*>::const_iterator soundIt;
    for (soundIt = m_soundMap.begin(); soundIt != m_soundMap.end(); soundIt++)
    {
        Mix_FreeChunk(soundIt->second);
    }

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
    Mix_Quit();
    TTF_Quit();
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

    // Load fonts
    LoadFont("res/fonts/dos.ttf", "dos");

    // Load Sounds
    LoadSound("res/sounds/chop.wav", "chop");
    LoadSound("res/sounds/drop.wav", "drop");

    // Load GameObjects
    m_tree = new Tree(this);
    m_tree2 = new Tree(this);
    m_tree3 = new Tree(this);
    m_bonfire = new Bonfire(this);

    m_tree->Load(Vector2D(90, 200), 32, 32, "tree");
    m_tree2->Load(Vector2D(200, 140), 32, 32, "tree");
    m_tree3->Load(Vector2D(320, 300), 32, 32, "tree");
    m_bonfire->Load(Vector2D(304, 224), 32, 32, "pile");

    m_gameObjects.push_back(m_tree);
    m_gameObjects.push_back(m_tree2);
    m_gameObjects.push_back(m_tree3);
    m_gameObjects.push_back(m_bonfire);

    for (int i = 0; i < 10; i++)
    {
        SpawnPeon();
    }

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

            m_selectionRect.x = mouseX;
            m_selectionRect.y = mouseY;
        }
        
        if (m_selecting)
        {
            m_selectionRect.w = mouseX - m_selectionRect.x;
            m_selectionRect.h = mouseY - m_selectionRect.y;
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
    }

    for (std::vector<Peon*>::const_iterator it = m_selectedPeons.begin(); it != m_selectedPeons.end(); it++)
    {
        RenderTexture("selection", (*it)->GetPosition().GetX(), (*it)->GetPosition().GetY(), (*it)->GetWidth(), (*it)->GetHeight());
    }

    if (m_selecting)
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(m_renderer, &m_selectionRect);
    }

    // Draw GUI
    sstream.str("");
    sstream << "Resources: " << m_resources;
    RenderText("dos", 0, 0, sstream.str());

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
    for (std::vector<Peon*>::const_iterator it = m_selectedPeons.begin(); it != m_selectedPeons.end(); it++)
    {
        CommandPeon(*it, nullptr);
    }

    /*
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
    */
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

void Game::SpawnPeon()
{
    Vector2D position(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
    int width = 32;
    int height = 32;
    Peon* obj = new Peon(this, position, width, height, "man");
    m_gameObjects.push_back(obj);
}

void Game::CommandPeon(Peon* peon, GameObject* target)
{
    peon->m_state = peon->WALKING;
    peon->dest = Vector2D(mouseX, mouseY);
}

void Game::DepositResources(int amount)
{
    m_resources += amount;
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

void Game::RenderTexture(const std::string& id, const double& x, const double& y, const double& width, const double& height)
{
    SDL_Rect srcRect = { 0, 0, width, height };
    SDL_Rect destRect = { x, y, width, height };

    SDL_RenderCopyEx(m_renderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

bool Game::LoadFont(const std::string& path, const std::string& id)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), 16);
    if (font == nullptr)
    {
        std::cerr << "Failed to load font! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    std::cout << "Font " << id << " loaded." << std::endl;
    m_fontMap[id] = font;
    return true;
}

void Game::RenderText(const std::string& fontID, const double& x, const double& y, const std::string& text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(m_fontMap[fontID], text.c_str(), color);
    if (surface == nullptr)
    {
        std::cerr << "Failed to render font to surface! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    int width;
    int height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    if (texture == nullptr)
    {
        std::cout << "Failed to create texture from surface! SDL error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect srcRect = { 0, 0, width, height };
    SDL_Rect destRect = { x, y, width, height };

    SDL_RenderCopyEx(m_renderer, texture, &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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