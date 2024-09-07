
#include "player.h"
#include "scene.h"
#include "components.h"

Player *player_create(Scene *scene)
{
    Player *player = malloc(sizeof(Player));
    player->jump_count = 0;
    player->is_jumping = false;
    player->move_speed = 18.0f;
    player->jump_height = 150.0f; // in newton

    Vector3 pos = Vector3Zero();
    Vector3 scale = { 50.0f, 50.0f, 50.0f };
    Vector3 rotation = Vector3Zero();

    player->go = create_game_object("player", scene, pos, scale, rotation);
    SpriteComponent *sprite = create_component(T_SPRITE);
    sprite->texture = load_sprite_texture("data/textures/checkerboard.png", (i32)scale.x, (i32)scale.y);
    sprite->tint_color = BLUE;
    add_component(player->go, sprite);

    BoxCollider2D *collider = create_component(T_BOX_COLLIDER_2D);
    collider->body_type = DynamicBody2D;
    collider->user_data = player->go;
    collider->gravity_scale = 10.0f;

    physics_2d_attach_box_collider(scene->b2_world_id, collider, *get_transform_component(player->go));
    add_component(player->go, collider);

    return player;
}

void player_destroy(Player *player)
{
    if (player)
        free(player);
}

void player_update(Player *player, f32 delta_time)
{
    if (player != NULL)
    {
        TransformComponent *transform = get_transform_component(player->go);
        BoxCollider2D *collider = get_component(player->go, T_BOX_COLLIDER_2D);
        if (transform == NULL || collider == NULL) return;

        player_collision_2d(player, collider);

        Vector2 velocity = {0.0f, 0.0f };

        if (player->jump_count <= 2)
        {
            if (IsKeyDown(KEY_SPACE))
            {
                velocity.y = player->jump_height;
                player->jump_count++;
            }

            if (IsKeyDown(KEY_A))
                velocity.x = -player->move_speed;
            else if (IsKeyDown(KEY_D))
                velocity.x = player->move_speed;
        }

        if(player->is_jumping)
        {
            if (IsKeyDown(KEY_A))
                velocity.x = -player->move_speed / 3.0f;
            else if (IsKeyDown(KEY_D))
                velocity.x = player->move_speed / 3.0f;
        }

        //set_linear_velocity(collider, velocity);
        if (velocity.x > 0.1f || velocity.x < -0.1f || velocity.y > 0.1f)
            apply_force_to_center(collider, velocity, true);
    }
}

void player_collision_2d(Player *player, BoxCollider2D *collider)
{
    for (int i = 0; i < collider->event_count; ++i)
    {
        Collision2DEvent* event = &collider->collision_events[i];

        if (event->is_begin) // collision enter
        {
            if (strcmp(event->game_object->name, "platform") == 0)
            {
                player->is_jumping = false;
                player->jump_count = 0;
            }

            if (strcmp(event->game_object->name, "enemy") == 0)
            {
                // TODO: add sound bank
                PlaySound(player->go->scene->sound_effect);
                destroy_game_object(event->game_object, player->go->scene);
            }

        }
        else if (event->is_hit)
        {
        }
        else // collision exit
        {
            if (strcmp(event->game_object->name, "platform") == 0)
            {
                player->is_jumping = true;
            }
        }
    }

    // Clear events after processing
    collider->event_count = 0;
    
}

Vector3 player_get_position(Player *player)
{
    return get_transform_component(player->go)->translation;
}