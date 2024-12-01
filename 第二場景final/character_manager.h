#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "character.h"

class CharacterManager
{
public:

    bool has_player_started() const { return player_started; }

    static CharacterManager* instance();

    Character* get_enemy()
    {
        return enemy;
    }

    Character* get_player()
    {
        return player;
    }

    void on_input(const ExMessage& msg);
    void on_update(float delta);
    void on_render();

private:
    static CharacterManager* manager;

    Character* enemy = nullptr;
    Character* player = nullptr;

private:

    bool player_started;

    CharacterManager();
    ~CharacterManager();
};

#endif