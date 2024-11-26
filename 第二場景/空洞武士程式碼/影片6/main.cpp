#include "util.h"
#include "resources_manager.h"
#include "collision_manager.h"
#include "character_manager.h"
#include "bullet_manager.h"

#include <chrono>
#include <thread>
#include <graphics.h>


while (!is_quit)
{
    while (peekmessage(&msg))
        CharacterManager::instance()->on_input(msg);

    steady_clock::time_point frame_start = steady_clock::now();
    duration<float> delta = duration<float>(frame_start - last_tick);

    float scaled_delta = BulletTimeManager::instance()->on_update(delta.count());
    CharacterManager::instance()->on_update(scaled_delta);
    CollisionManager::instance()->process_collide();

    setbkcolor(RGB(0, 0, 0));
    cleardevice();

    draw_background();
    CharacterManager::instance()->on_render();
    CollisionManager::instance()->on_debug_render();

    FlushBatchDraw();

    last_tick = frame_start;
    nanoseconds sleep_duration = frame_duration - (steady_clock::now() - frame_start);
    if (sleep_duration > nanoseconds(0))
        std::this_thread::sleep_for(sleep_duration);
}
