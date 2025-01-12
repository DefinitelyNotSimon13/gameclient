use std::io::{stdin, Read};

use super::jump::Jump;
use raylib::consts::KeyboardKey::*;
use raylib::{color::Color, math::Vector3, prelude::RaylibDraw3D, RaylibHandle};

const MOVE_SPEED: f32 = 0.2;

pub struct Player {
    pub postition: Vector3,
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
    pub fn draw(&self, d: &mut impl RaylibDraw3D) {
        d.draw_cube(self.postition, 1.0, 1.0, 1.0, self.primary_color);
        d.draw_cube_wires(self.postition, 1.0, 1.0, 1.0, self.secondary_color);
    }

    pub fn update_position(&mut self, rl: &RaylibHandle) {
        if rl.is_key_down(KEY_W) {
            self.postition.x -= MOVE_SPEED
        }
        if rl.is_key_down(KEY_S) {
            self.postition.x += MOVE_SPEED
        }
        if rl.is_key_down(KEY_A) {
            self.postition.z += MOVE_SPEED
        }
        if rl.is_key_down(KEY_D) {
            self.postition.z -= MOVE_SPEED
        }
    }
}

impl LocalPlayer {
    pub fn new(player: Player) -> Self {
        Self {
            player,
            jump: Jump::default(),
        }
    }

    pub fn draw(&self, d: &mut impl RaylibDraw3D) {
        self.player.draw(d);
    }

    pub fn jump(&mut self) {
        self.jump.start_jump(self.player.postition.y);
    }

    pub fn update_position(&mut self, rl: &RaylibHandle) {
        if rl.is_key_down(KEY_SPACE) {
            self.jump.start_jump(self.player.postition.y);
        }
        self.player.update_position(rl);

        if self.jump.is_jumping() {
            self.player.postition.y = self.jump.perform_jump(self.player.postition.y)
        }
        if self.jump.is_falling() {
            self.player.postition.y = self.jump.perform_fall(self.player.postition.y)
        }
    }
}
