use super::camera::Camera;
use super::jump::Jump;
use raylib::consts::KeyboardKey::*;
use raylib::math::Vector2;
use raylib::prelude::RaylibDraw;
use raylib::{color::Color, math::Vector3, prelude::RaylibDraw3D, RaylibHandle};

const MOVE_SPEED: f32 = 7.0;
const PLAYER_LABEL_FONT_SIZE: i32 = 10;

pub struct Player {
    pub username: String,
    username_len: i32,
    pub postition: Vector3,
    pub screen_position: Vector2,
    pub primary_color: Color,
    pub secondary_color: Color,
}

pub struct LocalPlayer {
    player: Player,
    jump: Jump,
}

pub struct RemotePlayer {
    player: Player,
}

impl Player {
    pub fn new(username: &str, rl: &RaylibHandle) -> Self {
        let username_len = rl.measure_text(username, PLAYER_LABEL_FONT_SIZE);
        Self {
            username: username.to_string(),
            username_len,
            postition: Vector3::new(0.0, 5.0, 0.0),
            screen_position: Vector2::new(0.0, 0.0),
            primary_color: Color::PURPLE,
            secondary_color: Color::DARKPURPLE,
        }
    }

    pub fn draw(&self, d: &mut impl RaylibDraw3D) {
        d.draw_cube(self.postition, 0.5, 0.5, 0.5, self.primary_color);
        d.draw_cube_wires(self.postition, 0.5, 0.5, 0.5, self.secondary_color);
    }

    pub fn draw_name(&self, d: &mut impl RaylibDraw) {
        d.draw_text(
            &self.username,
            self.screen_position.x.trunc() as i32 - (self.username_len / 2),
            self.screen_position.y.trunc() as i32 - 20,
            PLAYER_LABEL_FONT_SIZE,
            self.primary_color,
        );
    }

    pub fn update_position(&mut self, rl: &RaylibHandle, camera: &Camera) {
        let delta_time = rl.get_frame_time();
        let move_speed = MOVE_SPEED * delta_time;

        if rl.is_key_down(KEY_W) {
            self.postition.x -= move_speed;
            self.postition.z += move_speed;
        }
        if rl.is_key_down(KEY_S) {
            self.postition.x += move_speed;
            self.postition.z -= move_speed;
        }
        if rl.is_key_down(KEY_A) {
            self.postition.z += move_speed;
            self.postition.x += move_speed;
        }
        if rl.is_key_down(KEY_D) {
            self.postition.z -= move_speed;
            self.postition.x -= move_speed;
        }

        self.screen_position = rl.get_world_to_screen(self.postition, camera.camera);
    }
}

impl LocalPlayer {
    pub fn new(username: &str, rl: &RaylibHandle) -> Self {
        Self {
            player: Player::new(username, rl),
            jump: Jump::default(),
        }
    }

    pub fn draw(&self, d: &mut impl RaylibDraw3D) {
        self.player.draw(d);
    }

    pub fn draw_name(&self, d: &mut impl RaylibDraw) {
        self.player.draw_name(d);
    }

    pub fn jump(&mut self) {
        self.jump.start_jump(self.player.postition.y);
    }

    pub fn update_position(&mut self, rl: &RaylibHandle, camera: &Camera) {
        if rl.is_key_down(KEY_SPACE) {
            self.jump.start_jump(self.player.postition.y);
        }
        self.player.update_position(rl, camera);

        if self.jump.is_jumping() {
            let delta_time = rl.get_frame_time();
            self.player.postition.y = self.jump.perform_jump(self.player.postition.y, delta_time)
        }
        if self.jump.is_falling() {
            let delta_time = rl.get_frame_time();
            self.player.postition.y = self.jump.perform_fall(self.player.postition.y, delta_time)
        }
    }
}
