#include"enemy.h"
#include "player.h"
#include "character_manager.h"
#include "bullet_time_manager.h"

void CharacterManager::on_update(float delta)
{
    enemy->on_update(delta);
    player->on_update(delta);
}

void CharacterManager::on_render()
{
    enemy->on_render();
    BulletTimeManager::instance()->post_process();
    player->on_render();
}

CharacterManager::CharacterManager()
{
    enemy = new Enemy();
    player = new Player();
}
