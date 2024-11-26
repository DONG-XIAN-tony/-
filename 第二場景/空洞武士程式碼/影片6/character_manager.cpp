#include "player.h"
#include "character_manager.h"
#include "bullet_time_manager.h"

void CharacterManager::on_render()
{
    BulletTimeManager::instance()->post_process();
    player->on_render();
}
